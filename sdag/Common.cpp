#include "Common.h"

#include <toscore/common/Common.h>

using namespace dev;
// using namespace dev::sdag;

u256 dev::sdag::get_timestamp()
{
    struct timeval tp;

    gettimeofday(&tp, 0);
    return (u256)(unsigned long)tp.tv_sec << 10 | ((tp.tv_usec << 10) / 1000000);
}