#pragma once
#include <baseservice/transport/net.h>
#include <deps/utilstrencodings.h>

using namespace tos::P2P;

class NodeMessageHandler : public NetEventsInterface
{
public:

 bool SendRejectsAndCheckIfBanned(CNode* pnode, CConnman* connman);

void  ProcessGetData(CNode* pfrom, const Consensus::Params& consensusParams, CConnman* connman, const std::atomic<bool>& interruptMsgProc)

    virtual bool ProcessMessages(CNode* pnode, std::atomic<bool>& interrupt) override;


    virtual bool SendMessages(CNode* pnode, std::atomic<bool>& interrupt) override
    {
        printf("Trace | bool NodeMessageHandler::SendMessages(CNode* pnode, std::atomic<bool>& interrupt) called.\n");
        return true;
    }
    virtual void InitializeNode(CNode* pnode) override
    {
        printf("Trace | bool NodeMessageHandler::InitializeNode(CNode* pnode) called.\n");
        return ;
    }
    virtual void FinalizeNode(NodeId id, bool& update_connection_time)override
    {
        printf("Trace | bool NodeMessageHandler::FinalizeNode(NodeId id, bool& update_connection_time) called.\n");
        return ;
    }

};
