#pragma once

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
/** @file KeyAux.cpp
 * @author Gav Wood <i@gavwood.com>
 * @date 2014
 * CLI module for key management.
 */

#include <toscore/common/CommonIO.h>
#include <toscore/utils/FileSystem.h>
#include <toscore/crypto/SHA3.h>
#include <account/KeyManager.h>
#include <libethcore/TransactionBase.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <chrono>
#include <fstream>
#include <iosfwd>
#include <thread>

using namespace std;
using namespace dev;
using namespace dev::eth;
using namespace boost::algorithm;

namespace baseservice
{
namespace account
{
class BadArgument: public Exception {};

class account
{
public:

	enum class return_code
	{
		success = 0,
		error = -1
	};

    account(){ m_toSign.creation = true; }

	KeyPair makeKey() const;

	//sign key file/uuid/Address和密码得到Secret key
	int getSecret(std::string const& signKey,std::string const& pass_word,Secret& secret_key);

	int create_wallet(std::string const& pass_word);

	int DecodeTx();

	//create key return uuid
	h128 create_key(string& s_name,string s_pass_word="",string s_hint="");

	//delete key from uuid/key name
	int delete_key(string& input);

	std::vector<h128> List();

	int recode(string& input,string& old_pw,string& new_pw,string& new_hint);

	//输入name 密码 密码提示 导入key文件
	int Import(string& file,string& name,string& pass_word,string hint="");

	string get_hint(Address& a);

	//uid/account/hex Address sting  name to Address
	int userToAddress(std::string const& input,Address& a);

	//get SecretStore
	SecretStore& secretStore();

	//HEX string/HEX string file name to bytes
	static bytes inputData(std::string const& input , bool* isFile = nullptr);

	int openWallet(string const& pass_word);
private:

	KDF kdf() const { return m_kdf == "pbkdf2" ? KDF::PBKDF2_SHA256 : KDF::Scrypt; }

	/// Where the keys are.
	KeyManager m_keyManager;

	/// Wallet passphrase stuff
	string m_masterPassword;
	strings m_unlocks;
	string m_lock;
	string m_lockHint;
	bool m_icap = true;

	/// Creating/importing
	string m_name;
	Address m_address;

	/// Inspecting
	bool m_showSecret = false;

	/// Importing
	strings m_inputs;

	/// Signing
	string m_signKey;
	TransactionSkeleton m_toSign;
	u256 m_forceNonce;

	string m_kdf = "scrypt";
	map<string, string> m_kdfParams;
};
}
}