#!/usr/bin/bash

function clean(){
    rm -rf "$OUTPUTS_DIR"
    rm -rf "/tmp/test*.txt"
}

function ctrl_c(){
	echo -e "\n\n[!] Saliendo...\n"
    clean 2> /dev/null
	exit 1
}

trap ctrl_c SIGINT

ROOT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
INPUTS_DIR="$ROOT_DIR/../inputs"
OUTPUTS_DIR="$ROOT_DIR/../outputs"
EXPECTED_OUTPUTS_DIR="$ROOT_DIR/../expected_outputs"

FLUIDAPP="$ROOT_DIR/../build/fluidapp"
READFILE="$ROOT_DIR/../read_particles"

if [ -d "$OUTPUTS_DIR" ]; then
    rm -rf "$OUTPUTS_DIR"
fi
mkdir "$OUTPUTS_DIR"

if [ ! -f "$FLUIDAPP" ]; then
    echo -e "[!] Build FLUIDAPP before testing it"
    echo -e "[i] You can build it with the command: cmake -S . -B build -DCMAKE_BUILD_TYPE=Release; cmake --build build"
    exit 1
fi

echo -e "\n----------------"
echo -e "Testing FLUIDAPP"
echo -e "----------------\n"

echo -e "\nTest 1: Incorrect number of arguments"
echo -e "-----------------------------------------------------------------"

echo -n "fluidapp -> "
$FLUIDAPP 2> "/tmp/test1.txt"
if [ $? -eq 255 ]; then
    echo -e "Error: Invalid number of arguments: 0." > "/tmp/test1_expected.txt"
    diff "/tmp/test1.txt" "/tmp/test1_expected.txt"
    if [ $? -eq 0 ]; then
        echo -e "OK"
    else
        echo -e "Error: message is not correct"
    fi
else
        echo -e "Error: Invalid return code"
fi

echo -n "fluidapp 4 -> "
$FLUIDAPP 4 2> "/tmp/test1.txt"
if [ $? -eq 255 ]; then
    echo -e "Error: Invalid number of arguments: 1." > "/tmp/test1_expected.txt"
    diff "/tmp/test1.txt" "/tmp/test1_expected.txt"
    if [ $? -eq 0 ]; then
        echo -e "OK"
    else
        echo -e "Error: message is not correct"
    fi
else
        echo -e "Error: Invalid return code"
fi

echo -n "fluidapp 4 init.fld -> "
$FLUIDAPP 4 init.fld 2> "/tmp/test1.txt"
if [ $? -eq 255 ]; then
    echo -e "Error: Invalid number of arguments: 2." > "/tmp/test1_expected.txt"
    diff "/tmp/test1.txt" "/tmp/test1_expected.txt"
    if [ $? -eq 0 ]; then
        echo -e "OK"
    else
        echo -e "Error: message is not correct"
    fi
else
        echo -e "Error: Invalid return code"
fi

echo -n "fluidapp 4 init.fld final.fld 45 -> "
$FLUIDAPP 4 init.fld final.fld 45 2> "/tmp/test1.txt"
if [ $? -eq 255 ]; then
    echo -e "Error: Invalid number of arguments: 4." > "/tmp/test1_expected.txt"
    diff "/tmp/test1.txt" "/tmp/test1_expected.txt"
    if [ $? -eq 0 ]; then
        echo -e "OK"
    else
        echo -e "Error: message is not correct"
    fi
else
        echo -e "Error: Invalid return code"
fi


echo -e "\nTest 2: Time steps not numeric"
echo -e "-----------------------------------------------------------------"

echo -n "fluidapp test init.fld final.fld -> "
$FLUIDAPP test init.fld final.fld 2> "/tmp/test2.txt"
if [ $? -eq 255 ]; then
    echo -e "Error: time steps must be numeric." > "/tmp/test2_expected.txt"
    diff "/tmp/test2.txt" "/tmp/test2_expected.txt"
    if [ $? -eq 0 ]; then
        echo -e "OK"
    else
        echo -e "Error: message is not correct"
    fi
else
        echo -e "Error: Invalid return code"
fi


echo -e "\nTest 3: Negative time steps"
echo -e "-----------------------------------------------------------------"

echo -n "fluidapp -4 init.fld final.fld -> "
$FLUIDAPP -4 init.fld final.fld 2> "/tmp/test3.txt"
if [ $? -eq 254 ]; then
    echo -e "Error: Invalid number of time steps." > "/tmp/test3_expected.txt"
    diff "/tmp/test3.txt" "/tmp/test3_expected.txt"
    if [ $? -eq 0 ]; then
        echo -e "OK"
    else
        echo -e "Error: message is not correct"
    fi
else
        echo -e "Error: Invalid return code"
fi

echo -e "\nTest 4: Non-existent input file"
echo -e "-----------------------------------------------------------------"

echo -n "fluidapp 4 non-existent.fld final.fld -> "
$FLUIDAPP 4 non-existent.fld final.fld 2> "/tmp/test4.txt"
if [ $? -eq 253 ]; then
    echo -e "Error: Cannot open non-existent.fld for reading." > "/tmp/test4_expected.txt"
    diff "/tmp/test4.txt" "/tmp/test4_expected.txt"
    if [ $? -eq 0 ]; then
        echo -e "OK"
    else
        echo -e "Error: message is not correct"
    fi
else
        echo -e "Error: Invalid return code"
fi

echo -e "\nTest 5: Existent output file"
echo -e "-----------------------------------------------------------------"

# Print just the last value of each directory

echo -n "fluidapp 4 small.fld -> "
$FLUIDAPP 4 "$INPUTS_DIR/small.fld" "$INPUTS_DIR/small.fld" 2> "/tmp/test5.txt"
if [ $? -eq 252 ]; then
    echo -e "Error: Cannot open $INPUTS_DIR/small.fld for writing." > "/tmp/test5_expected.txt"
    diff "/tmp/test5.txt" "/tmp/test5_expected.txt"
    if [ $? -eq 0 ]; then
        echo -e "OK"
    else
        echo -e "Error: message is not correct"
    fi
else
        echo -e "Error: Invalid return code"
fi

for i in {1..5}; do
    echo -e "\nTest $((5+i)): Execution with $i time steps"
    echo -e "-----------------------------------------------------------------"

    echo -n "fluidapp $i small.fld -> "
    $FLUIDAPP $i "$INPUTS_DIR/small.fld" "$OUTPUTS_DIR/small-$i.fld" &> /dev/null
    if [ $? -eq 0 ]; then
        $READFILE $EXPECTED_OUTPUTS_DIR/small-$i.fld > /tmp/expected_small-$i.txt
        $READFILE $OUTPUTS_DIR/small-$i.fld > /tmp/small-$i.txt
        diff /tmp/expected_small-$i.txt /tmp/small-$i.txt &> /dev/null
        if [ $? -eq 0 ]; then
            echo -e "OK"
        else
            echo -e "Error: output file is not correct"
        fi
    else
        echo -e "Error: Invalid return code"
    fi
done

for i in {1..5}; do
    echo -e "\nTest $((10+i)): Execution with $i time steps"
    echo -e "-----------------------------------------------------------------"

    echo -n "fluidapp $i large.fld -> "
    $FLUIDAPP $i "$INPUTS_DIR/large.fld" "$OUTPUTS_DIR/large-$i.fld" &> /dev/null
    if [ $? -eq 0 ]; then
        $READFILE $EXPECTED_OUTPUTS_DIR/large-$i.fld > /tmp/expected_large-$i.txt
        $READFILE $OUTPUTS_DIR/large-$i.fld > /tmp/large-$i.txt
        diff /tmp/expected_large-$i.txt /tmp/large-$i.txt &> /dev/null
        if [ $? -eq 0 ]; then
            echo -e "OK"
        else
            echo -e "Error: output file is not correct"
        fi
    else
        echo -e "Error: Invalid return code"
    fi
done

echo -e "\nTest 16: Performance test on large dataset with 1000 iterations"
echo -e "-----------------------------------------------------------------"
perf stat $FLUIDAPP 1000 "$INPUTS_DIR/large.fld" "$OUTPUTS_DIR/large-1000.fld" > /dev/null 2> /tmp/test16.txt
cat /tmp/test16.txt | grep -v "Performance counter stats" | tail -n 18 | head -n 16
