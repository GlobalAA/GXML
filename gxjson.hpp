#ifndef GXJSON
#define GXJSON

#include <cstring>
#include <fstream>
#include <QFile>
#include <typeinfo>
#include <sstream>
#include "pugixml.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class gxjson
{
public:
    gxjson();

    void xml_to_json(const pugi::xml_node& node, json& jsonObj);
    void json_to_xml(const json& j, pugi::xml_node& node);
};


#endif
