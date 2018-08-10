#include "BlockHeader.h"
using namespace std;
using namespace dev;
using namespace dev::sdag;

BlockHeader::BlockHeader(bytesConstRef _rlpData)
{


	RLP const rlp(_rlpData);


}

std::string BlockHeader::encodeWithRLP()
{
    return NULL;

}
