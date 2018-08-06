// Copyright 2018 tos Authors.
// Licensed under the GNU General Public License v3. See the LICENSE file.

#pragma once

#include <libtvm/VMFace.h>

#include <tvmc/tvmc.h>

namespace dev
{
namespace eth
{
/// Translate the EVMSchedule to EVM-C revision.
tvmc_revision toRevision(EVMSchedule const& _schedule);

/// The RAII wrapper for an EVM-C instance.
class EVM
{
public:
    explicit EVM(tvmc_instance* _instance) noexcept;

    ~EVM() { m_instance->destroy(m_instance); }

    EVM(EVM const&) = delete;
    EVM& operator=(EVM) = delete;

    class Result
    {
    public:
        explicit Result(tvmc_result const& _result):
            m_result(_result)
        {}

        ~Result()
        {
            if (m_result.release)
                m_result.release(&m_result);
        }

        Result(Result&& _other) noexcept:
            m_result(_other.m_result)
        {
            // Disable releaser of the rvalue object.
            _other.m_result.release = nullptr;
        }

        Result(Result const&) = delete;
        Result& operator=(Result const&) = delete;

        tvmc_status_code status() const
        {
            return m_result.status_code;
        }

        int64_t gasLeft() const
        {
            return m_result.gas_left;
        }

        bytesConstRef output() const
        {
            return {m_result.output_data, m_result.output_size};
        }

    private:
        tvmc_result m_result;
    };

    /// Handy wrapper for tvmc_execute().
    Result execute(ExtVMFace& _ext, int64_t gas)
    {
        auto mode = toRevision(_ext.evmSchedule());
        tvmc_call_kind kind = _ext.isCreate ? TVMC_CREATE : TVMC_CALL;
        uint32_t flags = _ext.staticCall ? TVMC_STATIC : 0;
        assert(flags != TVMC_STATIC || kind == TVMC_CALL);  // STATIC implies a CALL.
        tvmc_message msg = {toTvmC(_ext.myAddress), toTvmC(_ext.caller), toTvmC(_ext.value),
            _ext.data.data(), _ext.data.size(), toTvmC(_ext.codeHash), gas,
            static_cast<int32_t>(_ext.depth), kind, flags};
        return Result{
            m_instance->execute(m_instance, &_ext, mode, &msg, _ext.code.data(), _ext.code.size())};
    }

private:
    /// The VM instance created with EVM-C <prefix>_create() function.
    tvmc_instance* m_instance = nullptr;
};


/// The wrapper implementing the VMFace interface with a EVM-C VM as a backend.
class TVMC : public EVM, public VMFace
{
public:
    explicit TVMC(tvmc_instance* _instance) : EVM(_instance) {}

    owning_bytes_ref exec(u256& io_gas, ExtVMFace& _ext, OnOpFunc const& _onOp) final;
};
}
}
