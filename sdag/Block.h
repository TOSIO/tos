#pragma once

#include <stdio.h>
#include <string>

#include <toscore/common/Common.h>
#include <toscore/common/Address.h>
#include <boost/optional.hpp>
#include "BlockHeader.h"
#include <toscrypto/Common.h>
#include <toscore/utils/RLP.h>
#include <toscore/crypto/SHA3.h>
#include "POW.h"
// #include "Transaction.h"
namespace dev
{
namespace sdag
{


enum IncludeSignature
{
    WithoutSignature = 0, ///< Do not include a signature.
    WithSignature = 1,    ///< Do include a signature.
};

enum BlockType
{
    BT_MAIN         = 0x01,
    BT_MINER        = 0x02,
    BT_TRANSACTION  = 0x03
};

enum BlockStatus
{
/*     BS_MAIN         = 0x01, //main block confirmed
	BS_MAIN_CHAIN   = 0x02, //main block not confirmed
	BS_APPLIED      = 0x04, //被主块确认同时未产生冲突
	BS_MAIN_REF     = 0x08, //被主块确认
	BS_REF          = 0x10, //被验证
	BS_OURS         = 0x20   */

    BS_NONE       = 0x00, //未被引用
    BS_REF        = 0x01, //已被引用(验证)
    BS_CONFIRM    = 0x02, //已确认(已被验证且被确认)
    BS_APPLIED    = 0x04  //已应用(已被验证且被确认、无冲突)
};

struct OutputStruct
{
    Address addr;
    u256 amount;
};
// class Block;
struct BlockLinkStruct
{
    h256 blockHash; /// link block hash
    u256 gasUsed;
    // Block block;
};
class Block;
using BlockRef = std::shared_ptr<Block>;

class Block
{
  public:
    Block();

    Block(bytes byts);
    Block(bytesConstRef byts);
    
    BlockHeader m_blockHeader;

    
    std::vector<OutputStruct> m_outputs;
    std::vector<BlockLinkStruct> m_links;

    bytes m_payload;
    
    boost::optional<SignatureStruct> m_vrs;
    u256 m_nonce;


    //local var
    BlockType   type;
    BlockStatus status;
    sdag_diff_t m_sumDifficulty;
    BlockRef    refMainBlock;

    Address m_sender;

    // the block is either main block or transaction block
    //bool isMain(){ return m_outputs.emppty() && m_playload.empty(); }
    BlockType getType(){return type;}
   
    h256 getHash();

    void streamRLP(RLPStream &_s, IncludeSignature _sig = WithSignature) const;

    // /// @returns the RLP serialisation of this transaction.
    // bytes rlp(IncludeSignature _sig = WithSignature) const
    // {
    //     RLPStream s;
    //     streamRLP(s, _sig);
    //     return s.out();
    // }
    h256 sha3(RLPStream &_s, IncludeSignature _sig = WithSignature) const;
    void sign(Secret const &_priv, RLPStream &_s); ///< Sign the block.


    bytes encode();

    void decode(bytes byts);
    void decodeBlockWithoutRSV(RLP rlp);
// RLPStream m_rlp;
private:
    mutable h256 m_hash;
    

static bool isZeroSignature(u256 const& _r, u256 const& _s) { return !_r && !_s; }
};

} // namespace sdag
} // namespace dev
