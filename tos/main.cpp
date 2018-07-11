#include <stdio.h>
#include <toscore/log/Log.h>
#include <toscore/crypto/Base64.h>
 
int main(int argc, char const *argv[])
{
    /* code */
printf("asdfasdf\n");
//BOOST_LOG_INLINE_GLOBAL_LOGGER_CTOR_ARGS('dsfdfsd', 12);
std::string str = "13123";

printf("asdfasdf\n  %c   %s", str, fromBase64(str));

    return 0;
}
