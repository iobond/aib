Aibcoin Core 0.13.2
=====================

Setup
---------------------
[Aibcoin Core](http://aibcoin.org/en/download) is the original Aibcoin client and it builds the backbone of the network. However, it downloads and stores the entire history of Aibcoin transactions (which is currently several GBs); depending on the speed of your computer and network connection, the synchronization process can take anywhere from a few hours to a day or more.

Running
---------------------
The following are some helpful notes on how to run Aibcoin on your native platform.

### Unix

Unpack the files into a directory and run:

- `bin/aibcoin-qt` (GUI) or
- `bin/aibcoind` (headless)

### Windows

Unpack the files into a directory, and then run aibcoin-qt.exe.

### OS X

Drag Aibcoin-Core to your applications folder, and then run Aibcoin-Core.

### Need Help?

* See the documentation at the [Aibcoin Wiki](https://aibcoin.info/)
for help and more information.
* Ask for help on [#aibcoin](http://webchat.freenode.net?channels=aibcoin) on Freenode. If you don't have an IRC client use [webchat here](http://webchat.freenode.net?channels=aibcoin).
* Ask for help on the [AibcoinTalk](https://aibcointalk.io/) forums.

Building
---------------------
The following are developer notes on how to build Aibcoin on your native platform. They are not complete guides, but include notes on the necessary libraries, compile flags, etc.

- [OS X Build Notes](build-osx.md)
- [Unix Build Notes](build-unix.md)
- [Windows Build Notes](build-windows.md)
- [OpenBSD Build Notes](build-openbsd.md)
- [Gitian Building Guide](gitian-building.md)

Development
---------------------
The Aibcoin repo's [root README](/README.md) contains relevant information on the development process and automated testing.

- [Developer Notes](developer-notes.md)
- [Multiwallet Qt Development](multiwallet-qt.md)
- [Release Notes](release-notes.md)
- [Release Process](release-process.md)
- [Source Code Documentation (External Link)](https://dev.visucore.com/bitcoin/doxygen/)
- [Translation Process](translation_process.md)
- [Translation Strings Policy](translation_strings_policy.md)
- [Unit Tests](unit-tests.md)
- [Unauthenticated REST Interface](REST-interface.md)
- [Shared Libraries](shared-libraries.md)
- [BIPS](bips.md)
- [Dnsseed Policy](dnsseed-policy.md)
- [Benchmarking](benchmarking.md)

### Resources
* Discuss on the [AibcoinTalk](https://aibcointalk.io/) forums.
* Discuss project-specific development on #aibcoin on Freenode. If you don't have an IRC client use [webchat here](http://webchat.freenode.net/?channels=aibcoin).

### Miscellaneous
- [Assets Attribution](assets-attribution.md)
- [Files](files.md)
- [Tor Support](tor.md)
- [Init Scripts (systemd/upstart/openrc)](init.md)

License
---------------------
Distributed under the [MIT software license](http://www.opensource.org/licenses/mit-license.php).
This product includes software developed by the OpenSSL Project for use in the [OpenSSL Toolkit](https://www.openssl.org/). This product includes
cryptographic software written by Eric Young ([eay@cryptsoft.com](mailto:eay@cryptsoft.com)), and UPnP software written by Thomas Bernard.
