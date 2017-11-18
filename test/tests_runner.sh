#!/bin/bash

black=`tput setaf 0`
red=`tput setaf 1`
green=`tput setaf 2`
yellow=`tput setaf 3`
blue=`tput setaf 4`
magenta=`tput setaf 5`
cyan=`tput setaf 6`
white=`tput setaf 7`

underline=`tput sgr 0 1`
reset=`tput sgr0`

tests=`find ./test/tests/* -maxdepth 1 -type d`
tests_number=`find ./test/tests/* -maxdepth 1 -type d | wc -l`
n=1

tests_total=0
errors_total=0

while [ $n -le $tests_number ]
do
    if [ $n -le 9 ]; then
        dir="0$n"
    else
        dir="$n"
    fi

    tests_dir=`find ./test/ -type d -name "$dir-*"`

    info=`cat $tests_dir/tests.info`
    echo "${magenta}Running tests:$cyan $info$reset"

    test=1

    while true
    do
        if [ $test -le 9 ]; then
            test_id="0$test"
        else
            test_id="$test"
        fi

        next=`find $tests_dir/ -maxdepth 1 -type f -name "$test_id*" | wc -l`
        if [ "$next" -eq 3 ]; then
            exitcode_expected=`cat $tests_dir/$test_id.exitcode`
            exitcode="$?"

            if [ "$exitcode" -eq "$exitcode_expected" ]; then
                stdout_expected=`cat ./test/ifj17.bas $tests_dir/$test_id.ifj > tmp.bas; fbc -w 1000 tmp.bas; ./tmp < $tests_dir/$test_id.stdin`
                gencode=`./ifj2017 < $tests_dir/$test_id.ifj > temp.code`
                stdout=`./test/ic17int temp.code < $tests_dir/$test_id.stdin 2>&1`

                if [ "$stdout" != "$stdout_expected" ]; then
                    echo "$red- ERROR [$test_id]:\n-- Expected stdout:\n $reset $stdout_expected $red\n-- Obtained: $reset\n$stdout.$reset"
                    errors_total=$(( errors_total+1 ))
                fi
            else
                echo "$red- ERROR [$test_id]: Expected exitcode: $exitcode_expected, obtained: $exitcode.$reset"
                errors_total=$(( errors_total+1 ))
            fi
            tests_total=$(( tests_total+1 ))
        else
            break
        fi

	    test=$(( test+1 ))
    done

	n=$(( n+1 ))
done

if [ "$errors_total" -eq "0" ]; then
    echo "${green}---------------------------------\n ALL TESTS ($tests_total) PASSED!\n---------------------------------$reset"
else
    echo "${red}---------------------------------\n Tests contains $errors_total errors!\n---------------------------------$reset"
fi