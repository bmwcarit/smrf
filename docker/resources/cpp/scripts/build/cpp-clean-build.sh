#!/bin/bash

source /data/src/docker/resources/base/scripts/global.sh

ENABLE_CLANG_FORMATTER='ON'
BUILD_TESTS='ON'
ADDITIONAL_CMAKE_ARGS=''
INSTALL='OFF'

function usage
{
    echo "usage: cpp-clean-build.sh [--jobs NUM_JOBS --enableclangformatter ON|OFF --buildtests ON|OFF --additionalcmakeargs <args> --install ON|OFF]"
}

while [ "$1" != "" ]; do
    case $1 in
        --jobs )                 shift
                                 JOBS=$1
                                 ;;
        --enableclangformatter ) shift
                                 ENABLE_CLANG_FORMATTER=$1
                                 ;;
        --buildtests )           shift
                                 BUILD_TESTS=$1
                                 ;;
        --additionalcmakeargs )  shift
                                 ADDITIONAL_CMAKE_ARGS=$1
                                 ;;
        --install )              shift
                                 INSTALL=$1
                                 ;;
        * )                      usage
                                 exit 1
    esac
    shift
done

log "CPP CLEAN BUILD: JOBS=$JOBS, \
     ENABLE_CLANG_FORMATTER=$ENABLE_CLANG_FORMATTER, \
     BUILD_TESTS=$BUILD_TESTS, \
     ADDITIONAL_CMAKE_ARGS=$ADDITIONAL_CMAKE_ARGS, \
     INSTALL=$INSTALL"

log "ENVIRONMENT"
env

SOURCE_DIRECTORY=/data/src/
BUILD_DIRECTORY=/data/build/smrf
mkdir -p $BUILD_DIRECTORY
cd $BUILD_DIRECTORY

set -x -e

cmake -DENABLE_CLANG_FORMATTER=$ENABLE_CLANG_FORMATTER \
      -DBUILD_TESTS=$BUILD_TESTS \
      $ADDITIONAL_CMAKE_ARGS \
      $SOURCE_DIRECTORY

make -j $JOBS

if [ "$INSTALL" == "ON" ]
then
  make install
fi
