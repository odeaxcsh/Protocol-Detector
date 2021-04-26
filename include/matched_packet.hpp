#if !defined(MATCHED_PACKET)
#define MATCHED_PACKET

#include "virtual_machine.hpp"

struct matched_protocol
{
  std::map<std::string, std::vector<unsigned char>> variables;
  std::string matched_protocol;
  int layer;
};

struct matched_packet
{
  std::vector<matched_protocol> protocols;
  int layer_count = 1;
  Index index;
};

#endif
