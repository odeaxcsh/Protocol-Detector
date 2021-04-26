#if !defined(PROTOCOL_HPP)
#define PROTOCOL_HPP

#include <map>
#include <tuple>
#include <string>
#include <json/json.h>

#include "bytecode.hpp"
#include "matched_packet.hpp"
#include "virtual_machine.hpp"

class Protocol
{
public:
  Protocol(const Json::Value& );
  std::tuple<matched_packet &, bool> match(std::vector<unsigned char> &, int len, matched_packet &);

  static Protocol from_file(const std::string &);
  static std::map<std::string, const Protocol*> protocol_mapper;
private:

  std::string name;
  Pattern pattern;

  int id;
  int layer;
  std::vector<std::vector<std::string>> requirements;

  static int id_generator_core;
};

#endif
