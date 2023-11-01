# Understanding the Concept of Platform Devices and Drivers

- Linux handles devices using buses - matching CPUs to devices
- Some buses allow discoverability using enumeration (e.g., USB, PCI, SATA)
- Not all buses support discoverability, as a result the platform concept was invented
- Non-discoverable devices are known as platform devices, because they use the concept of platform bus, maintaining the Linux concept where all devices are connected through buses.