// Copyright 2018 tos Authors.
// Licensed under the GNU General Public License v3. See the LICENSE file.

#include "TVMC.h"

#include <toscore/log/Log.h>
#include <libtvm/VM.h>
#include <libtvm/VMFactory.h>

namespace dev
{
namespace eth
{
EVM::EVM(tvmc_instance* _instance) noexcept : m_instance(_instance)
{
    assert(m_instance != nullptr);
    assert(m_instance->abi_version == TVMC_ABI_VERSION);

    // Set the options.
    for (auto& pair : tvmcOptions())
        m_instance->set_option(m_instance, pair.first.c_str(), pair.second.c_str());
}

owning_bytes_ref TVMC::exec(u256& io_gas, ExtVMFace& _ext, const OnOpFunc& _onOp)
{
    assert(_ext.envInfo().number() >= 0);
    assert(_ext.envInfo().timestamp() >= 0);

    constexpr int64_t int64max = std::numeric_limits<int64_t>::max();

    // TODO: The following checks should be removed by changing the types
    //       used for gas, block number and timestamp.
    (void)int64max;
    assert(io_gas <= int64max);
    assert(_ext.envInfo().gasLimit() <= int64max);
    assert(_ext.depth <= static_cast<size_t>(std::numeric_limits<int32_t>::max()));

    auto gas = static_cast<int64_t>(io_gas);
    EVM::Result r = execute(_ext, gas);

    switch (r.status())
    {
    case TVMC_SUCCESS:
        io_gas = r.gasLeft();
        // FIXME: Copy the output for now, but copyless version possible.
        return {r.output().toVector(), 0, r.output().size()};

    case TVMC_REVERT:
        io_gas = r.gasLeft();
        // FIXME: Copy the output for now, but copyless version possible.
        throw RevertInstruction{{r.output().toVector(), 0, r.output().size()}};

    case TVMC_OUT_OF_GAS:
    case TVMC_FAILURE:
        BOOST_THROW_EXCEPTION(OutOfGas());

    case TVMC_UNDEFINED_INSTRUCTION:
        BOOST_THROW_EXCEPTION(BadInstruction());

    case TVMC_BAD_JUMP_DESTINATION:
        BOOST_THROW_EXCEPTION(BadJumpDestination());

    case TVMC_STACK_OVERFLOW:
        BOOST_THROW_EXCEPTION(OutOfStack());

    case TVMC_STACK_UNDERFLOW:
        BOOST_THROW_EXCEPTION(StackUnderflow());

    case TVMC_STATIC_MODE_VIOLATION:
        BOOST_THROW_EXCEPTION(DisallowedStateChange());

    case TVMC_REJECTED:
        cwarn << "Execution rejected by TVMC, executing with default VM implementation";
        return VMFactory::create(VMKind::Legacy)->exec(io_gas, _ext, _onOp);

    default:
        BOOST_THROW_EXCEPTION(InternalVMError{} << errinfo_tvmcStatusCode(r.status()));
    }
}

tvmc_revision toRevision(EVMSchedule const& _schedule)
{
    if (_schedule.haveCreate2)
        return TVMC_CONSTANTINOPLE;
    if (_schedule.haveRevert)
        return TVMC_BYZANTIUM;
    if (_schedule.eip158Mode)
        return TVMC_SPURIOUS_DRAGON;
    if (_schedule.eip150Mode)
        return TVMC_TANGERINE_WHISTLE;
    if (_schedule.haveDelegateCall)
        return TVMC_HOMESTEAD;
    return TVMC_FRONTIER;
}
}  // namespace tvm
}  // namespace dev
