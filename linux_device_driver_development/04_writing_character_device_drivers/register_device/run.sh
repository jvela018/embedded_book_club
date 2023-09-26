make clean
make && clear
sudo insmod register.ko $1
dmesg | tail -1
ls -al /sys/module | grep hello
sudo rmmod register
dmesg | tail -1
ls -al /sys/module | grep hello

