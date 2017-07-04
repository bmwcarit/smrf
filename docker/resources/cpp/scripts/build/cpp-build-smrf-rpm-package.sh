#!/bin/bash

source /data/src/docker/resources/base/scripts/global.sh

function usage
{
    echo "usage: cpp-build-smrf-rpm-package.sh [--rpm-spec <RPM spec>]"
}

RPMSPEC="cpp/distribution/smrf.spec"

while [ "$1" != "" ]; do
    case $1 in
        --rpm-spec )            shift
                                RPMSPEC=$1
                                ;;
        -h | --help )           usage
                                exit
                                ;;
        * )                     usage
                                exit 1
    esac
    shift
done

log "Enable core dumps"
ulimit -c unlimited

START=$(date +%s)

log "ENVIRONMENT"
env

# prepare RPM environment
mkdir /data/build/smrf/package
mkdir /data/build/smrf/package/RPM
mkdir /data/build/smrf/package/RPM/BUILD
mkdir /data/build/smrf/package/RPM/BUILDROOT
mkdir /data/build/smrf/package/RPM/RPMS
mkdir /data/build/smrf/package/RPM/SOURCES
mkdir /data/build/smrf/package/RPM/SPECS
mkdir /data/build/smrf/package/RPM/SRPMS
mkdir /data/build/smrf/package/RPM/smrf

# copy RPM spec file
cp /data/src/$RPMSPEC /data/build/smrf/package/RPM/SPECS
RPMSPEC_BASENAME=`basename /data/src/$RPMSPEC`
cd /data/build/smrf

SRCDIR=/data/src
DESTDIR=/data/build/smrf/package/RPM/smrf
log "BUILD RPM PACKAGE"
make -j $JOBS install DESTDIR=$DESTDIR

rpm_with_flags=""

cd /data/build/smrf/package/RPM/SPECS

rpmbuild -bb ${rpm_with_flags} --buildroot $DESTDIR $RPMSPEC_BASENAME

END=$(date +%s)
DIFF=$(( $END - $START ))
log "build RPM package time: $DIFF seconds"
