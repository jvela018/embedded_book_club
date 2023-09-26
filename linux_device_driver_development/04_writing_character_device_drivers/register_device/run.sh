echo "--------------------------------------------------------------------"
echo "Cleaning previous build"
echo "make clean"
make clean
echo "--------------------------------------------------------------------"

echo "Building module"
echo "make"
make
echo "--------------------------------------------------------------------"

echo "Print information about the kernel object (.ko) generated "
echo "sudo modinfo register.ko"
sudo modinfo register.ko
echo "--------------------------------------------------------------------"

echo "File type"
echo "file register.ko"
file register.ko
echo "--------------------------------------------------------------------"

echo "Loading module"
echo "sudo insmod register.ko \$1 (\$1 default value is Embedded Book Club)"
sudo insmod register.ko $1
echo "--------------------------------------------------------------------"

echo "Printing kernel's ring buffer message after loading module"
echo "dmesg | tail -1"
dmesg | tail -1
echo "--------------------------------------------------------------------"

echo "Module loaded in sysfs"
echo "ls -al /sys/module | grep register"
ls -al /sys/module | grep register
echo "--------------------------------------------------------------------"

echo "Device available through /dev"
echo "ls -al /dev | grep ebc_device"
ls -al /dev | grep ebc_device
echo "--------------------------------------------------------------------"

echo "List new class and print device information under uevent"
echo "ls /sys/class/ebc_class/ebc_device/"
ls /sys/class/ebc_class/ebc_device/
echo ""
echo "cat /sys/class/ebc_class/ebc_device/uevent"
cat /sys/class/ebc_class/ebc_device/uevent
echo "--------------------------------------------------------------------"

echo "Unloading module"
echo "sudo rmmod register"
sudo rmmod register
echo "--------------------------------------------------------------------"

echo "Module unloaded in sysfs: If successful, no message"
echo "ls -al /sys/module | grep register"
ls -al /sys/module | grep register
echo "--------------------------------------------------------------------"

echo "List new class and print device information under uevent."
echo "cat /sys/class/ebc_class/ebc_device/uevent"
cat /sys/class/ebc_class/ebc_device/uevent
echo "--------------------------------------------------------------------"

echo "printing kernel's ring buffer message after unloading module"
echo "dmesg | tail -1"
dmesg | tail -1
echo "--------------------------------------------------------------------"


