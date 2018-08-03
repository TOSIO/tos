// tvmc -- Ethereum Client-VM Connector API
// Copyright 2018 Pawel Bylica.
// Licensed under the MIT License. See the LICENSE file.

#include "vmtester.hpp"

#include <cstring>

// Compile time checks:

static_assert(sizeof(tvmc_uint256be) == 32, "tvmc_uint256be is too big");
static_assert(sizeof(tvmc_address) == 20, "tvmc_address is too big");
static_assert(sizeof(tvmc_result) == 64, "tvmc_result does not fit cache line");
static_assert(sizeof(tvmc_instance) <= 64, "tvmc_instance does not fit cache line");
static_assert(sizeof(tvmc_message) <= 18 * 8, "tvmc_message not optimally packed");
static_assert(offsetof(tvmc_message, code_hash) % 8 == 0, "tvmc_message.code_hash not aligned");

// Check enums match int size.
// On GCC/clang the underlying type should be unsigned int, on MSVC int
static_assert(
    sizeof(tvmc_call_kind) == sizeof(int), "Enum `tvmc_call_kind` is not the size of int");
static_assert(sizeof(tvmc_revision) == sizeof(int), "Enum `tvmc_revision` is not the size of int");

static constexpr size_t optionalDataSize =
    sizeof(tvmc_result) - offsetof(tvmc_result, create_address);
static_assert(optionalDataSize == sizeof(tvmc_result_optional_data), "");

TEST_F(tvmc_vm_test, abi_version_match)
{
    ASSERT_EQ(vm->abi_version, TVMC_ABI_VERSION);
}

TEST_F(tvmc_vm_test, set_option_unknown)
{
    if (vm->set_option)
    {
        int r = vm->set_option(vm, "unknown_option_csk9twq", "v");
        EXPECT_EQ(r, 0);
        r = vm->set_option(vm, "unknown_option_csk9twq", "x");
        EXPECT_EQ(r, 0);
    }
}

TEST_F(tvmc_vm_test, set_option_empty_value)
{
    if (vm->set_option)
    {
        int r = vm->set_option(vm, "unknown_option_csk9twq", nullptr);
        EXPECT_EQ(r, 0);
    }
}

TEST_F(tvmc_vm_test, name)
{
    ASSERT_NE(vm->name, nullptr);
    EXPECT_GT(std::strlen(vm->name), 0) << "VM name cannot be empty";
}

TEST_F(tvmc_vm_test, version)
{
    ASSERT_NE(vm->version, nullptr);
    EXPECT_GT(std::strlen(vm->version), 0) << "VM name cannot be empty";
}