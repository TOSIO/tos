#include "Common.h"

#include <toscore/common/Common.h>

using namespace dev;
// using namespace dev::sdag;

int64_t dev::sdag::get_timestamp()
{
    struct timeval tp;
    gettimeofday(&tp, 0);
    
    return (int64_t)(unsigned long)tp.tv_sec << 10 | ((tp.tv_usec << 10) / 1000000);
}
