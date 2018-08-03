/*
	This file is part of tos.

	tos is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	tos is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with tos.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ExtVMFace.h"

namespace dev
{
namespace eth
{
namespace
{

static_assert(sizeof(Address) == sizeof(tvmc_address), "Address types size mismatch");
static_assert(alignof(Address) == alignof(tvmc_address), "Address types alignment mismatch");
static_assert(sizeof(h256) == sizeof(tvmc_uint256be), "Hash types size mismatch");
static_assert(alignof(h256) == alignof(tvmc_uint256be), "Hash types alignment mismatch");

int accountExists(tvmc_context* _context, tvmc_address const* _addr) noexcept
{
	auto& env = static_cast<ExtVMFace&>(*_context);
	Address addr = fromTvmC(*_addr);
	return env.exists(addr) ? 1 : 0;
}

void getStorage(
	tvmc_uint256be* o_result,
	tvmc_context* _context,
	tvmc_address const* _addr,
	tvmc_uint256be const* _key
) noexcept
{
	(void) _addr;
	auto& env = static_cast<ExtVMFace&>(*_context);
	assert(fromTvmC(*_addr) == env.myAddress);
	u256 key = fromTvmC(*_key);
	*o_result = toTvmC(env.store(key));
}

void setStorage(
	tvmc_context* _context,
	tvmc_address const* _addr,
	tvmc_uint256be const* _key,
	tvmc_uint256be const* _value
) noexcept
{
	(void) _addr;
	auto& env = static_cast<ExtVMFace&>(*_context);
	assert(fromTvmC(*_addr) == env.myAddress);
	u256 index = fromTvmC(*_key);
	u256 value = fromTvmC(*_value);
	if (value == 0 && env.store(index) != 0)                   // If delete
		env.sub.refunds += env.evmSchedule().sstoreRefundGas;  // Increase refund counter

	env.setStore(index, value);    // Interface uses native endianness
}

void getBalance(
	tvmc_uint256be* o_result,
	tvmc_context* _context,
	tvmc_address const* _addr
) noexcept
{
	auto& env = static_cast<ExtVMFace&>(*_context);
	*o_result = toTvmC(env.balance(fromTvmC(*_addr)));
}

size_t getCodeSize(tvmc_context* _context, tvmc_address const* _addr)
{
    auto& env = static_cast<ExtVMFace&>(*_context);
    return env.codeSizeAt(fromTvmC(*_addr));
}

size_t copyCode(tvmc_context* _context, tvmc_address const* _addr, size_t _codeOffset,
    byte* _bufferData, size_t _bufferSize)
{
    auto& env = static_cast<ExtVMFace&>(*_context);
    Address addr = fromTvmC(*_addr);
    bytes const& code = env.codeAt(addr);

    // Handle "big offset" edge case.
    if (_codeOffset >= code.size())
        return 0;

    size_t maxToCopy = code.size() - _codeOffset;
    size_t numToCopy = std::min(maxToCopy, _bufferSize);
    std::copy_n(&code[_codeOffset], numToCopy, _bufferData);
    return numToCopy;
}

void selfdestruct(
	tvmc_context* _context,
	tvmc_address const* _addr,
	tvmc_address const* _beneficiary
) noexcept
{
	(void) _addr;
	auto& env = static_cast<ExtVMFace&>(*_context);
	assert(fromTvmC(*_addr) == env.myAddress);
	env.suicide(fromTvmC(*_beneficiary));
}


void log(
	tvmc_context* _context,
	tvmc_address const* _addr,
	uint8_t const* _data,
	size_t _dataSize,
	tvmc_uint256be const _topics[],
	size_t _numTopics
) noexcept
{
	(void) _addr;
	auto& env = static_cast<ExtVMFace&>(*_context);
	assert(fromTvmC(*_addr) == env.myAddress);
	h256 const* pTopics = reinterpret_cast<h256 const*>(_topics);
	env.log(h256s{pTopics, pTopics + _numTopics},
			bytesConstRef{_data, _dataSize});
}

void getTxContext(tvmc_tx_context* result, tvmc_context* _context) noexcept
{
	auto& env = static_cast<ExtVMFace&>(*_context);
	result->tx_gas_price = toTvmC(env.gasPrice);
	result->tx_origin = toTvmC(env.origin);
	result->block_coinbase = toTvmC(env.envInfo().author());
	result->block_number = static_cast<int64_t>(env.envInfo().number());
	result->block_timestamp = static_cast<int64_t>(env.envInfo().timestamp());
	result->block_gas_limit = static_cast<int64_t>(env.envInfo().gasLimit());
	result->block_difficulty = toTvmC(env.envInfo().difficulty());
}

void getBlockHash(tvmc_uint256be* o_hash, tvmc_context* _envPtr, int64_t _number)
{
	auto& env = static_cast<ExtVMFace&>(*_envPtr);
	*o_hash = toTvmC(env.blockHash(_number));
}

void create(tvmc_result* o_result, ExtVMFace& _env, tvmc_message const* _msg) noexcept
{
	u256 gas = _msg->gas;
	u256 value = fromTvmC(_msg->value);
	bytesConstRef init = {_msg->input_data, _msg->input_size};
	// ExtVM::create takes the sender address from .myAddress.
	assert(fromTvmC(_msg->sender) == _env.myAddress);

	CreateResult result = _env.create(value, gas, init, Instruction::CREATE, u256(0), {});
	o_result->status_code = result.status;
	o_result->gas_left = static_cast<int64_t>(gas);
	o_result->release = nullptr;

	if (result.status == TVMC_SUCCESS)
	{
		o_result->create_address = toTvmC(result.address);
		o_result->output_data = nullptr;
		o_result->output_size = 0;
	}
	else
	{
		// Pass the output to the EVM without a copy. The EVM will delete it
		// when finished with it.

		// First assign reference. References are not invalidated when vector
		// of bytes is moved. See `.takeBytes()` below.
		o_result->output_data = result.output.data();
		o_result->output_size = result.output.size();

		// Place a new vector of bytes containing output in result's reserved memory.
		auto* data = tvmc_get_optional_data(o_result);
		static_assert(sizeof(bytes) <= sizeof(*data), "Vector is too big");
		new(data) bytes(result.output.takeBytes());
		// Set the destructor to delete the vector.
		o_result->release = [](tvmc_result const* _result)
		{
			auto* data = tvmc_get_const_optional_data(_result);
			auto& output = reinterpret_cast<bytes const&>(*data);
			// Explicitly call vector's destructor to release its data.
			// This is normal pattern when placement new operator is used.
			output.~bytes();
		};
	}
}

void call(tvmc_result* o_result, tvmc_context* _context, tvmc_message const* _msg) noexcept
{
	assert(_msg->gas >= 0 && "Invalid gas value");
	auto& env = static_cast<ExtVMFace&>(*_context);

	// Handle CREATE separately.
	if (_msg->kind == TVMC_CREATE)
		return create(o_result, env, _msg);

	CallParameters params;
	params.gas = _msg->gas;
	params.apparentValue = fromTvmC(_msg->value);
	params.valueTransfer =
		_msg->kind == TVMC_DELEGATECALL ? 0 : params.apparentValue;
	params.senderAddress = fromTvmC(_msg->sender);
	params.codeAddress = fromTvmC(_msg->destination);
	params.receiveAddress =
		_msg->kind == TVMC_CALL ? params.codeAddress : env.myAddress;
	params.data = {_msg->input_data, _msg->input_size};
	params.staticCall = (_msg->flags & TVMC_STATIC) != 0;
	params.onOp = {};

	CallResult result = env.call(params);
	o_result->status_code = result.status;
	o_result->gas_left = static_cast<int64_t>(params.gas);

	// Pass the output to the EVM without a copy. The EVM will delete it
	// when finished with it.

	// First assign reference. References are not invalidated when vector
	// of bytes is moved. See `.takeBytes()` below.
	o_result->output_data = result.output.data();
	o_result->output_size = result.output.size();

	// Place a new vector of bytes containing output in result's reserved memory.
	auto* data = tvmc_get_optional_data(o_result);
	static_assert(sizeof(bytes) <= sizeof(*data), "Vector is too big");
	new(data) bytes(result.output.takeBytes());
	// Set the destructor to delete the vector.
	o_result->release = [](tvmc_result const* _result)
	{
		auto* data = tvmc_get_const_optional_data(_result);
		auto& output = reinterpret_cast<bytes const&>(*data);
		// Explicitly call vector's destructor to release its data.
		// This is normal pattern when placement new operator is used.
		output.~bytes();
	};
}

tvmc_context_fn_table const fnTable = {
    accountExists,
    getStorage,
    setStorage,
    getBalance,
    getCodeSize,
    copyCode,
    selfdestruct,
    eth::call,
    getTxContext,
    getBlockHash,
    eth::log,
};
}

ExtVMFace::ExtVMFace(EnvInfo const& _envInfo, Address _myAddress, Address _caller, Address _origin,
    u256 _value, u256 _gasPrice, bytesConstRef _data, bytes _code, h256 const& _codeHash,
    unsigned _depth, bool _isCreate, bool _staticCall)
  : tvmc_context{&fnTable},
    m_envInfo(_envInfo),
    myAddress(_myAddress),
    caller(_caller),
    origin(_origin),
    value(_value),
    gasPrice(_gasPrice),
    data(_data),
    code(std::move(_code)),
    codeHash(_codeHash),
    depth(_depth),
    isCreate(_isCreate),
    staticCall(_staticCall)
{}

}
}
