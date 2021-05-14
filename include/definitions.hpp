#if !defined (DEFINITIONS_HPP)
#define DEFINITIONS_HPP

#define BUFFERS_MAX_LEN 1024

#define SAVE_MATCH_SIGN '$'
#define SAVE_BINARY_SIGN '@'
#define ITERATION_START_SIGN '<'
#define ITERATION_STOP_SIGN '>'

#define ANY_STRING "ANY"
#define NOTHING_STRING "NOTHING"

#include <string>
#include <algorithm>
#include <cctype>

bool is_number(const std::string& s);

#endif
