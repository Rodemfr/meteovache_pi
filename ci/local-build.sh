#!/bin/bash
OCPN_TARGET=bullseye
#BUILD_WX32=true
TARGET_TUPLE='ubuntu-wx32;22.04;x86_64'

here=$(cd $(dirname $0); pwd -P)

git submodule update --init opencpn-libs

# Set up build directory and a visible link in /
builddir=build-$OCPN_TARGET
test -d $builddir || mkdir $builddir  && rm -rf $builddir/*

OCPN_WX_ABI_OPT="-DOCPN_WX_ABI=wx32"

if [ -n "$TARGET_TUPLE" ]; then
  TARGET_OPT="-DOCPN_TARGET_TUPLE=$TARGET_TUPLE";
fi

cd $builddir

cmake -DCMAKE_BUILD_TYPE=Debug $OCPN_WX_ABI_OPT $TARGET_OPT ..
make -sj8 tarball
ldd app/*/lib/opencpn/*.so

