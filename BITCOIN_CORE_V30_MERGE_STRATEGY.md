# Bitcoin Core v30.0 Merge Strategy

## Overview

This document outlines the strategy for merging Bitcoin Core v30.0 into AIB Core while preserving AIB's unique features.

## Current State

- **AIB Core Version**: v5.13.3.14 (based on Bitcoin Core v0.13.x)
- **Target**: Bitcoin Core v30.0
- **Version Jump**: ~17 major versions (v0.13 → v30.0)
- **Branch**: `bitcoin-core-v30-merge`

## AIB-Specific Features to Preserve

###  1. Auxpow (Auxiliary Proof of Work)
**Location**: `src/auxpow/`
**Files**:
- `src/auxpow/auxpow.cpp`
- `src/auxpow/auxpow.h`
- `src/auxpow/consensus.h`
- `src/auxpow/serialize.h`

**Integration Points**:
- `src/primitives/block.h` - Block header with auxpow support
- `src/primitives/block.cpp` - Auxpow serialization
- `src/main.cpp` - Consensus validation with auxpow
- `src/miner.cpp` - Mining with auxpow support
- `src/pow.cpp` - Proof of work validation
- `src/rpc/mining.cpp` - RPC commands for auxpow mining
- `src/rpc/blockchain.cpp` - Blockchain RPC with auxpow data
- `src/chain.h/cpp` - Chain index with auxpow
- `src/txdb.h/cpp` - Database with auxpow

**Critical**: Auxpow enables merge mining compatibility with other chains.

### 2. Scrypt Algorithm
**Location**: `src/crypto/`
**Files**:
- `src/crypto/scrypt.cpp`
- `src/crypto/scrypt.h`
- `src/crypto/scrypt-sse2.cpp`

**Integration Points**:
- `src/primitives/block.cpp` - Block header hashing with scrypt
- `src/miner.cpp` - Mining with scrypt
- `src/init.cpp` - Scrypt initialization
- `src/test/scrypt_tests.cpp` - Unit tests

**Critical**: Scrypt is AIB's proof-of-work algorithm, different from Bitcoin's SHA256d.

### 3. AIB Branding
**Files to Update**:
- All references to "Bitcoin" → "AIB"
- Binary names: `bitcoind` → `aibd`, `bitcoin-cli` → `aib-cli`, `bitcoin-qt` → `aib-qt`
- RPC commands and help text
- GUI strings and translations
- Version strings
- Copyright notices (preserve AIB copyright)

**Configuration**:
- `configure.ac`: CLIENT_VERSION, binary names
- Port numbers (if different from Bitcoin)
- Network magic bytes
- Genesis block
- Default data directory

## Bitcoin Core v30.0 Major Changes

### Key Improvements (v0.13 → v30.0):
1. **SegWit** (v0.13.1+): Segregated Witness
2. **Taproot** (v22.0+): Schnorr signatures, MAST
3. **PSBT** (v0.17+): Partially Signed Bitcoin Transactions
4. **Descriptor Wallets** (v0.21+): Output script descriptors
5. **P2P Encryption** (v26.0+): BIP324 v2 transport
6. **TRUC Transactions** (v28.0+): Topologically Restricted Until Confirmation
7. **AssumeUTXO** (v28.0+): Fast sync with UTXO snapshots
8. **Testnet4** (v28.0+): New test network
9. **CMake Build System** (v28.0+): Alternative to autotools
10. **Modern C++**: C++17 → C++20

### Build System Changes:
- CMake support (v28.0+)
- Updated dependencies (Boost, Qt, OpenSSL, etc.)
- Modern compiler requirements (GCC 11+, Clang 16+)
- Removed Berkeley DB (optional descriptor wallets)

### API Changes:
- RPC interface updates
- ZMQ notification changes
- REST API updates
- Configuration option changes

## Merge Strategy

### Phase 1: Preparation (Current)
1. ✅ Create branch `bitcoin-core-v30-merge`
2. ✅ Fetch Bitcoin Core v30.0
3. ✅ Document AIB-specific features
4. ⏳ Extract AIB-specific patches
5. ⏳ Create compatibility layer design

### Phase 2: Initial Merge
1. Checkout Bitcoin Core v30.0 as base
2. Apply AIB branding changes
3. Re-integrate auxpow module
4. Re-integrate scrypt algorithm
5. Update build system for AIB features

### Phase 3: Feature Integration
1. Port auxpow consensus rules to new validation framework
2. Port scrypt to new crypto infrastructure
3. Update RPC commands for auxpow support
4. Update GUI for AIB branding
5. Migrate wallet format (if needed)

### Phase 4: Testing
1. Unit tests for auxpow and scrypt
2. Functional tests for merge mining
3. Regression tests
4. Performance benchmarks
5. Network compatibility testing

### Phase 5: Documentation
1. Update README with v30.0 changes
2. Document breaking changes
3. Update build instructions
4. Create migration guide
5. Update API documentation

## Challenges and Risks

### High Risk:
1. **Consensus Changes**: Any error in porting auxpow or scrypt could fork the chain
2. **Database Format**: Wallet and blockchain database migrations
3. **Network Protocol**: P2P protocol changes may break compatibility
4. **Build Dependencies**: Significantly updated requirements

### Medium Risk:
1. **RPC API Changes**: May break existing tools and integrations
2. **Configuration Changes**: May require user intervention
3. **Performance**: New features may impact resource usage

### Low Risk:
1. **GUI Changes**: Cosmetic updates to branding
2. **Documentation**: Can be updated iteratively

## Testing Strategy

### Pre-Release Testing:
1. **Testnet Deployment**: Deploy on AIB testnet first
2. **Community Testing**: Beta releases for community feedback
3. **Mining Pool Testing**: Test auxpow with actual pools
4. **Exchange Testing**: Verify wallet compatibility

### Automated Testing:
1. Unit tests for all ported code
2. Functional tests for core features
3. Integration tests for RPC and P2P
4. Continuous integration on GitHub Actions

## Rollback Plan

If critical issues are discovered:
1. Keep v5.13.3.14 as stable branch
2. Document all issues found
3. Create patch releases for v5.13.3.14 if needed
4. Delay v30.0 release until issues resolved

## Timeline Estimate

Given the scope of changes:
- **Phase 1 (Preparation)**: 1-2 days
- **Phase 2 (Initial Merge)**: 3-5 days
- **Phase 3 (Feature Integration)**: 5-10 days
- **Phase 4 (Testing)**: 7-14 days
- **Phase 5 (Documentation)**: 2-3 days

**Total**: 3-6 weeks for complete merge and testing

## Success Criteria

The merge is successful when:
1. ✅ All unit tests pass
2. ✅ All functional tests pass
3. ✅ Auxpow merge mining works correctly
4. ✅ Scrypt proof-of-work validates correctly
5. ✅ AIB network maintains consensus
6. ✅ No security regressions
7. ✅ Performance is acceptable
8. ✅ Builds on all target platforms (Linux, Windows, macOS)
9. ✅ Wallet migration works without loss
10. ✅ Community testing reveals no critical issues

## Next Steps

1. Create detailed patch files for AIB-specific changes
2. Analyze Bitcoin Core v30.0 code structure
3. Design compatibility layer for auxpow and scrypt
4. Begin incremental merge process
5. Set up testing infrastructure

## Notes

- This is a **major undertaking** - proceed with caution
- All consensus changes must be **thoroughly tested**
- Maintain **backward compatibility** where possible
- Keep **security** as top priority
- **Document everything** for future maintainability

## References

- Bitcoin Core v30.0 Release Notes: https://github.com/bitcoin/bitcoin/blob/master/doc/release-notes/release-notes-30.0.md
- AIB Current Version: v5.13.3.14
- Branch: `bitcoin-core-v30-merge`
- Testing Branch: Will be pushed to GitHub for community review
