make clean >/dev/null

i=16

time make -j $i 2>&1 | grep real 
make clean >/dev/null
sleep 2
time make -j $i 2>&1 | grep real
make clean >/dev/null
sleep 2
time make -j $i 2>&1 | grep real
make clean >/dev/null
sleep 2
time make -j $i 2>&1 | grep real 
make clean >/dev/null
sleep 2
time make -j $i 2>&1 | grep real 
make clean >/dev/null
