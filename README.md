# Anti-VM techniques

Basic implementation of several anti-vm techniques (Windows) for educational purpose. They rely on checking different parts of OS and hardware to find out if the script is running in a VM.

## Implemented techniques

[x] CPU hypervisor bit (CPUID)
[x] CPU id string (CPUID)
[x] CPU brand string (CPUID)
[x] BIOS manufacturer string
[X] BIOS version string
[x] Screen resolution
[x] Amount of physical memory
[x] Number of CPU cores
[x] Amount of disk space

## To be implemented

[ ] Global Descriptor Table location
[ ] Local Descriptor Table location
[ ] Interrupt Descriptor Table location
[ ] ACPI VM-based string checks
[ ] VM-based hostnames and usernames
[ ] VM-based MAC addresses
