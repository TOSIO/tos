#include "nodemsg_handler.h"

bool NodeMessageHandler::SendRejectsAndCheckIfBanned(CNode* pnode, CConnman* connman)
{
    AssertLockHeld(cs_main);
    CNodeState &state = *State(pnode->GetId());

    for (const CBlockReject& reject : state.rejects) {
        connman->PushMessage(pnode, CNetMsgMaker(INIT_PROTO_VERSION).Make(NetMsgType::REJECT, std::string(NetMsgType::BLOCK), reject.chRejectCode, reject.strRejectReason, reject.hashBlock));
    }
    state.rejects.clear();

    if (state.fShouldBan) {
        state.fShouldBan = false;
        if (pnode->fWhitelisted)
            LogPrintf("Warning: not punishing whitelisted peer %s!\n", pnode->addr.ToString());
        else if (pnode->m_manual_connection)
            LogPrintf("Warning: not punishing manually-connected peer %s!\n", pnode->addr.ToString());
        else {
            pnode->fDisconnect = true;
            if (pnode->addr.IsLocal())
                LogPrintf("Warning: not banning local peer %s!\n", pnode->addr.ToString());
            else
            {
                connman->Ban(pnode->addr, BanReasonNodeMisbehaving);
            }
        }
        return true;
    }
    return false;
}


void NodeMessageHandler::ProcessGetData(CNode* pfrom, const Consensus::Params& consensusParams, CConnman* connman, const std::atomic<bool>& interruptMsgProc)
{
    AssertLockNotHeld(cs_main);

    std::deque<CInv>::iterator it = pfrom->vRecvGetData.begin();
    std::vector<CInv> vNotFound;
    const CNetMsgMaker msgMaker(pfrom->GetSendVersion());
    {
        LOCK(cs_main);

        while (it != pfrom->vRecvGetData.end() && (it->type == MSG_TX || it->type == MSG_WITNESS_TX)) {
            if (interruptMsgProc)
                return;
            // Don't bother if send buffer is too full to respond anyway
            if (pfrom->fPauseSend)
                break;

            const CInv &inv = *it;
            it++;

            // Send stream from relay memory
            bool push = false;
            auto mi = mapRelay.find(inv.hash);
            int nSendFlags = (inv.type == MSG_TX ? SERIALIZE_TRANSACTION_NO_WITNESS : 0);
            if (mi != mapRelay.end()) {
                connman->PushMessage(pfrom, msgMaker.Make(nSendFlags, NetMsgType::TX, *mi->second));
                push = true;
            } else if (pfrom->timeLastMempoolReq) {
                auto txinfo = mempool.info(inv.hash);
                // To protect privacy, do not answer getdata using the mempool when
                // that TX couldn't have been INVed in reply to a MEMPOOL request.
                if (txinfo.tx && txinfo.nTime <= pfrom->timeLastMempoolReq) {
                    connman->PushMessage(pfrom, msgMaker.Make(nSendFlags, NetMsgType::TX, *txinfo.tx));
                    push = true;
                }
            }
            if (!push) {
                vNotFound.push_back(inv);
            }

            // Track requests for our stuff.
            GetMainSignals().Inventory(inv.hash);
        }
    } // release cs_main

    if (it != pfrom->vRecvGetData.end() && !pfrom->fPauseSend) {
        const CInv &inv = *it;
        if (inv.type == MSG_BLOCK || inv.type == MSG_FILTERED_BLOCK || inv.type == MSG_CMPCT_BLOCK || inv.type == MSG_WITNESS_BLOCK) {
            it++;
            //ProcessGetBlockData(pfrom, consensusParams, inv, connman, interruptMsgProc);
        }
    }

    pfrom->vRecvGetData.erase(pfrom->vRecvGetData.begin(), it);

    if (!vNotFound.empty()) {
        // Let the peer know that we didn't find what it asked for, so it doesn't
        // have to wait around forever. Currently only SPV clients actually care
        // about this message: it's needed when they are recursively walking the
        // dependencies of relevant unconfirmed transactions. SPV clients want to
        // do that because they want to know about (and store and rebroadcast and
        // risk analyze) the dependencies of transactions relevant to them, without
        // having to download the entire memory pool.
        connman->PushMessage(pfrom, msgMaker.Make(NetMsgType::NOTFOUND, vNotFound));
    }
}



bool NodeMessageHandler::ProcessMessages(CNode* pnode, std::atomic<bool>& interrupt) override
    {
        printf("Trace | bool NodeMessageHandler::ProcessMessages(CNode* pnode, std::atomic<bool>& interrupt) called.\n");
        
        const ChainParamsProxy& chainparams = Params();
    //
    // Message format
    //  (4) message start
    //  (12) command
    //  (4) size
    //  (4) checksum
    //  (x) data
    //
    bool fMoreWork = false;

    if (!pfrom->vRecvGetData.empty())
        ProcessGetData(pfrom, chainparams.GetConsensus(), connman, interruptMsgProc);

    if (pfrom->fDisconnect)
        return false;

    // this maintains the order of responses
    if (!pfrom->vRecvGetData.empty()) return true;

    // Don't bother if send buffer is too full to respond anyway
    if (pfrom->fPauseSend)
        return false;

    std::list<CNetMessage> msgs;
    {
        LOCK(pfrom->cs_vProcessMsg);
        if (pfrom->vProcessMsg.empty())
            return false;
        // Just take one message
        msgs.splice(msgs.begin(), pfrom->vProcessMsg, pfrom->vProcessMsg.begin());
        pfrom->nProcessQueueSize -= msgs.front().vRecv.size() + CMessageHeader::HEADER_SIZE;
        pfrom->fPauseRecv = pfrom->nProcessQueueSize > connman->GetReceiveFloodSize();
        fMoreWork = !pfrom->vProcessMsg.empty();
    }
    CNetMessage& msg(msgs.front());

    msg.SetVersion(pfrom->GetRecvVersion());
    // Scan for message start
    if (memcmp(msg.hdr.pchMessageStart, chainparams.MessageStart(), CMessageHeader::MESSAGE_START_SIZE) != 0) {
        LogPrint(BCLog::NET, "PROCESSMESSAGE: INVALID MESSAGESTART %s peer=%d\n", SanitizeString(msg.hdr.GetCommand()), pfrom->GetId());
        pfrom->fDisconnect = true;
        return false;
    }

    // Read header
    CMessageHeader& hdr = msg.hdr;
    if (!hdr.IsValid(chainparams.MessageStart()))
    {
        LogPrint(BCLog::NET, "PROCESSMESSAGE: ERRORS IN HEADER %s peer=%d\n", SanitizeString(hdr.GetCommand()), pfrom->GetId());
        return fMoreWork;
    }
    std::string strCommand = hdr.GetCommand();

    // Message size
    unsigned int nMessageSize = hdr.nMessageSize;

    // Checksum
    CDataStream& vRecv = msg.vRecv;
    const uint256& hash = msg.GetMessageHash();
    if (memcmp(hash.begin(), hdr.pchChecksum, CMessageHeader::CHECKSUM_SIZE) != 0)
    {
        LogPrint(BCLog::NET, "%s(%s, %u bytes): CHECKSUM ERROR expected %s was %s\n", __func__,
           SanitizeString(strCommand), nMessageSize,
           HexStr(hash.begin(), hash.begin()+CMessageHeader::CHECKSUM_SIZE),
           HexStr(hdr.pchChecksum, hdr.pchChecksum+CMessageHeader::CHECKSUM_SIZE));
        return fMoreWork;
    }

    // Process message
    bool fRet = false;
    try
    {
        /* fRet = ProcessMessage(pfrom, strCommand, vRecv, msg.nTime, chainparams, connman, interruptMsgProc);
        if (interruptMsgProc)
            return false; */
        if (!pfrom->vRecvGetData.empty())
            fMoreWork = true;
    }
    catch (const std::ios_base::failure& e)
    {
        connman->PushMessage(pfrom, CNetMsgMaker(INIT_PROTO_VERSION).Make(NetMsgType::REJECT, strCommand, REJECT_MALFORMED, std::string("error parsing message")));
        if (strstr(e.what(), "end of data"))
        {
            // Allow exceptions from under-length message on vRecv
            LogPrint(BCLog::NET, "%s(%s, %u bytes): Exception '%s' caught, normally caused by a message being shorter than its stated length\n", __func__, SanitizeString(strCommand), nMessageSize, e.what());
        }
        else if (strstr(e.what(), "size too large"))
        {
            // Allow exceptions from over-long size
            LogPrint(BCLog::NET, "%s(%s, %u bytes): Exception '%s' caught\n", __func__, SanitizeString(strCommand), nMessageSize, e.what());
        }
        else if (strstr(e.what(), "non-canonical ReadCompactSize()"))
        {
            // Allow exceptions from non-canonical encoding
            LogPrint(BCLog::NET, "%s(%s, %u bytes): Exception '%s' caught\n", __func__, SanitizeString(strCommand), nMessageSize, e.what());
        }
        else
        {
            PrintExceptionContinue(&e, "ProcessMessages()");
        }
    }
    catch (const std::exception& e) {
        PrintExceptionContinue(&e, "ProcessMessages()");
    } catch (...) {
        PrintExceptionContinue(nullptr, "ProcessMessages()");
    }

    if (!fRet) {
        LogPrint(BCLog::NET, "%s(%s, %u bytes) FAILED peer=%d\n", __func__, SanitizeString(strCommand), nMessageSize, pfrom->GetId());
    }

    LOCK(cs_main);
    SendRejectsAndCheckIfBanned(pfrom, connman);

    return fMoreWork;
        return true;
    }


    bool NodeMessageHandler::SendMessages(CNode* pnode, std::atomic<bool>& interrupt) override
    {
        printf("Trace | bool NodeMessageHandler::SendMessages(CNode* pnode, std::atomic<bool>& interrupt) called.\n");
        return true;
    const Consensus::Params& consensusParams = Params().GetConsensus();
    {
        // Don't send anything until the version handshake is complete
        if (!pto->fSuccessfullyConnected || pto->fDisconnect)
            return true;

        // If we get here, the outgoing message serialization version is set and can't change.
        const CNetMsgMaker msgMaker(pto->GetSendVersion());

        //
        // Message: ping
        //
        bool pingSend = false;
        if (pto->fPingQueued) {
            // RPC ping request by user
            pingSend = true;
        }
        if (pto->nPingNonceSent == 0 && pto->nPingUsecStart + PING_INTERVAL * 1000000 < GetTimeMicros()) {
            // Ping automatically sent as a latency probe & keepalive.
            pingSend = true;
        }
        if (pingSend) {
            uint64_t nonce = 0;
            while (nonce == 0) {
                GetRandBytes((unsigned char*)&nonce, sizeof(nonce));
            }
            pto->fPingQueued = false;
            pto->nPingUsecStart = GetTimeMicros();
            if (pto->nVersion > BIP0031_VERSION) {
                pto->nPingNonceSent = nonce;
                connman->PushMessage(pto, msgMaker.Make(NetMsgType::PING, nonce));
            } else {
                // Peer is too old to support ping command with nonce, pong will never arrive.
                pto->nPingNonceSent = 0;
                connman->PushMessage(pto, msgMaker.Make(NetMsgType::PING));
            }
        }

        TRY_LOCK(cs_main, lockMain); // Acquire cs_main for IsInitialBlockDownload() and CNodeState()
        if (!lockMain)
            return true;

        if (SendRejectsAndCheckIfBanned(pto, connman))
            return true;
        CNodeState &state = *State(pto->GetId());

        // Address refresh broadcast
        int64_t nNow = GetTimeMicros();
        if (!IsInitialBlockDownload() && pto->nNextLocalAddrSend < nNow) {
            AdvertiseLocal(pto);
            pto->nNextLocalAddrSend = PoissonNextSend(nNow, AVG_LOCAL_ADDRESS_BROADCAST_INTERVAL);
        }

        //
        // Message: addr
        //
        if (pto->nNextAddrSend < nNow) {
            pto->nNextAddrSend = PoissonNextSend(nNow, AVG_ADDRESS_BROADCAST_INTERVAL);
            std::vector<CAddress> vAddr;
            vAddr.reserve(pto->vAddrToSend.size());
            for (const CAddress& addr : pto->vAddrToSend)
            {
                if (!pto->addrKnown.contains(addr.GetKey()))
                {
                    pto->addrKnown.insert(addr.GetKey());
                    vAddr.push_back(addr);
                    // receiver rejects addr messages larger than 1000
                    if (vAddr.size() >= 1000)
                    {
                        connman->PushMessage(pto, msgMaker.Make(NetMsgType::ADDR, vAddr));
                        vAddr.clear();
                    }
                }
            }
            pto->vAddrToSend.clear();
            if (!vAddr.empty())
                connman->PushMessage(pto, msgMaker.Make(NetMsgType::ADDR, vAddr));
            // we only send the big addr message once
            if (pto->vAddrToSend.capacity() > 40)
                pto->vAddrToSend.shrink_to_fit();
        }

        // Start block sync
        if (pindexBestHeader == nullptr)
            pindexBestHeader = chainActive.Tip();
        bool fFetch = state.fPreferredDownload || (nPreferredDownload == 0 && !pto->fClient && !pto->fOneShot); // Download if this is a nice peer, or we have no nice peers and this one might do.
        if (!state.fSyncStarted && !pto->fClient && !fImporting && !fReindex) {
            // Only actively request headers from a single peer, unless we're close to today.
            if ((nSyncStarted == 0 && fFetch) || pindexBestHeader->GetBlockTime() > GetAdjustedTime() - 24 * 60 * 60) {
                state.fSyncStarted = true;
                state.nHeadersSyncTimeout = GetTimeMicros() + HEADERS_DOWNLOAD_TIMEOUT_BASE + HEADERS_DOWNLOAD_TIMEOUT_PER_HEADER * (GetAdjustedTime() - pindexBestHeader->GetBlockTime())/(consensusParams.nPowTargetSpacing);
                nSyncStarted++;
                const CBlockIndex *pindexStart = pindexBestHeader;
                /* If possible, start at the block preceding the currently
                   best known header.  This ensures that we always get a
                   non-empty list of headers back as long as the peer
                   is up-to-date.  With a non-empty response, we can initialise
                   the peer's known best block.  This wouldn't be possible
                   if we requested starting at pindexBestHeader and
                   got back an empty response.  */
                if (pindexStart->pprev)
                    pindexStart = pindexStart->pprev;
                LogPrint(BCLog::NET, "initial getheaders (%d) to peer=%d (startheight:%d)\n", pindexStart->nHeight, pto->GetId(), pto->nStartingHeight);
                connman->PushMessage(pto, msgMaker.Make(NetMsgType::GETHEADERS, chainActive.GetLocator(pindexStart), uint256()));
            }
        }

        // Resend wallet transactions that haven't gotten in a block yet
        // Except during reindex, importing and IBD, when old wallet
        // transactions become unconfirmed and spams other nodes.
        if (!fReindex && !fImporting && !IsInitialBlockDownload())
        {
            GetMainSignals().Broadcast(nTimeBestReceived, connman);
        }

        //
        // Try sending block announcements via headers
        //
        {
            // If we have less than MAX_BLOCKS_TO_ANNOUNCE in our
            // list of block hashes we're relaying, and our peer wants
            // headers announcements, then find the first header
            // not yet known to our peer but would connect, and send.
            // If no header would connect, or if we have too many
            // blocks, or if the peer doesn't want headers, just
            // add all to the inv queue.
            LOCK(pto->cs_inventory);
            std::vector<CBlock> vHeaders;
            bool fRevertToInv = ((!state.fPreferHeaders &&
                                 (!state.fPreferHeaderAndIDs || pto->vBlockHashesToAnnounce.size() > 1)) ||
                                pto->vBlockHashesToAnnounce.size() > MAX_BLOCKS_TO_ANNOUNCE);
            const CBlockIndex *pBestIndex = nullptr; // last header queued for delivery
            ProcessBlockAvailability(pto->GetId()); // ensure pindexBestKnownBlock is up-to-date

            if (!fRevertToInv) {
                bool fFoundStartingHeader = false;
                // Try to find first header that our peer doesn't have, and
                // then send all headers past that one.  If we come across any
                // headers that aren't on chainActive, give up.
                for (const uint256 &hash : pto->vBlockHashesToAnnounce) {
                    const CBlockIndex* pindex = LookupBlockIndex(hash);
                    assert(pindex);
                    if (chainActive[pindex->nHeight] != pindex) {
                        // Bail out if we reorged away from this block
                        fRevertToInv = true;
                        break;
                    }
                    if (pBestIndex != nullptr && pindex->pprev != pBestIndex) {
                        // This means that the list of blocks to announce don't
                        // connect to each other.
                        // This shouldn't really be possible to hit during
                        // regular operation (because reorgs should take us to
                        // a chain that has some block not on the prior chain,
                        // which should be caught by the prior check), but one
                        // way this could happen is by using invalidateblock /
                        // reconsiderblock repeatedly on the tip, causing it to
                        // be added multiple times to vBlockHashesToAnnounce.
                        // Robustly deal with this rare situation by reverting
                        // to an inv.
                        fRevertToInv = true;
                        break;
                    }
                    pBestIndex = pindex;
                    if (fFoundStartingHeader) {
                        // add this to the headers message
                        vHeaders.push_back(pindex->GetBlockHeader());
                    } else if (PeerHasHeader(&state, pindex)) {
                        continue; // keep looking for the first new block
                    } else if (pindex->pprev == nullptr || PeerHasHeader(&state, pindex->pprev)) {
                        // Peer doesn't have this header but they do have the prior one.
                        // Start sending headers.
                        fFoundStartingHeader = true;
                        vHeaders.push_back(pindex->GetBlockHeader());
                    } else {
                        // Peer doesn't have this header or the prior one -- nothing will
                        // connect, so bail out.
                        fRevertToInv = true;
                        break;
                    }
                }
            }
            if (!fRevertToInv && !vHeaders.empty()) {
                if (vHeaders.size() == 1 && state.fPreferHeaderAndIDs) {
                    // We only send up to 1 block as header-and-ids, as otherwise
                    // probably means we're doing an initial-ish-sync or they're slow
                    LogPrint(BCLog::NET, "%s sending header-and-ids %s to peer=%d\n", __func__,
                            vHeaders.front().GetHash().ToString(), pto->GetId());

                    int nSendFlags = state.fWantsCmpctWitness ? 0 : SERIALIZE_TRANSACTION_NO_WITNESS;

                    bool fGotBlockFromCache = false;
                    {
                        LOCK(cs_most_recent_block);
                        if (most_recent_block_hash == pBestIndex->GetBlockHash()) {
                            if (state.fWantsCmpctWitness || !fWitnessesPresentInMostRecentCompactBlock)
                                connman->PushMessage(pto, msgMaker.Make(nSendFlags, NetMsgType::CMPCTBLOCK, *most_recent_compact_block));
                            else {
                                CBlockHeaderAndShortTxIDs cmpctblock(*most_recent_block, state.fWantsCmpctWitness);
                                connman->PushMessage(pto, msgMaker.Make(nSendFlags, NetMsgType::CMPCTBLOCK, cmpctblock));
                            }
                            fGotBlockFromCache = true;
                        }
                    }
                    if (!fGotBlockFromCache) {
                        CBlock block;
                        bool ret = ReadBlockFromDisk(block, pBestIndex, consensusParams);
                        assert(ret);
                        CBlockHeaderAndShortTxIDs cmpctblock(block, state.fWantsCmpctWitness);
                        connman->PushMessage(pto, msgMaker.Make(nSendFlags, NetMsgType::CMPCTBLOCK, cmpctblock));
                    }
                    state.pindexBestHeaderSent = pBestIndex;
                } else if (state.fPreferHeaders) {
                    if (vHeaders.size() > 1) {
                        LogPrint(BCLog::NET, "%s: %u headers, range (%s, %s), to peer=%d\n", __func__,
                                vHeaders.size(),
                                vHeaders.front().GetHash().ToString(),
                                vHeaders.back().GetHash().ToString(), pto->GetId());
                    } else {
                        LogPrint(BCLog::NET, "%s: sending header %s to peer=%d\n", __func__,
                                vHeaders.front().GetHash().ToString(), pto->GetId());
                    }
                    connman->PushMessage(pto, msgMaker.Make(NetMsgType::HEADERS, vHeaders));
                    state.pindexBestHeaderSent = pBestIndex;
                } else
                    fRevertToInv = true;
            }
            if (fRevertToInv) {
                // If falling back to using an inv, just try to inv the tip.
                // The last entry in vBlockHashesToAnnounce was our tip at some point
                // in the past.
                if (!pto->vBlockHashesToAnnounce.empty()) {
                    const uint256 &hashToAnnounce = pto->vBlockHashesToAnnounce.back();
                    const CBlockIndex* pindex = LookupBlockIndex(hashToAnnounce);
                    assert(pindex);

                    // Warn if we're announcing a block that is not on the main chain.
                    // This should be very rare and could be optimized out.
                    // Just log for now.
                    if (chainActive[pindex->nHeight] != pindex) {
                        LogPrint(BCLog::NET, "Announcing block %s not on main chain (tip=%s)\n",
                            hashToAnnounce.ToString(), chainActive.Tip()->GetBlockHash().ToString());
                    }

                    // If the peer's chain has this block, don't inv it back.
                    if (!PeerHasHeader(&state, pindex)) {
                        pto->PushInventory(CInv(MSG_BLOCK, hashToAnnounce));
                        LogPrint(BCLog::NET, "%s: sending inv peer=%d hash=%s\n", __func__,
                            pto->GetId(), hashToAnnounce.ToString());
                    }
                }
            }
            pto->vBlockHashesToAnnounce.clear();
        }

        //
        // Message: inventory
        //
        std::vector<CInv> vInv;
        {
            LOCK(pto->cs_inventory);
            vInv.reserve(std::max<size_t>(pto->vInventoryBlockToSend.size(), INVENTORY_BROADCAST_MAX));

            // Add blocks
            for (const uint256& hash : pto->vInventoryBlockToSend) {
                vInv.push_back(CInv(MSG_BLOCK, hash));
                if (vInv.size() == MAX_INV_SZ) {
                    connman->PushMessage(pto, msgMaker.Make(NetMsgType::INV, vInv));
                    vInv.clear();
                }
            }
            pto->vInventoryBlockToSend.clear();

            // Check whether periodic sends should happen
            bool fSendTrickle = pto->fWhitelisted;
            if (pto->nNextInvSend < nNow) {
                fSendTrickle = true;
                // Use half the delay for outbound peers, as there is less privacy concern for them.
                pto->nNextInvSend = PoissonNextSend(nNow, INVENTORY_BROADCAST_INTERVAL >> !pto->fInbound);
            }

            // Time to send but the peer has requested we not relay transactions.
            if (fSendTrickle) {
                LOCK(pto->cs_filter);
                if (!pto->fRelayTxes) pto->setInventoryTxToSend.clear();
            }

            // Respond to BIP35 mempool requests
            if (fSendTrickle && pto->fSendMempool) {
                auto vtxinfo = mempool.infoAll();
                pto->fSendMempool = false;
                CAmount filterrate = 0;
                {
                    LOCK(pto->cs_feeFilter);
                    filterrate = pto->minFeeFilter;
                }

                LOCK(pto->cs_filter);

                for (const auto& txinfo : vtxinfo) {
                    const uint256& hash = txinfo.tx->GetHash();
                    CInv inv(MSG_TX, hash);
                    pto->setInventoryTxToSend.erase(hash);
                    if (filterrate) {
                        if (txinfo.feeRate.GetFeePerK() < filterrate)
                            continue;
                    }
                    if (pto->pfilter) {
                        if (!pto->pfilter->IsRelevantAndUpdate(*txinfo.tx)) continue;
                    }
                    pto->filterInventoryKnown.insert(hash);
                    vInv.push_back(inv);
                    if (vInv.size() == MAX_INV_SZ) {
                        connman->PushMessage(pto, msgMaker.Make(NetMsgType::INV, vInv));
                        vInv.clear();
                    }
                }
                pto->timeLastMempoolReq = GetTime();
            }

            // Determine transactions to relay
            if (fSendTrickle) {
                // Produce a vector with all candidates for sending
                std::vector<std::set<uint256>::iterator> vInvTx;
                vInvTx.reserve(pto->setInventoryTxToSend.size());
                for (std::set<uint256>::iterator it = pto->setInventoryTxToSend.begin(); it != pto->setInventoryTxToSend.end(); it++) {
                    vInvTx.push_back(it);
                }
                CAmount filterrate = 0;
                {
                    LOCK(pto->cs_feeFilter);
                    filterrate = pto->minFeeFilter;
                }
                // Topologically and fee-rate sort the inventory we send for privacy and priority reasons.
                // A heap is used so that not all items need sorting if only a few are being sent.
                CompareInvMempoolOrder compareInvMempoolOrder(&mempool);
                std::make_heap(vInvTx.begin(), vInvTx.end(), compareInvMempoolOrder);
                // No reason to drain out at many times the network's capacity,
                // especially since we have many peers and some will draw much shorter delays.
                unsigned int nRelayedTransactions = 0;
                LOCK(pto->cs_filter);
                while (!vInvTx.empty() && nRelayedTransactions < INVENTORY_BROADCAST_MAX) {
                    // Fetch the top element from the heap
                    std::pop_heap(vInvTx.begin(), vInvTx.end(), compareInvMempoolOrder);
                    std::set<uint256>::iterator it = vInvTx.back();
                    vInvTx.pop_back();
                    uint256 hash = *it;
                    // Remove it from the to-be-sent set
                    pto->setInventoryTxToSend.erase(it);
                    // Check if not in the filter already
                    if (pto->filterInventoryKnown.contains(hash)) {
                        continue;
                    }
                    // Not in the mempool anymore? don't bother sending it.
                    auto txinfo = mempool.info(hash);
                    if (!txinfo.tx) {
                        continue;
                    }
                    if (filterrate && txinfo.feeRate.GetFeePerK() < filterrate) {
                        continue;
                    }
                    if (pto->pfilter && !pto->pfilter->IsRelevantAndUpdate(*txinfo.tx)) continue;
                    // Send
                    vInv.push_back(CInv(MSG_TX, hash));
                    nRelayedTransactions++;
                    {
                        // Expire old relay messages
                        while (!vRelayExpiration.empty() && vRelayExpiration.front().first < nNow)
                        {
                            mapRelay.erase(vRelayExpiration.front().second);
                            vRelayExpiration.pop_front();
                        }

                        auto ret = mapRelay.insert(std::make_pair(hash, std::move(txinfo.tx)));
                        if (ret.second) {
                            vRelayExpiration.push_back(std::make_pair(nNow + 15 * 60 * 1000000, ret.first));
                        }
                    }
                    if (vInv.size() == MAX_INV_SZ) {
                        connman->PushMessage(pto, msgMaker.Make(NetMsgType::INV, vInv));
                        vInv.clear();
                    }
                    pto->filterInventoryKnown.insert(hash);
                }
            }
        }
        if (!vInv.empty())
            connman->PushMessage(pto, msgMaker.Make(NetMsgType::INV, vInv));

        // Detect whether we're stalling
        nNow = GetTimeMicros();
        if (state.nStallingSince && state.nStallingSince < nNow - 1000000 * BLOCK_STALLING_TIMEOUT) {
            // Stalling only triggers when the block download window cannot move. During normal steady state,
            // the download window should be much larger than the to-be-downloaded set of blocks, so disconnection
            // should only happen during initial block download.
            LogPrintf("Peer=%d is stalling block download, disconnecting\n", pto->GetId());
            pto->fDisconnect = true;
            return true;
        }
        // In case there is a block that has been in flight from this peer for 2 + 0.5 * N times the block interval
        // (with N the number of peers from which we're downloading validated blocks), disconnect due to timeout.
        // We compensate for other peers to prevent killing off peers due to our own downstream link
        // being saturated. We only count validated in-flight blocks so peers can't advertise non-existing block hashes
        // to unreasonably increase our timeout.
        if (state.vBlocksInFlight.size() > 0) {
            QueuedBlock &queuedBlock = state.vBlocksInFlight.front();
            int nOtherPeersWithValidatedDownloads = nPeersWithValidatedDownloads - (state.nBlocksInFlightValidHeaders > 0);
            if (nNow > state.nDownloadingSince + consensusParams.nPowTargetSpacing * (BLOCK_DOWNLOAD_TIMEOUT_BASE + BLOCK_DOWNLOAD_TIMEOUT_PER_PEER * nOtherPeersWithValidatedDownloads)) {
                LogPrintf("Timeout downloading block %s from peer=%d, disconnecting\n", queuedBlock.hash.ToString(), pto->GetId());
                pto->fDisconnect = true;
                return true;
            }
        }
        // Check for headers sync timeouts
        if (state.fSyncStarted && state.nHeadersSyncTimeout < std::numeric_limits<int64_t>::max()) {
            // Detect whether this is a stalling initial-headers-sync peer
            if (pindexBestHeader->GetBlockTime() <= GetAdjustedTime() - 24*60*60) {
                if (nNow > state.nHeadersSyncTimeout && nSyncStarted == 1 && (nPreferredDownload - state.fPreferredDownload >= 1)) {
                    // Disconnect a (non-whitelisted) peer if it is our only sync peer,
                    // and we have others we could be using instead.
                    // Note: If all our peers are inbound, then we won't
                    // disconnect our sync peer for stalling; we have bigger
                    // problems if we can't get any outbound peers.
                    if (!pto->fWhitelisted) {
                        LogPrintf("Timeout downloading headers from peer=%d, disconnecting\n", pto->GetId());
                        pto->fDisconnect = true;
                        return true;
                    } else {
                        LogPrintf("Timeout downloading headers from whitelisted peer=%d, not disconnecting\n", pto->GetId());
                        // Reset the headers sync state so that we have a
                        // chance to try downloading from a different peer.
                        // Note: this will also result in at least one more
                        // getheaders message to be sent to
                        // this peer (eventually).
                        state.fSyncStarted = false;
                        nSyncStarted--;
                        state.nHeadersSyncTimeout = 0;
                    }
                }
            } else {
                // After we've caught up once, reset the timeout so we can't trigger
                // disconnect later.
                state.nHeadersSyncTimeout = std::numeric_limits<int64_t>::max();
            }
        }

        // Check that outbound peers have reasonable chains
        // GetTime() is used by this anti-DoS logic so we can test this using mocktime
        ConsiderEviction(pto, GetTime());

        //
        // Message: getdata (blocks)
        //
        std::vector<CInv> vGetData;
        if (!pto->fClient && ((fFetch && !pto->m_limited_node) || !IsInitialBlockDownload()) && state.nBlocksInFlight < MAX_BLOCKS_IN_TRANSIT_PER_PEER) {
            std::vector<const CBlockIndex*> vToDownload;
            NodeId staller = -1;
            FindNextBlocksToDownload(pto->GetId(), MAX_BLOCKS_IN_TRANSIT_PER_PEER - state.nBlocksInFlight, vToDownload, staller, consensusParams);
            for (const CBlockIndex *pindex : vToDownload) {
                uint32_t nFetchFlags = GetFetchFlags(pto);
                vGetData.push_back(CInv(MSG_BLOCK | nFetchFlags, pindex->GetBlockHash()));
                MarkBlockAsInFlight(pto->GetId(), pindex->GetBlockHash(), pindex);
                LogPrint(BCLog::NET, "Requesting block %s (%d) peer=%d\n", pindex->GetBlockHash().ToString(),
                    pindex->nHeight, pto->GetId());
            }
            if (state.nBlocksInFlight == 0 && staller != -1) {
                if (State(staller)->nStallingSince == 0) {
                    State(staller)->nStallingSince = nNow;
                    LogPrint(BCLog::NET, "Stall started peer=%d\n", staller);
                }
            }
        }

        //
        // Message: getdata (non-blocks)
        //
        while (!pto->mapAskFor.empty() && (*pto->mapAskFor.begin()).first <= nNow)
        {
            const CInv& inv = (*pto->mapAskFor.begin()).second;
            if (!AlreadyHave(inv))
            {
                LogPrint(BCLog::NET, "Requesting %s peer=%d\n", inv.ToString(), pto->GetId());
                vGetData.push_back(inv);
                if (vGetData.size() >= 1000)
                {
                    connman->PushMessage(pto, msgMaker.Make(NetMsgType::GETDATA, vGetData));
                    vGetData.clear();
                }
            } else {
                //If we're not going to ask, don't expect a response.
                pto->setAskFor.erase(inv.hash);
            }
            pto->mapAskFor.erase(pto->mapAskFor.begin());
        }
        if (!vGetData.empty())
            connman->PushMessage(pto, msgMaker.Make(NetMsgType::GETDATA, vGetData));

        //
        // Message: feefilter
        //
        // We don't want white listed peers to filter txs to us if we have -whitelistforcerelay
        if (pto->nVersion >= FEEFILTER_VERSION && gArgs.GetBoolArg("-feefilter", DEFAULT_FEEFILTER) &&
            !(pto->fWhitelisted && gArgs.GetBoolArg("-whitelistforcerelay", DEFAULT_WHITELISTFORCERELAY))) {
            CAmount currentFilter = mempool.GetMinFee(gArgs.GetArg("-maxmempool", DEFAULT_MAX_MEMPOOL_SIZE) * 1000000).GetFeePerK();
            int64_t timeNow = GetTimeMicros();
            if (timeNow > pto->nextSendTimeFeeFilter) {
                static CFeeRate default_feerate(DEFAULT_MIN_RELAY_TX_FEE);
                static FeeFilterRounder filterRounder(default_feerate);
                CAmount filterToSend = filterRounder.round(currentFilter);
                // We always have a fee filter of at least minRelayTxFee
                filterToSend = std::max(filterToSend, ::minRelayTxFee.GetFeePerK());
                if (filterToSend != pto->lastSentFeeFilter) {
                    connman->PushMessage(pto, msgMaker.Make(NetMsgType::FEEFILTER, filterToSend));
                    pto->lastSentFeeFilter = filterToSend;
                }
                pto->nextSendTimeFeeFilter = PoissonNextSend(timeNow, AVG_FEEFILTER_BROADCAST_INTERVAL);
            }
            // If the fee filter has changed substantially and it's still more than MAX_FEEFILTER_CHANGE_DELAY
            // until scheduled broadcast, then move the broadcast to within MAX_FEEFILTER_CHANGE_DELAY.
            else if (timeNow + MAX_FEEFILTER_CHANGE_DELAY * 1000000 < pto->nextSendTimeFeeFilter &&
                     (currentFilter < 3 * pto->lastSentFeeFilter / 4 || currentFilter > 4 * pto->lastSentFeeFilter / 3)) {
                pto->nextSendTimeFeeFilter = timeNow + GetRandInt(MAX_FEEFILTER_CHANGE_DELAY) * 1000000;
            }
        }
    }
    return true;
}