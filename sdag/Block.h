#include <stdio.h>
#include <string>
#include <toscore/common/Common.h>
#include <toscore/common/Address.h>
#include <boost/optional.hpp>
#include "BlockHeader.h"

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

struct BlockLinkStruct
{
    h256 blockHash; /// link block hash
    u256 gas;
    Block *block;
};

class Block
{
  public:
    BlockHeader blockHeader;
    std::vector<OutputStruct> outputs;
    std::vector<BlockLinkStruct> links;

    bytes playload;
    boost::optional<SignatureStruct> vrs;
    u256 nonce;

    Address sender;

    /// Constructs a null transaction.
    Block(){};

    void sign(Secret const &_priv); ///< Sign the block.

    h256 getHash();

    void streamRLP(RLPStream &_s, IncludeSignature _sig = WithSignature, bool _forEip155hash = false) const;

    /// @returns the RLP serialisation of this transaction.
    bytes rlp(IncludeSignature _sig = WithSignature) const
    {
        RLPStream s;
        streamRLP(s, _sig);
        return s.out();
    }
};
} // namespace sdag
} // namespace dev
