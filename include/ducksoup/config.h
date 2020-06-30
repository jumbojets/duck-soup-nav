#ifndef CONFIG_H
#define CONFIG_H

#include<string>

/*
 * The struct Config represents a particular configuration based on the
 * contents of the file, duck.cnf. It stores important information such
 * as paths to the graph network data and networking information
 */

typedef struct Config {
	std::string nodes_path;
	std::string edges_path;
	std::string ip_address;
	int port;
} Config;

/*
 * This reads the config information from duck.cnf file, packages it into
 * the Config struct and returns it. It ensures that it is a proper config
 * file as well
 *
 * @return config - config struct with relevant information
 */

Config parse_config();

#endif