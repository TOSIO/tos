/*
    This file is part of cpp-ethereum.

    cpp-ethereum is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    cpp-ethereum is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file Exceptions.h
 * @author Gav Wood <i@gavwood.com>
 * @date 2014
 */

#pragma once

#include <crypto/FixedHash.h>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/exception/errinfo_api_function.hpp>
#include <boost/exception/exception.hpp>
#include <boost/exception/info.hpp>
#include <boost/exception/info_tuple.hpp>
#include <boost/throw_exception.hpp>
#include <boost/tuple/tuple.hpp>
#include <exception>
#include <string>

namespace dev
{
/// Base class for all exceptions.
struct Exception : virtual std::exception, virtual boost::exception
{
    const char* what() const noexcept override { return boost::diagnostic_information_what(*this); }
};

#define DEV_SIMPLE_EXCEPTION(X)  \
    struct X : virtual Exception \
    {                            \
    }

/// Base class for all RLP exceptions.
struct RLPException : virtual Exception
{
};
#define DEV_SIMPLE_EXCEPTION_RLP(X) \
    struct X : virtual RLPException \
    {                               \
    }

DEV_SIMPLE_EXCEPTION_RLP(BadCast);
DEV_SIMPLE_EXCEPTION_RLP(BadRLP);
DEV_SIMPLE_EXCEPTION_RLP(OversizeRLP);
DEV_SIMPLE_EXCEPTION_RLP(UndersizeRLP);

DEV_SIMPLE_EXCEPTION(BadHexCharacter);
DEV_SIMPLE_EXCEPTION(NoNetworking);
DEV_SIMPLE_EXCEPTION(NoUPnPDevice);
DEV_SIMPLE_EXCEPTION(RootNotFound);
DEV_SIMPLE_EXCEPTION(BadRoot);
DEV_SIMPLE_EXCEPTION(FileError);
DEV_SIMPLE_EXCEPTION(Overflow);
DEV_SIMPLE_EXCEPTION(FailedInvariant);
DEV_SIMPLE_EXCEPTION(ValueTooLarge);
DEV_SIMPLE_EXCEPTION(UnknownField);
DEV_SIMPLE_EXCEPTION(MissingField);
DEV_SIMPLE_EXCEPTION(WrongFieldType);
DEV_SIMPLE_EXCEPTION(InterfaceNotSupported);
DEV_SIMPLE_EXCEPTION(ExternalFunctionFailure);

// error information to be added to exceptions
using errinfo_invalidSymbol = boost::error_info<struct tag_invalidSymbol, char>;
using errinfo_wrongAddress = boost::error_info<struct tag_address, std::string>;
using errinfo_comment = boost::error_info<struct tag_comment, std::string>;
using errinfo_required = boost::error_info<struct tag_required, bigint>;
using errinfo_got = boost::error_info<struct tag_got, bigint>;
using errinfo_min = boost::error_info<struct tag_min, bigint>;
using errinfo_max = boost::error_info<struct tag_max, bigint>;
using RequirementError = boost::tuple<errinfo_required, errinfo_got>;
using RequirementErrorComment = boost::tuple<errinfo_required, errinfo_got, errinfo_comment>;
using errinfo_hash256 = boost::error_info<struct tag_hash, h256>;
using errinfo_required_h256 = boost::error_info<struct tag_required_h256, h256>;
using errinfo_got_h256 = boost::error_info<struct tag_get_h256, h256>;
using Hash256RequirementError = boost::tuple<errinfo_required_h256, errinfo_got_h256>;
using errinfo_extraData = boost::error_info<struct tag_extraData, bytes>;
using errinfo_externalFunction = boost::errinfo_api_function;
using errinfo_interface = boost::error_info<struct tag_interface, std::string>;
using errinfo_path = boost::error_info<struct tag_path, std::string>;

namespace eth
{

// information to add to exceptions
using errinfo_name = boost::error_info<struct tag_field, std::string>;
using errinfo_field = boost::error_info<struct tag_field, int>;
using errinfo_data = boost::error_info<struct tag_data, std::string>;
using errinfo_nonce = boost::error_info<struct tag_nonce, h64>;
using errinfo_difficulty = boost::error_info<struct tag_difficulty, u256>;
using errinfo_target = boost::error_info<struct tag_target, h256>;
using errinfo_seedHash = boost::error_info<struct tag_seedHash, h256>;
using errinfo_mixHash = boost::error_info<struct tag_mixHash, h256>;
using errinfo_ethashResult = boost::error_info<struct tag_ethashResult, std::tuple<h256, h256>>;
using BadFieldError = boost::tuple<errinfo_field, errinfo_data>;

DEV_SIMPLE_EXCEPTION(OutOfGasBase);
DEV_SIMPLE_EXCEPTION(OutOfGasIntrinsic);
DEV_SIMPLE_EXCEPTION(NotEnoughAvailableSpace);
DEV_SIMPLE_EXCEPTION(NotEnoughCash);
DEV_SIMPLE_EXCEPTION(GasPriceTooLow);
DEV_SIMPLE_EXCEPTION(BlockGasLimitReached);
DEV_SIMPLE_EXCEPTION(FeeTooSmall);
DEV_SIMPLE_EXCEPTION(TooMuchGasUsed);
DEV_SIMPLE_EXCEPTION(ExtraDataTooBig);
DEV_SIMPLE_EXCEPTION(ExtraDataIncorrect);
DEV_SIMPLE_EXCEPTION(TransactionIsUnsigned);
DEV_SIMPLE_EXCEPTION(InvalidSignature);
DEV_SIMPLE_EXCEPTION(InvalidTransactionFormat);
DEV_SIMPLE_EXCEPTION(InvalidBlockFormat);
DEV_SIMPLE_EXCEPTION(InvalidUnclesHash);
DEV_SIMPLE_EXCEPTION(TooManyUncles);
DEV_SIMPLE_EXCEPTION(UncleTooOld);
DEV_SIMPLE_EXCEPTION(UncleIsBrother);
DEV_SIMPLE_EXCEPTION(UncleInChain);
DEV_SIMPLE_EXCEPTION(UncleParentNotInChain);
DEV_SIMPLE_EXCEPTION(InvalidStateRoot);
DEV_SIMPLE_EXCEPTION(InvalidGasUsed);
DEV_SIMPLE_EXCEPTION(InvalidTransactionsRoot);
DEV_SIMPLE_EXCEPTION(InvalidDifficulty);
DEV_SIMPLE_EXCEPTION(InvalidGasLimit);
DEV_SIMPLE_EXCEPTION(InvalidReceiptsStateRoot);
DEV_SIMPLE_EXCEPTION(InvalidTimestamp);
DEV_SIMPLE_EXCEPTION(InvalidLogBloom);
DEV_SIMPLE_EXCEPTION(InvalidNonce);
DEV_SIMPLE_EXCEPTION(InvalidBlockHeaderItemCount);
DEV_SIMPLE_EXCEPTION(InvalidBlockNonce);
DEV_SIMPLE_EXCEPTION(InvalidParentHash);
DEV_SIMPLE_EXCEPTION(InvalidUncleParentHash);
DEV_SIMPLE_EXCEPTION(InvalidNumber);
DEV_SIMPLE_EXCEPTION(InvalidZeroSignatureTransaction);
DEV_SIMPLE_EXCEPTION(InvalidTransactionReceiptFormat);
DEV_SIMPLE_EXCEPTION(TransactionReceiptVersionError);
DEV_SIMPLE_EXCEPTION(BlockNotFound);
DEV_SIMPLE_EXCEPTION(UnknownParent);
DEV_SIMPLE_EXCEPTION(AddressAlreadyUsed);

DEV_SIMPLE_EXCEPTION(DatabaseAlreadyOpen);
DEV_SIMPLE_EXCEPTION(DAGCreationFailure);
DEV_SIMPLE_EXCEPTION(DAGComputeFailure);

DEV_SIMPLE_EXCEPTION(UnsupportedSnapshotManifestVersion);
DEV_SIMPLE_EXCEPTION(InvalidSnapshotManifest);
DEV_SIMPLE_EXCEPTION(StateTrieReconstructionFailed);
DEV_SIMPLE_EXCEPTION(InvalidStateChunkData);
DEV_SIMPLE_EXCEPTION(InvalidBlockChunkData);
DEV_SIMPLE_EXCEPTION(AccountAlreadyImported);
DEV_SIMPLE_EXCEPTION(InvalidWarpStatusPacket);
DEV_SIMPLE_EXCEPTION(FailedToDownloadManifest);
DEV_SIMPLE_EXCEPTION(FailedToDownloadDaoForkBlockHeader);
}

}
