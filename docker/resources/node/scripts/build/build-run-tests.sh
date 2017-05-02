#!/bin/bash

set -x -e

ADDITIONAL_NPM_ARGS=''

function usage
{
    echo "usage: build-run-tests.sh [--additional-npm-args <args>]"
}

while [ "$1" != "" ]; do
    case $1 in
        --additional-npm-args )  shift
                                 ADDITIONAL_NPM_ARGS=$1
                                 ;;
        * )                      usage
                                 exit 1
    esac
    shift
done


export CI=true

cd /data/src/node
npm install $ADDITIONAL_NPM_ARGS
npm test
