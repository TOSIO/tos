#pragma once
#include <uint256.h>
#include <vector>

class CRollingBloomFilter
{
public:
    // A random bloom filter calls GetRand() at creation time.
    // Don't create global CRollingBloomFilter objects, as they may be
    // constructed before the randomizer is properly initialized.
    CRollingBloomFilter(const unsigned int nElements, const double nFPRate);

    void insert(const std::vector<unsigned char>& vKey);
    void insert(const uint256& hash);
    bool contains(const std::vector<unsigned char>& vKey) const;
    bool contains(const uint256& hash) const;

    void reset();

private:
    int nEntriesPerGeneration;
    int nEntriesThisGeneration;
    int nGeneration;
    std::vector<uint64_t> data;
    unsigned int nTweak;
    int nHashFuncs;
};