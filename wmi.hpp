#pragma once
#include <string>
#include <Wbemidl.h>
#include <functional>

namespace wmi {
	int get_wmi(std::string wql_query, std::function<void(IWbemClassObject*, VARIANT*)> get_property);
};