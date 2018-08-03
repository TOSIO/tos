/* EVMC: Ethereum Client-VM Connector API.
 * Copyright 2018 Pawel Bylica.
 * Licensed under the MIT License. See the LICENSE file.
 */

#pragma once

#include <tvmc/tvmc.h>

/**
 * Creates EVMC Example VM.
 */
struct tvmc_instance* tvmc_create_examplevm(void);
