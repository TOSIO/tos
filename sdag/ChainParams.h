#pragma once

#include <vector>
#include <string>
#include <memory>
#include <toscore/common/Common.h>

namespace dev
{
namespace sdag
{
class ChainParams
{
public:
  int TimeSlice() { return timeSlice; }

  uint32_t nRuleChangeActivationThreshold;
  uint32_t nMinerConfirmationWindow;

  /** Proof of work parameters */
  u256 powLimit;
  bool fPowAllowMinDifficultyBlocks;
  bool fPowNoRetargeting;
  int64_t nPowTargetSpacing;
  int64_t nPowTargetTimespan;

  int64_t DifficultyAdjustmentInterval() const { return nPowTargetTimespan / nPowTargetSpacing; }
  
  u256 nMinimumChainWork;
  u256 defaultAssumeValid;

  int timeSlice;
  bytes pchMessageStart;
  int nDefaultPort;
  uint64_t nPruneAfterHeight;
  std::vector<std::string> vSeeds;
  std::string bech32_hrp;
  std::string strNetworkID;

  Block genesis;

  bool fDefaultConsistencyChecks;
  bool fRequireStandard;
  bool fMineBlocksOnDemand;

  bool m_fallback_fee_enabled;
};

using ChainParamsRef = std::shared_ptr<ChainParams>;
} // namespace sdag

} // namespace dev