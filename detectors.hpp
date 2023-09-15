#pragma once
#include <string>
#include <vector>

namespace detect {
	extern std::vector<std::string> vm_names;
	extern std::vector<std::string> legit_cpu_ids;

	bool cpu_hypervisor_bit();
	bool cpu_id();
	bool cpu_brand();
	bool bios_manufacturer();
	bool bios_version();
	bool screen_resolution();
	bool memory_amount();
	bool cpu_cores();
	bool disk_space();
};