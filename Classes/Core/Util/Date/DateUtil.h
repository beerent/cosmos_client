#pragma once

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <codecvt>
#include <locale>

namespace Util {
    std::time_t StringToDateTime(const std::string& dateTime);
}
