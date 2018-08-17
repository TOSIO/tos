#include <toscore/common/Common.h>
#include <toscore/crypto/FixedHash.h>

using sdag_diff_t = u128;

inline sdag_diff_t calculateWork(h256 hash)
{
    sdag_diff_t res = sdag_diff_t(hash);
    sdag_diff_t max = -1;
    res >>= 32;
    return max / res;
}