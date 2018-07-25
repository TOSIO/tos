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
/*         const Consensus::Params& GetConsensus() const 
        { 

            return consensus; 
        } */
        const CMessageHeader::MessageStartChars& MessageStart() const 
        { 
            static CMessageHeader::MessageStartChars start;
            return start; 
        }
        
        int GetDefaultPort() const { return 0; }

         const std::vector<std::string>& DNSSeeds() const 
         { 
             static std::vector<std::string> vSeeds;
             return vSeeds; 
        }
        const std::vector<SeedSpec6>& FixedSeeds() const 
        { 
            static std::vector<SeedSpec6> vSeeds;
            return vSeeds; 
        }
    };
}