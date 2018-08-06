#include <stdio.h>
#include <string>
#include <toscore/common/Common.h>
#include <toscore/common/Address.h>
#include <boost/optional.hpp>
// #include <toscore>

namespace dev
{
namespace sdag 
{

enum IncludeSignature
{
	WithoutSignature = 0,	///< Do not include a signature.
	WithSignature = 1,		///< Do include a signature.
};

struct OutputStruct
{
    Address addr; 
    u256 amount; 
};

struct BlockLinkStruct
{
    h256 blockHash;  /// link block hash
    u256 gas;  
    Block *block;
};

class Block 
{
public:
    u256 version;
    u256 type;
    u256 time;
    u256 gasPrice;
    u256 gasLimit;

    std::vector<OutputStruct> outputs;
    std::vector<StructBlockLink> links;

    bytes playload;
    boost::optional<SignatureStruct> vrs;
    u256 nonce; 

    Address sender;

    /// Constructs a null transaction.
	Block() {};

    void sign(Secret const& _priv);			///< Sign the block.

    h256 getHash();

    void streamRLP(RLPStream& _s, IncludeSignature _sig = WithSignature, bool _forEip155hash = false) const;

	/// @returns the RLP serialisation of this transaction.
	bytes rlp(IncludeSignature _sig = WithSignature) const { RLPStream s; streamRLP(s, _sig); return s.out(); }
};
}
}


// #define TOS_BLOCK_FIELDS 16

// typedef uint64_t tos_time_t;
// typedef uint64_t tos_amount_t;
// typedef uint64_t tos_hash_t[4];
// typedef uint64_t tos_hashlow_t[3];

// struct BlockInternal
// {
//     /* data */
// };

// struct tos_field {
// 	union {
// 		struct {
// 			union {
// 				struct {
//                     char16_t version;
// 					uint64_t transport_header;
// 					uint64_t type;
// 					tos_time_t time;
//                     uint8_t gasLimit;
//                     float gasPrice;
// 				};
// 				tos_hashlow_t hash;
// 			};
// 			union {
// 				tos_amount_t amount;
// 				tos_time_t end_time;
// 			};
// 		};
// 		tos_hash_t data;
// 	};
// };



// struct TOS_Block
// {
//     BlockInternal blockInternal;

//     struct tos_field field[TOS_BLOCK_FIELDS];

// };
