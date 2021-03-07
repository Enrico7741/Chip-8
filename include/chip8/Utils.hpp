//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#include <string>
#include <sstream>
#include <iomanip>

/**
 * Methode which returns the string representation of the register value
 * Only useable if T is an integral type.
 */
template<typename T, int size = sizeof(T), class = typename std::enable_if_t<std::is_integral_v<T>>>
std::string getValueString(T val)
{
    auto value = static_cast<int>(val);
    std::stringstream stream;
    stream << std::hex << std::setw(2 * size) << std::setfill('0') << value;
    return stream.str();
}