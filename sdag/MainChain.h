#include "Block.h"
#include "MemPool.h"

namespace dev
{
namespace sdag
{
class MainChain {
    public:
    MainChain(BlockMemPool* pool):m_pool(pool){}
    void check();
    void addMinerBlock(Block &b);
    
    //return the last main block of the current active main chain
    BlockRef tip(){ return pre_main_chain; }


    private:
    BlockRef pre_main_chain;
    BlockRef confirmBlock;
    BlockRef getBlockFromPool(h256 &hash);
    void confirmTx();
    BlockRef getMaxDifficultyBlock(Block &block);
    void recursionCheck(Block &block,Block &confirmBlock);

    BlockMemPool* m_pool;


};
}
}