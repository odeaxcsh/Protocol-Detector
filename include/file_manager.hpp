#if !defined(FILE_MANAGER_HPP)
#define FILE_MANAGER_HPP

#include <string>
#include <vector>

#include "protocol.hpp"

class File_manager
{
public:
    File_manager(const std::string &);
    std::vector<std::vector<Protocol>> get_protocols();

private:
    std::vector<std::string> protocols;

    void include_config_file(const std::string &);
};

#endif