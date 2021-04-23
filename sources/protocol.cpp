#include "protocol.hpp"
#include "lexer.hpp"
#include "pattern_parser.hpp"
#include "virtual_machine.hpp"

#include <algorithm>
#include <optional>
#include <tuple>
#include <fstream>
#include <json/json.h>
#include <vector>
#include <map>

int Protocol::id_generator_core = 0;
std::map<std::string, const Protocol*> Protocol::protocol_mapper;

Protocol Protocol::from_file(const std::string &path)
{
  std::ifstream file(path, std::ifstream::binary);

  Json::Value json;
  Json::Reader reader;
  bool parsingSuccessful = reader.parse( file, json, false );
  return (Protocol(json));
}

Protocol::Protocol(const Json::Value &value) : requirements(3)
{
  this->id = ++id_generator_core;
  this->name = value["name"].asString();
  this->layer = value["layer"].asInt();
  const std::string &&pattern = value["pattern"].asString();
  Lexer lexer = (pattern.c_str());
  this->pattern = Pattern_parser(lexer).parse();

  for(auto object : value["requirements"])
    for(auto value : object["protocols"])
      this->requirements[object["layer"].asInt()]
        .push_back(value.asString());

  this->protocol_mapper[name] = this;
}

std::tuple<matched_packet &, bool> Protocol::match(const char *packet, matched_packet &matched)
{
  if(this->layer != matched.layer_count)
    return {matched, false};

  for(int i = 0; i < this->layer-1; ++i) {
    if(std::find(requirements[i].begin(), requirements[i].end(), matched.protocols[i].matched_protocol) ==  requirements[i].end())
      return {matched, false};
  }

  VM vm(this->pattern);
  auto variables = vm.run(packet, matched.index);
  if(variables.find("Failed") == variables.end())
    return {matched, false};

  matched.index = vm.get_last_index();
  matched_protocol protocol;
  protocol.variables = variables;
  protocol.matched_protocol = this->name;
  protocol.layer = this->layer;
  matched.protocols.push_back(protocol);
  ++matched.layer_count;
  return {matched, true};
}
