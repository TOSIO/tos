#include <tvmc/tvmc.h>

#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "examplevm/examplevm.h"


struct tvmc_uint256be balance(struct tvmc_context* context,
                              const struct tvmc_address* address)
{
    struct tvmc_uint256be ret = {.bytes = {1, 2, 3, 4}};
    return ret;
}

struct tvmc_address address(struct tvmc_context* context)
{
    struct tvmc_address ret = {.bytes = {1, 2, 3, 4}};
    return ret;
}

static void print_address(const struct tvmc_address* address)
{
    int i = 0;
    for (i = 0; i < sizeof(address->bytes); ++i)
        printf("%x", address->bytes[i] & 0xff);
}

static int account_exists(struct tvmc_context* context,
                          const struct tvmc_address* address) {
    printf("TVM-C: EXISTS @");
    print_address(address);
    printf("\n");
    return 0;
}

static void get_storage(struct tvmc_uint256be* result,
                        struct tvmc_context* context,
                        const struct tvmc_address* address,
                        const struct tvmc_uint256be* key)
{
    printf("TVM-C: SLOAD @");
    print_address(address);
    printf("\n");
}

static void set_storage(struct tvmc_context* context,
                        const struct tvmc_address* address,
                        const struct tvmc_uint256be* key,
                        const struct tvmc_uint256be* value)
{
    printf("TVM-C: SSTORE @");
    print_address(address);
    printf("\n");
}

static void get_balance(struct tvmc_uint256be* result,
                        struct tvmc_context* context,
                        const struct tvmc_address* address)
{
    printf("TVM-C: BALANCE @");
    print_address(address);
    printf("\n");
    *result = balance(context, address);
}

static size_t get_code_size(struct tvmc_context* context, const struct tvmc_address* address)
{
    printf("TVM-C: CODESIZE @");
    print_address(address);
    printf("\n");
    return 0;
}

static size_t copy_code(struct tvmc_context* context, const struct tvmc_address* address,
    size_t code_offset, uint8_t* buffer_data, size_t buffer_size)
{
    printf("TVM-C: COPYCODE @");
    print_address(address);
    printf("\n");
    return 0;
}

static void selfdestruct(struct tvmc_context* context,
                         const struct tvmc_address* address,
                         const struct tvmc_address* beneficiary)
{
    printf("TVM-C: SELFDESTRUCT ");
    print_address(address);
    printf(" -> ");
    print_address(beneficiary);
    printf("\n");
}

static void call(struct tvmc_result* result,
                 struct tvmc_context* context,
                 const struct tvmc_message* msg)
{
    printf("TVM-C: CALL (depth: %d)\n", msg->depth);
    result->status_code = TVMC_FAILURE;
}

static void get_tx_context(struct tvmc_tx_context* result, struct tvmc_context* context)
{

}

static void get_block_hash(struct tvmc_uint256be* result, struct tvmc_context* context,
                           int64_t number)
{

}

/// TVM log callback.
///
/// @note The `tvm_log` name is used to avoid conflict with `log()` C function.
static void tvm_log(struct tvmc_context* context, const struct tvmc_address* address,
                    const uint8_t* data, size_t data_size,
                    const struct tvmc_uint256be topics[], size_t topics_count)
{
    printf("TVM-C: LOG%d\n", (int)topics_count);
}

static const struct tvmc_context_fn_table ctx_fn_table = {
    account_exists,
    get_storage,
    set_storage,
    get_balance,
    get_code_size,
    copy_code,
    selfdestruct,
    call,
    get_tx_context,
    get_block_hash,
    tvm_log
};

/// Example how the API is supposed to be used.
int main(int argc, char *argv[]) {
    struct tvmc_instance* jit = tvmc_create_examplevm();
    if (jit->abi_version != TVMC_ABI_VERSION)
        return 1;  // Incompatible ABI version.

//    uint8_t const code[] = "Place some EVM bytecode here";
//	  uint8_t const code[] = "30600052596000f3";
//	  uint8_t const code[] = "30600052596000f3";
//	  uint8_t const code[] = "600160005401600055";
//    const size_t code_size = sizeof(code)-1;
    uint8_t *code=NULL;
    size_t code_size=0;
    struct tvmc_uint256be code_hash = {.bytes = {1, 2, 3,}};
    uint8_t const input[] = "Hello World!";
    struct tvmc_uint256be value = {{1, 0,}};
    struct tvmc_address addr = {{0, 1, 2,}};
    int64_t gas = 200000;

    struct tvmc_context ctx = {&ctx_fn_table};

    struct tvmc_message msg = {addr, addr, value, input, sizeof(input),
                              code_hash, gas, 0};
    if (argc<2)
    {
      printf("usage:%s [1-4]  \n",argv[0]);
      return 0;
    }

    switch (atoi(argv[1]))
    {
      case 0: printf("usage:%s [1-4]  \n",argv[0]);
              return 0;
      case 1: code =(uint8_t *) "";
              code_size=strlen((char *)code);
              break;
      case 2: code =(uint8_t *) "30600052596000f3";
              code_size=strlen((char *)code);
              break;
      case 3: code =(uint8_t *) "600160005401600055";
              code_size=strlen((char *)code);
              break;
      case 4: code =(uint8_t *) "6001600054016000551";
              code_size=strlen((char *)code);
      default:
            break;
    }

    struct tvmc_result result =
        jit->execute(jit, &ctx, TVMC_HOMESTEAD, &msg, code, code_size);

    printf("Execution result:\n");
    if (result.status_code != TVMC_SUCCESS) {
      printf("  EVM execution failure: %d\n", result.status_code);
    } else {
        printf("  Gas used: %" PRId64 "\n", gas - result.gas_left);
        printf("  Gas left: %" PRId64 "\n", result.gas_left);
        printf("  Output size: %zd\n", result.output_size);

        printf("  Output: ");
        size_t i = 0;
        for (i = 0; i < result.output_size; i++) {
            printf("%02x ", result.output_data[i]);
        }
        printf("\n");
    }

    if (result.release)
        result.release(&result);
    jit->destroy(jit);

    return 0;
}
