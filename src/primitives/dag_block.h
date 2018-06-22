//
// Created by tony on 2018/6/22.
//

#ifndef TOSCHAIN_DAG_BLOCK_H
#define TOSCHAIN_DAG_BLOCK_H

#include <stdint.h>

#define DAG_BLOCK_FIELDS 16

typedef uint64_t dag_time_t;
typedef uint64_t dag_amount_t;
typedef uint64_t dag_hash_t[4];
typedef uint64_t dag_hashlow_t[3];

struct dag_field {
    union {
        struct {
            union {
                struct {
                    uint64_t transport_header;
                    uint64_t type;
                    dag_time_t time;
                };
                dag_hashlow_t hash;
            };
            union {
                dag_amount_t amount;
                dag_time_t end_time;
            };
        };
        dag_hash_t data;
    };
};



class dag_block {
    struct dag_field field[DAG_BLOCK_FIELDS];
};


#endif //TOSCHAIN_DAG_BLOCK_H
