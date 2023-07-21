#include <iostream>
#include <cpp_httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

std::string MyServer() {

	std::cout << "MyServer function successfully called";

	// Define the URL of your Flask web page
	std::string url = "http://127.0.0.1:8080"; // Replace with the correct URL

	std::string name = "No Name";
	int coolness = 0;

	// Make a GET request to the Flask web page
	httplib::Client client(url.c_str());
	auto res = client.Get("/json");

	// Check if the request was successful
	if (res && res->status == 200) {
		// Parse the JSON response
		json response = json::parse(res->body);

		// Access the JSON data
		name = response["name"];
		int coolness = response["coolness"];

		// Print the JSON data
		std::cout << "Name: " << name << std::endl;
		std::cout << "Coolness: " << coolness << std::endl;
	}
	else {
		std::cerr << "\nRequest failed.\n" << std::endl;
	}

	return name;
}

std::string MainToServer(std::string function) {


	std::string fun = "failed";
	if (function == "server") {
		fun = MyServer();
	}
	
	std::cout << "Main to server function successfully called";

	return fun;

}
