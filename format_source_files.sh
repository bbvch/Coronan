#!/usr/bin/env bash

set -e

SELF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"

find "${SELF_DIR}" \( -name '*.h' -o -name '*.cpp' \) -print0 | xargs -0 clang-format -i -style=file
find "${SELF_DIR}" \( -name 'CMakeLists.txt' -o -name '*.cmake' \) -print0 | xargs -0 cmake-format -i
