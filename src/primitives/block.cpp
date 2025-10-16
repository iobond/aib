// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>

#include <hash.h>
#include <tinyformat.h>
#include <crypto/scrypt.h>
#include <streams.h>

uint256 CBlockHeader::GetHash() const
{
    return (HashWriter{} << *this).GetHash();
}

uint256 CBlockHeader::GetPoWHash() const
{
    // AIB: Serialize block header and compute scrypt hash
    DataStream ss{};
    ss << *this;

    // Block header should be 80 bytes
    assert(ss.size() == 80);

    uint256 thash;
    scrypt_1024_1_1_256((const char*)&ss[0], (char*)thash.begin());
    return thash;
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}
