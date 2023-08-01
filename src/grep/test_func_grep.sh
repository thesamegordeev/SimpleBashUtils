#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
"s test_0_grep.txt VAR"
"for grep.c grep.h Makefile VAR"
"for grep.c VAR"
"-e for -e ^int grep.c grep.h Makefile VAR"
"-e for -e ^int grep.c VAR"
"-e regex -e ^print grep.c VAR -f test_ptrn_grep.txt"
"-e while -e void grep.c Makefile VAR -f test_ptrn_grep.txt"
)

declare -a extra=(
"-n for test_1_grep.txt test_2_grep.txt"
"-n -e ^\} test_1_grep.txt"
"-ce ^int test_1_grep.txt test_2_grep.txt"
"-nivh = test_1_grep.txt test_2_grep.txt"
"-e"
"-ie INT test_3_grep.txt"
"-echar test_1_grep.txt test_2_grep.txt"
"-ne = -e out test_3_grep.txt"
"-iv INT test_3_grep.txt"
"-in int test_3_grep.txt"
"-cl aboba test_1_grep.txt test_3_grep.txt"
"-v test_1_grep.txt -e ank"
"-l for test_1_grep.txt test_2_grep.txt"
"-e = -e out test_3_grep.txt"
"-e ing -e as -e the -e not -e is test_4_grep.txt"
"-l for no_file.txt test_2_grep.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./grep $t > test_grep.log
    grep $t > test_sys_grep.log
    DIFF_RES="$(diff -s test_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
      echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m grep $t"
    else
      (( FAIL++ ))
      echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m grep $t"
    fi
    rm test_grep.log test_sys_grep.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in v c l n h
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 2 параметра
for var1 in v c l n h
do
    for var2 in v c l n h
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

# 3 параметра
for var1 in v c l n h
do
    for var2 in v c l n h
    do
        for var3 in v c l n h
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done

# 2 сдвоенных параметра
for var1 in v c l n h
do
    for var2 in v c l n h
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                testing $i
            done
        fi
    done
done

# 3 строенных параметра
for var1 in v c l n h
do
    for var2 in v c l n h
    do
        for var3 in v c l n h
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3"
                    testing $i
                done
            fi
        done
    done
done

echo "\033[31mFAIL: $FAIL\033[0m"
echo "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"
