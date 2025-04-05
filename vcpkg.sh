#!/bin/sh

set -e

export VCPKG_ROOT=~/vcpkg

dir_vcpkg=$VCPKG_ROOT

if ! [ -d "$dir_vcpkg" ]; then
  git clone https://github.com/microsoft/vcpkg $dir_vcpkg
  $dir_vcpkg/bootstrap-vcpkg.sh
fi

$dir_vcpkg/vcpkg install fmt
$dir_vcpkg/vcpkg install sdl2
$dir_vcpkg/vcpkg install gtest
