#include <string>
#include "detectors.hpp"
#include <windows.h>
#include <intrin.h>
#include <array>
#include <string>
#include "wmi.hpp"
#include <Wbemidl.h>
#include <iostream>
#include "utils.hpp"
#include <vector>

std::vector<std::string> detect::vm_names = { "virtual", "qemu", "vmware", "oracle", "innotek" };
std::vector<std::string> detect::legit_cpu_ids = { "AuthenticAMD", "GenuineIntel" };

bool detect::cpu_hypervisor_bit() {
	/*
		Check for hypervisor - enabled bit.
		eax = 0x1, 31st-bit of ECX
	*/
	std::array<int, 4> cpuInfo = { 0, 0, 0, 0 };
	__cpuid(cpuInfo.data(), 0x1);

	return (cpuInfo[2] >> 31) & 0x1;
}

bool detect::cpu_id() {
	/*
		Check for fake-vm CPU ID string.
		eax = 0x0, EBX + ECX + EDX (12 bytes)
	*/

	// TODO: I think there is something wrong with the regs order.
	std::array<int, 4> cpuInfo = { 0, 0, 0, 0 };
	std::string cpu = "";
	__cpuid(cpuInfo.data(), 0x0);

	for (int i = 1; i <= 3; i++) {
		cpu += cpuInfo[i] & 0xff;
		cpu += (cpuInfo[i] >> 8) & 0xff;
		cpu += (cpuInfo[i] >> 16) & 0xff;
		cpu += (cpuInfo[i] >> 24) & 0xff;
	}

	return !utils::str_includes(cpu, detect::legit_cpu_ids);
}


bool detect::cpu_brand() {
	/*
		Check for fake-vm CPU brand name.
		eax = 0x80000002, 0x80000003, 0x80000004
		(EAX + EBX + ECX + EDX) x 3 = 36 bytes
	*/
	std::array<int, 4> cpuInfo = { 0, 0, 0, 0 };
	std::string cpu = "";

	for (int id = 2; id <= 4; id++) {
		__cpuid(cpuInfo.data(), 0x80000000 + id);

		for (auto i : cpuInfo) {
			cpu += std::tolower(i & 0xff);
			cpu += std::tolower((i >> 8) & 0xff);
			cpu += std::tolower((i >> 16) & 0xff);
			cpu += std::tolower((i >> 24) & 0xff);
		}
	}

	return utils::str_includes(cpu, detect::vm_names);
}


bool detect::bios_manufacturer() {
	/*
		Check for fake-vm BIOS manufacturer string.
	*/
	std::string manufacturer = "";
	wmi::get_wmi("select * from Win32_BIOS", [&](IWbemClassObject* pclsObj, VARIANT* vtProp) {
		HRESULT hr = pclsObj->Get(L"Manufacturer", 0, vtProp, 0, 0);
		manufacturer = utils::lowercase(utils::bstr_to_str(vtProp->bstrVal));
		});

	std::cout << manufacturer << std::endl;

	return utils::str_includes(manufacturer, detect::vm_names);
}

bool detect::bios_version() {
	/*
		Check for fake-vm BIOS version string.
	*/
	std::string version = "";
	wmi::get_wmi("select * from Win32_BIOS", [&](IWbemClassObject* pclsObj, VARIANT* vtProp) {
		HRESULT hr = pclsObj->Get(L"SMBIOSBIOSVersion", 0, vtProp, 0, 0);
		version = utils::lowercase(utils::bstr_to_str(vtProp->bstrVal));
		});

	return utils::str_includes(version, detect::vm_names);
}

bool detect::screen_resolution() {
	/*
		Check for uncommon screen resolutions.
	*/
	auto w = GetSystemMetrics(SM_CXSCREEN);
	auto h = GetSystemMetrics(SM_CYSCREEN);

	// Standard and common resolutions
	if (w == 1600 && h == 900) return false;
	if (w == 1920 && h == 1080) return false;
	if (w == 1920 && h == 1200) return false;
	if (w == 2560 && h == 1440) return false;
	if (w == 3840 && h == 2160) return false;

	return true;
}

bool detect::memory_amount() {
	/*
		Check for suspicious amount of physical memory.
	*/
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);

	if (GlobalMemoryStatusEx(&statex) == FALSE) {
		return true;
	}

	// Minimum 4 GB
	return statex.ullTotalPhys / 1024 / 1024 < 4096;
}

bool detect::cpu_cores() {
	/*
		Check for suspicious amount of CPU cores.
	*/
	SYSTEM_INFO info;
	GetSystemInfo(&info);

	return info.dwNumberOfProcessors < 4;
}

bool detect::disk_space() {
	/*
		Check for suspicious amount of total disk space.
	*/
	ULARGE_INTEGER bytes;
	BOOL result = GetDiskFreeSpaceExA(
		"\\\\?\\c:\\",
		NULL,
		&bytes,
		NULL
	);

	if (result == FALSE) return true;

	return bytes.QuadPart / 1024 / 1024 / 1024 < 100;
}