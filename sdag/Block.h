#include <stdio.h>
#include <string>
#include <toscore/common/Common.h>
#include <toscore/common/Address.h>
#include <boost/optional.hpp>
#include "BlockHeader.h"
#include <toscrypto/Common.h>
#include <toscore/utils/RLP.h>
#include <toscore/crypto/SHA3.h>
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

class Block
{
  public:
    BlockHeader m_blockHeader;

    
    std::vector<OutputStruct> m_outputs;
    std::vector<BlockLinkStruct> m_links;

    bytes m_playload;
    


    boost::optional<SignatureStruct> m_vrs;
    u256 m_nonce;
    /// Constructs a null transaction.
    // Block();


    

    Address m_sender;

    h256 getHash();

    void streamRLP(RLPStream &_s, IncludeSignature _sig = WithSignature) const;

    /// @returns the RLP serialisation of this transaction.
    bytes rlp(IncludeSignature _sig = WithSignature) const
    {
        RLPStream s;
        streamRLP(s, _sig);
        return s.out();
    }
    h256 sha3(IncludeSignature _sig = WithSignature) const;
    void sign(Secret const &_priv); ///< Sign the block.


    void encode();

    void decode(RLPStream &rlp);


private:
    mutable h256 m_hash;
    


};
} // namespace sdag
} // namespace dev
