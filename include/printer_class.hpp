#if !defined(PRINTER_CLASS)
#define PRINTER_CLASS

#include <vector>
#include <string>

#include "matched_packet.hpp"

class Printer
{
public:
    Printer();
    int update(const matched_packet &);
};

#endif
