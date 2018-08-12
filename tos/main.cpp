#include <stdio.h>
#include <toscore/log/Log.h>
#include <toscore/log/LoggingProgramOptions.h>
#include <toscore/crypto/SHA3.h>
#include <toscore/common/Common.h>
#include <toscore/utils/RLP.h>
#include <vector>
#include <string>
#include <toscore/log/Log.h>
#include <toscore/utils/args_manager.h>
#include <baseservice/transport/net.h>
#include <baseservice/transport/netbase.h>
#include <scheduler.h>
#include <thread>
//  #include <boost/program_options/options_description.hpp>

#include <boost/program_options.hpp>

#include <sdag/BlockHeader.h>


class NodeMessageHandler : public NetEventsInterface
{
public:
    virtual bool ProcessMessages(CNode* pnode, std::atomic<bool>& interrupt) override
    {
        printf("Trace | bool NodeMessageHandler::ProcessMessages(CNode* pnode, std::atomic<bool>& interrupt) called.\n");
        return true;
    }
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


namespace po = boost::program_options;
using dev::LoggingOptions;

tos::ArgsManager g_args;
std::unique_ptr<CConnman> g_P2P;
static CScheduler scheduler;
NodeMessageHandler g_nodeMessageHandler;

unsigned const _lineWidth = 160;
void setupLog();
void setupP2P();
void setupSdag();

/* template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " "));
    return os;
}
 */
int main(int argc, char const *argv[])
{
    //init log
    setupLog();

    setupSdag();

    cnote << "************start toschain*********";
    po::options_description defaultMode("TOS CLIENT MODE (default)", _lineWidth);
    auto addClientOption = defaultMode.add_options();
    addClientOption("test", "Use the main network protocol");
   

/*     std::vector<std::string> binds;
    std::vector<std::string> whiteBinds;
    std::vector<std::string> whitelist;
    std::vector<std::string> seedNode;*/
    std::vector<std::string> connects; 

    po::options_description clientNetworking("CLIENT NETWORKING", _lineWidth);
    auto addNetworkingOption = clientNetworking.add_options();
    addNetworkingOption("bootstrap,b",
        "Connect to the default Tos peer servers (default unless --no-discovery used)");

    addNetworkingOption("no-bootstrap",
        "Do not connect to the default Tos peer servers (default only when --no-discovery is "
        "used)");

    addNetworkingOption("listen-ip", po::value<std::string>()->value_name("<ip>(:<port>)"),
        "Listen on the given IP for incoming connections (default: 0.0.0.0)");

    addNetworkingOption("listen", po::value<unsigned short>()->value_name("<port>"),
        "Listen on the given port for incoming connections (default: 80909)");

    /* addNetworkingOption("connect", po::value< std::vector<std::string> >(&connects)->multitoken(), //"<host>(:<port>)"
        "Connect to the given remote host (default: none)"); */

/*     addNetworkingOption("forcednsseed", po::value<bool>()->value_name("<port>"),
        "Listen on the given port for incoming connections (default: 80909)"); */

    addNetworkingOption("connect,C", po::value<std::vector<std::string> >(), //"<host>(:<port>)"
        "Connect to the given remote host (default: none)");

    addNetworkingOption("seeds,S", po::value<std::vector<std::string> >(), //"<host>(:<port>)"
        "Connect to the given DNS seed (default: none)");

    addNetworkingOption("port", po::value<short>()->value_name("<port>"),
        "Connect to the given remote port (default: 30303)");


    po::options_description allowedOptions("Allowed options");
    allowedOptions.add(defaultMode)
    .add(clientNetworking);
    
    po::variables_map vm;
    std::vector<std::string> unrecognisedOptions;

    try
    {
        po::parsed_options parsed = po::command_line_parser(argc, argv).options(allowedOptions).allow_unregistered().run();
        unrecognisedOptions = collect_unrecognized(parsed.options, po::include_positional);
        po::store(parsed, vm);
        po::notify(vm);
    }
    catch (po::error const& e)
    {
        std::cerr << e.what();
        return -1;
    }

    g_args.Set(vm);

    setupP2P();
    while (true)
    {
        std::chrono::milliseconds dura(2000);
        std::this_thread::sleep_for(dura);
    }
    return 0;
}

/*
init log
*/
void setupLog()
{
    LoggingOptions loggingOptions;
    po::options_description loggingProgramOptions(
        dev::createLoggingProgramOptions(_lineWidth, loggingOptions));
    dev::setupLogging(loggingOptions);
}

void setupP2P()
{
    assert(!g_P2P);
    g_P2P = std::unique_ptr<CConnman>(new CConnman(GetRand(std::numeric_limits<uint64_t>::max()), GetRand(std::numeric_limits<uint64_t>::max())));
    tos::P2P::setArgs(&g_args);
    CConnman& connman = *g_P2P;
    
    uint64_t nMaxOutboundLimit = 0; //unlimited unless -maxuploadtarget is set
    uint64_t nMaxOutboundTimeframe = MAX_UPLOAD_TIMEFRAME;

    if (g_args.IsArgSet("-maxuploadtarget")) {
        nMaxOutboundLimit = g_args.GetArg("-maxuploadtarget", DEFAULT_MAX_UPLOAD_TARGET)*1024*1024;
    }

    CConnman::Options connOptions;
    connOptions.nLocalServices = ServiceFlags(NODE_NETWORK | NODE_NETWORK_LIMITED);
    connOptions.nMaxConnections = 100;
    connOptions.nMaxOutbound = std::min(MAX_OUTBOUND_CONNECTIONS, connOptions.nMaxConnections);
    connOptions.nMaxAddnode = 100;
    connOptions.nMaxFeeler = 1;
    connOptions.nBestHeight = 0;
    //connOptions.uiInterface = &uiInterface;
    connOptions.m_msgproc = &g_nodeMessageHandler;
    connOptions.nSendBufferMaxSize = 1000*g_args.GetArg("-maxsendbuffer", DEFAULT_MAXSENDBUFFER);
    connOptions.nReceiveFloodSize = 1000*g_args.GetArg("-maxreceivebuffer", DEFAULT_MAXRECEIVEBUFFER);
    connOptions.m_added_nodes = g_args.GetArgs<std::string>("-addnode");

    connOptions.nMaxOutboundTimeframe = nMaxOutboundTimeframe;
    connOptions.nMaxOutboundLimit = nMaxOutboundLimit;



/*     for (const std::string& strBind : g_args.GetArgs("-bind")) {
        CService addrBind;
        if (!Lookup(strBind.c_str(), addrBind, GetListenPort(), false)) {
            printf("Trace | Lookup bind failed.\n");
            return ;
            //return InitError(ResolveErrMsg("bind", strBind));
        }
        connOptions.vBinds.push_back(addrBind);
    } */
    {
        std::string bind = g_args.GetArg<std::string>("listen-ip","");
        printf("Trace | main setupP2P bind : %s\n",bind.c_str());
        CService addrBind;
        if (!Lookup(bind.c_str(), addrBind, GetListenPort(), false)) {
            printf("Trace | Lookup bind failed.\n");
            return ;
            //return InitError(ResolveErrMsg("bind", strBind));
        }
         connOptions.vBinds.push_back(addrBind);
    }
   
    for (const std::string& strBind : g_args.GetArgs<std::string>("whitebind")) {
        CService addrBind;
        if (!Lookup(strBind.c_str(), addrBind, 0, false)) {
            printf("Trace | Lookup whitebind failed.\n");
            return ;
            //return InitError(ResolveErrMsg("whitebind", strBind));
        }
        if (addrBind.GetPort() == 0) {
            //return InitError(strprintf(_("Need to specify a port with -whitebind: '%s'"), strBind));
            printf("Trace | Lookup Need to specify a port with -whitebind: '%s'.\n",strBind.c_str());
            return ;
        }
        connOptions.vWhiteBinds.push_back(addrBind);
    }

    for (const auto& net : g_args.GetArgs<std::string>("whitelist")) {
        CSubNet subnet;
        LookupSubNet(net.c_str(), subnet);
        if (!subnet.IsValid())
        {
            //return InitError(strprintf(_("Invalid netmask specified in -whitelist: '%s'"), net));
            printf("Trace | Invalid netmask specified in -whitelist: '%s'", net.c_str());
            return ;
        }
            
        connOptions.vWhitelistedRange.push_back(subnet);
    }

    connOptions.vSeedNodes = g_args.GetArgs<std::string>("seednode");

    // Initiate outbound connections unless connect=0
    connOptions.m_use_addrman_outgoing = !g_args.IsArgSet("connect");
    if (!connOptions.m_use_addrman_outgoing) {
        auto connect = g_args.GetArgs<std::string>("connect");
         if (connect.size() != 1 || connect[0] != "0") {
            connOptions.m_specified_outgoing = connect;
        } 
        //connOptions.m_specified_outgoing.emplace_back(connect);
    }

    if (!connman.Start(scheduler, connOptions)) {
        return ;
    }
}


void setupSdag()
{

dev::sdag::BlockHeader blockHeader(1, 100, 1000);
RLPStream stream = blockHeader.encodeWithRLP();
cnote << "sdag header rlp " << stream.out();

dev::sdag::BlockHeader blockHeader1(stream);


}

