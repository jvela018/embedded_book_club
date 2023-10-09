# File Operations Example


1. Build module
```bash
make
```

2. Load module
```bash
sudo insmod file_operations.ko
```

3. Write to pseudo device
```bash
echo -n "Hello" > /dev/ebc_device
```

4. Read from device
```bash
cat /dev/ebc_device
```

5. Unload module
```bash
sudo rmmod file_operations
```

6. Check kernel ring buffer log
```bash
dmesg
```

You can use the run.sh script to make sure that the module loads, and unloads without any issues.
