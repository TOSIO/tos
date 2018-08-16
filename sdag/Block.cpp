#include "Block.h"
#include <toscore/log/Log.h>
// #include "BlockHeader.h"

using namespace dev;
using namespace dev::sdag;


Block::Block()
{

}

Block(bytes byts)
{


	
}


Block::Block(bytesConstRef byts)
{

	// encode(byts);

}

void Block::streamRLP(RLPStream &_s, IncludeSignature _sig) const
{
	RLPStream headerStream;
	((BlockHeader)m_blocbyteskHeader).encode(headerStream);
	_s.appendList(4);
	_s.appendRaw(headerStream.out());

	RLPStream outputsStream;
	outputsStream.appendList(m_outputs.size());
	for (unsigned int i = 0; i < m_outputs.size(); i++)
	{
		OutputStruct output = m_outputs[i];
		RLPStream outputS;
		outputS.appendList(2);
		outputS << output.addr << output.amount;
		outputsStream.appendRaw(outputS.out());
		cnote << i << " output:" << toHex(outputS.out());
		outputS.clear();
	}
	_s.appendRaw(outputsStream.out());
	cnote << "outputsStream:" << toHex(outputsStream.out());
	RLPStream linksStream;
	linksStream.appendList(m_links.size());
	for (unsigned int i = 0; i < m_links.size(); i++)
	{
		BlockLinkStruct link = m_links[i];
		RLPStream linkS;
		linkS.appendList(2);
		// RLPStream linkBlockStream;
		// link.block.streamRLP(linkBlockStream);
		// linksStream.appendRaw(linkBlockStream.out());
		linkS << link.blockHash << link.gasUsed;
		linksStream.appendRaw(linkS.out());
		cnote << i << " linkS:" << toHex(linkS.out());
		linkS.clear();
	}
	cnote << "linksStream:" << toHex(linksStream.out());
	_s.appendRaw(linksStream.out());
	_s << m_payload;

	cnote << "Block::streamRLP:" << toHex(_s.out());
}

void Block::sign(Secret const &_priv, RLPStream &_s)
{

	cnote << " Block::sign" << _priv;
	auto sig = dev::sign(_priv, sha3(_s, WithSignature));
	SignatureStruct sigStruct = *(SignatureStruct const *)&sig;
	if (sigStruct.isValid())
		m_vrs = sigStruct;
}

h256 Block::sha3( RLPStream &_s, IncludeSignature _sig) const
{
	if (_sig == WithSignature && m_hash)
		return m_hash;
	auto ret = dev::sha3(_s.out());
	if (_sig == WithSignature)
		m_hash = ret;
	return ret;
}

bytes Block ::encode()
{
	RLPStream bStream;
	streamRLP(bStream);
	RLPStream _s;
	_s.appendList(5);
	_s.appendRaw(bStream.out());
	_s << m_vrs->v;
	_s << (u256)m_vrs->r << (u256)m_vrs->s;
	_s << m_nonce;
	return _s.out();
}

void Block ::decodeBlockWithoutRSV(bytes bs)
{
	// BlockHeader tHeader(bs[0]);
	// m_blockHeader = tHeader;
	// RLP outPuts(bs[1]);
	// RLP links(bs[2]);


	// RLP payLoad(bs[3]);

	// m_payload = payLoad


}

void Block ::decode(bytesConstRef byts)
{

	RLP rlp(byts);
	if (!rlp.isList())
	{
		cerror << "NOT INVILATE DATA";
		return;
	}

	bytes bs = rlp[0].toBytes();

	decodeBlockWithoutRSV(bs);

	int const v = rlp[1].toInt<int>();
	h256 const r = rlp[2].toInt<u256>();
	h256 const s = rlp[3].toInt<u256>();

	// if (isZeroSignature(r, s))
	// {
	m_vrs = SignatureStruct{r, s, static_cast<byte>(v)};
	// }

	m_nonce = rlp[4].toInt<u256>();
}
