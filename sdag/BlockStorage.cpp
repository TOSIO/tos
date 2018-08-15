#include "BlockStorage.h"
#include <toscore/storage/DBImpl.h>

dev::sdag::BlockStorage(const std::string& path)
{
    m_blocksDB.reset(new db::DBImpl(path));
}


void dev::sdag::BlockStorage::write(const Block& block)
{
    std::unique_ptr<db::WriteBatchFace> blocksWriteBatch = m_blocksDB->createWriteBatch();
    bytes code = block.blockHeader.encode().out();
    blocksWriteBatch->insert(toSlice(block.getHash()), db::Slice(code));

    m_blocksDB->commit(std::move(blocksWriteBatch));
}

std::string dev::sdag::BlockStorage::read(db::Slice slice)
{
    return m_blocksDB->lookup(slice);
}