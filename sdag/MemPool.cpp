#include "MemPool.h"
#include <toscore/concurrent/Guards.h>

using namespace dev::sdag;

void BlockMemPool::add(BlockRef block)
{
    Guard guard(_lock);

    for (auto blk : block->m_links)
    {
        if (m_orphanBlocks.find(blk.blockHash) != m_orphanBlocks.end() // the block's direct link is in orphan set.
            || m_blocks.find(blk.blockHash) == m_blocks.end()          // the block's direct link is not in non-orphan set.
            || !m_blockStorage->exists(dev::toSlice(blk.blockHash)))
        {
            // the block's ancestor are orphan block
            m_orphanBlocks.insert(make_pair(block->getHash(), block));
            // request ancestor block....
            return;
        }
    }

    /* if (block->status & BS_MAIN 
        || block->status & BS_APPLIED
        || block->status & BS_MAIN_REF)
    {
       _confirmBlocks[block->getHash()] = block;
    }
    else if (block->status & BS_MAIN_CHAIN 
        || block->status & BS_REF)
    {
        _unconfirmBlocks[block->getHash()] = block;
    }
    else
    {
        for (auto blk : block->m_links)
        {
            //if ()
        }
    } */
    m_blocks[block->getHash()] = block;
}

BlockRef BlockMemPool::get(h256 hash)
{
    Guard guard(_lock);
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
            m_blocks[hash] = ret;
            return ret;
        }
    }
}