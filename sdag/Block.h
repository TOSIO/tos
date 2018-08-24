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
    BT_MINER = 0x00,
    BT_MAIN = 0x01,
    BT_TRANSACTION = 0x02
};

enum BlockStatus
{
    /*     BS_MAIN         = 0x01, //main block confirmed
	BS_MAIN_CHAIN   = 0x02, //main block not confirmed
	BS_APPLIED      = 0x04, //被主块确认同时未产生冲突
	BS_MAIN_REF     = 0x08, //被主块确认
	BS_REF          = 0x10, //被验证
	BS_OURS         = 0x20   */

    BS_NONE = 0x00,    //未被引用
    BS_REF = 0x01,     //已被引用(验证)
    BS_CONFIRM = 0x02, //已确认(已被验证且被确认)
    BS_APPLIED = 0x04  //已应用(已被验证且被确认、无冲突)
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
    BlockType   m_type;
    BlockStatus m_status;
    sdag_diff_t m_sumDifficulty;
    BlockRef    refMainBlock;

    // the block is either main block or transaction block
    //bool isMain(){ return m_outputs.emppty() && m_playload.empty(); }
    BlockType getType(){return m_type;}
    
    // int64_t getTime(){return m_blockHeader.m_time;}
    
    h256 getHash();

    void streamRLP(RLPStream &_s, IncludeSignature _sig = WithSignature);

    void sign(Secret const &_priv); ///< Sign the block.

    bytes encode(); //block rlp value

    void decode(bytes byts);

    /// Synonym for safeSender().
    Address from() { return safeSender(); }

    u256 getGasPrice() { return m_blockHeader.getGasPrice(); }
    u256 getGasLimit() { return m_blockHeader.getGasLimit(); }
    int64_t getTime() { return m_blockHeader.getTime(); }
  private:
    mutable Address m_sender;
    RLPStream m_unSignStream;
    mutable h256 m_hash;
    bytes m_rlpData;
    void decodeBlockWithoutRSV(RLP rlp);
    h256 sha3(IncludeSignature _sig = WithSignature);

    static bool isZeroSignature(u256 const &_r, u256 const &_s) { return !_r && !_s; }
    /// @returns true if the transaction was signed
    bool hasSignature() const { return m_vrs.is_initialized(); }

    /// @returns true if the transaction was signed with zero signature
    bool hasZeroSignature() const { return m_vrs && isZeroSignature(m_vrs->r, m_vrs->s); }

    RLPStream getStreamWithoutRSV();

    Address const &safeSender();

    Address const &sender();

    bool isLinksVilidate();

};

} // namespace sdag
} // namespace dev
