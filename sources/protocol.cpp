#include "protocol.hpp"
#include "lexer.hpp"
#include "pattern_parser.hpp"
#include "virtual_machine.hpp"
#include "elicitation_utils.hpp"

#include <regex>
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

Protocol::Protocol(const Json::Value &value) : requirements(3), conditions(4)
{
  this->id = ++id_generator_core;
  this->name = value["name"].asString();
  this->layer = value["layer"].asInt();
  std::string &&pattern = value["pattern"].asString();
  std::string &&ignores = value["pattern-ignores"].asString();
  std::map<std::string, std::string> escapes = {{R"(\\n)", "\n"}, {R"(\\r)", "\r"}, {R"(\\t)", "\t"}};

  for(auto [c, e] : escapes)
      ignores = std::regex_replace(ignores, std::regex(c), e);

  pattern.erase(std::remove_if(pattern.begin(), pattern.end(),
		[&ignores](char c) {
			return ignores.find(c) != std::string::npos;
		}),
		pattern.end()
	);

  for(auto [c, e] : escapes)
    pattern = std::regex_replace(pattern, std::regex(c), e);

  Lexer lexer = (pattern.c_str());
  this->pattern = Pattern_parser(lexer).parse();

  this->size = Formula_parser().parse<int>(value["size"].asString());

  for(auto object : value["requirements"])
    for(auto value : object["protocols"])
      this->requirements[object["layer"].asInt() - 1]
        .push_back(value.asString());

  for(auto object : value["conditions"])
    for(auto value : object["conditions"])
      this->conditions[object["layer"].asInt() - 1]
        .push_back(
          Formula_parser().parse<int>(value.asString())
        );

  this->protocol_mapper[name] = this;
}

std::tuple<matched_packet &, bool> Protocol::match(std::vector<unsigned char> &packet, int len, matched_packet &matched)
{
  if(this->layer != matched.layer_count)
    return {matched, false};

  for(int i = 0; i < this->layer-1; ++i) {
    if(std::find_if(requirements[i].begin(), requirements[i].end(),
      [&matched, &i](auto x){
        return x == matched.protocols[i].matched_protocol or x == "ANY";
      }
    ) ==  requirements[i].end())
      return {matched, false};
  }

  for(int i = 0; i < matched.layer_count - 1; ++i) {
    for(auto condition : conditions[i]) {
      if(not condition->eval(matched.protocols[i].variables))
        return {matched, false};
    }
  }

  VM vm(this->pattern);
  auto variables = vm.run(packet, len, matched.index);
  if(variables.find("Failed") != variables.end())
    return {matched, false};
  
  for(auto condition : conditions[matched.layer_count - 1]) {
    if(not condition->eval(matched.protocols[matched.layer_count - 1].variables))
      return {matched, false};
  }

  if(this->size != nullptr)
    matched.index.byte += size->eval(variables);
  else
    matched.index = vm.get_last_index();

  matched_protocol protocol;
  protocol.variables = variables;
  protocol.matched_protocol = this->name;
  protocol.layer = this->layer;
  matched.protocols.push_back(protocol);
  ++matched.layer_count;
  return {matched, true};
}
