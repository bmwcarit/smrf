#!/bin/bash

source /data/src/docker/resources/base/scripts/global.sh
cd /data/build/smrf/bin

./smrf-unit-test --gtest_shuffle --gtest_output="xml:g_UnitTests.junit.xml"
