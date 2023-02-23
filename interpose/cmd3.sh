# unset LD_PRELOAD
# ./cmd4.sh
# unset LD_PRELOAD
export LD_PRELOAD="./libempty.so"
/usr/bin/echo "Running cmd3.sh:/usr/bin/echo"
./cmd5.sh
