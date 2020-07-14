#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

#include "ducksoup/config.h"

/* represents the key found on each line */

enum Key {
	Invalid,
	NodesPath,
	EdgesPath,
	Hostname,
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
	else if (key == "hostname")
		return Hostname;
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

/*
 * uses the config's fields found encoding to determine if a field is missing
 *
 * @param fields_found_enc - the fields found encoding
 * @return null_field - the first missing field (nullptr if none are missing)
 */

const char* validate_fields(short fields_found_enc) {
	if (fields_found_enc % 2 == 0) {
		return "nodes";
	} else if (fields_found_enc / 2 % 2 == 0) {
		return "edges";
	} else if (fields_found_enc / 4 % 2 == 0) {
		return "hostname";
	} else if (fields_found_enc / 8 % 2 == 0) {
		return "port";
	} else {
		return nullptr;
	}
}

namespace ducksoup {

	Config parse_config() {
		Config config;

		std::ifstream configfile ("duck.cnf");

		std::string line, key, value;

		short fields_found_enc = 0;

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

				short resolved_key = resolve_key(key);

				switch (resolved_key) {
					case Invalid:
						std::cout << "Error: Received invalid key: " << key << std::endl;
						exit(1);

					case NodesPath:
						config.nodes_path = value;
						break;

					case EdgesPath:
						config.edges_path = value;
						break;

					case Hostname:
						config.hostname = value;
						break;

					case Port:
						config.port = std::stoi(value);
						break;
				}

				fields_found_enc += pow(2, resolved_key - 1);

			}

		} else {
			std::cout << "Error: Could not open config file: duck.cnf" << std::endl;
			exit(1);
		}

		// ensure that we have all of the neccesary fields
		const char *null_field = validate_fields(fields_found_enc);

		if (null_field != nullptr) {
			std::cout << "Error: Could not find in the config file the key: " << null_field << std::endl;
			exit(1);
		}

		return config;
	}

}