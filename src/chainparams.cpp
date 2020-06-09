// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The Bitcoin Core developers
// Copyright (c) 2013-2020 The AIB Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>

#include <chainparamsseeds.h>

//#define WTMINT_GENESIS_BLOCK	"0xf26d27dea5cbcc44df399960f0c2be03231a597ba8baed380f4df0fdea6d7592"
#define WTMINT_GENESIS_BLOCK	"0x062550537871c387faaa5cd91bb8a2cf4b67d3a4ec816799ecc1f5cb7fd54bcc"
#define WTMINT_pszTimestamp		"Asian Investment Bond By IOBond.com"
#define WTMINT_scriptPubKey		"04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f"

#define WTMINT_BLOCK_nTime			1428195753
#define WTMINT_BLOCK_nNonce			433798
#define WTMINT_BLOCK_nBits			0x1e0ffff0
#define WTMINT_POWLMT_nBits         "00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
#define WTMINT_BLOCK_hashMerkleRoot	"0x85f6c5fd0e39b5aa8688a844b283accef86973fcfdb7deac12ab2d4bebdc7c82"

#define WTMINT_TESTNET_GENESIS_BLOCK	    "e5d9b5c45d7b5a5c27c6033ee1aa1583827cab50ebed951153d12777e432d10b"
#define WTMINT_TESTNET_BLOCK_hashMerkleRoot "85f6c5fd0e39b5aa8688a844b283accef86973fcfdb7deac12ab2d4bebdc7c82"
        
#define WTMINT_TESTNET_BLOCK_nTime			1540597459 //2018-10-26 4:44
#define WTMINT_TESTNET_BLOCK_nNonce			262929
// TestNet uses same nBits as main net - WTMINT_BLOCK_nBits

#define WTMINT_REGTEST_GENESIS_BLOCK	"ee03549187be5e68bc87256637458db42de8463bc05c7d8c659e728939da2e64"
#define WTMINT_REGTEST_BLOCK_nTime		1405166035
#define WTMINT_REGTEST_BLOCK_nNonce	    0
#define WTMINT_REGTEST_POWLMT_nBits     "7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
#define WTMINT_REGTEST_BLOCK_nBits      0x207fffff

#define WTMINT_RPC_PORT         51413
#define WTMINT_TESTNET_RPC_PORT 51423
#define WTMINT_REGTEST_RPC_PORT 51433

#define WTMINT_TESTNET_SERVER_PORT  31425
#define WTMINT_SERVER_PORT          31415

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

#define WTMINT_BLOCK_nTargetTimespan            14400  //4 * 60 * 60 //  : every 4 hours
#define WTMINT_BLOCK_TESTNET_nTargetTimespan    3600  //1 * 60 * 60 //  : every 1 hours
#define WTMINT_BLOCK_nTargetSpacing             60 //  : 1 minutes

#define WTMINT_ENFORCE_BIP34_HEIGHT 2400000 // TODO: AIB MERGE RECEHCK change to actual number after release 
#define WTMINT_ENFORCE_BIP66_HEIGHT 2400000 // TODO: AIB MERGE RECEHCK change to actual number after release 

//Wallet starting letter
#define WTMINT_PUBKEY_ADDRESS 23    // Dec.  Start with Capital A
#define WTMINT_SCRPIT_ADDRESS 83    // Dec.  Start with Smallcase a
#define WTMINT_SECRET_ADDRESS 151   // Dec.

#define WTMINT_TESTNET_PUBKEY_ADDRESS 65 // Dec.
#define WTMINT_TESTNET_SECRET_ADDRESS 255 // Dec.

#define WTMINT_REGTEST_PUBKEY_ADDRESS 49 // Dec.
#define WTMINT_REGTEST_SECRET_ADDRESS 239 // Dec.
                                  
#define WTMINT_TESTNET_MINIMUM_CHAINWORK    "000000000000000000000000000000000000000000000000f0d281707ee29c85"
#define WTMINT_MINIMUM_CHAINWORK            "00000000000000000000000000000000000000000000000020664ecd44b439d2" //height 1216821
#define WTMINT_DEFAULT_ASSUMVALID           "0x61b6d34447bdea6a9a17ccddf2a38130114a4db584ecf7439aa7ad6d901003e4"

#define DEBUG(title,strout) std::cout<<title<<":    "<< strout<<"\n"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);

    //genesis.nVersion = 1;
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
    const char* pszTimestamp = WTMINT_pszTimestamp;
    const CScript genesisOutputScript = CScript() << ParseHex(WTMINT_scriptPubKey) << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
	    // TODO: AIB MERGE REMOVE
    /*const char* pszTimestamp = "NY Times 05/Oct/2011 Steve Jobs, Apple�s Visionary, Dies at 56";
    const CScript genesisOutputScript = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
    */
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
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
        /*consensus.nSubsidyHalvingInterval = 657000;
        consensus.nMajorityEnforceBlockUpgrade = 15000;
        consensus.nMajorityRejectBlockOutdated = 19000;
        consensus.nMajorityWindow = 20000;*/
        consensus.BIP16Exception = uint256S("0x00000000000002dc756eebf4f49723ed8d30cc28a5f108eb94b1ba88ac4f9c22");
        consensus.BIP34Height = WTMINT_ENFORCE_BIP34_HEIGHT;
        consensus.BIP34Hash = uint256S(WTMINT_GENESIS_BLOCK);
	
        //TODO: AIB Merge Recheck
        //consensus.BIP65Height = 388381; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        //consensus.BIP66Height = 363725; // 00000000000000000379eaa19dce8c9b722d46ae6a57c2f1a988119488b50931
        consensus.powLimit = uint256S(WTMINT_POWLMT_nBits);
        consensus.nPowTargetTimespan = WTMINT_BLOCK_nTargetTimespan; // two weeks
        consensus.nPowTargetSpacing = WTMINT_BLOCK_nTargetSpacing;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
	    consensus.nRuleChangeActivationThreshold = 8100; // 75% of nMinerConfirmationWindow
        //consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 10800; // 3 days BIP 9 soft fork related
         
	    /* TODO: AIB Merge Recheck Aux support
	    consensus.nCLTVStartBlock = 598725;
        consensus.nBIP66MinStartBlock = WTMINT_ENFORCE_BIP66_HEIGHT;
        */
        consensus.nAuxPowStartHeight = AuxPow::START_MAINNET;
        /* consensus.nWitnessStartHeight = 4040000;
		*/
	
	
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 6; // TODO: AIB Merge Recheck 6 for AIB value
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
	    consensus.nMinimumChainWork = uint256S(WTMINT_MINIMUM_CHAINWORK);
        	
        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S(WTMINT_DEFAULT_ASSUMVALID); //1144053

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
		
        nPruneAfterHeight = 100000;

	    genesis = CreateGenesisBlock(WTMINT_BLOCK_nTime, WTMINT_BLOCK_nNonce, WTMINT_BLOCK_nBits, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        
        // TODO: AIB MERGE RECHECK mainnet suppose to generate following genesisblock from CreateGenesisBlock function so assert to validate.
        assert(consensus.hashGenesisBlock == uint256S(WTMINT_GENESIS_BLOCK));
        assert(genesis.hashMerkleRoot == uint256S(WTMINT_BLOCK_hashMerkleRoot));
        
        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        
         vSeeds.emplace_back("seed.aib.one");
         vSeeds.emplace_back("seed.aib.cash");
         vSeeds.emplace_back("seed.iobond.com");
       
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,WTMINT_PUBKEY_ADDRESS);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,WTMINT_SCRPIT_ADDRESS);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,WTMINT_SECRET_ADDRESS);
	    base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};
				
	    bech32_hrp = "aib";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

	    //fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        
	//TODO: AIB Merge Recheck AIB specific code
	//fSkipProofOfWorkCheck = false;
	
        //fTestnetToBeDeprecatedFieldRPC = false;

        // TODO: AIB MERGE RECHECK CRITICAL      
        checkpointData = {
            {  
                {       0, uint256S("0x062550537871c387faaa5cd91bb8a2cf4b67d3a4ec816799ecc1f5cb7fd54bcc")},
                {  225020, uint256S("0xdea0bb494b8884893e3a1065ce39878b5fec42e2ce85a972d2bfeadb380353b3")},
                {  930000, uint256S("0x8f70c1b144d1d4c80373b2726375d2316e6112e06a828ac97c9f94b9572fbfd2")},
                {  935888, uint256S("0x8f1b9aee7f3df37e7c84fa44ebc86110097b18b537a62757fdb5e64fe8262387")},
                {  999000, uint256S("0xd2bb717d1bf8552e38ec4887007956286e63da7f1f7e2418704ace0670e28a3c")},    
                { 1126570, uint256S("0x6621385447902763a41ed49c771270f506b06c23667c3885485c3162a514b440")},    
                { 1154619, uint256S("0xb3df98957ccfeb9da0eb5ef929cf929ae9a6bf1917f79578688b55c25cf6b67d")},    
                { 1213390, uint256S("0x098ed783d5cf8428215686ddef1a88ba372d58b28f3f571a437f15bbaae59d55")},
                { 1216820, uint256S("0xe69b85a94471d42b254be77062f65edba84f3a5972a1d2a4bb8b14e1526cef52")}, 
                // 1513895220, // * UNIX timestamp of last checkpoint block
                // 1283706,   // * total number of transactions between genesis and last checkpoint   
                { 1224490, uint256S("0xc20d8dab92dc16854d6cddefe9ce7905aa18b0ad949cc7848dc7008338641c4c")},    
                { 2192860, uint256S("0x0916d22a70d1b934a31934a2ea1482604c594bf3a1a9e58a73a627fde91dfebd")},    
                { 2210000, uint256S("0x4f58bae8c6f572e7821bd2d3e6c66f2f82dce048b31302ef44f8ca8fd9ac139f")},
                { 2260000, uint256S("0xd04a48d6324eaf14e86c9de975eccf32449d6d6a3beb595868fdd273af1d9502")},    
                { 2300000, uint256S("0x98bdbd3a6fa9d9daa714b3197506e905f0802866ef1bd8c669aba20ec19dfc1c")},    
                { 2358115, uint256S("0x2eb558ba005a7f3ff2cbcb2ce3a1dc1384c824f9ac401aa4c3fa1c9ebe19f44e")},
            }            
	    };
	
 	    chainTxData = ChainTxData{
            // Data from rpc: getchaintxstats 4096 0000000000000000002e63058c023a9a1de233554f28c7b21380b6c9003f36a8
            /* nTime    */ 1591084557,
            /* nTxCount */ 2475276,
            /* dTxRate  */ 1440.0
        };
        
        /* disable fallback fee on mainnet */
        m_fallback_fee_enabled = false;
    }
};

class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 657000;
        consensus.BIP16Exception = uint256S("0x00000000dd30457c001f4095d208cc1296b0eed002427aa599874af7a432b105");
        consensus.BIP34Height = 21111;
        consensus.BIP34Hash = uint256S(WTMINT_TESTNET_GENESIS_BLOCK);
        ///consensus.BIP65Height = 581885; // 00000000007f6655f22f98e72ed80d8b06dc761d5da09df0fa1dc4be4f861eb6
        //consensus.BIP66Height = 330776; // 000000002104c8c45e99a8853285a3b592602a3ccde2b832481da85e9e4ba182
        consensus.powLimit = uint256S(WTMINT_POWLMT_nBits);
        consensus.nPowTargetTimespan = WTMINT_BLOCK_nTargetTimespan;
        consensus.nPowTargetSpacing = WTMINT_BLOCK_nTargetSpacing;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 10800; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1456790400; // March 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1462060800; // May 1st 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1493596800; // May 1st 2017

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S(WTMINT_TESTNET_MINIMUM_CHAINWORK);

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S(WTMINT_DEFAULT_ASSUMVALID);

        pchMessageStart[0] = WTMINT_TESTNET_MAGIC_1;
        pchMessageStart[1] = WTMINT_TESTNET_MAGIC_2;
        pchMessageStart[2] = WTMINT_TESTNET_MAGIC_3;
        pchMessageStart[3] = WTMINT_TESTNET_MAGIC_4;
        nDefaultPort = WTMINT_TESTNET_SERVER_PORT;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(WTMINT_TESTNET_BLOCK_nTime, WTMINT_TESTNET_BLOCK_nNonce, WTMINT_BLOCK_nBits, 1, 0 * COIN);
        //printf("Testnet genesis: %s\n",genesis.GetHash().GetHex().c_str());
        //printf("Testnet merkleroot: %s\n",genesis.hashMerkleRoot.GetHex().c_str());
        //printf("Testnet minmum chain of work: %s\n",genesis.GetPoWHash().GetHex().c_str());
        
        //LogPrint(BCLog::NET,"Genesis: %s\n", genesis.ToString().c_str());
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S(WTMINT_TESTNET_GENESIS_BLOCK));
        assert(genesis.hashMerkleRoot == uint256S(WTMINT_TESTNET_BLOCK_hashMerkleRoot));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("seed.testnet-wtmint.com");
        vSeeds.emplace_back("seed.testnet-iobond.com");
        vSeeds.emplace_back("seed.testnet-aib.one");
        vSeeds.emplace_back("testnet-seed.aib.one"); // Just a static list of stable node(s), only supports x9

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,WTMINT_TESTNET_PUBKEY_ADDRESS);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,WTMINT_SCRPIT_ADDRESS);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,WTMINT_TESTNET_SECRET_ADDRESS);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tb";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;


        checkpointData = {
            {
	    }
	};
	
	chainTxData = ChainTxData{
            // Data from rpc: getchaintxstats 4096 0000000000000037a8cd3e06cd5edbfe9dd1dbcc5dacab279376ef7cfc2b4c75
            /* nTime    */ 1531929919,
            /* nTxCount */ 1,
            /* dTxRate  */ 0.626,
        };

        /* enable fallback fee on testnet */
        m_fallback_fee_enabled = true;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S(WTMINT_REGTEST_POWLMT_nBits);
        consensus.nPowTargetTimespan = WTMINT_BLOCK_nTargetTimespan; // two weeks
        consensus.nPowTargetSpacing = WTMINT_BLOCK_nTargetSpacing;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = WTMINT_REGTEST_MAGIC_1;
        pchMessageStart[1] = WTMINT_REGTEST_MAGIC_2;
        pchMessageStart[2] = WTMINT_REGTEST_MAGIC_3;
        pchMessageStart[3] = WTMINT_REGTEST_MAGIC_4;
        nDefaultPort = WTMINT_REGTEST_RPC_PORT;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(WTMINT_REGTEST_BLOCK_nTime, WTMINT_REGTEST_BLOCK_nNonce, WTMINT_REGTEST_BLOCK_nBits, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        //assert(consensus.hashGenesisBlock == uint256S("0x0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"));
        //assert(genesis.hashMerkleRoot == uint256S("0x4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = {
            {
                {0, uint256S(WTMINT_REGTEST_GENESIS_BLOCK)},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,WTMINT_REGTEST_PUBKEY_ADDRESS);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,WTMINT_SCRPIT_ADDRESS);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,WTMINT_REGTEST_SECRET_ADDRESS);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "bcrt";

        /* enable fallback fee on regtest */
        m_fallback_fee_enabled = true;
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}

