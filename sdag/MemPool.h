#pragma once
#include <Block.h>
#include <BlockStorage.h>
#include <toscore/concurrent/Guards.h>

#include <memory>
#include <map>

namespace dev
{
namespace sdag
{
using BlockMap = std::map<h256, BlockRef>;
using BlockMapIterator = std::map<h256, BlockRef>::iterator;
//using BlockMapConstIterator = std::map<h256,BlockRef>::const_iterator;

class BlockMemPool
{

  public:
    void add(BlockRef block);
    
    BlockRef get(h256 hash);
    
    bool isOrphan(h256 hash);

  private:
    BlockMap m_blocks;
    BlockMap m_orphanBlocks;
    boost::shared_mutex m_lock;

    BlockStorageRef m_blockStorage;

};
} // namespace sdag
} // namespace dev