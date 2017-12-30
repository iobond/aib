
// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

//#define WTMINT_GENESIS_BLOCK	"0xf26d27dea5cbcc44df399960f0c2be03231a597ba8baed380f4df0fdea6d7592"
#define WTMINT_GENESIS_BLOCK	"0x062550537871c387faaa5cd91bb8a2cf4b67d3a4ec816799ecc1f5cb7fd54bcc"
#define WTMINT_pszTimestamp		"Asian Investment Bond By IOBond.com"
#define WTMINT_scriptPubKey		"04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f"

#define WTMINT_BLOCK_nTime			1428195753
#define WTMINT_BLOCK_nNonce			433798
#define WTMINT_BLOCK_nBits			0x1e0ffff0
#define WTMINT_POWLMT_nBits     "00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
#define WTMINT_BLOCK_hashMerkleRoot	"0x85f6c5fd0e39b5aa8688a844b283accef86973fcfdb7deac12ab2d4bebdc7c82"

#define WTMINT_TESTNET_GENESIS_BLOCK	"4843c056cccbd89dcc4324e2cae07051096f4c9fad0f8db49aa6f36363117bc2"
#define WTMINT_TESTNET_BLOCK_nTime			1405168772
#define WTMINT_TESTNET_BLOCK_nNonce			262929
// TestNet uses same nBits as main net - WTMINT_BLOCK_nBits

#define WTMINT_REGTEST_GENESIS_BLOCK	"ee03549187be5e68bc87256637458db42de8463bc05c7d8c659e728939da2e64"
#define WTMINT_REGTEST_BLOCK_nTime			1405166035
#define WTMINT_REGTEST_BLOCK_nNonce			0
#define WTMINT_REGTEST_POWLMT_nBits   "7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
#define WTMINT_REGTEST_BLOCK_nBits                      0x207fffff

#define WTMINT_RPC_PORT 51413
#define WTMINT_TESTNET_RPC_PORT 51423
#define WTMINT_REGTEST_RPC_PORT 21423

#define WTMINT_TESTNET_SERVER_PORT 18523
#define WTMINT_SERVER_PORT 31455

//Magic Header
#define WTMINT_MAGIC_1	0xa3
#define WTMINT_MAGIC_2	0xb1
#define WTMINT_MAGIC_3	0xc4
#define WTMINT_MAGIC_4	0xd1

#define WTMINT_TESTNET_MAGIC_1	0xa4
#define WTMINT_TESTNET_MAGIC_2	0xb2
#define WTMINT_TESTNET_MAGIC_3	0xc5
#define WTMINT_TESTNET_MAGIC_4	0xd2

#define WTMINT_REGTEST_MAGIC_1	0xa6
#define WTMINT_REGTEST_MAGIC_2	0xb3
#define WTMINT_REGTEST_MAGIC_3	0xc6
#define WTMINT_REGTEST_MAGIC_4	0xd3

#define WTMINT_BLOCK_nTargetTimespan 14400  //4 * 60 * 60 //  : every 4 hours
#define WTMINT_BLOCK_TESTNET_nTargetTimespan 3600  //1 * 60 * 60 //  : every 1 hours
#define WTMINT_BLOCK_nTargetSpacing 60 //  : 1 minutes

#define WTMINT_ENFORCE_BIP34_HEIGHT 2400000 // TODO: AIB MERGE RECEHCK change to actual number after release 
#define WTMINT_ENFORCE_BIP66_HEIGHT 2400000 // TODO: AIB MERGE RECEHCK change to actual number after release 

//Wallet starting letter
#define WTMINT_PUBKEY_ADDRESS 23 // Dec.
#define WTMINT_SCRPIT_ADDRESS 5 // Dec.
#define WTMINT_SECRET_ADDRESS 151 // Dec.

#define DEBUG(title,strout) std::cout<<title<<":    "<< strout<<"\n"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    //txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    
    genesis.nVersion = 1;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=4e9b54001f9976049830128ec0331515eaabe35a70970d79971da1539a400ba1, PoW=000001a16729477595c7247e1b49b4ec93acca8345037177cabbe898ce8a5783, ver=1, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000,
 *     hashMerkleRoot=0317d32e01a2adf6f2ac6f58c7cdaab6c656edc6fdb45986c739290053275200,
 *     nTime=1405164774, nBits=1e01ffff, nNonce=4016033, vtx=1)
 *   CTransaction(hash=0317d32e01, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *   CTxIn(COutPoint(0000000000, 4294967295), coinbase 04ffff001d01044c4e426c6f636b20233331303337393a30303030303030303030303030303030323431323532613762623237626539376265666539323138633132393064666633366331666631323965633732313161)
 *   CTxOut(nValue=0.00000000, scriptPubKey=0459934a6a228ce9716fa0b13aa1cd)
 * vMerkleTree: 0317d32e01a2adf6f2ac6f58c7cdaab6c656edc6fdb45986c739290053275200
 */

/*static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Block #310379:0000000000000000241252a7bb27be97befe9218c1290dff36c1ff129ec7211a";
    const CScript genesisOutputScript = CScript() << ParseHex("0459934a6a228ce9716fa0b13aa1cdc01593fca5f8599473c803a5109ff834dfdaf4c9ee35f2218c9ee3e7cf7db734e1179524b9d6ae8ebbeba883d4cb89b6c7bf") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}
*/

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, 
 * hashMerkleRoot=4a5e1e, 
 * nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = WTMINT_pszTimestamp ;
    const CScript genesisOutputScript = CScript() << ParseHex(WTMINT_scriptPubKey) << OP_CHECKSIG;
    
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
    // TODO: AIB MERGE REMOVE
    /*const char* pszTimestamp = "NY Times 05/Oct/2011 Steve Jobs, Apple’s Visionary, Dies at 56";
    const CScript genesisOutputScript = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
    */
}


/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 657000;
        consensus.nMajorityEnforceBlockUpgrade = 15000;
        consensus.nMajorityRejectBlockOutdated = 19000;
        consensus.nMajorityWindow = 20000;
        consensus.BIP34Height = WTMINT_ENFORCE_BIP34_HEIGHT;
        // TODO: AIB MERGE RECHECK replaced with AIB genesis block so recheck
        consensus.BIP34Hash = uint256S(WTMINT_GENESIS_BLOCK);// ("19ff9d27c0adae6e4b9eccc93590642019b22108e5883ade45cfe1bf665aca67");
        //consensus.BIP34Hash = uint256S("0x4e9b54001f9976049830128ec0331515eaabe35a70970d79971da1539a400ba1");
        consensus.powLimit = uint256S(WTMINT_POWLMT_nBits); // ~uint25(0) >> 23
        consensus.nPowTargetTimespan = WTMINT_BLOCK_nTargetTimespan;
                //14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = WTMINT_BLOCK_nTargetSpacing;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 8100; // 75% of nMinerConfirmationWindow
        consensus.nMinerConfirmationWindow = 10800; // 3 days
        consensus.nCLTVStartBlock = 598725;
        consensus.nBIP66MinStartBlock = WTMINT_ENFORCE_BIP66_HEIGHT;
        consensus.nAuxPowStartHeight = AuxPow::START_MAINNET;
        consensus.nWitnessStartHeight = 4040000;

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1232032894; // start + (1year/25)

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1498694400; // June 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1530230400;   // June 28, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1498694400; // June 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1530230400;   // June 28, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000318d732407b3fcacb");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = WTMINT_MAGIC_1;
        pchMessageStart[1] = WTMINT_MAGIC_2;
        pchMessageStart[2] = WTMINT_MAGIC_3;
        pchMessageStart[3] = WTMINT_MAGIC_4;
        nDefaultPort = WTMINT_SERVER_PORT;
        
        // TODO: AIB MERGE remove
        /*pchMessageStart[0] = 0x0f;
        pchMessageStart[1] = 0x68;
        pchMessageStart[2] = 0xc6;
        pchMessageStart[3] = 0xcb;
        nDefaultPort = 5223;*/
        
        nPruneAfterHeight = 100000;

        // TODO: AIB MERGE REMOVE
        genesis = CreateGenesisBlock(WTMINT_BLOCK_nTime, WTMINT_BLOCK_nNonce, WTMINT_BLOCK_nBits, 1, 0 * COIN);
        //genesis = CreateGenesisBlock(1405164774, 4016033, 0x1e01ffff, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        
        // TODO: AIB MERGE RECHECK mainnet suppose to generate following genesisblock from CreateGenesisBlock function so assert to validate.
        assert(consensus.hashGenesisBlock == uint256S(WTMINT_GENESIS_BLOCK));
        assert(genesis.hashMerkleRoot == uint256S(WTMINT_BLOCK_hashMerkleRoot));
        //assert(consensus.hashGenesisBlock == uint256S("0x4e9b54001f9976049830128ec0331515eaabe35a70970d79971da1539a400ba1"));
        //assert(genesis.hashMerkleRoot == uint256S("0x0317d32e01a2adf6f2ac6f58c7cdaab6c656edc6fdb45986c739290053275200"));
        

        // TODO: AIB MERGE RECHECK related to aib node check etc may need to remove
        // Note that of those with the service bits flag, most only support a subset of possible options
        // TODO - LED - Check which aib nodes support service bits and add the 'true' flag
        vSeeds.push_back(CDNSSeedData("wtmint.com", "seed.wtmint.com"));
        vSeeds.push_back(CDNSSeedData("iobond.com", "seed.iobond.com"));
        vSeeds.push_back(CDNSSeedData("aib.one", "seed.aib.one"));
        
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,WTMINT_PUBKEY_ADDRESS);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,WTMINT_SCRPIT_ADDRESS);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,WTMINT_SECRET_ADDRESS);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        
        // TODO: AIB MERGE RECHECK CRITICAL
        
        checkpointData = (CCheckpointData) {
		boost::assign::map_list_of
            (  0, uint256S("0x062550537871c387faaa5cd91bb8a2cf4b67d3a4ec816799ecc1f5cb7fd54bcc"))
            (  500000, uint256S("0xd7f084ecf92896c76a50cdc12ed6f7e9d35f3682f9e3e2faf65dddab13cffb81"))
            (  930000, uint256S("0x8f70c1b144d1d4c80373b2726375d2316e6112e06a828ac97c9f94b9572fbfd2"))
            (  935888, uint256S("0x8f1b9aee7f3df37e7c84fa44ebc86110097b18b537a62757fdb5e64fe8262387"))
            (  999000, uint256S("0xd2bb717d1bf8552e38ec4887007956286e63da7f1f7e2418704ace0670e28a3c"))    
            ( 1000000, uint256S("0x55b6dfafb9ad4683481d9c7d5523881c49135beea1775d1c7d6cff9de77bc759"))    
            ( 1100000, uint256S("0x71e7d2d8f748e0123d22d3c525bb8f5534a0903ea733dd219ad3ee3fa48b585f"))    
            ( 1144053, uint256S("0x61b6d34447bdea6a9a17ccddf2a38130114a4db584ecf7439aa7ad6d901003e4")),
            1414056601, // * UNIX timestamp of last checkpoint block
            154911,   // * total number of transactions between genesis and last checkpoint
			//   (the tx=... number in the SetBestChain debug.log lines)
            1500.0     // * estimated number of transactions per day after checkpoint
	};
        
        
	/*checkpointData = (CCheckpointData) {
		boost::assign::map_list_of
            ( 3901, uint256S("0x39c94020b653871bbcb29c9489bb12f84c5e89adef75f2e5c5f59e88869129d9"))
            ( 40821, uint256S("0xe0471675f9c98aa5ed321ed951d140d4603d96254a4ca9fbca07b8da5ee11954"))
            ( 41433, uint256S("0x627e18cc08a276282781705bac09508992dc8b665391edd7bde8a601f011954c"))
            ( 44606, uint256S("0x5ceeec38564a36ee3e1e5404970f5715efe0420e92c8e92bedfdfef782c49320"))
            ( 3470255, uint256S("0xcf8eb1b123884e9947482194e1f1e8ff4a4e9d4e92d9c9edccba71a10873de87")),
            1489654782, // * UNIX timestamp of last checkpoint block
            3821389,   // * total number of transactions between genesis and last checkpoint
			//   (the tx=... number in the SetBestChain debug.log lines)
			10000.0     // * estimated number of transactions per day after checkpoint
	};*/

    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 657000;
        consensus.nMajorityEnforceBlockUpgrade = 510;
        consensus.nMajorityRejectBlockOutdated = 750;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = -1;
        consensus.BIP34Hash = uint256S("0x0");
        consensus.powLimit = uint256S(WTMINT_POWLMT_nBits); // ~uint25(0) >> 19
        consensus.nPowTargetTimespan = WTMINT_BLOCK_TESTNET_nTargetTimespan;
                //3.5 * 24 * 60 * 60; // 3.5 days
        consensus.nPowTargetSpacing = WTMINT_BLOCK_nTargetSpacing;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 2700; // 75% of nMinerConfirmationWindow
        consensus.nMinerConfirmationWindow = 3600; // nPowTargetTimespan / nPowTargetSpacing
        consensus.nCLTVStartBlock = 502664;
        consensus.nBIP66MinStartBlock = 800000;
        consensus.nAuxPowStartHeight = AuxPow::START_TESTNET;
        consensus.nWitnessStartHeight = 4040000;

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1232032894; // start + (1year/25)

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1494547200; // May 12, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1526083200; // May 12, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1494547200; // May 12, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1526083200; // May 12, 2018

        // The best chain should have at least this much work.
        // consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000000006fce5d67766e");
        consensus.nMinimumChainWork = uint256S("0x0");

        // TODO: AIB MERGE RECHECK testnet magic number 
        pchMessageStart[0] = WTMINT_TESTNET_MAGIC_1;
        pchMessageStart[1] = WTMINT_TESTNET_MAGIC_2;
        pchMessageStart[2] = WTMINT_TESTNET_MAGIC_3;
        pchMessageStart[3] = WTMINT_TESTNET_MAGIC_4;
        nDefaultPort = WTMINT_TESTNET_RPC_PORT;
        nPruneAfterHeight = 1000;

        // TODO: AIB MERGE REMOVE
        //genesis = CreateGenesisBlock(1405168772, 262929, 0x1e1fffff, 1, 0 * COIN);
        genesis = CreateGenesisBlock(WTMINT_TESTNET_BLOCK_nTime, WTMINT_TESTNET_BLOCK_nNonce, WTMINT_BLOCK_nBits, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        
        //DEBUG("testnet hash", genesis.GetHash().GetHex());
        
        // TODO: AIB MERGE RECHECK
        // using old testnet genesis block for now.
        assert(consensus.hashGenesisBlock == uint256S(WTMINT_TESTNET_GENESIS_BLOCK));
        // assert(genesis.hashMerkleRoot == uint256S(WTMINT_BLOCK_hashMerkleRoot));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.push_back(CDNSSeedData("bootstrap-testnet.aib.net", "testnet.aib.net"));
        vSeeds.push_back(CDNSSeedData("aib.net", "seed-testnet.aib.net"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,127);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,255);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = true;


        checkpointData = (CCheckpointData) {
                boost::assign::map_list_of
                ( 4230, uint256S("0x15a29dde01cbad777180c089bc8fcf0d7b4bd18993b47d8c301c41fc90ce8c8f")),
                1405625749,
                4440,
                5000
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 7500;
        consensus.nMajorityRejectBlockOutdated = 9500;
        consensus.nMajorityWindow = 10000;
        consensus.BIP34Height = -1;
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S(WTMINT_REGTEST_POWLMT_nBits); // ~uint25(0) >> 1
        consensus.nPowTargetTimespan = WTMINT_BLOCK_nTargetTimespan; // !  two weeks
        consensus.nPowTargetSpacing = WTMINT_BLOCK_nTargetSpacing;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.nCLTVStartBlock = 1;
        consensus.nBIP66MinStartBlock = 1;
        consensus.nAuxPowStartHeight = AuxPow::START_REGTEST;
        consensus.nWitnessStartHeight = 20000;

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // TODO: AIB MERGE RECHECK
        pchMessageStart[0] = WTMINT_REGTEST_MAGIC_1;
        pchMessageStart[1] = WTMINT_REGTEST_MAGIC_2;
        pchMessageStart[2] = WTMINT_REGTEST_MAGIC_3;
        pchMessageStart[3] = WTMINT_REGTEST_MAGIC_4;
        nDefaultPort = WTMINT_REGTEST_RPC_PORT;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(WTMINT_REGTEST_BLOCK_nTime, WTMINT_REGTEST_BLOCK_nNonce, WTMINT_REGTEST_BLOCK_nBits, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        
        //DEBUG("regtest hash", genesis.GetHash().GetHex());
        // TODO: AIB MERGE RECHECK
        //assert(consensus.hashGenesisBlock == uint256S(WTMINT_GENESIS_BLOCK));
        //assert(genesis.hashMerkleRoot == uint256S(WTMINT_BLOCK_hashMerkleRoot));
        
        // TODO: AIB MERGE REMOVE
        assert(consensus.hashGenesisBlock == uint256S(WTMINT_REGTEST_GENESIS_BLOCK));
        /*assert(genesis.hashMerkleRoot == uint256S("0x97ddfbbae6be97fd6cdf3e7ca13232a3afff2353e29badfab7f73011edd4ced9"));*/

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        // TODO: AIB MERGE REMOVE
        /*checkpointData = (CCheckpointData){
         boost::assign::map_list_of
            ( 0, uint256S("0xf0dae070f24fbc35311533a22aa85c0a616c84a1f22881612304d802acda286f")),
            1405166035,
			0,
			0
	};*/
	checkpointData = (CCheckpointData){
        boost::assign::map_list_of
            ( 0, uint256S(WTMINT_GENESIS_BLOCK)),
            1405166035,
			0,
			0
	};

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
