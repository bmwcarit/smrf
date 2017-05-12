#!/bin/bash

set -e -x

function usage
{
    echo "usage: build.sh --repository mydockerrepo.org --tag tag"
}

DOCKER_MAKE=$(which docker-make)
if [ ! -x "$DOCKER_MAKE" ] ; then
    echo "could not find docker-make in PATH"
    echo "install it using 'pip install DockerMake'"
    exit 1
fi

TAG=latest

while [ "$1" != "" ]; do
    echo "PARAM is: $1"
    case $1 in
        -r | --repository )     shift
                                echo "REPO"
                                REPOSITORY=${1%/}
                                ;;
        -t | --tag )            shift
                                TAG="$1"
                                ;;
        -h | --help )           usage
                                exit
                                ;;
        * )                     usage
                                exit 1
    esac
    shift
done

declare -a DOCKER_IMAGES=(
    "smrf-base"
    "smrf-cpp"
    "smrf-libfuzzer"
    "smrf-node"
    "smrf-java"
    "smrf-compatibility"
)

if [ $REPOSITORY ]; then
    REPOSITORY="--repository $REPOSITORY"
else
    REPOSITORY=
fi

for IMAGE in "${DOCKER_IMAGES[@]}"
do
    $DOCKER_MAKE --makefile smrf.yml ${IMAGE} ${REPOSITORY} --tag ${TAG}
done
