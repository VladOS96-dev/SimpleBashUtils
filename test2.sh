#!/bin/sh

FILES=(../datasets/test_dataset/file_1.txt ../datasets/test_dataset/file_2.txt ../datasets/test_dataset/file_3.txt grep/s21_grep.c grep/s21_grep.h)
N_FILES=3
FLAGS=(-i -v -c -l -n -h -s -o)
PATTERNS=(ABC DEF 123 foo "//[^\r\n]*[\r\n]")
PATTERN_FILES=(../datasets/test_dataset/pattern_file_1.txt ../datasets/test_dataset/pattern_file_2.txt )
N_PATTERN_FILES=2

CASE_RESULT=""
N_FAILS=0
N_SUCCESS=0
IS_OK=1

for flag in ${FLAGS[@]}
do
    for pattern in ${PATTERNS[@]}
    do
        for filename in ${FILES[@]}
        do
            TEST_CASE="$flag $pattern $filename"
            grep/s21_grep $TEST_CASE > s21_case_result.txt
            grep $TEST_CASE > std_case_result.txt
            CASE_RESULT=$(diff -s s21_case_result.txt std_case_result.txt)
            if [ "$CASE_RESULT" == "Files s21_case_result.txt and std_case_result.txt are identical" ]
                then
                    (( N_SUCCESS++ ))
                    echo "$TEST_CASE PASSED"
                else
                    (( N_FAILS++ ))
                    echo "$TEST_CASE FAILED"
                    IS_OK=0
                    echo "S21 OUTPUT"
                    cat s21_case_result.txt
                    echo "STD OUTPUT"
                    cat std_case_result.txt
                    break
            fi
            rm s21_case_result.txt std_case_result.txt
        done
        if [ "$IS_OK" == 0 ]
            then
                break
        fi
    done
    if [ "$IS_OK" == 0 ]
        then
            break
    fi
done

for flag in ${FLAGS[@]}
do
    for pattern in ${PATTERNS[@]}
    do
        for filename in ${FILES[@]}
        do
            TEST_CASE="$flag -e $pattern $filename"
            grep/s21_grep $TEST_CASE > s21_case_result.txt
            grep $TEST_CASE > std_case_result.txt
            CASE_RESULT=$(diff -s s21_case_result.txt std_case_result.txt)
            if [ "$CASE_RESULT" == "Files s21_case_result.txt and std_case_result.txt are identical" ]
                then
                    (( N_SUCCESS++ ))
                    echo "$TEST_CASE PASSED"
                else
                    (( N_FAILS++ ))
                    echo "$TEST_CASE FAILED"
                    IS_OK=0
                    echo "S21 OUTPUT"
                    cat s21_case_result.txt
                    echo "STD OUTPUT"
                    cat std_case_result.txt
                    break
            fi
            rm s21_case_result.txt std_case_result.txt
        done
        if [ "$IS_OK" == 0 ]
            then
                break
        fi
    done
    if [ "$IS_OK" == 0 ]
        then
            break
    fi
done

MORE_FLAGS=(-i -v -c -l -n -h -s)

for flag in ${MORE_FLAGS[@]}
do
    for pattern in ${PATTERN_FILES[@]}
    do
        for filename in ${FILES[@]}
        do
            TEST_CASE="$flag -f $pattern $filename"
            grep/s21_grep $TEST_CASE > s21_case_result.txt
            grep $TEST_CASE > std_case_result.txt
            CASE_RESULT=$(diff -s s21_case_result.txt std_case_result.txt)
            if [ "$CASE_RESULT" == "Files s21_case_result.txt and std_case_result.txt are identical" ]
                then
                    (( N_SUCCESS++ ))
                    echo "$TEST_CASE PASSED"
                else
                    (( N_FAILS++ ))
                    echo "$TEST_CASE FAILED"
                    IS_OK=0
                    echo "S21 OUTPUT"
                    cat s21_case_result.txt
                    echo "STD OUTPUT"
                    cat std_case_result.txt
                    break
            fi
            rm s21_case_result.txt std_case_result.txt
        done
        if [ "$IS_OK" == 0 ]
            then
                break
        fi
    done
    if [ "$IS_OK" == 0 ]
        then
            break
    fi
done



for flag_1 in ${MORE_FLAGS[@]}
do
    for flag_2 in ${MORE_FLAGS[@]}
    do
        for pattern in ${PATTERN_FILES[@]}
        do
            for filename in ${FILES[@]}
            do
                if [ $flag_1 != $flag_2 ]
                then
                    TEST_CASE="$flag_1 $flag_2 -f $pattern $filename"
                    grep/s21_grep $TEST_CASE > s21_case_result.txt
                    grep $TEST_CASE > std_case_result.txt
                    CASE_RESULT=$(diff -s s21_case_result.txt std_case_result.txt)
                    if [ "$CASE_RESULT" == "Files s21_case_result.txt and std_case_result.txt are identical" ]
                        then
                            (( N_SUCCESS++ ))
                            echo "$TEST_CASE PASSED"
                        else
                            (( N_FAILS++ ))
                            echo "$TEST_CASE FAILED"
                            IS_OK=0
                            echo "S21 OUTPUT"
                            cat s21_case_result.txt
                            echo "STD OUTPUT"
                            cat std_case_result.txt
                            break
                    fi
                    rm s21_case_result.txt std_case_result.txt
                fi
            done
            if [ "$IS_OK" == 0 ]
                then
                    break
            fi
        done
        if [ "$IS_OK" == 0 ]
            then
                break
        fi
    done
    if [ "$IS_OK" == 0 ]
        then
            break
    fi
done


echo "$N_SUCCESS tests are passed and $N_FAILS tests are failed"