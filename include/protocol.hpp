#if !defined(PROTOCOL_HPP)
#define PROTOCOL_HPP

#include <map>
#include <tuple>
#include <string>
#include <json/json.h>

#include "bytecode.hpp"
#include "matched_packet.hpp"
#include "virtual_machine.hpp"
#include "elicitation_utils.hpp"

class Protocol
{
public:
  Protocol(const Json::Value& );
  std::tuple<matched_packet &, bool> match(std::vector<unsigned char> &, int len, matched_packet &);

  int get_layer() const { return layer; }
  Pattern get_pattern() const { return pattern; }
  std::string get_name() const { return name; }
  std::vector<std::vector<std::string>> get_requirements() const { return requirements; }

  static Protocol from_file(const std::string &);
  static std::map<std::string, const Protocol*> protocol_mapper;
private:

  std::string name;
  Pattern pattern;

  int id;
  int layer;
  std::vector<std::vector<std::string>> requirements;
  std::vector<std::vector<Base_expression<int>*>> conditions;

  Base_expression<int> *size;

  static int id_generator_core;
};

#endif
