#include <stdio.h>
#include <string>
#define TOS_BLOCK_FIELDS 16

typedef uint64_t tos_time_t;
typedef uint64_t tos_amount_t;
typedef uint64_t tos_hash_t[4];
typedef uint64_t tos_hashlow_t[3];

struct BlockInternal
{
    /* data */
};

struct tos_field {
	union {
		struct {
			union {
				struct {
                    char16_t version;
					uint64_t transport_header;
					uint64_t type;
					tos_time_t time;
                    uint8_t gasLimit;
                    float gasPrice;
				};
				tos_hashlow_t hash;
			};
			union {
				tos_amount_t amount;
				tos_time_t end_time;
			};
		};
		tos_hash_t data;
	};
};

struct TOS_Block
{
    BlockInternal blockInternal;

    struct tos_field field[TOS_BLOCK_FIELDS];

};
