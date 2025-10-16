// Copyright (c) 2011 Vince Durham
// Distributed under the MIT/X11 software license, see the accompanying
// file license.txt or http://www.opensource.org/licenses/mit-license.php.

#include <auxpow/auxpow.h>

#include <primitives/block.h>
#include <consensus/params.h>
#include <hash.h>
#include <logging.h>
#include <script/script.h>
#include <uint256.h>
#include <pubkey.h>

#include <algorithm>
#include <map>
#include <vector>

/** Global dirty block merged mining entries. */
std::map<uint256, std::shared_ptr<CAuxPow>> mapDirtyAuxPow;

unsigned char pchMergedMiningHeader[] = { 0xfa, 0xbe, 'm', 'm' };

int GetAuxPowStartBlock(const Consensus::Params& params)
{
    return params.nAuxPowStartHeight;
}

void RemoveMergedMiningHeader(std::vector<unsigned char>& vchAux)
{
    auto it = std::search(vchAux.begin(), vchAux.end(),
                         std::begin(pchMergedMiningHeader),
                         std::end(pchMergedMiningHeader));
    if (it == vchAux.end())
        throw std::runtime_error("merged mining aux too short");
    vchAux.erase(vchAux.begin(), vchAux.begin() + sizeof(pchMergedMiningHeader));
}

uint256 CAuxPow::CheckMerkleBranch(const uint256& hash, const std::vector<uint256>& vMerkleBranch, int nIndex) const
{
    uint256 thash = hash;
    if (nIndex == -1)
        return uint256();
    for (const auto& otherside : vMerkleBranch)
    {
        if (nIndex & 1)
            thash = Hash(otherside, thash);
        else
            thash = Hash(thash, otherside);
        nIndex >>= 1;
    }
    return thash;
}

bool CAuxPow::Check(const uint256& hashAuxBlock, int nChainID, const Consensus::Params& params) const
{
    if (nIndex != 0) {
        LogPrintf("AuxPow is not a generate\n");
        return false;
    }

    if (!params.fPowAllowMinDifficultyBlocks && parentBlockHeader.GetChainID() == nChainID) {
        LogPrintf("Aux POW parent has our chain ID\n");
        return false;
    }

    if (vChainMerkleBranch.size() > 30) {
        LogPrintf("Aux POW chain merkle branch too long\n");
        return false;
    }

    // Check that the chain merkle root is in the coinbase
    const uint256 nRootHash = CheckMerkleBranch(hashAuxBlock, vChainMerkleBranch, nChainIndex);
    std::vector<unsigned char> vchRootHash(nRootHash.begin(), nRootHash.end());
    std::reverse(vchRootHash.begin(), vchRootHash.end()); // correct endian

    // Check that we are in the parent block merkle tree
    if (CheckMerkleBranch(coinbaseTx->GetHash(), vMerkleBranch, nIndex) != parentBlockHeader.hashMerkleRoot) {
        LogPrintf("Aux POW merkle root incorrect\n");
        return false;
    }

    if (!coinbaseTx || coinbaseTx->vin.empty()) {
        LogPrintf("Aux POW coinbase transaction invalid\n");
        return false;
    }

    const CScript script = coinbaseTx->vin[0].scriptSig;

    // Check that the same work is not submitted twice to our chain.
    CScript::const_iterator pcHead =
        std::search(script.begin(), script.end(),
                   std::begin(pchMergedMiningHeader),
                   std::end(pchMergedMiningHeader));

    CScript::const_iterator pc =
        std::search(script.begin(), script.end(), vchRootHash.begin(), vchRootHash.end());

    if (pcHead == script.end()) {
        LogPrintf("MergedMiningHeader missing from parent coinbase\n");
        return false;
    }

    if (pc == script.end()) {
        LogPrintf("Aux POW missing chain merkle root in parent coinbase\n");
        return false;
    }

    if (pcHead != script.end())
    {
        // Enforce only one chain merkle root by checking that a single instance of the merged
        // mining header exists just before.
        if (script.end() != std::search(pcHead + 1, script.end(),
                                       std::begin(pchMergedMiningHeader),
                                       std::end(pchMergedMiningHeader))) {
            LogPrintf("Multiple merged mining headers in coinbase\n");
            return false;
        }
        if (pcHead + sizeof(pchMergedMiningHeader) != pc) {
            LogPrintf("Merged mining header is not just before chain merkle root\n");
            return false;
        }
    }
    else
    {
        // For backward compatibility.
        // Enforce only one chain merkle root by checking that it starts early in the coinbase.
        // 8-12 bytes are enough to encode extraNonce and nBits.
        if (pc - script.begin() > 20) {
            LogPrintf("Aux POW chain merkle root must start in the first 20 bytes of the parent coinbase\n");
            return false;
        }
    }


    // Ensure we are at a deterministic point in the merkle leaves by hashing
    // a nonce and our chain ID and comparing to the index.
    pc += vchRootHash.size();
    if (script.end() - pc < 8) {
        LogPrintf("Aux POW missing chain merkle tree size and nonce in parent coinbase\n");
        return false;
    }

    int nSize;
    memcpy(&nSize, &pc[0], 4);
    if (nSize != (1 << vChainMerkleBranch.size())) {
        LogPrintf("Aux POW merkle branch size does not match parent coinbase\n");
        return false;
    }

    int nNonce;
    memcpy(&nNonce, &pc[4], 4);

    // Choose a pseudo-random slot in the chain merkle tree
    // but have it be fixed for a size/nonce/chain combination.
    //
    // This prevents the same work from being used twice for the
    // same chain while reducing the chance that two chains clash
    // for the same slot.
    unsigned int rand = nNonce;
    rand = rand * 1103515245 + 12345;
    rand += nChainID;
    rand = rand * 1103515245 + 12345;

    if (nChainIndex != (rand % nSize)) {
        LogPrintf("Aux POW wrong index\n");
        return false;
    }

    return true;
}

// GetAuxpowMiningKey removed - wallet-specific functionality
// Will be implemented in wallet module if needed
CKeyID GetAuxpowMiningKey(void)
{
    // Placeholder for wallet integration
    return CKeyID();
}
