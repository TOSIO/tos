#include "MemPool.h"
#include <toscore/concurrent/Guards.h>

using namespace dev::sdag;

bool BlockMemPool::isOrphan(h256 hash)
{
    ReadGuard rguard(m_lock);
    if (m_orphanBlocks.find(hash) != m_orphanBlocks.end() // the block's direct link is in orphan set.
        || m_blocks.find(hash) == m_blocks.end()          // the block's direct link is not in non-orphan set.
        || !m_blockStorage->exists(dev::toSlice(hash)))
    {
        return true;
    }
    return false;
}

void BlockMemPool::add(BlockRef block)
{
    if (!block.get())
    {
        return;
    }
    
    UpgradableGuard rguard(m_lock);
    for (auto blk : block->m_links)
    {
        if (isOrphan(blk.blockHash))
        {
            // the block's ancestors are orphan block
            UpgradeGuard wguard(rguard);
            m_orphanBlocks.insert(make_pair(block->getHash(), block));
            // request ancestor block....
            return;
        }
    }
    
    UpgradeGuard wguard(rguard);
    m_blocks[block->getHash()] = block;
    m_blockStorage->write(*block);
}

BlockRef BlockMemPool::get(h256 hash)
{
    UpgradableGuard rguard(m_lock);
    BlockMapIterator itr = m_blocks.find(hash);

    if (itr != m_blocks.end())
    {
        return itr->second;
    }
    else if ((itr = m_orphanBlocks.find(hash)) != m_orphanBlocks.end())
    {
        return itr->second;
    }
    else
    {
        // try to load block form local storage(database or local file)
        std::string enc = m_blockStorage->read(dev::toSlice(hash));
        if (enc.empty())
        {
            return BlockRef(nullptr);
        }
        else
        {
            BlockRef ret(new Block(bytesConstRef((unsigned char *)enc.data(), enc.size())));
            UpgradeGuard wguard(rguard);
            m_blocks[hash] = ret;
            return ret;
        }
    }
}