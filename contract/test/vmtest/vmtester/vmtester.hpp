// tvmc: Ethereum Client-VM Connector API
// Copyright 2018 Pawel Bylica.
// Licensed under the MIT License. See the LICENSE file.

#pragma once

#include <tvmc/tvmc.h>

#include <gtest/gtest.h>

tvmc_instance* get_vm_instance();

class tvmc_vm_test : public ::testing::Test
{
protected:
    tvmc_instance* vm = nullptr;

    void SetUp() override
    {
        vm = get_vm_instance();
        ASSERT_NE(vm, nullptr);
    }
};
