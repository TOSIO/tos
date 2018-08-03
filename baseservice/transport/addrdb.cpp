// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <addrdb.h>

#include <addrman.h>
//#include <chainparams.h>
//#include <clientversion.h>
//#include <hash.h>
#include <random.h>
//#include <streams.h>
#include <tinyformat.h>
//#include <util.h>

#include <deps/streams.h>
#include <toscore/utils/FileSystem.h>
#include <toscore/common/CommonIO.h>
#include <toscore/crypto/Hash.h>

#include <utility>
#include <deps/clientversion.h>
//#include <chainparams_proxy.h>

using namespace dev;

namespace tos {

void Serialize(DataStream& stream,banmap_t& banSet)
{
    bytes start;
    start.emplace_back(0xf9);
    start.emplace_back(0xbe);
    start.emplace_back(0xb4);
    start.emplace_back(0xd9);
    
    DataStream rlpstream(stream.getType(),stream.getVersion());
    rlpstream.stream()->appendList(2);
    rlpstream.stream()->append(start);
    rlpstream.stream()->appendList(banSet.size());
    for (auto item : banSet)
    {
        rlpstream.stream()->appendList(2);
        CSubNet net = item.first;
        net.Serialize(rlpstream);
        item.second.Serialize(rlpstream);
    }
    bytes hashsource = rlpstream.stream()->out();

    h256 hash = dev::hash(&hashsource);

    stream.stream()->appendList(2);
    stream.stream()->append(hashsource);
    stream.stream()->append(hash);
}

/* void UnSerialize(bytesConstRef& stream,banmap_t& banSet,int type, int version,bool fCheckSum = true)
{
    RLP rlp(stream);
    if (!rlp.isList() || rlp.itemCount() != 3)
    {
        BOOST_THROW_EXCEPTION(RLPException() << errinfo_comment("Unexpected data format."));
    }

    bytes start = rlp[0].toBytes();
    for (auto item : rlp[1])
    {
        RLPs kv = item.toList();
        CSubNet key;
        CBanEntry value;
        bytesConstRef keyRef = kv[0].data();
        bytesConstRef valueRef = kv[1].data();
        key.UnSerialize(keyRef,type,version);
        value.UnSerialize(valueRef,type,version);
        banSet.insert(std::make_pair(key,value));
    }
    if (fCheckSum)
    {
        h256 decHash = rlp[2].toHash<h256>();
        bytes bans = rlp[1].toBytes();
        start.insert(start.end(),bans.begin(),bans.end());
        h256 streamHash = dev::hash(&start);
        if (decHash != streamHash)
        {
             BOOST_THROW_EXCEPTION(RLPException() << errinfo_comment("Checksum is inconsistent."));
        }
    }
} */

void UnSerializeInternal(bytesConstRef & stream, int type,int version,banmap_t& banSet)
{
    RLP rlp(stream);
    for (auto item : rlp)
    {
        RLPs kv = item.toList();
        CSubNet key;
        CBanEntry value;
        bytesConstRef keyRef = kv[0].data();
        bytesConstRef valueRef = kv[1].data();
        key.UnSerialize(keyRef,type,version);
        value.UnSerialize(valueRef,type,version);
        banSet.insert(std::make_pair(key,value));
    }
}

void UnSerializeInternal(bytesConstRef & stream, int type,int version,CAddrMan& addrman)
{
    printf("Trace | void UnSerializeInternal(bytesConstRef & stream, int type,int version,CAddrMan& addrman) enter\n");
    addrman.UnSerialize(stream,type,version);
    printf("Trace | void UnSerializeInternal(bytesConstRef & stream, int type,int version,CAddrMan& addrman) leave\n");
}

template<typename T>
void UnSerialize(DataStream& stream, T& data,bool fCheckSum = true)
{
    printf("Trace | template<typename T> void UnSerialize(DataStream& stream, T& data,bool fCheckSum = true) enter\n");
    RLP rlp(stream.stream()->out());
    if (!rlp.isList() || rlp.itemCount() != 2)
    {
        BOOST_THROW_EXCEPTION(RLPException() << errinfo_comment("Unexpected data format."));
    }

    bytes content = rlp[0].toBytes();
    RLP rlpContent(content);

    bytes start = rlpContent[0].toBytes();
/*     for (auto item : rlp[1])
    {
        RLPs kv = item.toList();
        CSubNet key;
        CBanEntry value;
        bytesConstRef keyRef = kv[0].data();
        bytesConstRef valueRef = kv[1].data();
        key.UnSerialize(keyRef,type,version);
        value.UnSerialize(valueRef,type,version);
        banSet.insert(std::make_pair(key,value));
    } */
    bytesConstRef encStream = rlpContent[1].data();
    UnSerializeInternal(encStream,stream.getType(),stream.getVersion(),data);

    if (fCheckSum)
    {
        h256 decHash = rlp[1].toHash<h256>();
        //bytesConstRef source = rlp[0].to();
        //start.insert(start.end(),bans.begin(),bans.end());
        h256 streamHash = dev::hash(&content);
        if (decHash != streamHash)
        {
             BOOST_THROW_EXCEPTION(RLPException() << errinfo_comment("Checksum is inconsistent."));
        } 
    }

    printf("Trace | template<typename T> void UnSerialize(DataStream& stream, T& data,bool fCheckSum = true) leave\n");
}


/* template <typename Stream, typename Data>
bool SerializeDB(Stream& stream, const Data& data)
{
    // Write and commit header, data
    try {
        CHashWriter hasher(SER_DISK, CLIENT_VERSION);
        stream << FLATDATA(Params().MessageStart()) << data;
        hasher << FLATDATA(Params().MessageStart()) << data;
        stream << hasher.GetHash();
    } catch (const std::exception& e) {
        return error("%s: Serialize or I/O error - %s", __func__, e.what());
    }

    return true;
} */

/* template <typename Data>
bool SerializeFileDB(const std::string& prefix, const fs::path& path, const Data& data)
{
    // Generate random temporary filename
    unsigned short randv = 0;
    GetRandBytes((unsigned char*)&randv, sizeof(randv));
    std::string tmpfn = strprintf("%s.%04x", prefix, randv);

    // open temp output file, and associate with CAutoFile
    fs::path pathTmp = GetDataDir() / tmpfn;
    FILE *file = fsbridge::fopen(pathTmp, "wb");
    CAutoFile fileout(file, SER_DISK, CLIENT_VERSION);
    if (fileout.IsNull())
        return error("%s: Failed to open file %s", __func__, pathTmp.string());

    // Serialize
    if (!SerializeDB(fileout, data)) return false;
    FileCommit(fileout.Get());
    fileout.fclose();

    // replace existing file, if any, with new file
    if (!RenameOver(pathTmp, path))
        return error("%s: Rename-into-place failed", __func__);

    return true;
}
 */

/* template <typename Stream, typename Data>
bool DeserializeDB(Stream& stream, Data& data, bool fCheckSum = true)
{
     try {
        CHashVerifier<Stream> verifier(&stream);
        // de-serialize file header (network specific magic number) and ..
        unsigned char pchMsgTmp[4];
        verifier >> FLATDATA(pchMsgTmp);
        // ... verify the network matches ours
        if (memcmp(pchMsgTmp, Params().MessageStart(), sizeof(pchMsgTmp)))
            return error("%s: Invalid network magic number", __func__);

        // de-serialize data
        verifier >> data;

        // verify checksum
        if (fCheckSum) {
            uint256 hashTmp;
            stream >> hashTmp;
            if (hashTmp != verifier.GetHash()) {
                return error("%s: Checksum mismatch, data corrupted", __func__);
            }
        }
    }
    catch (const std::exception& e) {
        return error("%s: Deserialize or I/O error - %s", __func__, e.what());
    } 

    return true;
}
 */
/* template <typename Data>
bool DeserializeFileDB(const fs::path& path, Data& data)
{
    // open input file, and associate with CAutoFile
    FILE *file = fsbridge::fopen(path, "rb");
    CAutoFile filein(file, SER_DISK, CLIENT_VERSION);
    if (filein.IsNull())
        return error("%s: Failed to open file %s", __func__, path.string());

    return DeserializeDB(filein, data);
} */

}

CBanDB::CBanDB()
{
    pathBanlist = dev::getDataDir() / "banlist.dat";
}

bool CBanDB::Write( banmap_t& banSet)
{
    //return SerializeFileDB("banlist", pathBanlist, banSet);
    try
    {
        DataStream stream(SER_DISK,CLIENT_VERSION);
        tos::Serialize(stream,banSet);
        dev::writeFile(pathBanlist, stream.stream()->out(),true);
    } 
    catch (const std::exception& e) 
    {
        return error("%s: Serialize or I/O error - %s", __func__, e.what());
    }
    return true;
}

bool CBanDB::Read(banmap_t& banSet)
{
    //return DeserializeFileDB(pathBanlist, banSet);
    try
    {
        bytes content = dev::contents(pathBanlist);
        bytesConstRef contentRef(content.data(),content.size());
        tos::UnSerializeInternal(contentRef,SER_DISK,CLIENT_VERSION,banSet);
    }
    catch (const std::exception& e) 
    {
        //return error("%s: Serialize or I/O error - %s", __func__, e.what());
        return false;
    }
    return true;
}

CAddrDB::CAddrDB()
{
    pathAddr = dev::getDataDir() / "peers.dat";
}

bool CAddrDB::Write(CAddrMan& addr)
{
    //return SerializeFileDB("peers", pathAddr, addr);
    try
    {
        DataStream stream(SER_DISK,CLIENT_VERSION);
        addr.Serialize(stream);
        dev::writeFile(pathAddr, stream.stream()->out(),true);
    } 
    catch (const std::exception& e) 
    {
        return error("%s: Serialize or I/O error - %s", __func__, e.what());
    }
    return true;
}

bool CAddrDB::Read(CAddrMan& addr)
{
    //return DeserializeFileDB(pathAddr, addr);
    try
    {
        bytes content = dev::contents(pathAddr);
        bytesConstRef contentRef(content.data(),content.size());
        addr.UnSerialize(contentRef,SER_DISK,CLIENT_VERSION);
    }
    catch (const std::exception& e) 
    {
        //return error("%s: Serialize or I/O error - %s", __func__, e.what());
        return false;
    }
    return true;
}

 bool CAddrDB::Read(CAddrMan& addr, DataStream& ssPeers)
{
/*     bool ret = DeserializeDB(ssPeers, addr, false);
    if (!ret) {
        // Ensure addrman is left in a clean state
        addr.Clear();
    } */
    try
    {
        printf("Trace |  bool CAddrDB::Read(CAddrMan& addr, DataStream& ssPeers) enter\n");
        tos::UnSerialize(ssPeers,addr);
        printf("Trace |  bool CAddrDB::Read(CAddrMan& addr, DataStream& ssPeers) leave\n");
    }
    catch (const std::exception& e) 
    {
        //return error("%s: Serialize or I/O error - %s", __func__, e.what());
        addr.Clear();
        return false;
    }
    return true;
}
