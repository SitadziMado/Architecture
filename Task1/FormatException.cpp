#include "stdafx.h"
#include "FormatException.h"

FormatException::FormatException(const std::string& what_arg)
    : std::runtime_error(what_arg)
{
}

FormatException::FormatException(const char* what_arg)
    : std::runtime_error(what_arg)
{
}