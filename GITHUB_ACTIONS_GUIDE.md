# GitHub Actions Workflow Guide

This repository now includes automated GitHub Actions workflows for building and releasing AIB Core wallet binaries.

## Available Workflows

### 1. Build and Release (build-release.yml)

Automatically builds AIB Core binaries for multiple platforms and creates GitHub releases.

**Triggers:**
- **Tag push**: Automatically runs when you push a version tag (e.g., `v5.13.3.14`)
- **Manual trigger**: Can be run manually from GitHub Actions tab

**Platforms:**
- Linux (x86_64)
- Windows (x86_64)
- macOS (x86_64)

**Outputs:**
- `aib-linux-x86_64.tar.gz` - Linux binaries
- `aib-windows-x86_64.zip` - Windows binaries
- `aib-macos-x86_64.tar.gz` - macOS binaries
- `SHA256SUMS.txt` - Checksums for verification

### 2. Continuous Integration (ci.yml)

Runs automated tests and code quality checks on every push and pull request.

**Triggers:**
- Push to `master`, `develop`, or `security-*` branches
- Pull requests to `master` or `develop`

**Checks:**
- Build and compile tests (Linux)
- Code quality (trailing whitespace, tabs)
- Security scanning (shellcheck, sensitive data detection)
- Large file detection

## How to Create a Release

### Method 1: Using Git Tags (Recommended)

```bash
# Make sure you're on the branch you want to release
git checkout security-audit-2025-dev

# Create and push a version tag
git tag -a v5.13.3.14 -m "AIB Core v5.13.3.14 - Security Update"
git push origin v5.13.3.14
```

The workflow will automatically:
1. Build binaries for all platforms
2. Create SHA256 checksums
3. Create a GitHub release
4. Upload all binaries and checksums

### Method 2: Manual Workflow Dispatch

1. Go to your repository on GitHub
2. Click on "Actions" tab
3. Select "Build and Release AIB Wallet" workflow
4. Click "Run workflow" button
5. Enter the version number (e.g., `v5.13.3.14`)
6. Click "Run workflow"

## Accessing Released Binaries

After a successful release:

1. Go to the "Releases" section on GitHub
2. Find your version (e.g., `v5.13.3.14`)
3. Download the appropriate binary for your platform:
   - **Linux**: `aib-linux-x86_64.tar.gz`
   - **Windows**: `aib-windows-x86_64.zip`
   - **macOS**: `aib-macos-x86_64.tar.gz`

## Verifying Downloads

Always verify your downloads using the SHA256 checksums:

```bash
# Download the checksums file
wget https://github.com/iobond/aib/releases/download/v5.13.3.14/SHA256SUMS.txt

# Verify your download
sha256sum -c SHA256SUMS.txt
```

## Workflow Status

Check the status of your workflows:
- GitHub repository → "Actions" tab
- View logs for each build
- Download artifacts even if release creation fails

## Troubleshooting

### Build Fails

If a platform build fails:
1. Check the workflow logs in the Actions tab
2. The release will still be created with available binaries
3. Windows builds are marked as `continue-on-error` due to dependency complexity

### Manual Download of Artifacts

If you need to download build artifacts before release:
1. Go to Actions tab
2. Click on the workflow run
3. Scroll down to "Artifacts" section
4. Download individual platform builds

## Build Configuration

The workflows use the following build configuration:

**Linux & macOS:**
- Hardening enabled
- GUI support (Qt5)
- Reduced exports for security
- BerkeleyDB 4.8 for wallet compatibility

**Windows:**
- Cross-compiled from Linux using MinGW
- Depends system for dependencies
- Best effort build (may require manual fixes)

## Next Steps

### For First Release

1. **Test the workflow**:
   ```bash
   git tag v5.13.3.14-test
   git push origin v5.13.3.14-test
   ```

2. **Verify builds work**: Check Actions tab for success

3. **Create production release**:
   ```bash
   git tag v5.13.3.14
   git push origin v5.13.3.14
   ```

### For Regular Releases

1. Merge your changes to the release branch
2. Create and push a version tag
3. Wait for builds to complete (usually 20-40 minutes)
4. Verify and announce the release

## Security Notes

- All binaries are built in isolated GitHub Actions runners
- Checksums are automatically generated
- No credentials or secrets are stored in workflows
- Source code is checked out directly from the repository

## Support

For issues with the build system:
- Check workflow logs in Actions tab
- Review individual job outputs
- Verify dependencies are available
- Check GitHub Actions service status

## File Structure

```
.github/
└── workflows/
    ├── build-release.yml  # Release builds and distribution
    └── ci.yml             # Continuous integration tests
```

## Customization

To modify build parameters, edit the workflow files:
- `.github/workflows/build-release.yml` - Release builds
- `.github/workflows/ci.yml` - CI/CD tests

Key sections to customize:
- `./configure` flags
- Platform-specific dependencies
- Release notes template
- Artifact naming
