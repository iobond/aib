#!/bin/bash
# Copyright (c) 2014-2019 The AIB Community developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.


git checkout master
git reset --hard HEAD
git pull
cd depends/
make -j8
cd ../
./autogen.sh
./configure --enable-glibc-back-compat --prefix=`pwd`/depends/x86_64-pc-linux-gnu LDFLAGS="-static-libstdc++"   --disable-tests  --disable-bench
make -j8
mkdir release
mv src/aibd release/
mv src/aib-cli release/
mv src/aib-tx release/
strip release/*