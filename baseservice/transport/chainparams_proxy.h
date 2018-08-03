#pragma once
#include "protocol.h"

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
            _seeds.emplace_back("192.168.1.31");
        }
/*         const Consensus::Params& GetConsensus() const 
        { 

            return consensus; 
        } */
        const CMessageHeader::MessageStartChars& MessageStart() const 
        { 
            return _msgStart; 
        }
        
        int GetDefaultPort() const { return 9090; }

         const std::vector<std::string>& DNSSeeds() const 
         { 
             return _seeds; 
        }
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