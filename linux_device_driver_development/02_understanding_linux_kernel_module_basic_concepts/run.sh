make clean
make && clear
sudo insmod hello.ko $1
dmesg | tail -1
ls -al /sys/module | grep hello
sudo rmmod hello
dmesg | tail -1
ls -al /sys/module | grep hello

