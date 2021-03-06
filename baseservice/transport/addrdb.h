// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_ADDRDB_H
#define BITCOIN_ADDRDB_H

//#include <devcore/utils/serialize.h>

#include <string>
#include <map>
#include <boost/filesystem.hpp>
#include <deps/streams.h>


namespace fs = boost::filesystem;

class CSubNet;
class CAddrMan;
class CDataStream;

typedef enum BanReason
{
    BanReasonUnknown          = 0,
    BanReasonNodeMisbehaving  = 1,
    BanReasonManuallyAdded    = 2
} BanReason;

class CBanEntry
{
public:
    static const int CURRENT_VERSION=1;
    int nVersion;
    int64_t nCreateTime;
    int64_t nBanUntil;
    uint8_t banReason;

    CBanEntry()
    {
        SetNull();
    }

    explicit CBanEntry(int64_t nCreateTimeIn)
    {
        SetNull();
        nCreateTime = nCreateTimeIn;
    }

    //ADD_SERIALIZE_METHODS;

/*     template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(this->nVersion);
        READWRITE(nCreateTime);
        READWRITE(nBanUntil);
        READWRITE(banReason);
    } */

    void Serialize(DataStream& stream)
    {
        stream.stream()->appendList(4);
        *stream.stream() << this->nVersion<<bigint(nCreateTime)<<bigint(nBanUntil)<<banReason;
    }

    void UnSerialize(bytesConstRef& stream,int type, int version)
    {
        RLP rlp(stream);
        if (!rlp.isList() || rlp.itemCount() != 4)
        {
            BOOST_THROW_EXCEPTION(RLPException() << errinfo_comment("Unexpected data format."));
        }
        this->nVersion = rlp[0].toInt();
        nCreateTime = rlp[1].toInt<int64_t>();
        nBanUntil = rlp[2].toInt<int64_t>();
        banReason =  rlp[3].toInt<uint8_t>();
    }

    void SetNull()
    {
        nVersion = CBanEntry::CURRENT_VERSION;
        nCreateTime = 0;
        nBanUntil = 0;
        banReason = BanReasonUnknown;
    }

    std::string banReasonToString() const
    {
        switch (banReason) {
        case BanReasonNodeMisbehaving:
            return "node misbehaving";
        case BanReasonManuallyAdded:
            return "manually added";
        default:
            return "unknown";
        }
    }
};

typedef std::map<CSubNet, CBanEntry> banmap_t;

/** Access to the (IP) address database (peers.dat) */
class CAddrDB
{
private:
    fs::path pathAddr;
public:
    CAddrDB();
    bool Write(CAddrMan& addr);
    bool Read(CAddrMan& addr);
    static bool Read(CAddrMan& addr, DataStream& ssPeers); 
};

/** Access to the banlist database (banlist.dat) */
class CBanDB
{
private:
    fs::path pathBanlist;
public:
    CBanDB();
    bool Write( banmap_t& banSet);
    bool Read(banmap_t& banSet);
};

#endif // BITCOIN_ADDRDB_H
