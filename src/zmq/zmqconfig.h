// Copyright (c) 2014-2017 The Bitcoin Core developers
// Copyright (c) 2017-2018 The Toschain Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef TOSCHAIN_ZMQ_ZMQCONFIG_H
#define TOSCHAIN_ZMQ_ZMQCONFIG_H

#if defined(HAVE_CONFIG_H)
#include <config/toschain-config.h>
#endif

#include <stdarg.h>
#include <string>

#if ENABLE_ZMQ
#include <zmq.h>
#endif

#include <primitives/block.h>
#include <primitives/transaction.h>

void zmqError(const char *str);

#endif // TOSCHAIN_ZMQ_ZMQCONFIG_H
