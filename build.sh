#!/bin/bash

set -e

BUILD_DIR="build"
CMAKE=cmake
COVERAGE=false
COVERAGE_OUT=""
BUILD_TYPE=Debug
BUILD_TARGET=""
NINJA=""
COMPILER="g++"

print_usage() {
cat << EOM
Usage: build.sh [options] [build_dir]
  Available options:
    -h|--help          Print this help
    --cov=output_file  Build debug version with coverage enabled.
    -r|--release       Build release version.
                       Note: is ignored when --cov is set
    --cmake=path       Path to cmake (default is the system cmake)
    -i                 Install
    -p                 Create package
    --clang=version    Build with clang (ver: version).
                       Note: is ignored when --cov is set
   build_dir:      Directory to build in (default: build)
EOM
}

if [ $# -ge 1 ]; then
    while [[ $# -gt 0 ]] ; do
        key="$1"

        case "$key" in
        --cov=*)
            COVERAGE=true
            COVERAGE_OUT="${key#*=}"
            shift # past argument=value
            ;;
        -r|--release)
            BUILD_TYPE=Release
            shift # past argument
            ;;
        --clang=*)
            COMPILER="clang++-${key#*=}"
            shift # past argument=value
            ;;
        --cmake=*)
            CMAKE="${key#*=}"
            shift # past argument=value
            ;;
        -h|--help)
            print_usage
            exit 1
            ;;
        -i)
            BUILD_TARGET="--target install"
            shift # past argument
            ;;
        -p)
            BUILD_TARGET="--target package"
            shift # past argument
            ;;
        *)
            BUILD_DIR=$key
            shift # past argument=value
            ;;
        esac
    done
fi

if [ "$COVERAGE" = true -a -z "${COVERAGE_OUT}" ]; then
    echo "Error: Please specify the coverage output_file."
    echo ""
    print_usage
    exit 1
fi

if [ command -v ninja &> /dev/null ]
then
    CMAKE_GENERATOR="-G Ninja"
fi

if [ "$COVERAGE" = true ] ; then
    "${CMAKE}" -S . -B ${BUILD_DIR} $CMAKE_GENERATOR -DCODE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug
else
    "${CMAKE}" -S . -B ${BUILD_DIR} $CMAKE_GENERATOR -DCMAKE_CXX_COMPILER="${COMPILER}" -DCODE_COVERAGE=OFF -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"
fi

num_threads=$(grep -c '^processor' /proc/cpuinfo)
"${CMAKE}" --build "${BUILD_DIR}" --target docs -- -j"${num_threads}"
"${CMAKE}" --build "${BUILD_DIR}" ${BUILD_TARGET} -- -j"${num_threads}"

if [ "$COVERAGE" = true ] ; then
    lcov --capture --directory . --output-file "${COVERAGE_OUT}"
    lcov --remove "${COVERAGE_OUT}" '/usr/*' --output-file "${COVERAGE_OUT}"
fi


exit 0
