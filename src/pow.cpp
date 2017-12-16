// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "chain.h"
#include "chainparams.h"
#include "primitives/block.h"
#include "auxpow/auxpow.h"
#include "uint256.h"
#include "util.h"
#include "bignum.h"

unsigned int static CalculateNextWorkRequired_V1(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params) {
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    //if ( pindexLast->nVersion <= 70002 ) {
    if (fDebug)
        LogPrintf("  nActualTimespan = %d before bounds\n", nActualTimespan);
    if (pindexLast->nHeight + 1 > 10000) {
        if (nActualTimespan < params.nPowTargetTimespan / 4)
            nActualTimespan = params.nPowTargetTimespan / 4;
        if (nActualTimespan > params.nPowTargetTimespan * 4)
            nActualTimespan = params.nPowTargetTimespan * 4;
    } else if (pindexLast->nHeight + 1 > 5000) {
        if (nActualTimespan < params.nPowTargetTimespan / 8)
            nActualTimespan = params.nPowTargetTimespan / 8;
        if (nActualTimespan > params.nPowTargetTimespan * 4)
            nActualTimespan = params.nPowTargetTimespan * 4;
    } else {
        if (nActualTimespan < params.nPowTargetTimespan / 16)
            nActualTimespan = params.nPowTargetTimespan / 16;
        if (nActualTimespan > params.nPowTargetTimespan * 4)
            nActualTimespan = params.nPowTargetTimespan * 4;
    }
    //} 
    /*else {
        if (nActualTimespan < params.nPowTargetTimespan/4)
            nActualTimespan = params.nPowTargetTimespan/4;
        if (nActualTimespan > params.nPowTargetTimespan*4)
            nActualTimespan = params.nPowTargetTimespan*4;
    }*/
    // Retarget
    arith_uint256 bnNew;
    arith_uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
    bnOld = bnNew;
    // Aib: intermediate uint256 can overflow by 1 bit
    bool fShift = bnNew.bits() > 232;
    if (fShift)
        bnNew >>= 1;
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;

    if (fShift)
        bnNew <<= 1;

    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    /// debug print
    LogPrintf("CalculateNextWorkRequired_V1 RETARGET\n");
    LogPrintf("params.nPowTargetTimespan = %d    nActualTimespan = %d\n", params.nPowTargetTimespan, nActualTimespan);
    LogPrintf("Before: %08x  %s\n", pindexLast->nBits, bnOld.ToString());
    LogPrintf("After:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString());

    return bnNew.GetCompact();
}

unsigned int static GetNextWorkRequired_V1(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params) {
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    // Go back by what we want to be 14 days worth of blocks
    // Aib: This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    int nInterval = params.DifficultyAdjustmentInterval();

    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight + 1) % params.DifficultyAdjustmentInterval() != 0) {
        // TestNet mode
        if (params.fPowAllowMinDifficultyBlocks) {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing * 2)
                return nProofOfWorkLimit;
            else {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    int blockstogoback = (pindexLast->nHeight + 1 != nInterval ? nInterval : nInterval - 1);

    // Go back by what we want to be 14 days worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;



    for (int i = 0; pindexFirst && i < blockstogoback; i++)
        pindexFirst = pindexFirst->pprev;

    assert(pindexFirst);

    return CalculateNextWorkRequired_V1(pindexLast, pindexFirst->GetBlockTime(), params);
}



// AntiGravityWave by reorder, derived from code by Evan Duffield - evan@darkcoin.io

unsigned int static AntiGravityWave(int64 version, const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params) {
    const CBlockIndex *BlockLastSolved = pindexLast;
    const CBlockIndex *BlockReading = pindexLast;
    int64_t nActualTimespan = 0;
    int64_t LastBlockTime = 0;
    int64_t PastBlocksMin = 24;
    int64_t PastBlocksMax = 24;
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    if (version == 1) {
        PastBlocksMin = 24;
        PastBlocksMax = 24;
    } else if (version == 2) {
        PastBlocksMin = 72;
        PastBlocksMax = 72;
    }

    int64_t CountBlocks = 0;
    arith_uint256 PastDifficultyAverage;
    arith_uint256 PastDifficultyAveragePrev;

    if (BlockLastSolved == NULL || BlockLastSolved->nHeight == 0 || BlockLastSolved->nHeight < PastBlocksMin) {
        return nProofOfWorkLimit;
    }

    for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) {
        if (PastBlocksMax > 0 && i > PastBlocksMax) {
            break;
        }
        CountBlocks++;

        if (CountBlocks <= PastBlocksMin) {
            if (CountBlocks == 1)
                PastDifficultyAverage.SetCompact(BlockReading->nBits);
            else {
                PastDifficultyAverage = ((PastDifficultyAveragePrev * CountBlocks)+(arith_uint256().SetCompact(BlockReading->nBits))) / (CountBlocks + 1);
            }
            PastDifficultyAveragePrev = PastDifficultyAverage;
        }

        if (LastBlockTime > 0) {
            int64_t Diff = (LastBlockTime - BlockReading->GetBlockTime());
            nActualTimespan += Diff;
        }
        LastBlockTime = BlockReading->GetBlockTime();

        if (BlockReading->pprev == NULL) {
            assert(BlockReading);
            break;
        }
        BlockReading = BlockReading->pprev;
    }

    arith_uint256 bnNew(PastDifficultyAverage);

    if (version == 2)
        --CountBlocks;

    int64_t nTargetTimespan = CountBlocks * params.nPowTargetSpacing;

    int64_t div = 3;
    if (version == 1)
        div = 3;
    else if (version == 2)
        div = 2;

    if (nActualTimespan < nTargetTimespan / div)
        nActualTimespan = nTargetTimespan / div;
    if (nActualTimespan > nTargetTimespan * div)
        nActualTimespan = nTargetTimespan * div;

    // Retarget
    bnNew *= nActualTimespan;
    bnNew /= nTargetTimespan;

    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    if (bnNew > bnPowLimit) {
        bnNew = bnPowLimit;
    }

    return bnNew.GetCompact();
}

const int WTMINT_BLOCK_nTargetTimespan = 14400; //4 * 60 * 60 //  : every 4 hours
const int WTMINT_BLOCK_nTargetSpacing = 60; //  : 1 minutes
const int WTMINT_KGW_StartBlock = 6000;

/*static const int64 nTargetTimespan  =   WTMINT_BLOCK_nTargetTimespan;//4 * 60 * 60; // TempleCoin: every 4 hours
static const int64 nTargetSpacing   =   WTMINT_BLOCK_nTargetSpacing;//60; // TempleCoin: 1 minutes
static const int64 nInterval        =   nTargetTimespan / nTargetSpacing;*/
unsigned int static KimotoGravityWell(const CBlockIndex* pindexLast, const CBlockHeader *pblock,
        uint64 TargetBlocksSpacingSeconds, uint64 PastBlocksMin, uint64 PastBlocksMax,
        const Consensus::Params& params) {
    /* current difficulty formula, megacoin - kimoto gravity well */
    const CBlockIndex *BlockLastSolved = pindexLast;
    const CBlockIndex *BlockReading = pindexLast;
    //const CBlockHeader *BlockCreating				= pblock;
    // TODO: AIB MERGE REMOVE
    //BlockCreating				= BlockCreating;
    uint64 PastBlocksMass = 0;
    int64 PastRateActualSeconds = 0;
    int64 PastRateTargetSeconds = 0;
    double PastRateAdjustmentRatio = double(1);
    CBigNum PastDifficultyAverage;
    CBigNum PastDifficultyAveragePrev;
    double EventHorizonDeviation;
    double EventHorizonDeviationFast;
    double EventHorizonDeviationSlow;
    const CBigNum bnProofOfWorkLimit(params.powLimit);
    
    if (BlockLastSolved == NULL || BlockLastSolved->nHeight == 0 || (uint64) BlockLastSolved->nHeight < PastBlocksMin) {
        return bnProofOfWorkLimit.GetCompact();
    }

    for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) {
        if (PastBlocksMax > 0 && i > PastBlocksMax) {
            break;
        }
        PastBlocksMass++;

        if (i == 1) {
            PastDifficultyAverage.SetCompact(BlockReading->nBits);
        } else {
            PastDifficultyAverage = ((CBigNum().SetCompact(BlockReading->nBits) - PastDifficultyAveragePrev))/i + PastDifficultyAveragePrev;
        }
        PastDifficultyAveragePrev = PastDifficultyAverage;

        PastRateActualSeconds = BlockLastSolved->GetBlockTime() - BlockReading->GetBlockTime();
        PastRateTargetSeconds = TargetBlocksSpacingSeconds * PastBlocksMass;
        PastRateAdjustmentRatio = double(1);
        if (PastRateActualSeconds < 0) {
            PastRateActualSeconds = 0;
        }
        if (PastRateActualSeconds != 0 && PastRateTargetSeconds != 0) {
            PastRateAdjustmentRatio = double(PastRateTargetSeconds) / double(PastRateActualSeconds);
        }
        EventHorizonDeviation = 1 + (0.7084 * pow((double(PastBlocksMass) / double(28.2)), -1.228));
        EventHorizonDeviationFast = EventHorizonDeviation;
        EventHorizonDeviationSlow = 1 / EventHorizonDeviation;

        if (PastBlocksMass >= PastBlocksMin) {
            if ((PastRateAdjustmentRatio <= EventHorizonDeviationSlow) || (PastRateAdjustmentRatio >= EventHorizonDeviationFast)) {
                assert(BlockReading);
                break;
            }
        }
        if (BlockReading->pprev == NULL) {
            assert(BlockReading);
            break;
        }
        BlockReading = BlockReading->pprev;
    }

    CBigNum bnNew = PastDifficultyAverage;
    if (PastRateActualSeconds != 0 && PastRateTargetSeconds != 0) {  
        bnNew *= PastRateActualSeconds;
        bnNew /= PastRateTargetSeconds;
    }    
    if (bnNew > bnProofOfWorkLimit)
        bnNew = bnProofOfWorkLimit;

    /// debug print
    //LogPrint("aibdbg","Difficulty Retarget - Kimoto Gravity Well\n");
    //LogPrint("aibdbg","PastRateAdjustmentRatio = %g\n", PastRateAdjustmentRatio);
    //LogPrint("aibdbg","Before: %08x  %s\n", BlockLastSolved->nBits, CBigNum().SetCompact(BlockLastSolved->nBits).GetHex().c_str());
    //LogPrint("aibdbg","After:  %08x  %s\n", bnNew.GetCompact(), bnNew.GetHex().c_str());

    return bnNew.GetCompact();
}

unsigned int static GetNextWorkRequired_V2(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params ) {
    static const int64_t BlocksTargetSpacing = WTMINT_BLOCK_nTargetSpacing; // 1 minute
    unsigned int TimeDaySeconds = 60 * 60 * 24;
    int64 PastSecondsMin = TimeDaySeconds * 0.01;
    int64 PastSecondsMax = TimeDaySeconds * 0.14;
    int64 PastBlocksMin = PastSecondsMin / BlocksTargetSpacing;
    int64 PastBlocksMax = PastSecondsMax / BlocksTargetSpacing;

    return KimotoGravityWell(pindexLast, pblock, BlocksTargetSpacing, PastBlocksMin, PastBlocksMax, params);
}

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params) {
    // -regtest mode 
    bool isRegTestMode = params.fPowNoRetargeting;
    bool isTestNetMode = params.fPowAllowMinDifficultyBlocks;
    if (isRegTestMode) {
        printf("Running in RegTest Mode...");
        return pindexLast->nBits;
    }
    // TODO: AIB MERGE RECEHCEK
    // -testnet mode
    if (isTestNetMode && pindexLast->nHeight + 1 >= 50) {
        if (fDebug) LogPrintf("Running to V2 GWP\n");
        return GetNextWorkRequired_V2(pindexLast, pblock,params);
    }
    // not in TestNetMode due to lazy eval
    if (pindexLast->nHeight + 1 >= WTMINT_KGW_StartBlock) {
        if (fDebug) LogPrintf("Running to V2 GWP\n");
        return GetNextWorkRequired_V2(pindexLast, pblock, params);
    }
    if (fDebug) LogPrintf("Running to V1 GWP\n");
    return GetNextWorkRequired_V1(pindexLast, pblock, params);

    /*if ( ( isTestNetMode && pindexLast->nHeight+1 >= 50 ) ||
         ( !isTestNetMode && pindexLast->nHeight+1 >= WTMINT_KGW_StartBlock )) {
        if ( isTestNetMode )
            printf("Running in TestNet mode");
        else 
            printf("Running in Normal mode");
        return GetNextWorkRequired_V2(pindexLast, pblock);
    }
    if (fDebug)
        LogPrintf("Running to V1 GWP\n");
    /*if (pindexLast->nHeight+1 >= 4510000 || (params.fPowAllowMinDifficultyBlocks && pindexLast->nHeight+1 >= 300000)) {
        return AntiGravityWave(2, pindexLast, pblock, params);
    } else if (pindexLast->nHeight+1 >= 3600) {
        return AntiGravityWave(1, pindexLast, pblock, params);
    } else {
        return GetNextWorkRequired_V1(pindexLast, pblock, params);
    }
    return GetNextWorkRequired_V1(pindexLast, pblock, params);
     */
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params) {
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);


    if (Params().SkipProofOfWorkCheck())
        return true;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > bnPowLimit) {
        std::cout << "fNetgetive=" << fNegative << " bnTarget=" << bnTarget.ToString() << " fOverflow=" << fOverflow << " bnPowLimit=" << bnPowLimit.ToString() << "\n";
        return error("CheckProofOfWork() : nBits below minimum work");
    }
    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return error("CheckProofOfWork() : hash doesn't match nBits");

    return true;
}

bool CheckBlockProofOfWork(const CBlockHeader *pblock, const Consensus::Params& params) {
    // LogPrint("txdb", "CheckBlockProofOfWork(): block: %s\n", pblock->ToString());  // LEDTMP

    if (pblock->auxpow && (pblock->auxpow.get() != NULL)) {
        if (!pblock->auxpow->Check(pblock->GetHash(), pblock->GetChainID(), params))
            return error("CheckBlockProofOfWork() : AUX POW is not valid");
        // Check proof of work matches claimed amount
        if (!CheckProofOfWork(pblock->auxpow->GetParentBlockHash(), pblock->nBits, params))
            return error("CheckBlockProofOfWork() : AUX proof of work failed");
    } else {
        // Check proof of work matches claimed amount
        if (!CheckProofOfWork(pblock->GetPoWHash(), pblock->nBits, params))
            return error("CheckBlockProofOfWork() : proof of work failed");
    }
    return true;
}

bool CheckAuxPowValidity(const CBlockHeader* pblock, const Consensus::Params& params) {
    if (!params.fPowAllowMinDifficultyBlocks) {
        if (pblock->GetChainID() != AuxPow::CHAIN_ID)
            return error("CheckAuxPowValidity() : block does not have our chain ID");
    }
    return true;
}

// TODO LED TMP temporary public interface for passing the build of test/pow_tests.cpp only
// TODO LED TMP this code should be removed and test/pow_test.cpp changed to call
// TODO LED TMP our interface to PoW --> GetNextWorkRequired()

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params) {
    return CalculateNextWorkRequired_V1(pindexLast, nFirstBlockTime, params);
}
