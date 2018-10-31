#include "BlockStorage.h"
#include <toscore/storage/DBImpl.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/tss.hpp>

using namespace dev;
using namespace dev::sdag;

db::Slice dev::toSlice(h256 const& _h, unsigned _sub)
{
#if ALL_COMPILERS_ARE_CPP11_COMPLIANT
    static thread_local FixedHash<33> h = _h;
    h[32] = (uint8_t)_sub;
    return (db::Slice)h.ref();
#else
    static boost::thread_specific_ptr<FixedHash<33>> t_h;
    if (!t_h.get())
        t_h.reset(new FixedHash<33>);
    *t_h = FixedHash<33>(_h);
    (*t_h)[32] = (uint8_t)_sub;
    return (db::Slice)t_h->ref();
#endif //ALL_COMPILERS_ARE_CPP11_COMPLIANT
}

db::Slice dev::toSlice(uint64_t _n, unsigned _sub)
{
#if ALL_COMPILERS_ARE_CPP11_COMPLIANT
    static thread_local FixedHash<33> h;
    toBigEndian(_n, bytesRef(h.data() + 24, 8));
    h[32] = (uint8_t)_sub;
    return (db::Slice)h.ref();
#else
    static boost::thread_specific_ptr<FixedHash<33>> t_h;
    if (!t_h.get())
        t_h.reset(new FixedHash<33>);
    
    bytesRef ref(t_h->data() + 24, 8);
    toBigEndian(_n, ref);
    (*t_h)[32] = (uint8_t)_sub;
    return (db::Slice)t_h->ref();
#endif
}

BlockStorage::BlockStorage(const std::string& path)
{
    m_blocksDB.reset(new db::DBImpl(path));
}

void BlockStorage::write(Block& block)
{
    std::unique_ptr<db::WriteBatchFace> blocksWriteBatch = m_blocksDB->createWriteBatch();
    bytes code = block.encode();
    blocksWriteBatch->insert(dev::toSlice(block.getHash()), db::Slice((char*)code.data(),code.size()));
    m_blocksDB->commit(std::move(blocksWriteBatch));
}


void BlockStorage::write(std::vector<Block*>& blocks)
{
    if (blocks.empty())
    {
        return ;
    }

    std::unique_ptr<db::WriteBatchFace> blocksWriteBatch = m_blocksDB->createWriteBatch();
    for (auto block : blocks)
    {
        bytes code = block->encode();
        blocksWriteBatch->insert(dev::toSlice(block->getHash()), db::Slice((char*)code.data(),code.size()));
    }
    m_blocksDB->commit(std::move(blocksWriteBatch));
}

std::string BlockStorage::read(db::Slice slice)
{
    return m_blocksDB->lookup(slice);
}

bool BlockStorage::exists(db::Slice slice)
{
    return m_blocksDB->exists(slice);
}

