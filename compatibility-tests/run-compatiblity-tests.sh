#!/bin/bash
set -x

source /data/src/docker/resources/base/scripts/global.sh
export CI=true

TEST_SRC_DIR=/data/src/compatibility-tests
CPP_TEST_SRC_DIR=$TEST_SRC_DIR/cpp
NODE_TEST_SRC_DIR=$TEST_SRC_DIR/node
JS_TEST_SRC_DIR=$TEST_SRC_DIR/js
JAVA_TEST_SRC_DIR=$TEST_SRC_DIR/java

TEST_BUILD_DIR=/data/build/smrf-compatibility-tests
mkdir -p $TEST_BUILD_DIR
CPP_TEST_BUILD_DIR=$TEST_BUILD_DIR/cpp
mkdir -p $CPP_TEST_BUILD_DIR
NODE_TEST_BUILD_DIR=$NODE_TEST_SRC_DIR

OUTPUT_DIR=/data/output
mkdir -p $OUTPUT_DIR
CPP_OUTPUT_DIR=$OUTPUT_DIR/cpp
mkdir -p $CPP_OUTPUT_DIR
NODE_OUTPUT_DIR=$OUTPUT_DIR/node
mkdir -p $NODE_OUTPUT_DIR
JS_OUTPUT_DIR=$OUTPUT_DIR/js
mkdir -p $JS_OUTPUT_DIR
JAVA_OUTPUT_DIR=$OUTPUT_DIR/java
mkdir -p $JAVA_OUTPUT_DIR

declare -a IMPLEMENTATIONS=("cpp" "node" "java" "js")
declare -a COMPRESSION=("compressed" "uncompressed")


######################################################################
# build each libsmrf and compatibility-test
######################################################################

CPP_INSTALL_PREFIX=/data/install
CPP_TEST_BUILD_DIR=/data/build/compatibility-test
mkdir -p $CPP_TEST_BUILD_DIR

# C++
(
    /data/src/docker/resources/cpp/scripts/build/cpp-clean-build.sh \
        --enableclangformatter OFF \
        --buildtests OFF \
        --install ON \
        --additionalcmakeargs "-DCMAKE_INSTALL_PREFIX=$CPP_INSTALL_PREFIX"

    cd $CPP_TEST_BUILD_DIR
    cmake -DCMAKE_PREFIX_PATH=$CPP_INSTALL_PREFIX $CPP_TEST_SRC_DIR
    make
)

# Node.js & JS
(
    cd $NODE_TEST_BUILD_DIR
    # install 'smrf-native' addon
    npm install /data/src/javascript/smrf-native --SMRF_LIB_DIR=$CPP_INSTALL_PREFIX/lib64 --SMRF_INCLUDE_DIR=$CPP_INSTALL_PREFIX/include
    npm install
)

# Java
(
    /data/src/docker/resources/java/scripts/build/build.sh
)

######################################################################
# run each serializer to generate compressed and uncompressed output
######################################################################

# C++
(
    cd $CPP_TEST_BUILD_DIR
    for MODE in "${COMPRESSION[@]}"
    do
        ./serializer --path=$CPP_OUTPUT_DIR/$MODE --compressed=$MODE
    done
)

# Node.js
(
    cd $NODE_TEST_BUILD_DIR
    for MODE in "${COMPRESSION[@]}"
    do
        npm run serializer --compatibility-test:path=$NODE_OUTPUT_DIR/$MODE --compatibility-test:compressed=$MODE --compatibility-test:smrf="js"
        npm run serializer --compatibility-test:path=$JS_OUTPUT_DIR/$MODE --compatibility-test:compressed=$MODE --compatibility-test:smrf="native"
    done
)

# Java
(
    cd $JAVA_TEST_SRC_DIR
    for MODE in "${COMPRESSION[@]}"
    do
        mvn exec:java -Dexec.mainClass="io.joynr.smrf.tests.CompatibilitySerializer" -Dexec.args="--path $JAVA_OUTPUT_DIR/$MODE --compressed $MODE"
    done
)
######################################################################
# run each deserializer with the output of all serializers
######################################################################

# C++
(
    cd $CPP_TEST_BUILD_DIR
    for IMPL in "${IMPLEMENTATIONS[@]}"
    do
        for MODE in "${COMPRESSION[@]}"
        do
            ./deserializer --path=$OUTPUT_DIR/$IMPL/$MODE --compressed=$MODE
        done
    done
)

# Node.js
(
    cd $NODE_TEST_BUILD_DIR
    for IMPL in "${IMPLEMENTATIONS[@]}"
    do
        for MODE in "${COMPRESSION[@]}"
        do
            npm run-script deserializer --compatibility-test:path=$OUTPUT_DIR/$IMPL/$MODE --compatibility-test:compressed=$MODE --compatibility-test:smrf="js"
            npm run-script deserializer --compatibility-test:path=$OUTPUT_DIR/$IMPL/$MODE --compatibility-test:compressed=$MODE --compatibility-test:smrf="native"
        done
    done
)

# Java.js
(
    cd $JAVA_TEST_SRC_DIR
    for IMPL in "${IMPLEMENTATIONS[@]}"
    do
        for MODE in "${COMPRESSION[@]}"
        do
            mvn exec:java -Dexec.mainClass="io.joynr.smrf.tests.CompatibilityDeserializer" -Dexec.args="--path $OUTPUT_DIR/$IMPL/$MODE --compressed $MODE"
        done
    done
)
