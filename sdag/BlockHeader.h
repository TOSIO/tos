#include <stdio.h>
#include <toscore/common/Common.h>
#include <toscore/common/Address.h>
#include <boost/optional.hpp>

namespace DEV
{
namespace sdag
{
class BlockHeader
{

    u256 version;
    u256 type;
    u256 time;
    u256 gasPrice;
    u256 gasLimit;
};

} // namespace sdag
} // namespace DEV
