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

#include <toscore/utils/FileSystem.h>
#include <toscore/common/CommonIO.h>

using namespace dev;

namespace {

bool Serialize(RLPStream& stream,banmap_t& banSet)
{
    bytes start;
    stream.appendList(3);
    stream.appendRaw(start);
    stream.appendList(banSet.size());
    for (auto item : banSet)
    {
        stream.appendList(2);
        stream << item;
    }
    h256 hash = sha256(stream.out());
    stream.append(hash);
}

bool Deserialize(bytesConstRef bytes,banmap_t& banSet)
{
    RLP rlp(bytes);
}

bool SerializeFileDB(const fs::path& path, const RLPStream& stream)
{
    try
    {
        dev::writeFile(path, stream.out(),true);
        fileout.write(stream.out());
    }
    catch (const std::exception& e) {
        return error("%s: Serialize or I/O error - %s", __func__, e.what());
    }
    return true;
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

bool CBanDB::Write(const banmap_t& banSet)
{
    //return SerializeFileDB("banlist", pathBanlist, banSet);
    RLPStream stream;
    if(!Serialize(stream,banSet) && !SerializeFileDB(pathBanlist,stream.out()))
    {
        return false;
    }
    return true;
}

bool CBanDB::Read(banmap_t& banSet)
{
    //return DeserializeFileDB(pathBanlist, banSet);
    bytes content = dev::contents(pathBanlist);
    return Deserialize(&contents,banSet);
}

CAddrDB::CAddrDB()
{
    pathAddr = dev::getDataDir() / "peers.dat";
}

bool CAddrDB::Write(const CAddrMan& addr)
{
    //return SerializeFileDB("peers", pathAddr, addr);
    RLPStream stream;
    if(!addr.Serialize(stream) && !SerializeFileDB(pathAddr,stream.out()))
    {
        return false;
    }
    return true;
}

bool CAddrDB::Read(CAddrMan& addr)
{
    //return DeserializeFileDB(pathAddr, addr);
    bytes content = dev::contents(pathAddr);
    return addr.Deserialize(&contents);
}

/* bool CAddrDB::Read(CAddrMan& addr, CDataStream& ssPeers)
{
    bool ret = DeserializeDB(ssPeers, addr, false);
    if (!ret) {
        // Ensure addrman is left in a clean state
        addr.Clear();
    }
    return ret;
} */
