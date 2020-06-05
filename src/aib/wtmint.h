// Copyright (c) 2014-2018 The AIB developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WTMINT_H
#define WTMINT_H

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
#define WTMINT_TESTNET_MAGIC_3	0xc4
#define WTMINT_TESTNET_MAGIC_4	0xd1

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

#endif /* WTMINT_H */

