#pragma once

#include <stdexcept>
#include <string>

class FormatException : public std::runtime_error
{
public:
    explicit FormatException(const std::string& what_arg);
    explicit FormatException(const char* what_arg);
};

