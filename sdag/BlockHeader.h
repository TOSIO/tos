#include <stdio.h>
#include <toscore/common/Common.h>
#include <toscore/common/Address.h>
#include <boost/optional.hpp>
#include <toscore/utils/RLP.h>
#include <string>
#include "Common.h"

namespace dev
{
namespace sdag
{
class BlockHeader
{

    u256 m_version;
    u256 m_type; //
    u256 m_time; //current time
    u256 m_gasPrice;
    u256 m_gasLimit;

    BlockHeader(u256 const &type, u256 const &gasPrice, u256 const &gasLimit) : m_type(type), m_gasPrice(gasPrice), m_gasLimit(gasLimit){};

    BlockHeader(bytesConstRef _rlpData);

    std::string encodeWithRLP();
    
};

} // namespace sdag
} // namespace DEV
