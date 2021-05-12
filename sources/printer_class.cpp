#include "printer_class.hpp"

#include <algorithm>

Printer::Printer()
{

}

int Printer::update(const matched_packet &packet)
{
    if(packet.layer_count == 5) {
        std::cout << "HTTP packet detected!" << std::endl;
        for(auto [key, value] : packet.protocols[3].variables) {
            std::string str_value;
            std::transform(value.begin(), value.end(), std::back_inserter(str_value), [](unsigned c) { return static_cast<unsigned char> (c); });
            std::cout << key << ": " << str_value << std::endl;
        }

        auto convert_to_ip_format = [](std::vector<unsigned char> ip){
            std::string str;
            for(auto c : ip)
                str += (std::to_string(c) + ".");
            return str;
        };

        std::cout << "source ip:" << convert_to_ip_format(packet.protocols[1].variables.at("source")) << std::endl;
        std::cout << "source ip:" << convert_to_ip_format(packet.protocols[1].variables.at("dest")) << std::endl;
        std::cout << std::endl;
        return 1;
    }
    return 0;
}
