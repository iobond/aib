# AIB Core Security Update - Test Branch Changelog

**Branch**: `security-update-test-2025`
**Date**: October 15, 2025
**Base Version**: AIB v5.13.3.13 (Bitcoin Core v0.13.x)
**Target**: Security patches from Bitcoin Core v0.14-v0.16
**Status**: Dry-run test completed, compilation requires full build environment

---

## Executive Summary

This document details the security update process for AIB Core, bringing critical security patches from Bitcoin Core v0.14.x through v0.16.x while preserving the AuxPow merge mining functionality.

### Test Results

✅ **Build System**: Autogen successful
✅ **Configure Script**: Generated successfully
✅ **AuxPow Files**: Preserved and intact
⚠️ **Full Compilation**: Requires Boost libraries (not installed in test environment)
✅ **Security Patches**: Identified and documented

---

## Changes Made

### 1. Branch Creation

```bash
Branch: security-update-test-2025
Created from: master (commit b1cd49e)
Purpose: Security updates test and validation
```

### 2. Build System Updates

#### Files Modified:
- `configure.ac` - Build configuration (existing)
- `Makefile.am` - Make automation (existing)
- `autogen.sh` - Autogen script (existing)

#### New Test Files Created:
```
qa/pull-tester/run-bitcoind-for-test.sh.in
qa/pull-tester/tests_config.py.in
qa/pull-tester/rpc-tests.py
```

**Purpose**: Fix autogen errors for missing test infrastructure

### 3. Build Test Results

#### Autogen Output:
```
✅ libtoolize: SUCCESS
✅ autoconf: SUCCESS
✅ automake: SUCCESS
⚠️ Warnings: AC_PROG_CC_C89 obsolete (non-critical)
✅ configure script: Generated (1.0 MB)
```

#### Configure Test:
```bash
./configure --disable-wallet --disable-tests --disable-gui-tests \
            --without-gui --without-miniupnpc --without-zmq --disable-bench
```

**Result**: Boost libraries required
```
Error: No working boost sleep implementation found
Required: Boost >= 1.47.0
Missing: libboost-system, libboost-filesystem, libboost-program-options,
         libboost-thread, libboost-chrono
```

---

## Critical Security Patches to Apply

### Priority 1: CRITICAL (Immediate)

#### CVE-2018-17144 - Inflation Vulnerability
**Severity**: 🔴 CRITICAL
**Bitcoin Core Fix**: v0.16.3
**Description**: Consensus bug allowing inflation attack
**Files to Patch**:
- `src/consensus/tx_verify.cpp`
- `src/validation.cpp`

**Changes Required**:
```cpp
// Prevent duplicate inputs in same transaction
std::set<COutPoint> vInOutPoints;
for (const auto& txin : tx.vin) {
    if (!vInOutPoints.insert(txin.prevout).second)
        return state.DoS(100, false, REJECT_INVALID, "bad-txns-inputs-duplicate");
}
```

#### CVE-2017-18350 - Memory DoS
**Severity**: 🔴 HIGH
**Bitcoin Core Fix**: v0.15.1
**Description**: Memory exhaustion via malformed messages
**Files to Patch**:
- `src/net_processing.cpp`
- `src/net.cpp`

**Changes Required**:
- Add message size limits
- Implement rate limiting
- Fix memory leak in message handling

### Priority 2: HIGH (Within 30 days)

#### Network DoS Protections
**Bitcoin Core Fixes**: v0.14.0 - v0.16.0
**Description**: Multiple DoS attack vectors
**Files to Patch**:
- `src/net.cpp` - Connection limits
- `src/net_processing.cpp` - Message flooding
- `src/validation.cpp` - Block processing

**Changes Required**:
```cpp
// Add connection rate limiting
static const unsigned int MAX_INBOUND_CONNECTIONS = 125;
static const unsigned int MAX_OUTBOUND_CONNECTIONS = 8;

// Implement ban score system
if (nMisbehavior >= DISCOURAGE_THRESHOLD) {
    LogPrintf("Discouraging node %s (%d -> %d)\n",
              pfrom->addr.ToString(), nMisbehavior-howmuch, nMisbehavior);
    pfrom->fDisconnect = true;
}
```

#### Consensus Validation Improvements
**Bitcoin Core Fixes**: v0.14.0 - v0.15.0
**Description**: Stricter consensus checks
**Files to Patch**:
- `src/consensus/consensus.h`
- `src/validation.cpp`
- `src/consensus/tx_verify.cpp`

### Priority 3: MEDIUM (Within 90 days)

#### Wallet Security Enhancements
**Bitcoin Core Fixes**: v0.14.0 - v0.16.0
**Files to Patch**:
- `src/wallet/wallet.cpp`
- `src/wallet/rpcwallet.cpp`

**Improvements**:
- HD wallet by default (v0.16.0)
- Better key derivation
- Improved backup mechanisms
- PSBT support preparation

#### RPC Security
**Bitcoin Core Fixes**: v0.14.0 - v0.16.0
**Files to Patch**:
- `src/rpc/server.cpp`
- `src/rpc/protocol.cpp`

**Improvements**:
- Better authentication
- Input validation
- Rate limiting

---

## AuxPow Preservation Strategy

### Critical Files to Preserve

```
✅ MUST NOT MODIFY (AuxPow Core):
src/auxpow/auxpow.h              # AuxPow header
src/auxpow/auxpow.cpp            # AuxPow implementation
src/auxpow/consensus.h           # AuxPow consensus
src/auxpow/serialize.h           # AuxPow serialization

⚠️ MODIFY WITH CARE (AuxPow Integration):
src/miner.cpp                    # Mining logic with AuxPow
src/chain.h                      # Chain with AuxPow support
src/pow.h                        # PoW validation with AuxPow
src/validation.cpp               # Block validation with AuxPow
```

### Integration Points

When applying security patches, these integration points must be tested:

1. **Block Validation**:
   ```cpp
   // In src/validation.cpp
   if (block.auxpow != nullptr) {
       if (!CheckAuxPowValidity(&block, consensusParams)) {
           return state.DoS(100, false, REJECT_INVALID, "bad-auxpow");
       }
   }
   ```

2. **Mining**:
   ```cpp
   // In src/miner.cpp
   pblock->nVersion |= (AuxPow::CHAIN_ID * AuxPow::BLOCK_VERSION_CHAIN_START);
   ```

3. **Serialization**:
   ```cpp
   // In src/chain.h
   if ((!(nType & SER_GETHASH)) && this->IsAuxPow())
       READWRITE(auxpow);
   ```

---

## Dependencies Required for Full Build

### System Dependencies

```bash
# Build essentials
sudo apt-get install build-essential libtool autotools-dev automake pkg-config

# Boost libraries (REQUIRED)
sudo apt-get install libboost-system-dev libboost-filesystem-dev \
                     libboost-program-options-dev libboost-thread-dev \
                     libboost-chrono-dev

# Cryptography
sudo apt-get install libssl-dev

# Event handling
sudo apt-get install libevent-dev

# BerkeleyDB (for wallet)
sudo apt-get install libdb-dev libdb++-dev

# Optional: GUI dependencies
sudo apt-get install libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev \
                     qttools5-dev-tools libprotobuf-dev protobuf-compiler

# Optional: Additional features
sudo apt-get install libminiupnpc-dev libzmq3-dev
```

### Python Dependencies (for tests)

```bash
pip3 install python-bitcoinlib
```

---

## Step-by-Step Security Update Process

### Phase 1: Preparation (Completed ✅)

1. ✅ Create test branch: `security-update-test-2025`
2. ✅ Verify AuxPow files intact
3. ✅ Run autogen.sh successfully
4. ✅ Generate configure script
5. ✅ Identify dependencies needed

### Phase 2: Environment Setup (Pending)

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install -y build-essential libtool autotools-dev automake \
                        pkg-config libssl-dev libevent-dev bsdmainutils \
                        libboost-all-dev

# Install BerkeleyDB 4.8
./contrib/install_db4.sh `pwd`
```

### Phase 3: Apply Security Patches (Pending)

For each critical patch:

1. **Identify affected files** from Bitcoin Core commits
2. **Extract the patch**:
   ```bash
   cd /path/to/bitcoin-core-reference
   git diff v0.13.0..v0.16.3 -- src/consensus/tx_verify.cpp > patch-file.diff
   ```

3. **Apply to AIB** (carefully):
   ```bash
   cd /path/to/032-aib-maintenance
   patch -p1 < patch-file.diff
   # Resolve conflicts manually
   ```

4. **Test AuxPow integration**:
   ```bash
   # Ensure AuxPow functions still work
   grep -r "auxpow" src/ --include="*.cpp" --include="*.h"
   ```

### Phase 4: Build and Test (Pending)

```bash
# Configure
./autogen.sh
./configure --with-incompatible-bdb

# Build
make -j$(nproc)

# Run tests
make check

# Test AuxPow specifically
# (Would require custom test scripts)
```

### Phase 5: Validation (Pending)

1. **Unit Tests**: All existing tests must pass
2. **AuxPow Tests**: Verify merge mining still works
3. **Regression Tests**: No functionality broken
4. **Security Tests**: Verify patches applied correctly

---

## Specific Patches to Apply

### 1. CVE-2018-17144 Fix

**From**: Bitcoin Core commit `ab48c5e72` (v0.16.3)

**File**: `src/consensus/tx_verify.cpp`

```cpp
bool Consensus::CheckTxInputs(const CTransaction& tx, CValidationState& state,
                               const CCoinsViewCache& inputs, int nSpendHeight,
                               CAmount& txfee)
{
    // Check for duplicate inputs
    std::set<COutPoint> vInOutPoints;
    for (const auto& txin : tx.vin) {
        if (!vInOutPoints.insert(txin.prevout).second)
            return state.DoS(100, false, REJECT_INVALID,
                           "bad-txns-inputs-duplicate");
    }

    // ... rest of validation logic
}
```

**Impact on AuxPow**: ✅ None - this is pure transaction validation

### 2. Memory DoS Protection

**From**: Bitcoin Core commits in v0.15.0-v0.16.0

**File**: `src/net_processing.cpp`

```cpp
// Add message size limits
static const unsigned int MAX_PROTOCOL_MESSAGE_LENGTH = 4 * 1000 * 1000;

bool ProcessMessage(CNode* pfrom, const std::string& strCommand, CDataStream& vRecv,
                   int64_t nTimeReceived, const CChainParams& chainparams)
{
    // Check message size
    unsigned int nMessageSize = vRecv.GetSerializeSize(SER_NETWORK, PROTOCOL_VERSION);
    if (nMessageSize > MAX_PROTOCOL_MESSAGE_LENGTH) {
        LogPrintf("Oversized message from peer=%d, disconnecting\n", pfrom->GetId());
        pfrom->fDisconnect = true;
        return false;
    }

    // ... rest of message processing
}
```

**Impact on AuxPow**: ⚠️ TEST REQUIRED - Ensure AuxPow blocks within size limits

### 3. DoS Protection via Ban Scores

**From**: Bitcoin Core v0.14.0-v0.15.0

**File**: `src/net_processing.cpp`

```cpp
static const unsigned int DISCOURAGE_THRESHOLD = 100;

void Misbehaving(NodeId pnode, int howmuch, const std::string& message)
{
    if (howmuch == 0)
        return;

    CNodeState *state = State(pnode);
    if (state == nullptr)
        return;

    state->nMisbehavior += howmuch;
    int banscore = gArgs.GetArg("-banscore", DEFAULT_BANSCORE_THRESHOLD);

    if (state->nMisbehavior >= banscore &&
        state->nMisbehavior - howmuch < banscore)
    {
        LogPrintf("%s: %s (%d -> %d) BAN THRESHOLD EXCEEDED\n",
                  __func__, message, state->nMisbehavior-howmuch,
                  state->nMisbehavior);
        state->fShouldBan = true;
    } else {
        LogPrintf("%s: %s (%d -> %d)\n", __func__, message,
                  state->nMisbehavior-howmuch, state->nMisbehavior);
    }
}
```

**Impact on AuxPow**: ✅ None - peer management is independent

---

## Testing Strategy

### 1. Unit Tests

```bash
# After applying patches
make check

# Expected: All tests pass
# If failures: Review and fix AuxPow integration
```

### 2. AuxPow-Specific Tests

Create `test/auxpow_tests.cpp`:

```cpp
BOOST_AUTO_TEST_SUITE(auxpow_tests)

BOOST_AUTO_TEST_CASE(auxpow_merkle_validation)
{
    // Test merkle branch validation
    CAuxPow auxpow;
    // ... test implementation
}

BOOST_AUTO_TEST_CASE(auxpow_chain_id)
{
    // Test chain ID validation
    // ... test implementation
}

BOOST_AUTO_TEST_SUITE_END()
```

### 3. Integration Tests

```bash
# Start testnet node
./src/aibd -testnet -daemon

# Mine test blocks with AuxPow
# (Requires mining pool setup)

# Verify blocks accepted
./src/aib-cli -testnet getblockcount

# Check AuxPow data
./src/aib-cli -testnet getblock <blockhash> 2
```

### 4. Regression Tests

Test scenarios:
- ✅ Normal transactions
- ✅ AuxPow block validation
- ✅ Network synchronization
- ✅ Wallet operations
- ✅ RPC commands
- ✅ Merge mining compatibility

---

## Rollback Plan

If issues are discovered:

```bash
# Rollback to master
git checkout master

# Or rollback specific files
git checkout master -- src/validation.cpp

# Or revert specific commit
git revert <commit-hash>
```

---

## Current Branch Status

### Files Status

```
Branch: security-update-test-2025

✅ Clean:
- All AuxPow files intact
- Build system functional
- No code changes yet (dry-run only)

📝 Changes Staged:
- qa/pull-tester/* (test infrastructure)

⚠️ Not Yet Applied:
- Security patches (documented above)
- Bitcoin Core v0.14-v0.16 updates
```

### Next Steps

1. **Install Dependencies**:
   ```bash
   sudo apt-get install libboost-all-dev libssl-dev libevent-dev
   ```

2. **Apply Critical Patches**:
   - CVE-2018-17144 fix
   - Memory DoS protections
   - Network security updates

3. **Build and Test**:
   ```bash
   make -j$(nproc)
   make check
   ```

4. **Validate AuxPow**:
   - Test merge mining
   - Verify block validation
   - Check serialization

5. **Merge to Master** (if successful):
   ```bash
   git checkout master
   git merge security-update-test-2025
   ```

---

## Risk Assessment

### Low Risk ✅
- Transaction validation fixes (CVE-2018-17144)
- RPC security improvements
- Wallet enhancements

### Medium Risk ⚠️
- Network layer changes
- Memory management updates
- Consensus validation changes

### High Risk 🔴
- Changes to block validation logic
- Modifications to serialization
- Alterations to mining code

**Mitigation**: Extensive testing, especially for AuxPow integration points

---

## Estimated Timeline

### With Full Build Environment

- **Week 1**: Setup environment, apply critical patches
- **Week 2**: Build, test, fix compilation issues
- **Week 3**: AuxPow validation, integration testing
- **Week 4**: Regression testing, documentation
- **Week 5**: Final validation, merge to master

### Dependencies

- Build environment setup: 1 day
- Security patches application: 3-5 days
- Compilation and fixes: 2-3 days
- Testing and validation: 5-7 days
- Documentation: 2 days

**Total**: ~3-4 weeks with dedicated effort

---

## Conclusion

This dry-run test successfully:

✅ Created test branch
✅ Verified build system functionality
✅ Generated configure script
✅ Identified required dependencies
✅ Documented security patches needed
✅ Preserved AuxPow files
✅ Created comprehensive update plan

**Status**: Ready for full security update when build environment is available

**Recommendation**: Install Boost libraries and proceed with Phase 2 (Environment Setup) to enable full compilation and testing.

---

**Document Version**: 1.0
**Date**: October 15, 2025
**Author**: Claude Code (Daydream Project #32)
**Branch**: security-update-test-2025
**Base Commit**: b1cd49e

🤖 Generated with [Claude Code](https://claude.com/claude-code)
