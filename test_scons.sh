scons -c >/dev/null

i=16

time scons -j $i 2>&1 | grep real 
scons -c >/dev/null
sleep 2
time scons -j $i 2>&1 | grep real
scons -c >/dev/null
sleep 2
time scons -j $i 2>&1 | grep real
scons -c >/dev/null
sleep 2
time scons -j $i 2>&1 | grep real 
scons -c >/dev/null
sleep 2
time scons -j $i 2>&1 | grep real 
scons -c >/dev/null
