# Bitcoin Core v30.0 → AIB Proof of Concept Merge

## Status: INITIAL INTEGRATION COMPLETE ✅

This document describes the proof-of-concept merge of Bitcoin Core v30.0 with AIB's unique features (auxpow, scrypt, branding).

## What Has Been Done

### ✅ Phase 1: Foundation (Completed)
1. **Created merge branch**: `bitcoin-core-v30-merge`
2. **Fetched Bitcoin Core v30.0**: All source code from official Bitcoin repository
3. **Backed up AIB features**:
   - Auxpow module (`src/auxpow/`)
   - Scrypt algorithm (`src/crypto/scrypt*`)
4. **Created Bitcoin Core v30.0 base**: Clean checkout as foundation

### ✅ Phase 2: Feature Integration (Completed)
1. **Restored Auxpow Module**:
   - Location: `src/auxpow/`
   - Files: `auxpow.cpp`, `auxpow.h`, `consensus.h`, `serialize.h`
   - Integrated into: `bitcoin_consensus` library in CMake

2. **Restored Scrypt Algorithm**:
   - Location: `src/crypto/`
   - Files: `scrypt.cpp`, `scrypt.h`, `scrypt-sse2.cpp`
   - Integrated into: `bitcoin_crypto` library in CMake
   - SSE2 optimization: Conditional compilation support added

3. **Updated Build System**:
   - Modified `src/CMakeLists.txt`: Added auxpow to consensus library
   - Modified `src/crypto/CMakeLists.txt`: Added scrypt and scrypt-sse2
   - Conditional SSE2 compilation for scrypt performance

## Current State

### What Works:
- ✅ Bitcoin Core v30.0 codebase as foundation
- ✅ Auxpow module physically present
- ✅ Scrypt algorithm physically present
- ✅ CMake build system updated to include both

### What Needs Work:

#### 🔧 Critical - Consensus Integration (HIGH PRIORITY)
The auxpow and scrypt modules are present but not yet integrated into the consensus code:

1. **Block Header Changes** (`src/primitives/block.h/cpp`):
   - Add auxpow data structure to CBlockHeader
   - Add auxpow serialization/deserialization
   - Update GetHash() to use scrypt instead of SHA256d

2. **Proof-of-Work Validation** (`src/pow.cpp`):
   - Integrate scrypt hashing for block verification
   - Add auxpow validation logic
   - Update CheckProofOfWork() function

3. **Consensus Rules** (`src/validation.cpp`):
   - Add auxpow consensus checks
   - Update block validation with auxpow rules
   - Preserve AIB's consensus parameters

4. **Mining** (`src/node/miner.cpp`):
   - Update block template creation for auxpow
   - Add merge mining support
   - Integrate scrypt for mining

5. **RPC Interface** (`src/rpc/blockchain.cpp`, `src/rpc/mining.cpp`):
   - Add getauxblock RPC command
   - Add auxpow-specific RPCs
   - Update getblocktemplate for merge mining

#### 🎨 Important - Branding (MEDIUM PRIORITY)
Update Bitcoin branding to AIB throughout:

1. **Binary Names**:
   - `bitcoind` → `aibd`
   - `bitcoin-cli` → `aib-cli`
   - `bitcoin-qt` → `aib-qt`
   - `bitcoin-tx` → `aib-tx`
   - `bitcoin-wallet` → `aib-wallet`

2. **Configuration**:
   - Update `configure.ac` or CMake project name
   - Change default ports if different
   - Update network magic bytes
   - Set AIB genesis block

3. **Documentation**:
   - Update README.md
   - Update help text
   - Update GUI strings

4. **Version Information**:
   - Set AIB version numbers
   - Update copyright notices

#### 🧪 Testing (HIGH PRIORITY)
Before any production use:

1. **Compilation Test**:
   - Attempt to build with CMake
   - Fix compilation errors
   - Resolve dependency issues

2. **Unit Tests**:
   - Test auxpow functionality
   - Test scrypt hashing
   - Create new tests for AIB features

3. **Functional Tests**:
   - Test merge mining
   - Test network compatibility
   - Test wallet functionality

4. **Integration Tests**:
   - Test on AIB testnet
   - Verify consensus compatibility
   - Test with mining pools

## File Changes Summary

### New Files Added:
```
src/auxpow/
├── auxpow.cpp
├── auxpow.h
├── consensus.h
└── serialize.h

src/crypto/
├── scrypt.cpp
├── scrypt.h
└── scrypt-sse2.cpp

Documentation/
├── BITCOIN_CORE_V30_MERGE_STRATEGY.md
├── V30_MERGE_STATUS.md
└── PROOF_OF_CONCEPT_SUMMARY.md (this file)
```

### Modified Files:
```
src/CMakeLists.txt              - Added auxpow to consensus library
src/crypto/CMakeLists.txt       - Added scrypt to crypto library
```

### Files Needing Modification (Not Yet Done):
```
src/primitives/block.h          - Add auxpow support
src/primitives/block.cpp        - Implement auxpow serialization
src/pow.cpp                     - Integrate scrypt and auxpow validation
src/validation.cpp              - Add auxpow consensus rules
src/node/miner.cpp              - Update mining for auxpow
src/rpc/blockchain.cpp          - Add auxpow RPCs
src/rpc/mining.cpp              - Add getauxblock RPC
src/chainparams.cpp             - Set AIB parameters
CMakeLists.txt (root)           - Update project name and version
... and many more for complete integration
```

## Next Steps

### Immediate (For Testing):
1. **Attempt Compilation**:
   ```bash
   cmake -B build
   cmake --build build
   ```
   - Expect errors - this is normal for POC
   - Document compilation errors
   - Create list of required fixes

2. **Review Integration Points**:
   - Identify all locations where auxpow/scrypt need integration
   - Create detailed task list
   - Prioritize by criticality

3. **Community Review**:
   - Push this branch to GitHub
   - Get feedback on approach
   - Discuss integration strategy

### Short Term (1-2 Weeks):
1. Integrate auxpow into primitives/block
2. Integrate scrypt into pow validation
3. Update consensus rules for auxpow
4. Update mining code
5. Add RPC commands
6. Achieve successful compilation

### Medium Term (2-4 Weeks):
1. Complete branding updates
2. Write comprehensive tests
3. Test on AIB testnet
4. Fix bugs and edge cases
5. Performance optimization

### Long Term (1-2 Months):
1. Security audit
2. Community testing
3. Mining pool integration testing
4. Exchange compatibility verification
5. Production release preparation

## Risks and Challenges

### High Risk:
- ⚠️ **Consensus Breaking**: Any error in auxpow or scrypt could fork the chain
- ⚠️ **Data Corruption**: Improper serialization could corrupt blocks
- ⚠️ **Network Incompatibility**: Changes might break P2P protocol

### Medium Risk:
- ⚠️ **Compilation Errors**: Many integration points need updates
- ⚠️ **API Changes**: Bitcoin Core v30 has different APIs than v0.13
- ⚠️ **Dependency Issues**: Modern requirements may conflict

### Mitigations:
- ✅ Extensive testing on testnet before mainnet
- ✅ Gradual rollout with monitoring
- ✅ Maintain v5.13.3.14 as stable fallback
- ✅ Community review and feedback

## Success Criteria

This POC is successful if:
- [x] Bitcoin Core v30.0 base established
- [x] Auxpow module present
- [x] Scrypt algorithm present
- [x] Build system updated
- [ ] Code compiles successfully
- [ ] Basic tests pass
- [ ] Auxpow validation works
- [ ] Scrypt hashing works
- [ ] Network maintains consensus

## Compilation Status

**Not Yet Attempted** - This is a code integration POC showing the approach. Compilation will reveal integration issues that need to be addressed.

Expected issues:
- Missing includes in auxpow/scrypt files
- API incompatibilities with v30.0 structures
- Namespace conflicts
- Type mismatches

These are normal and expected for a merge of this magnitude.

## How to Use This Branch

### For Review:
```bash
git clone https://github.com/iobond/aib.git
git checkout bitcoin-core-v30-merge
# Review the integration approach
# Provide feedback on GitHub
```

### For Development:
```bash
git checkout bitcoin-core-v30-merge
# Start fixing compilation errors
# Integrate auxpow into block.h/cpp
# Integrate scrypt into pow.cpp
# Test incrementally
```

### For Testing:
```bash
# Once compilation succeeds:
cmake -B build -DBUILD_TESTS=ON
cmake --build build
ctest --test-dir build
```

## Conclusion

This proof-of-concept demonstrates:
1. ✅ Bitcoin Core v30.0 can serve as a base
2. ✅ AIB features can be physically integrated
3. ✅ Build system can be updated
4. ⏳ Significant work remains for full integration

**Estimated completion time**: 3-6 weeks of focused development

**Recommendation**: Proceed with incremental integration, testing each component thoroughly before moving to the next.

## Questions or Issues?

For discussion:
- GitHub Issues: https://github.com/iobond/aib/issues
- Pull Request: https://github.com/iobond/aib/pull/new/bitcoin-core-v30-merge

---

**Generated**: 2025-10-15
**Branch**: `bitcoin-core-v30-merge`
**Base**: Bitcoin Core v30.0
**Status**: Proof of Concept - Integration Phase
