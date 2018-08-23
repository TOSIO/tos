#include "MainChain.h"
#include <toscore/log/Log.h>
#include "POW.h"
#include <set>
#define CONFIRM_NEED_VERIFY_BLOCK_NUM 32
using namespace dev;
using namespace dev::sdag;
using namespace std;

#define m_links_is_order
#ifndef m_links_is_order
BlockRef MainChain::getMaxDifficultyBlock(Block &block)
{
    BlockRef pMaxDifficultyBlock;
    BlockRef pCurrentBlock;
    for( auto linkBlock : block.m_links)
    {
        pCurrentBlock = m_pool->get(linkBlock.blockHash);
        if(pMaxDifficultyBlock)
        {
            if(pCurrentBlock->m_sumDifficulty > pMaxDifficultyBlock->m_sumDifficulty)
            {
                pMaxDifficultyBlock = pCurrentBlock;
            }
        }
        else
        {
            pMaxDifficultyBlock = pCurrentBlock;
        }
    }
    return pMaxDifficultyBlock;
}
#else
BlockRef MainChain::getMaxDifficultyBlock(Block &block)
{
    return m_pool->get(block.m_links[0].blockHash);
}
#endif
BlockRef MainChain::getBlockFromPool(h256 &hash)
{
    return m_pool->get(hash);
}

void MainChain::addMinerBlock(Block &block)
{
    BlockRef pMaxDifficultyBlock = getMaxDifficultyBlock(block);
    block.m_sumDifficulty = calculateWork(block.getHash())+pMaxDifficultyBlock->m_sumDifficulty;
    if(block.m_sumDifficulty > pre_main_chain->m_sumDifficulty)
    {
        if(pMaxDifficultyBlock != pre_main_chain)
        {
            //已分叉
            BlockRef forkBegin(&block);
            while(forkBegin->type != BT_MAIN)
            {
                forkBegin = getMaxDifficultyBlock(*forkBegin);
            }
            BlockRef p1(&block);
            BlockRef p2(pre_main_chain);
            while(p1!=forkBegin)
            {
                p1->type = BT_MAIN;
                p1 = getMaxDifficultyBlock(*p1);
            }
            while(p2!=forkBegin)
            {
                p2->type = BT_MINER;
                p1 = getMaxDifficultyBlock(*p2);
            }
        }
        block.type = BT_MAIN;
        pre_main_chain.reset(&block);
    }
}

void MainChain::recursionCheck(Block &block,Block &confirmBlock)
{
    if(block.status==BS_REF||//如果区块未被确认或者
        (//确认它的主块现在成了挖矿区块
            (block.status==BS_CONFIRM||block.status==BS_APPLIED)&&
            block.refMainBlock->type==BT_MINER
        )
    )
    {
        cnote<<"recursionCheck confirming"<<"block hash:"<<block.getHash();
        block.status = BS_CONFIRM;
        block.refMainBlock.reset(&confirmBlock);
        //addToMPT();

        for(auto blockLinkStruct:block.m_links)
        {
            recursionCheck(*getBlockFromPool(blockLinkStruct.blockHash),block.type==BT_MAIN?block:confirmBlock);
        }
    }
    else if(block.status==BS_NONE)
    {
        cerror<<"error:recursionCheck block status==BS_NONE"<<"block hash:"<<block.getHash();
    }
    else
    {
        cnote<<"recursionCheck already confirm"<<"block hash:"<<block.getHash();
    }
}
void MainChain::check()
{
    BlockRef pCurrentBlock = pre_main_chain;
    for(int i = 0;i<CONFIRM_NEED_VERIFY_BLOCK_NUM;++i)
    {
        pCurrentBlock = getMaxDifficultyBlock(*pCurrentBlock);
    }
    recursionCheck(*pCurrentBlock,*pre_main_chain);
}