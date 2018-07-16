#include <stdio.h>
#include <toscore/log/Log.h>
#include <toscore/crypto/SHA3.h>
#include <toscore/common/Common.h>
#include <toscore/utils/RLP.h>
#include <vector>
#include <string>
 
using std::cout;


using dev::RLP;
//template<typename T>
int main(int argc, char const *argv[])
{
    /* code */
    printf("asdfasdf\n");
    //BOOST_LOG_INLINE_GLOBAL_LOGGER_CTOR_ARGS('dsfdfsd', 12);
    const std::string str = "13123weriouweoruweo";

    cout << str << std::endl;

    std::vector<unsigned char> valList = {'0', '1', '3', '4'};
    // std::string str1;// = dev::toBase64(valList);
    // cout<<str1<< std::endl;

    dev::bytesRef vec(valList.data(), valList.size());
    dev::RLP rlp(vec, dev::RLP::AllowNonCanon);
    // rlp::isInt();
    cout << valList.data() << std::endl;
    cout << rlp.sizeStrict() << std::endl;

    return 0;
}
