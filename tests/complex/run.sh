#!/bin/bash

# set -xe

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"

ARTIFACTS="${SCRIPT_DIR}/artifacts"
if [ -d "${ARTIFACTS}" ]; then
  rm -r "${ARTIFACTS}"
fi

BIN="${ARTIFACTS}/bin"
mkdir -p "${BIN}"

GENERATED="${ARTIFACTS}/generated.txt"
PROCESSED="${ARTIFACTS}/processed.txt"

ln -s "${SCRIPT_DIR}/../../bin/process_json" "${BIN}/process_json"

python3 "${SCRIPT_DIR}/generate_jsons.py" 10000 > "${GENERATED}"

cat "${GENERATED}" | "${BIN}/process_json" > "${PROCESSED}"

python3 "${SCRIPT_DIR}/check.py" "${GENERATED}" "${PROCESSED}" && echo OK || echo FAIL

# echo OK