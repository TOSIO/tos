#pragma once
#include "protocol.h"
#include <deps/consensus/params.h>
#include <memory>

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};


namespace tos
{
    class ChainParamsProxy
    {
        public:
        ChainParamsProxy()
        {
            _msgStart[0] = 0xf9;
            _msgStart[0] = 0xbe;
            _msgStart[0] = 0xb4;
            _msgStart[0] = 0xd9;
            //_seeds.emplace_back("192.168.1.31");
        }
        const Consensus::Params& GetConsensus() const 
        { 
            static Consensus::Params params;
            return params; 
        }
        const CMessageHeader::MessageStartChars& MessageStart() const 
        { 
            return _msgStart; 
        }
        
        int GetDefaultPort() const { return 9090; }

        const std::vector<std::string>& DNSSeeds() const;

        const std::vector<SeedSpec6>& FixedSeeds() const 
        { 
            return _fixedSeeds; 
        }

        private:
        CMessageHeader::MessageStartChars _msgStart;
        std::vector<std::string> _seeds;
        std::vector<SeedSpec6>   _fixedSeeds;
    };
}

namespace tos
{
    class ArgsManager;
    class ArgsProxy;

    namespace P2P{
        void setArgs(ArgsManager* manager);

        std::shared_ptr<ArgsProxy> Args();
        ChainParamsProxy& Params();
    }
    
}

extern std::shared_ptr<tos::ArgsProxy> g_argsProxy;

