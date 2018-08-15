#include "Block.h"
// #include "BlockHeader.h"

using namespace dev;
using namespace dev::sdag;

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

void Block ::encode()
{
	RLPStream bStream;
	streamRLP(bStream);
	RLPStream _s;
	_s.appendList(5);
	_s.appendRaw(bStream.out());
	_s << m_vrs->v;
	_s << (u256)m_vrs->r << (u256)m_vrs->s;
	_s << m_nonce;
}

void Block ::decode(RLPStream &rlpStream)
{

	RLP rlp(rlpStream.out());
	if (!rlp.isList())
	{
		// cerr << "NOT INVILATE DATA";
	}
}
