#include "protocol.hpp"
#include "lexer.hpp"
#include "pattern_parser.hpp"

#include <json/json.h>
#include <vector>

int Protocol::id_generator_core = 0;

Protocol::Protocol(const Json::Value &value) : requirements(3), ports()
{
  this->id = ++id_generator_core;
  this->name = value["name"].asString();
  this->layer = value["layer"].asInt();
  const std::string &&pattern = value["pattern"].asString();
  Lexer lexer = (pattern.c_str());
  this->pattern = Pattern_parser(lexer).parse();

  for(auto object : value["port"])
    this->ports.push_back(object.asInt());

  for(auto object : value["requirements"])
    this->requirements.push_back(object.asInt());
}
