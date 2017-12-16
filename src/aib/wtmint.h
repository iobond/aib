// Copyright (c) 2015-2015 The IOBond developers
// Copyright (c) 2014-2015 The WTMint developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


 

#ifndef WTMINT_H
#define WTMINT_H

#include "uint256.h"
#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <map>
#include "amount.h"

#define TO_GENESIS_BLOCK		false

// TODO: AIB MERGE
//#define CLIENT_VERSION_MAJOR       4
//#define CLIENT_VERSION_MINOR       1
//#define CLIENT_VERSION_REVISION    0
//#define CLIENT_VERSION_BUILD       0

// Move to amount.h
#define WTMINT_MAX_MONEY			 3141592653589793238
                                   // 271828182845904523  
// not yet found usage in migration
#define WTMINT_MAX_SEND				  429496729600000000
								   
//#define WTMINT_GENESIS_BLOCK	"0xc741436d354a9fb337fbd79e4b2750732571f00e8b78d04493f9e9283cf4ad0b" //keep the same length
// corresponding to consensus.h static const int COINBASE_MATURITY = 3600;
#define WTMINT_COINBASE_MATURITY 2

// Moved to Chainparams.h
//#define WTMINT_GENESIS_BLOCK	"0xf26d27dea5cbcc44df399960f0c2be03231a597ba8baed380f4df0fdea6d7592"
#define WTMINT_GENESIS_BLOCK	"0x062550537871c387faaa5cd91bb8a2cf4b67d3a4ec816799ecc1f5cb7fd54bcc"
#define WTMINT_pszTimestamp		"Asian Investment Bond By IOBond.com"
#define WTMINT_scriptPubKey		"04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f"

#define WTMINT_BLOCK_nTime			1428195753
#define WTMINT_BLOCK_nNonce			433798
#define WTMINT_BLOCK_nBits			0x1e0ffff0
#define WTMINT_BLOCK_hashMerkleRoot	"0x85f6c5fd0e39b5aa8688a844b283accef86973fcfdb7deac12ab2d4bebdc7c82"

#define WTMINT_TESTNET_GENESIS_BLOCK	"4843c056cccbd89dcc4324e2cae07051096f4c9fad0f8db49aa6f36363117bc2"
#define WTMINT_TESTNET_BLOCK_nTime			1405168772
#define WTMINT_TESTNET_BLOCK_nNonce			262929
// TestNet uses same nBits as main net - WTMINT_BLOCK_nBits

#define WTMINT_REGTEST_GENESIS_BLOCK	"ee03549187be5e68bc87256637458db42de8463bc05c7d8c659e728939da2e64"
#define WTMINT_REGTEST_BLOCK_nTime			1405166035
#define WTMINT_REGTEST_BLOCK_nNonce			0
#define WTMINT_REGTEST_BLOCK_nBits                      0x207fffff

#define WTMINT_BLOCK_nTargetTimespan 14400  //4 * 60 * 60 //  : every 4 hours
#define WTMINT_BLOCK_nTargetSpacing 60 //  : 1 minutes


//Magic Header - move to chainparams.h
#define WTMINT_MAGIC_1	0xa3
#define WTMINT_MAGIC_2	0xb1
#define WTMINT_MAGIC_3	0xc4
#define WTMINT_MAGIC_4	0xd1

#define WTMINT_TESTNET_MAGIC_1	0xa3
#define WTMINT_TESTNET_MAGIC_2	0xb1
#define WTMINT_TESTNET MAGIC_3	0xc4
#define WTMINT_TESTNET MAGIC_4	0xd1

#define WTMINT_REGTEST_MAGIC_1	0xa3
#define WTMINT_REGTEST_MAGIC_2	0xb1
#define WTMINT_REGTEST_MAGIC_3	0xc4
#define WTMINT_REGTEST_MAGIC_4	0xd1

//Wallet starting letter Moved to chainparams.cpp
#define WTMINT_PUBKEY_ADDRESS 23 // Dec.

//Moved to chainparams.h
#define WTMINT_RPC_PORT 51413
#define WTMINT_TESTNET_RPC_PORT 51423
#define WTMINT_REGTEST_RPC_PORT 21423

#define WTMINT_TESTNET_SERVER_PORT 18523
#define WTMINT_SERVER_PORT 31415

#define WTMINT_RPC_SUBJECT "AIB"

// Moved to blockheader.h
#define WTMINT_AUX_ChainID 0x0025  //Hex
// moved to consensus.h
#define WTMINT_AUX_StartBlock 3  // from block checking block 1 has 24xxxx block version so it's aux already
// moved to pow.cpp
#define WTMINT_KGW_StartBlock 6000


#define WTMINT_CHKPNT_LAST_TIMESTAMP 1414056601    // * UNIX timestamp of last checkpoint block
#define WTMINT_CHKPNT_TX_QUANTITY 154911		// * total number of transactions between genesis and last checkpoint  (the tx=... number in the SetBestChain debug.log lines)
#define WTMINT_CHKPNT_ESTIMATED_TX 1500   // * estimated number of transactions per day after checkpoint
 
//////////////////////////////////////
typedef long long  int64;
typedef unsigned long long  uint64;
typedef std::map<int, uint256> MapCheckpoints;

// GetBlockValue merged in GetBlockSubsidy in main.cpp
//////////////////////////////////////
		
class uint256;


/** Block-chain checkpoints are compiled-in sanity checks.
 * They are updated every release or three.
 
 mapCheckpoints = boost::assign::map_list_of

        (  0, uint256("0xc741436d354a9fb337fbd79e4b2750732571f00e8b78d04493f9e9283cf4ad0b"))
		(  100, uint256("0xf9360dcc30cb9cea913c1dfe03989560a466c8c6faa5948cd839637f3d5f12a0"))
		(  1000, uint256("0b6c05a423705b97c3ea30279506a1afdd583077f7650d02641d2a3a37a8ea15"))
		(  10000, uint256("973266a50b103d7057acbd07d49ca2b6f981fe534dae23438828221a65188639"))
		(  100000, uint256("95dc645ee15c791b94ec4b7076d7e3922325878767921c87f154b0327a6c7c7a"))
		(  133133, uint256("2f41bc8e8d5be6952d8d49f08c5cc2015d2b9a3f1c160116ef10f1a63355abbc"))
        (  150879, uint256("abda2abf9abc0bd24e9827c5e3795e254ad1dcfaf307a4386c5b20fedc3c2df9"))
	;
 
 */

// AIB MERGE v5
/* namespace WTMint
{
   
    int64 GetBlockValue(int diffTime,int nBits,int nHeight, int64 nFees, uint256 prevHash);
	
	static MapCheckpoints  mapCheckpoints = boost::assign::map_list_of

        (  0, uint256S("0x062550537871c387faaa5cd91bb8a2cf4b67d3a4ec816799ecc1f5cb7fd54bcc"))
        (  10000, uint256S("94e27efa6264d5adfcf30adcc0669b26c1ffcbdd3fff4ba70a804280ccee444a"))
        (  100000, uint256S("964ba54366fd2d867882ea6be03b3f8ee817a94e0b46b1d9dd3caa379d9ff118"))
        (  500000, uint256S("d7f084ecf92896c76a50cdc12ed6f7e9d35f3682f9e3e2faf65dddab13cffb81"))
        (  538888, uint256S("0120e15f267d25108a37d8b564246d72b505674061021b51d7e9b0a393444a19"))
        (  930000, uint256S("8f70c1b144d1d4c80373b2726375d2316e6112e06a828ac97c9f94b9572fbfd2"))
        (  930001, uint256S("801c3a1e84ed652bc9cdb6bfdc7d87f3ffe0131b602817d77227fc96e4609a54"))
        (  930002, uint256S("28b5eb89d0049b8ac5a21951cf11b48fb8a8570de19c8da745fa7de9fec67605"))
	;

}*/

namespace WTMint
{  
    int64 GetBlockValue(int diffTime,int nBits,int nHeight, int64 nFees, uint256 prevHash);
	
	static MapCheckpoints  mapCheckpoints = boost::assign::map_list_of

        (  0, uint256S("0x062550537871c387faaa5cd91bb8a2cf4b67d3a4ec816799ecc1f5cb7fd54bcc"))
        (  10000, uint256S("94e27efa6264d5adfcf30adcc0669b26c1ffcbdd3fff4ba70a804280ccee444a"))
        (  100000, uint256S("964ba54366fd2d867882ea6be03b3f8ee817a94e0b46b1d9dd3caa379d9ff118"))
        (  500000, uint256S("d7f084ecf92896c76a50cdc12ed6f7e9d35f3682f9e3e2faf65dddab13cffb81"))
        (  538888, uint256S("0120e15f267d25108a37d8b564246d72b505674061021b51d7e9b0a393444a19"))
        (  800000, uint256S("554f4d57e49345013c560235c8946e0ade0ce1c2a77f2c168986fc4b08b0a42c"))
        (  1000000, uint256S("55b6dfafb9ad4683481d9c7d5523881c49135beea1775d1c7d6cff9de77bc759"))
        (  1144019, uint256S("e8cdc4f7b5c17e464833b01047e2650cc441dc62b4954867f61f618a2a20d612"))
	;
}



#endif

