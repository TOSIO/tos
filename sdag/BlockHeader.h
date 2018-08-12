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

    BlockHeader(u256 const &type, u256 const &gasPrice, u256 const &gasLimit);

    BlockHeader(RLPStream stream);

    RLPStream encodeWithRLP();
    


private:
    u256 m_version;
    u256 m_type; //
    u256 m_time; //current time
    u256 m_gasPrice;
    u256 m_gasLimit;

void printBlockHeader();
};

} // namespace sdag
} // namespace DEV
