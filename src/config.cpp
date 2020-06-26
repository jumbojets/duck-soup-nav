#include <iostream>
#include <fstream>
#include <sstream>

#include "config.h"

enum Keys {
	Invalid,
	NodesPath,
	EdgesPath,
	IpAddress,
	Port,
};

Keys resolveKey(std::string input) {
	if (input == "nodes")
		return NodesPath;
	else if (input == "edges")
		return EdgesPath;
	else if (input == "ipaddr")
		return IpAddress;
	else if (input == "port")
		return Port;
	return Invalid;
}

std::string remove_extra_whitespace(const std::string input) {
	std::string out;
	unique_copy (input.begin(), input.end(), std::back_insert_iterator<std::string>(out),
		[](char a,char b){ return isspace(a) && isspace(b);});
	return out;
}

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

			switch (resolveKey(key)) {
				case Invalid:
					std::cout << "Error: Received invalid key: " << key << std::endl;
					exit(1);

				case NodesPath:
					config.nodes_path = value;

				case EdgesPath:
					config.edges_path = value;

				case IpAddress:
					config.ip_address = value;

				case Port:
					config.port = value;
			}

		}

	} else {
		std::cout << "Error: Could not open config file: duck.cnf" << std::endl;
		exit(1);
	}

	// panic if there a field in the config file does not exist

	return config;
}