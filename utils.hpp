#pragma once
#include <string>
#include <vector>
#include <wtypes.h>

namespace utils {
	std::string bstr_to_str(BSTR bstr);
	std::string lowercase(std::string str);
	bool str_includes(std::string str, std::vector<std::string> includes);
};