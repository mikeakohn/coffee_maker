#!/usr/bin/env sh

SEQUENCE="0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19"

cd sample/

echo "Normal"
LD_LIBRARY_PATH=.. java -classpath ../jni:. Sample ${SEQUENCE}

echo "Force Java JIT compiler"
LD_LIBRARY_PATH=.. java -Xcomp -classpath ../jni:. Sample ${SEQUENCE}

