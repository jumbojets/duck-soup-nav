#include <iostream>

#include "ducksoup/config.h"
#include "ducksoup/roadnet.h"
#include "cpp-httplib/httplib.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main() {

	ducksoup::Config config = ducksoup::parse_config();

	ducksoup::RoadNet net = ducksoup::RoadNet(config.nodes_path, config.edges_path);

	std::cout << "setting up server..." << std::endl;

	httplib::Server svr;

	svr.Post("/route", [&](const httplib::Request &req, httplib::Response &res) {
		auto body = json::parse(req.body);

		auto path = net.route(body["start"][0], body["start"][1], body["destination"][0], body["destination"][1]);

		json ret;

		for (const auto & n : path) {
			json point;
			point.push_back(n->longitude);
			point.push_back(n->latitude);
			ret.push_back(point);
		}

		res.set_content(ret.dump(), "application/json");
	});

	std::cout << "listening on " << config.ip_address << ":" << config.port << "..." << std::endl;

	svr.listen(config.ip_address.c_str(), config.port);

	return 0;
}
