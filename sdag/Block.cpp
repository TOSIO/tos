#include "Block.h"
#include <toscore/log/Log.h>
// #include "BlockHeader.h"

using namespace dev;
using namespace dev::sdag;

Block::Block()
{
}

Block::Block(bytes byts)
{

	decode(byts);
}

Block::Block(bytesConstRef byts)
{

	// encode(byts);
}

void Block::streamRLP(RLPStream &_s, IncludeSignature _sig) const
{
	RLPStream headerStream;
	((BlockHeader)m_blockHeader).encode(headerStream);
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

h256 Block::sha3(RLPStream &_s, IncludeSignature _sig) const
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

void Block ::decodeBlockWithoutRSV(RLP rlp)
{
	BlockHeader tHeader(rlp[0]);
	m_blockHeader = tHeader;

	RLP outputs = rlp[1];
	cnote << "****outputs:" << outputs;
	if (outputs.isList())
	{
		cnote << "outputs.itemCount" << outputs[0].itemCount();
		for (size_t i = 0; i < outputs.itemCount(); i++)
		{
			RLP output = outputs[i];

			cnote << "outputs " + i << output;
			if (output.isList())
			{
				Address add = output[0].toHash<h160>();
				u256 amount = output[1].toInt<u256>();
				cnote << "output " << add << " + " << amount;
				OutputStruct out = {add, amount};
				m_outputs.push_back(out);
			}
		}
	}

	RLP links = rlp[2];
	cnote << "****links:" << links;
	if (links.isList())
	{
		cnote << "links.itemCount" << links[0].itemCount();
		for (size_t i = 0; i < links.itemCount(); i++)
		{
			RLP link = links[i];
			if (link.isList())
			{
				h256 blockHash = link[0].toHash<h256>();
				u256 gasUsed = link[1].toInt<u256>();
				cnote << "link " << blockHash << " + " << gasUsed;

				BlockLinkStruct link = {blockHash, gasUsed};
				m_links.push_back(link);
			}
		}
	}
	m_payload = rlp[3].toBytes();

	cnote << "m_outputs\n"
		  << m_outputs.size() << "\n"
		  << "m_links\n"
		  << m_links.size() << "\n"
		  << "m_payload\n"
		  << m_payload;
}

void Block ::decode(bytes byts)
{

	RLP rlp(byts);
	if (!rlp.isList())
	{
		cerror << "NOT INVILATE DATA";
		return;
	}
	cnote << "decode \n"
		  << rlp[0];
	// bytes bs = rlp[0];

	decodeBlockWithoutRSV(rlp[0]);

	int const v = rlp[1].toInt<int>();
	h256 const r = rlp[2].toInt<u256>();
	h256 const s = rlp[3].toInt<u256>();

	// if (isZeroSignature(r, s))
	// {
	m_vrs = SignatureStruct{r, s, static_cast<byte>(v)};
	// }
cnote << "decode RSV \n"
		  << r << "   " << s << "  " << v;
	m_nonce = rlp[4].toInt<u256>();
}
