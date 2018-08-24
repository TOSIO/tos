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
            while(forkBegin->m_type != BT_MAIN)
            {
                forkBegin = getMaxDifficultyBlock(*forkBegin);
            }
            BlockRef p1(&block);
            BlockRef p2(pre_main_chain);
            while(p1!=forkBegin)
            {
                p1->m_type = BT_MAIN;
                p1 = getMaxDifficultyBlock(*p1);
            }
            while(p2!=forkBegin)
            {
                p2->m_type = BT_MINER;
                p1 = getMaxDifficultyBlock(*p2);
            }
        }
        block.m_type = BT_MAIN;
        pre_main_chain.reset(&block);
    }
}

void MainChain::recursionCheck(Block &block,Block &confirmBlock)
{
    if(block.m_status&BS_REF||//如果区块未被确认或者
        (//确认它的主块现在成了挖矿区块
            (block.m_status&BS_CONFIRM||block.m_status&BS_APPLIED)&&getBlockFromPool(block.m_refMainBlock)&&
            getBlockFromPool(block.m_refMainBlock)->m_type==BT_MINER
        )
    )
    {
        for(auto blockLinkStruct:block.m_links)
        {
            recursionCheck(*getBlockFromPool(blockLinkStruct.blockHash),block.m_type==BT_MAIN?block:confirmBlock);
        }

        cnote<<"recursionCheck confirming"<<"block hash:"<<block.getHash();
        block.m_status = BS_CONFIRM;
        block.m_refMainBlock = confirmBlock.getHash();

        //addToMPT();
        //sdagHeight();

        if(block.m_type==BT_MAIN)
        {
            //addCommit();
        }
    }
    else if(block.m_status==BS_NONE)
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
    BlockRef pBeforBlock = pre_main_chain;
    for(int i = 0;i<CONFIRM_NEED_VERIFY_BLOCK_NUM;++i)
    {
        pBeforBlock = pCurrentBlock;
        pCurrentBlock = getMaxDifficultyBlock(*pCurrentBlock);
    }
    recursionCheck(*pCurrentBlock,*pBeforBlock);
}