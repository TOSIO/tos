#pragma once
#include <Block.h>
#include <BlockStorage.h>
#include <memory>
#include <map>
#include <mutex>

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

  private:
  

    BlockMap m_blocks;
    BlockMap m_orphanBlocks;
    std::mutex _lock;


    BlockStorageRef m_blockStorage;

};
} // namespace sdag
} // namespace dev