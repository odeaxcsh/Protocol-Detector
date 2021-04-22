#if !defined(PROTOCOL_HPP)
#define PROTOCOL_HPP

#include <string>
#include <json/json.h>

#include "bytecode.hpp"

class Protocol
{
public:
  Protocol(const Json::Value& );

private:
  std::string name;

  Pattern pattern;

  int id;
  int layer;
  std::vector<int> ports;
  std::vector<int> requirements;

  static int id_generator_core;
};

#endif
