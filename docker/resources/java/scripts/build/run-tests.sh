#!/bin/bash

source /data/src/docker/resources/base/scripts/global.sh

set -x -e

cd /data/src

mvn -B test
