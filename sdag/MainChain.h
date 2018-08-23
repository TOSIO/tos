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