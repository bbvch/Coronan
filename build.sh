#!/bin/bash

set -e

BUILD_DIR=""
CMAKE=cmake


print_usage() {
cat << EOM
Usage: build.sh [options] build_dir
  Available options:
    -h          Print this help
    --cmake     Path to cmake (default is teh system cmake)
EOM
}

if [ $# -lt 1 ]; then
    echo "Error: Please specify the build folder."
    echo ""
    print_usage
    exit 1
fi

if [ $# -ge 1 ]; then
    while [[ $# -gt 0 ]] ; do
        key="$1"

        case $key in
        --cmake)
            CMAKE="$2"
            shift # past argument
            ;;
        -h|--help)
            print_usage
            exit 1
            ;;
        *)
            BUILD_DIR=$1
            shift # past argument
            ;;
        esac
    done
fi

if [ -z "${BUILD_DIR}" ]; then
    echo "Error: Please specify the build folder."
    echo ""
    print_usage
    exit 1
fi

[[ -d "${BUILD_DIR}" ]] || mkdir ${BUILD_DIR}


(cd ${BUILD_DIR} && conan install ..)
(cd ${BUILD_DIR} && ${CMAKE} ..)

num_threads=`grep -c '^processor' /proc/cpuinfo`
${CMAKE} --build ${BUILD_DIR} -- -j${num_threads}

exit 0