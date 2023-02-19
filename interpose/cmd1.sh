# ldd t1
gcc -c empty.c
ls .
export VAR=111
/usr/bin/echo "$VAR"
export LD_PRELOAD=""
/usr/bin/echo "after export LD_PRELOAD="
# ldd t1
unset LD_PRELOAD
/usr/bin/echo "after unset LD_PRELOAD"
# ldd t1
./cmd2.sh
