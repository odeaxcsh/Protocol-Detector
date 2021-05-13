
#include <string>
#include <vector>
#include <fstream>
#include <json/json.h>

#include "file_manager.hpp"

Json::Value open_json_file(const std::string &path)
{
    std::ifstream file(path, std::ifstream::binary);

    Json::Value json;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( file, json, false );

    return json;
}

File_manager::File_manager(const std::string &config_file)
{
    include_config_file(config_file);
}

void File_manager::include_config_file(const std::string &path)
{
    const size_t last_slash_idx = path.rfind('/');
    auto base_dir =  (std::string::npos == last_slash_idx) ? "" : path.substr(0, last_slash_idx + 1);

    auto json = open_json_file(path);
    for(auto object : json) {
        auto type = object["type"].asString();
        auto path = object["path"].asString();

        if(type == "protocol") {
            protocols.push_back(base_dir + path);
        } else if(type == "include") {
            include_config_file(base_dir + path);
        }
    }
}

std::vector<std::vector<Protocol>> File_manager::get_protocols()
{
    std::vector<std::vector<Protocol>> protocols_object;
    for(auto path : protocols) {
        auto json = open_json_file(path);
        if(json.isArray()) {
            for(auto protocol : json) {
                Protocol protocol_object{protocol};
                int layer = protocol_object.get_layer();
                if(protocols_object.size() < layer)
                    protocols_object.resize(layer);

                protocols_object[layer-1].push_back(protocol_object);
            }
        } else {
            Protocol protocol_object{json};
            int layer = protocol_object.get_layer();
            if(protocols_object.size() < layer)
                protocols_object.resize(layer);

            protocols_object[layer-1].push_back(protocol_object);
        }
    }
    return protocols_object;
}