// Copyright (c) 2009-2010 Satoshi Nakamoto
// Distributed under the MIT/X11 software license, see the accompanying
// file license.txt or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_AUXPOW_AUXPOW_H
#define BITCOIN_AUXPOW_AUXPOW_H

#include <cstring>
#include <memory>
#include <consensus/params.h>
#include <primitives/transaction.h>
#include <primitives/block.h>
#include <serialize.h>
#include <uint256.h>
#include <auxpow/consensus.h>


class CAuxPow
{
public:
    CAuxPow(CTransactionRef txIn) : coinbaseTx(txIn)
    {
    }

    CAuxPow() : coinbaseTx(MakeTransactionRef())
    {
    }

    // Coinbase transaction from parent chain
    CTransactionRef coinbaseTx;

    // Merkle branch and block hash from parent chain (replaces old CMerkleTx fields)
    uint256 hashBlock;
    std::vector<uint256> vMerkleBranch;
    int nIndex;

    // Merkle branch with root vchAux
    // root must be present inside the coinbase
    std::vector<uint256> vChainMerkleBranch;

    // Index of chain in chains merkle tree
    unsigned int nChainIndex;
    CBlockHeader parentBlockHeader;

    SERIALIZE_METHODS(CAuxPow, obj) {
        READWRITE(TX_WITH_WITNESS(obj.coinbaseTx), obj.hashBlock, obj.vMerkleBranch, obj.nIndex);
        READWRITE(obj.vChainMerkleBranch, obj.nChainIndex);
        READWRITE(obj.parentBlockHeader);
    }

    uint256 CheckMerkleBranch(const uint256& hash, const std::vector<uint256>& vMerkleBranch, int nIndex) const;
    bool Check(const uint256& hashAuxBlock, int nChainID, const Consensus::Params& params) const;

    inline uint256 GetParentBlockHash()
    {
            return parentBlockHeader.GetPoWHash();
//            return uint256();
    }
};

template<typename Stream> void SerReadWrite(Stream& s, std::shared_ptr<CAuxPow>& pobj, int nType, int nVersion, CSerActionSerialize ser_action)
{
    if (nVersion & AuxPow::BLOCK_VERSION_AUXPOW){
        ::Serialize(s, *pobj, nType, nVersion);
    }
}

template<typename Stream> void SerReadWrite(Stream& s, std::shared_ptr<CAuxPow>& pobj, int nType, int nVersion, CSerActionUnserialize ser_action)
{
    if (nVersion & AuxPow::BLOCK_VERSION_AUXPOW){
        pobj.reset(new CAuxPow());
        ::Unserialize(s, *pobj, nType, nVersion);
    } else
        pobj.reset();
}


extern void RemoveMergedMiningHeader(std::vector<unsigned char>& vchAux);
extern int GetAuxPowStartBlock(const Consensus::Params& params);
extern CKeyID GetAuxpowMiningKey(void);

#endif
