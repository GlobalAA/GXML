#include "gxjson.hpp"
#include <iostream>

gxjson::gxjson() {}

void gxjson::xml_to_json(const pugi::xml_node& node, json& jsonObj) {
	const char* nodeValue = node.child_value();
	bool hasText = nodeValue && std::strlen(nodeValue) > 0;

	if (hasText) {
		jsonObj["#text"] = nodeValue;
	}

	for (const pugi::xml_attribute& attr : node.attributes()) {
		jsonObj["@" + std::string(attr.name())] = attr.value();
	}

	for (const pugi::xml_node& child : node.children()) {
		if (child.type() == pugi::node_element) {
			jsonObj[child.name()] = json::object();
			this->xml_to_json(child, jsonObj[child.name()]);
		}
	}
}

void gxjson::json_to_xml(const json& j, pugi::xml_node& node) {
	if (j.is_object()) {
		for (json::const_iterator it = j.begin(); it != j.end(); ++it) {
            try {
                const std::string& key = it.key();
                if (key == "#text") {
                    node.append_child(pugi::node_pcdata).set_value(it.value().get<std::string>().c_str());
                } else if (key[0] == '@') {
                    std::string attributeName = key.substr(1);
                    node.append_attribute(attributeName.c_str()) = it.value().get<std::string>().c_str();
                } else {
                    pugi::xml_node child = node.append_child(key.c_str());
                    this->json_to_xml(it.value(), child);
                }
            } catch (std::exception& e) {
                std::cerr << "Exception: " << e.what() << std::endl;
            }
		}
	} else if (j.is_array()) {
		for (const auto& el : j) {
			pugi::xml_node child = node.append_child("item");
			this->json_to_xml(el, child);
		}
	} else {
		node.append_child(pugi::node_pcdata).set_value(j.dump().c_str());
	}
}
