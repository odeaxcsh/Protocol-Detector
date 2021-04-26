#include "pattern_parser.hpp"
#include "virtual_machine.hpp"
#include "protocol.hpp"

#include <iostream>
#include <json/json.h>
#include <tins/tins.h>
using namespace Tins;

std::ostream& operator<<(std::ostream& out, std::vector<unsigned char> str)
{
    for(auto x : str) {
        out << (int)x;
    }
    return out;
}

bool callback(const PDU &pdu) {

    const IP &x = pdu.rfind_pdu<IP>(); // non-const works as well
    std::cout << "Destination address: " << x.dst_addr() << std::endl;
    const TCP &y = pdu.rfind_pdu<TCP>();
    std::cout << "POrt" << y.sport() << " " << y.dport() << std::endl;

  EthernetII packet = pdu.rfind_pdu<EthernetII>();

  PDU::serialization_type buffer = packet.serialize();
  Protocol ethernet = Protocol::from_file("/home/sophie/Desktop/network_intership_phistaz-rayanesh/p3-advanced/configurations/Ethernet.conf");
  Protocol ip = Protocol::from_file("/home/sophie/Desktop/network_intership_phistaz-rayanesh/p3-advanced/configurations/IP.conf");
  Protocol tcp = Protocol::from_file("/home/sophie/Desktop/network_intership_phistaz-rayanesh/p3-advanced/configurations/TCP.conf");
  Protocol http = Protocol::from_file("/home/sophie/Desktop/network_intership_phistaz-rayanesh/p3-advanced/configurations/HTTP.conf");

  auto vec = static_cast<std::vector<unsigned char>>(buffer);
  matched_packet p;
  auto [a, b] = ethernet.match(vec, vec.size(), p);
  auto [ia, ib] = ip.match(vec, vec.size(), a);
  auto [ta, tb] = tcp.match(vec, vec.size(), ia);
  auto [ha, hb] = http.match(vec, vec.size(), ta);

  std::cout << buffer << std::endl;
  std::cout << b << ib << tb << hb << std::endl;

  for(auto [key, value] : ia.protocols[0].variables)
    std::cout << key << ": " << value << std::endl;

  for(auto [key, value] : ia.protocols[1].variables)
      std::cout << key << ": " << value << std::endl;

  for(auto [key, value] : ia.protocols[2].variables)
      std::cout << key << ": " << value << std::endl;

  for(auto [key, value] : ia.protocols[3].variables)
      std::cout << key << ": " << value << std::endl;

  return false;
}

int main(int argc, char *argv[])
{
  auto x = Sniffer("lo");
  x.set_filter("tcp");
  x.sniff_loop(callback);
}
