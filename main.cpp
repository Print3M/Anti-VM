#include <stdio.h>
#include <windows.h>
#include <intrin.h>
#include <array>
#include <string>
#include <Wbemidl.h>
#include <iostream>
#include "detectors.hpp"

/*
	TODO:
		- memory && CPU resources
*/

int main(int argc, char* argv[]) {
	printf("CPU Brand Name: %d\n", detect::cpu_brand());
	printf("CPU ID: %d\n", detect::cpu_id());
	printf("CPU Hypervisor bit: %d\n", detect::cpu_hypervisor_bit());
	printf("BIOS Manufacturer: %d\n", detect::bios_manufacturer());
	printf("BIOS Version: %d\n", detect::bios_version());
	printf("Scren resolution: %d\n", detect::screen_resolution());
	printf("Memory amount: %d\n", detect::memory_amount());
	printf("CPU cores: %d\n", detect::cpu_cores());
	printf("Disk space: %d\n", detect::disk_space());
}
