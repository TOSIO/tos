#pragma once
#include <stdio.h>
#include <toscore/common/Common.h>
#include <toscore/common/Address.h>
#include <boost/optional.hpp>
#include <toscore/utils/RLP.h>
#include <string>

namespace dev
{
namespace sdag
{
class BlockHeader
{

  public:
    BlockHeader();
    BlockHeader(u256 const &gasPrice, u256 const &gasLimit);

    BlockHeader(u256 const &type, u256 const &gasPrice, u256 const &gasLimit);

    BlockHeader(RLPStream stream);
    BlockHeader(RLP rlp);
    void encode(RLPStream &stream);

    u256 getGasPrice(){ return m_gasPrice; }
    u256 getGasLimit(){ return m_gasLimit; }
    int64_t getTime(){ return m_time; }
    
  private:
    u256 m_version;
    u256 m_type; //
    int64_t m_time; //current time
    u256 m_gasPrice;
    u256 m_gasLimit;

    void printBlockHeader();
};

} // namespace sdag
} // namespace dev
