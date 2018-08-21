#include "account.h"
using namespace baseservice::account;

KeyPair account::makeKey() const
{
    KeyPair k(Secret::random());
    while (m_icap && k.address()[0])
        k = KeyPair(Secret(sha3(k.secret().ref())));
    return k;
}

//sign key file/uuid/Address和密码得到Secret key
//
int account::getSecret(std::string const& signKey,std::string const& pass_word,Secret& secret_key)
{
    string json = contentsString(signKey);
    if (!json.empty())
        secret_key = Secret(secretStore().secret(secretStore().readKeyContent(json), [&](){ return pass_word; }));
    else
    {
        if (h128 u = fromUUID(signKey))
            secret_key= Secret(secretStore().secret(u, [&](){ return pass_word; }));
        Address a;
        try
        {
            a = toAddress(signKey);
        }
        catch (...)
        {
            for (Address const& aa: m_keyManager.accounts())
                if (m_keyManager.accountName(aa) == signKey)
                {
                    a = aa;
                    break;
                }
        }
        if (a)
            secret_key = m_keyManager.secret(a, [&](){ return pass_word; });
        else
            return -1;
    }
    return 0;
}

int account::create_wallet(std::string const& pass_word)
{
    m_masterPassword = pass_word;
    try
    {
        m_keyManager.create(m_masterPassword);
    }
    catch (Exception const& _e)
    {
        return -1;
    }
    return 0;
}

int account::DecodeTx()
{
    return 0;
}

//create key return uuid
h128 account::create_key(string& s_name,string s_pass_word,string s_hint)
{
    auto k = makeKey();
    h128 u = s_pass_word.empty() ? m_keyManager.import(k.secret(), m_name) : m_keyManager.import(k.secret(), s_name, s_pass_word, s_hint);
    return u;
}

//delete key from uuid/key name
int account::delete_key(string& input)
{
    Address a;
    if (!userToAddress(input,a))
    {
        m_keyManager.kill(a);
        return 0;
    }
    return -1;
}

std::vector<h128> account::List()
{
    return m_keyManager.store().keys();
}

int account::recode(string& input,string& old_pw,string& new_pw,string& new_hint)
{
    Address a;
    if (!userToAddress(input,a))
    {
        Secret s = m_keyManager.secret(a, [&](){ return old_pw;});
        if (!s)
        {
            return -1;
        }
        
        if (m_keyManager.recode(a, new_pw, new_hint, [&](){ return new_pw; }, kdf()))
            return 0;
        else
            return -1;;
    }
    else
        return -1;
}

//输入name 密码 密码提示 导入key文件
int account::Import(string& file,string& name,string& pass_word,string hint)
{
    h128 u = m_keyManager.store().importKey(file);
    bytesSec s = m_keyManager.store().secret(u, [&](){ return pass_word; });
    if (s.size() != 32)
    {
        return -1;
    }

    m_keyManager.importExisting(u, name, pass_word, hint);
    return 0;
}

string account::get_hint(Address& a)
{
    return m_keyManager.passwordHint(a);
}

//HEX string/HEX string file name to bytes
bytes account::inputData(std::string const& input , bool* isFile)
{
    bytes b = fromHex(input);
    if (isFile)
        *isFile = false;
    if (b.empty())
    {
        if (isFile)
            *isFile = true;
        std::string s = boost::trim_copy_if(contentsString(input), is_any_of(" \t\n"));
        b = fromHex(s);
        if (b.empty())
            b = asBytes(s);
    }
    return b;
}

int account::openWallet(string const& pass_word)
{
    m_masterPassword = pass_word;

    if (m_keyManager.load(m_masterPassword))
        return -1;
    return 0;
}

//uid/account/hex Address sting  name to Address
int account::userToAddress(std::string const& input,Address& a)
{
    if (h128 u = fromUUID(input))
    {	
        a = m_keyManager.address(u);
        return 0;
    }

    try
    {
        a = toAddress(input);
        return 0;
    }
    catch(...){}

    for (Address const& a_tmep: m_keyManager.accounts())
        if (m_keyManager.accountName(a) == input)
            a = a_tmep;
    return -1;
}

//get SecretStore
SecretStore& account::secretStore()
{
    return m_keyManager.store();
}
