#ifndef CONFIG_H
#define CONFIG_H

#include<string>

typedef struct Config {
	std::string nodes_path;
	std::string edges_path;
	std::string ip_address;
	std::string port;
} Config;

Config parse_config();

#endif