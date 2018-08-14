#include "BlockHeader.h"
#include <toscore/common/CommonData.h>
#include <toscore/log/Log.h>
#include "Common.h"
using namespace std;
using namespace dev;
using namespace dev::sdag;

void BlockHeader::printBlockHeader()
{

cnote << "block header content " << "m_version: " << m_version<< " m_type: " << m_type<< " m_time: " << m_time<< " m_gasPrice: " << m_gasPrice<<" m_gasLimit: "<< m_gasLimit;

}

BlockHeader::BlockHeader(u256 const &gasPrice, u256 const &gasLimit){
    m_version = 1;
    m_type = 1;
    m_time = get_timestamp();
    m_gasPrice = gasPrice;
    m_gasLimit = gasLimit;
    printBlockHeader();
}


BlockHeader::BlockHeader(u256 const &type, u256 const &gasPrice, u256 const &gasLimit)
{
    m_version = 1;
    m_type = type;
    m_time = get_timestamp();
    m_gasPrice = gasPrice;
    m_gasLimit = gasLimit;
    printBlockHeader();
}

BlockHeader::BlockHeader(RLPStream stream)
{
    RLP const rlp(stream.out());
    m_version = rlp[0].toInt<u256>();
    m_type = rlp[1].toInt<u256>();
    m_time = rlp[2].toInt<u256>();
    m_gasPrice = rlp[3].toInt<u256>();
    m_gasLimit = rlp[4].toInt<u256>();
    printBlockHeader();
}

void BlockHeader::encode(RLPStream &stream)
{ //%cd018086016db58ef680648203e8
  //%cc0186016db591526b648203e8
    // RLPStream stream;
    stream.appendList(5);
    stream<<m_version<<m_type<<m_time<<m_gasPrice<<m_gasLimit;
    // return stream;
}






