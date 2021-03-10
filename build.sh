#!/bin/bash

set -e

BUILD_DIR=""
CMAKE=cmake
CONAN=conan
COVERAGE=false
COVERAGE_OUT=""
BUILD_TYPE=Debug



print_usage() {
cat << EOM
Usage: build.sh [options] build_dir
  Available options:
    -h|--help          Print this help
    --cov output_file  Build debug version with coverage enabled.
    -r|--release       Build release version. Note: is ignored when --cov is enabled
    --conan  path      Path to conan (default is the system conan)
    --cmake  path      Path to cmake (default is the system cmake)
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
        --cov)
            COVERAGE=true
            COVERAGE_OUT="$2"
            shift # past argument
            shift # past value
            ;;
        -r|--release)
            BUILD_TYPE=Release
            shift # past argument
            ;;
        --cmake)
            CMAKE="$2"
            shift # past argument
            shift # past value
            ;;
        --conan)
            CONAN="$2"
            shift # past argument
            shift # past value
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

if command -v ninja &> /dev/null ]
then
    CMAKE_GENERATOR="-G Ninja"
fi

if [ "$COVERAGE" = true ] ; then
    (cd ${BUILD_DIR} && ${CMAKE} ${CMAKE_GENERATOR} -DCODE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ..)
else
    (cd ${BUILD_DIR} && ${CMAKE} ${CMAKE_GENERATOR} -DCODE_COVERAGE=ON -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ..)
fi

num_threads=`grep -c '^processor' /proc/cpuinfo`
${CMAKE} --build ${BUILD_DIR} -- -j${num_threads}
${CMAKE} --build ${BUILD_DIR} --target docs -- -j${num_threads}

if [ "$COVERAGE" = true ] ; then
    lcov --capture --directory . --output-file ${COVERAGE_OUT}
    lcov --remove coverage.info '/usr/*' --output-file ${COVERAGE_OUT}
fi

exit 0