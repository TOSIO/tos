#include "Block.h"
#include <toscore/log/Log.h>
// #include "BlockHeader.h"

using namespace dev;
using namespace dev::sdag;


Block::Block(bytesConstRef byts)
{

	// encode(byts);

}

void Block::streamRLP(RLPStream &_s, IncludeSignature _sig) const
{
	RLPStream headerStream;
	((BlockHeader)m_blockHeader).encode(headerStream);
	_s.appendList(6);
	_s.appendRaw(headerStream.out());

	RLPStream outputsStream;
	outputsStream.appendList(m_outputs.size());
	for (unsigned int i = 0; i < m_outputs.size(); i++)
	{
		OutputStruct output = m_outputs[i];
		outputsStream << output.addr << output.amount;
		outputsStream.appendRaw(outputsStream.out());
		outputsStream.clear();
	}
	_s.appendRaw(outputsStream.out());

	RLPStream linksStream;
	linksStream.appendList(m_links.size());
	for (unsigned int i = 0; i < m_links.size(); i++)
	{
		BlockLinkStruct link = m_links[i];

		// RLPStream linkBlockStream;
		// link.block.streamRLP(linkBlockStream);
		// linksStream.appendRaw(linkBlockStream.out());
		linksStream << link.blockHash << link.gasUsed;
		linksStream.appendRaw(linksStream.out());
		linksStream.clear();
	}
	_s.appendRaw(linksStream.out());

	_s << m_playload;
}

void Block::sign(Secret const &_priv)
{
	auto sig = dev::sign(_priv, sha3(WithoutSignature));
	SignatureStruct sigStruct = *(SignatureStruct const *)&sig;
	if (sigStruct.isValid())
		m_vrs = sigStruct;
}

h256 Block::sha3(IncludeSignature _sig) const
{
	if (_sig == WithSignature && m_hash)
		return m_hash;

	RLPStream s;
	streamRLP(s, _sig);

	auto ret = dev::sha3(s.out());
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
