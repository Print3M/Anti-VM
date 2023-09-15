#include "utils.hpp"
#include "detectors.hpp"
#include <string>
#include <codecvt>
#include <vector>

std::string utils::bstr_to_str(BSTR bstr) {
	if (!bstr) return std::string("");

	std::wstring ws(bstr, SysStringLen(bstr));
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::string narrow = converter.to_bytes(ws);

	return narrow;
}

std::string utils::lowercase(std::string str) {
	std::string result = "";

	for (auto c : str) {
		result += std::tolower(c);
	}

	return result;
}

bool utils::str_includes(std::string str, std::vector<std::string> includes) {
	for (auto i : includes) {
		if (str.find(i) != std::string::npos) {
			return true;
		}
	}

	return false;
}