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

BlockHeader::BlockHeader()
{

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
    RLP rlp(stream.out());
    if(!rlp.isList())
    {
        cerror << "NOT INVILATE DATA";
        BOOST_THROW_EXCEPTION(BadCast());
    }
    
    m_type = rlp[0].toInt<u256>();
    m_time = rlp[1].toPositiveInt64();
    m_gasPrice = rlp[2].toInt<u256>();
    m_gasLimit = rlp[3].toInt<u256>();
    printBlockHeader();
}

BlockHeader::BlockHeader(RLP rlp)
{
    if(!rlp.isList())
    {
        cerror << "NOT INVILATE DATA";
        BOOST_THROW_EXCEPTION(BadCast());
    }

    m_type = rlp[0].toInt<u256>();
    m_time = rlp[1].toPositiveInt64();
    m_gasPrice = rlp[2].toInt<u256>();
    m_gasLimit = rlp[3].toInt<u256>();
    printBlockHeader();

}

void BlockHeader::encode(RLPStream &stream)
{
    stream.appendList(4);
    stream<<m_type<<bigint(m_time)<<m_gasPrice<<m_gasLimit;
}






