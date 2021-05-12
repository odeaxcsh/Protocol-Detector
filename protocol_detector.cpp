#include "pattern_parser.hpp"
#include "virtual_machine.hpp"
#include "protocol.hpp"
#include "elicitation_utils.hpp"
#include "file_manager.hpp"
#include "printer_class.hpp"
#include <iostream>
#include <json/json.h>
#include "tins/tins.h"
using namespace Tins;

class Matcher
{
public:
  Matcher(const std::string &path, int count_limit)
  {
    File_manager files(path);
    protocols =  files.get_protocols();
    layers_count = protocols.size();
    this->count_limit = count_limit;
    count = 0;
  }

  void run()
  {
    Sniffer sniffer("wlp3s0");
    sniffer.sniff_loop(make_sniffer_handler(this, &Matcher::handle));
  }

  bool handle(PDU& pdu)
  {
    EthernetII packet = pdu.rfind_pdu<EthernetII>();
    std::vector<unsigned char> vec = static_cast<std::vector<unsigned char>>(packet.serialize());
    matched_packet p;

    for(int layer = 0; layer < layers_count; ++layer) {
      for(auto protocol : protocols[layer]) {
        if(auto [variables, matched] = protocol.match(vec, vec.size(), p); matched == true)
          break;
      }
    }

    count += printer.update(p);
    return count < count_limit;
  }

private:
  std::vector<std::vector<Protocol>> protocols;
  Printer printer;
  int layers_count;
  int count_limit;
  int count;
};

int main(int argc, char *argv[])
{
  if(argc < 3) {
    throw "Invalid input\n usage: <packet_count: int> <main configuration file path: str>";
  }

  std::string path = argv[2];
  int count_limit = std::stoi(argv[1]);

  Matcher matcher(path, count_limit);
  matcher.run();
}
