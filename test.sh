#! /bin/sh

runtest_return() {
    ./mycc "$1" > tmp.s

    gcc -o tmp tmp.s
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

    gcc -o tmp tmp.s $3
    ./tmp
    out=`./tmp`

    if [ "$out" != "$2" ]; then
        echo "$1: $2 expected, but got $out"
        exit 1
    fi
    echo "$1 => $2"
    rm tmp 
}


runtest_return "3+2;" 5
runtest_return "3+4+1;" 8
runtest_return "80-30;" 50
runtest_return "8-5+3;" 6
runtest_return "6*4;" 24
runtest_return "4/2;" 2
runtest_return "6*3/2;" 9
runtest_return "5+2*4-8;" 5
runtest_return "2*(4+6);" 20
runtest_return "(4+6)*2;" 20
runtest_return "(3+6)/(1+2);" 3
runtest_return "(3+6)*((1+2+12)/(4+5-6));" 45
runtest_return "6==3;" 0
runtest_return "2==2;" 1
runtest_return "10!=10;" 0
runtest_return "10!=7;" 1
runtest_return "(3+6) == (3*3);" 1
runtest_return "5+4;  8-3;" 5
runtest_return "5+4*3;  8-3; 10*2-5;" 15
runtest_return "5+4*3;  8*3; 4*3 == 8+4*2/2;" 1
runtest_return "a = 4; a;" 4
runtest_return "a = 4; a = a + 1; a;" 5
runtest_return "a = 4; b = 5; c = a + b; c;" 9
runtest_return "foo = 5; hoge = 2; foo;" 5

runtest_print "foo();" "OK" "test.o"
runtest_print "a = 10; print(a);" 10 "test.o"
runtest_print "a = 10; print(a/2*5);" 25 "test.o"
runtest_print "add(5,8,20);" 33 "test.o"
runtest_print "a = 5; b = 8; c = a*b; add(a,b,c);" 53 "test.o"
runtest_print "a = 5; b = 8; c = a*b; add(a*b,b,c);" 88 "test.o"

echo 'Success!!'

