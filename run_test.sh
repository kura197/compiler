#! /bin/sh

runtest_return() {
    ./mycc "$1" > tmp.s

    gcc -o tmp tmp.s -g
    ./tmp

    out=$?
    if [ "$out" != "$2" ]; then
        echo "$1: $2 expected, but got $out"
        exit 1
    fi
    echo "$1 => $2"
    rm tmp 
}

runtest_print() {
    ./mycc "$1" > tmp.s

    gcc -o tmp tmp.s ./test/lib.o -g
    ./tmp
    out=`./tmp`

    if [ "$out" != "$2" ]; then
        echo "$1: $2 expected, but got $out"
        exit 1
    fi
    #echo "$1 => $2"
    rm tmp 
}

if [ $# -gt 0 ] ;
then
    ./mycc "test/wip.c" > tmp.s
    gcc -o tmp tmp.s ./test/lib.o -g
    ./tmp
else
    runtest_print test/t1.c 10
    runtest_print test/t2.c 3
    runtest_print test/t3.c 1024
    runtest_print test/t4.c 75
    runtest_print test/t5.c 3628800
    runtest_print test/t6.c 3
    runtest_print test/t7.c 10
    runtest_print test/t8.c 5
    runtest_print test/t9.c 8
    runtest_print test/t10.c 5
    runtest_print test/t11.c 1
    runtest_print test/t12.c 21
    runtest_print test/t13.c 51
    runtest_print test/t14.c 3
    runtest_print test/t15.c 35
    runtest_print test/t16.c 9
    runtest_print test/t17.c 10
    runtest_print test/t18.c 7
    runtest_print test/t19.c 10
    runtest_print test/t20.c "hello, world : 51"
    runtest_print test/t21.c 1
    runtest_print test/t22.c 2
    runtest_print test/t23.c 2
    runtest_print test/t24.c 2048
    runtest_print test/t25.c "Yes"
    runtest_print test/t26.c 10
    runtest_print test/t27.c "x"
    runtest_print test/t28.c "OK"
    runtest_print test/t29.c "OK"
    runtest_print test/t30.c 3
    runtest_print test/t31.c "x=3, y=2"
    runtest_print test/t32.c 7
    runtest_print test/t33.c "6, 11"
    runtest_print test/t34.c 14
    runtest_print test/t35.c "3, 3, 3"
    runtest_print test/t36.c "28, 1"
    runtest_print test/t37.c "OK"
    runtest_print test/t38.c "0 2 4 1 3 5 "
    runtest_print test/t39.c "Hello, World. 14"
    echo 'Success!!'
fi


