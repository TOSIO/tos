#include <stdio.h>
#include <sys/time.h>
#include <toscore/common/Common.h>
// #include <mpir/gettimeofday.h>
#include<unistd.h>
namespace dev
{
    namespace sdag {
u256 get_timestamp(void)
{
    struct timeval tp;
    
    gettimeofday(&tp, 0);
    
    return (u256)(unsigned long)tp.tv_sec << 10 | ((tp.tv_usec << 10) / 1000000);
}
        
    }
} // DEV

