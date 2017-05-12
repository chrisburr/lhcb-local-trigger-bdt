#!/usr/bin/env bash
# Set bash strict mode: http://redsymbol.net/articles/unofficial-bash-strict-mode/
set -euo pipefail
IFS=$'\n\t'

function run_rate_test {
    BDT_THRESHOLD="${1}"

    # Ensure the PATH is valid to import by replacing '.' with '_'
    MOORE_DIRECTORY=$(echo "MooreDev_v26r2_BDT_Threshold_${BDT_THRESHOLD}" | sed 's/\./_/g')

    # Ensure the directory doesn't already exist
    if [ -d "${MOORE_DIRECTORY}" ]; then
        echo "Skipping: Directory already exists - ${MOORE_DIRECTORY}"
    fi

    # Setup Moore
    lb-dev --name "${MOORE_DIRECTORY}" Moore/v26r2 > /dev/null 2>&1
    cd "${MOORE_DIRECTORY}"
    git lb-use Hlt > /dev/null 2>&1
    git lb-checkout Hlt/2017-reference Hlt/HltSettings > /dev/null
    getpack PRConfig head > /dev/null

    # Set the BDT Threshold
    set_bdt_threshold "${BDT_THRESHOLD}"

    echo "Building Moore in ${MOORE_DIRECTORY}"
    make -j1 > /dev/null 2>&1

    echo "Running rate tests in ${MOORE_DIRECTORY}"
    ./run python PRConfig/python/MooreTests/Moore_RateTest.py --inputdata=2016_Hlt1_0x11361609 \
        --L0TCK=0x1609 --evtmax=1000 --input_rate=100.e3 > Moore_RateTest.log 2>&1

    echo "Completed for ${MOORE_DIRECTORY}"
    # Go back to the original directory
    cd ..
}

function set_bdt_threshold {
    BDT_THRESHOLD="${1}"

    SOURCE_FILENAME=Hlt/HltSettings/python/HltSettings/CharmHad/CharmHad_pp_2017.py
    TMP_FILE=$(mktemp --suffix=".py")

    # Try to make the substitution (disable strict mode)
    set +e
    OLD="'BDT_Threshold': 1.455," NEW="'BDT_Threshold': ${BDT_THRESHOLD}," perl -pe '
        $count += s/$ENV{OLD}/$ENV{NEW}/g;
        END{exit $count}
    ' "${SOURCE_FILENAME}" > "${TMP_FILE}"
    N_REPLACEMENTS=$?
    set -e
    mv "${TMP_FILE}" "${SOURCE_FILENAME}"

    # Ensure only one line has been replaced
    if [ "${N_REPLACEMENTS}" -ne "1" ]; then
        echo "ERROR: Failed to modifying BDT Threshold"
        exit 1
    fi
}

# Parse the arguments
if [ "$#" -eq 1 ]; then
    BDT_THRESHOLD="${1}"
    echo "Running for threshold ${BDT_THRESHOLD}"
    run_rate_test "${BDT_THRESHOLD}"
elif [ "$#" -eq 3 ]; then
    LOW="${1}"
    HIGH="${2}"
    STEP="${3}"
    echo "Will run for thresholds: $(for i in $(seq "${LOW}" "${STEP}" "${HIGH}"); do echo -ne "${i} "; done)"
    # Run one job per CPU core until all thresholds have been tested
    seq "${LOW}" "${STEP}" "${HIGH}" | xargs -i --max-procs="$(nproc)" "./${0}" {}
else
    echo "ERROR: Invalid number of arguments passed ($#)"
    exit 1
fi

