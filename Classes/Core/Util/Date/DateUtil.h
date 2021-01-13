#pragma once

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <codecvt>
#include <locale>

namespace Util {
    // Converts UTC time string to a time_t value.
    std::time_t StringToDateTime(const std::string& dateTime)
    {
        std::wstring wStringDateTime = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(dateTime);
       // Let's consider we are getting all the input in
       // this format: '2014-07-25T20:17:22Z' (T denotes
       // start of Time part, Z denotes UTC zone).
       // A better approach would be to pass in the format as well.
       static const std::wstring dateTimeFormat{ L"%Y-%m-%dT%H:%M:%SZ" };

       // Create a stream which we will use to parse the string,
       // which we provide to constructor of stream to fill the buffer.
       std::wistringstream ss{wStringDateTime };

       // Create a tm object to store the parsed date and time.
       std::tm dt;

       // Now we read from buffer using get_time manipulator
       // and formatting the input appropriately.
       ss >> std::get_time(&dt, dateTimeFormat.c_str());

       // Convert the tm structure to time_t value and return.
       return std::mktime(&dt);
    }
}
