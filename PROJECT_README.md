# Project #32: AIB Blockchain Maintenance

## Project Overview

**AIB (Advanced Internet Blockchain)** - Experimental digital asset maintenance project.

This project maintains the AIB Core codebase, which is based on Bitcoin Core and provides peer-to-peer cryptocurrency functionality.

## Repository

- **GitHub**: https://github.com/iobond/aib
- **Official Website**: https://www.aib.one
- **Current Branch**: master (V5.14 stable)

## What is AIB?

AIB is an experimental digital asset that enables instant payments to anyone, anywhere in the world. AIB uses peer-to-peer technology to operate with no central authority: managing transactions and issuing money are carried out collectively by the network.

AIB also implements a dual value system to enable special token economic systems for small projects, reducing risk for small and distributed investments.

## Project Structure

```
032-aib-maintenance/
├── README.md               # Main project documentation
├── CONTRIBUTING.md         # Contribution guidelines
├── src/                    # C++ source code
├── doc/                    # Documentation
├── contrib/                # Contributed scripts and tools
├── depends/                # Dependency management
├── share/                  # Shared resources
├── build-aux/              # Build automation files
├── config/                 # Configuration files
└── var/                    # Variable data
```

## Key Files

- `Makefile.am` - Automake configuration
- `configure.ac` - Autoconf configuration
- `build.properties` - Build properties
- `build-linux.sh` - Linux build script
- `autogen.sh` - Autogen script

## Technology Stack

- **Language**: C++ (Bitcoin Core based)
- **Build System**: Autotools (autoconf/automake)
- **License**: MIT License
- **Base**: Bitcoin Core architecture
- **Database**: LevelDB/BerkeleyDB

## Available Branches

- `master` - Current stable version (V5.14)
- `aib-5.14`, `aib-5.14.1`, `aib-5.14.2` - Version branches
- `5.17`, `5.19` - Development versions
- `development` - Active development branch
- `token` - Token economic features
- `supernode` - Supernode implementation
- `bitcore` - Bitcore integration

## Build Instructions

### Prerequisites

```bash
sudo apt-get install build-essential libtool autotools-dev automake pkg-config
sudo apt-get install libssl-dev libevent-dev bsdmainutils
sudo apt-get install libboost-all-dev
```

### Building from Source

```bash
cd 032-aib-maintenance

# Generate configure script
./autogen.sh

# Configure
./configure

# Build
make

# Optional: Run tests
make check

# Optional: Install
sudo make install
```

### Quick Build (Linux)

```bash
./build-linux.sh
```

## Documentation

See the `/doc` directory for comprehensive documentation:

- `/doc/build-unix.md` - Unix/Linux build instructions
- `/doc/build-osx.md` - macOS build instructions
- `/doc/build-windows.md` - Windows build instructions
- `/doc/README.md` - General documentation
- `/doc/unit-tests.md` - Unit testing guide
- `/doc/release-process.md` - Release process

## Testing

### Unit Tests

```bash
make check
```

### RPC Tests (Python)

```bash
qa/pull-tester/rpc-tests.py
```

## Maintenance Tasks

This project is used for:

1. **Code Maintenance** - Keep AIB Core up to date
2. **Security Updates** - Apply security patches
3. **Feature Development** - Add new features
4. **Bug Fixes** - Fix reported issues
5. **Version Management** - Manage version branches
6. **Build Testing** - Test builds on different platforms

## Development Workflow

1. Clone the repository
2. Create a feature branch
3. Make changes and test
4. Submit pull requests to main AIB repository
5. Sync with upstream regularly

## Syncing with Upstream

```bash
# Add upstream remote (if not already added)
git remote add upstream https://github.com/iobond/aib.git

# Fetch latest changes
git fetch upstream

# Merge changes
git merge upstream/master
```

## Status

**Current Status**: Building (Maintenance Mode)

- Repository cloned: ✅
- Build environment: Pending setup
- Documentation: ✅
- Testing: Pending

## Links

- **GitHub**: https://github.com/iobond/aib
- **Website**: https://www.aib.one
- **Travis CI**: https://travis-ci.org/iobond/aib
- **License**: MIT License

## Notes

- This is a maintenance project for keeping AIB Core synchronized
- Based on Bitcoin Core architecture
- Follows Bitcoin Core development practices
- MIT licensed open source project
- Part of the Daydream project ecosystem (#32)

## Contact

For AIB-specific questions, refer to:
- GitHub Issues: https://github.com/iobond/aib/issues
- Developer Mailing List: https://groups.google.com/forum/#!forum/aib-development
- IRC: #aib-dev on Freenode

---

**Project ID**: #32
**Created**: October 15, 2025
**Category**: Fintech / Blockchain
**Status**: Building (Maintenance)
