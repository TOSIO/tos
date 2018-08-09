#include <stdio.h>
#include <toscore/common/Common.h>
#include <toscore/common/Address.h>
#include <boost/optional.hpp>

namespace DEV
{
namespace sdag
{

class TransactionBase
{
protected:
    /// Type of transaction.
    enum Type
    {
        NullTransaction,   ///< Null transaction.
        ContractCreation,  ///< Transaction to create contracts - receiveAddress() is ignored.
        MessageCall        ///< Transaction to invoke a message call - receiveAddress() is used.
    };

    Type m_type = NullTransaction;  ///< Is this a contract-creation transaction or a message-call
                                    ///< transaction?
    u256 m_nonce;                   ///< The transaction-count of the sender.
    u256 m_value;  ///< The amount of ETH to be transferred by this transaction. Called 'endowment'
                   ///< for contract-creation transactions.
    Address m_receiveAddress;  ///< The receiving address of the transaction.
    u256 m_gasPrice;           ///< The base fee and thus the implied exchange rate of ETH to GAS.
    u256 m_gas;  ///< The total gas to convert, paid for from sender's account. Any unused gas gets
                 ///< refunded once the contract is ended.
    bytes m_data;  ///< The data associated with the transaction, or the initialiser if it's a
                   ///< creation transaction.
    boost::optional<SignatureStruct> m_vrs;  ///< The signature of the transaction. Encodes the
                                             ///< sender.
    int m_chainId = -4;                      ///< EIP155 value for calculating transaction hash
                                             ///< https://github.com/ethereum/EIPs/issues/155

    mutable h256 m_hashWith;   ///< Cached hash of transaction with signature.
    mutable Address m_sender;  ///< Cached sender, determined from signature.



};

} // namespace sdag
} // namespace DEV
