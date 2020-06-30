#include <iostream>
#include <fstream>
#include <sstream>

#include "ducksoup/config.h"

/* represents the key found on each line */

enum Key {
	Invalid,
	NodesPath,
	EdgesPath,
	IpAddress,
	Port,
};

/*
 * resolves a string key found as the enum Key
 *
 * @param key - key as string found in the config file
 * @return Key - resolved key as an enum
 */

Key resolve_key(std::string key) {
	if (key == "nodes")
		return NodesPath;
	else if (key == "edges")
		return EdgesPath;
	else if (key == "ipaddr")
		return IpAddress;
	else if (key == "port")
		return Port;
	return Invalid;
}

/*
 * removes unnecesary whitespace on each line to enable splitting the key 
 * and value on single spaces
 *
 * @param input - a line of the config file
 * @return out - the same line with redundant whitespace removed
 */

std::string remove_extra_whitespace(const std::string input) {
	std::string out;
	unique_copy (input.begin(), input.end(), std::back_insert_iterator<std::string>(out),
		[](char a,char b){ return isspace(a) && isspace(b);});
	return out;
}

namespace ducksoup {

	Config parse_config() {
		Config config;

		std::ifstream configfile ("duck.cnf");

		std::string line, key, value;

		if (configfile.is_open()) {

			while (std::getline(configfile, line)) {
				line = remove_extra_whitespace(line);

				if (line[0] == '#' || line[1] == '#')
					continue;

				std::stringstream ss (line);

				if (line == "")
					continue;

				std::getline(ss, key, ' ');
				std::getline(ss, value, ' ');

				switch (resolve_key(key)) {
					case Invalid:
						std::cout << "Error: Received invalid key: " << key << std::endl;
						exit(1);

					case NodesPath:
						config.nodes_path = value;
						break;

					case EdgesPath:
						config.edges_path = value;
						break;

					case IpAddress:
						config.ip_address = value;
						break;

					case Port:
						config.port = std::stoi(value);
						break;
				}

			}

		} else {
			std::cout << "Error: Could not open config file: duck.cnf" << std::endl;
			exit(1);
		}

		// panic if there a field in the config file does not exist

		return config;
	}

}