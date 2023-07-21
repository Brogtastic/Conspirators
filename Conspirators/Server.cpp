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
	std::string test = "No Test";
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
		test = response["test"];

		// Print the JSON data
		std::cout << "Name: " << name << std::endl;
		std::cout << "Coolness: " << coolness << std::endl;
		std::cout << "Test: " << test << std::endl;
	}
	else {
		std::cerr << "\nRequest failed.\n" << std::endl;
	}

	return test;
}

void FrankServer(int num) {
	std::cout << "MyServer function successfully called";

	// Define the URL of your Flask web page
	std::string url = "http://127.0.0.1:8080"; // Replace with the correct URL
	std::string name = "Billy Bob" + std::to_string(num); // Set the desired name

	// Make a GET request to the Flask web page with the name as a query parameter
	httplib::Client client(url.c_str());
	auto res = client.Get(("/profile?name=" + name).c_str());
}

std::string CheckCode() {
	// Define the URL of your Flask web page
	std::string url = "http://127.0.0.1:8080";

	// Make a GET request to the Flask web page with the roomCode as a query parameter
	httplib::Client client(url.c_str());
	auto res = client.Get((url + "/newroom?roomcode=1234").c_str());

	std::string code;

	if (res && res->status == 200) {
		// Parse the JSON response
		json response = json::parse(res->body);

		// Access the JSON data
		code = response["code"];

	}
	else {
		std::cerr << "\nRequest failed.\n" << std::endl;
		code = "Failed :(";
	}

	return code;

}

std::string MainToServer(std::string function, int num) {


	std::string fun = "failed";
	if (function == "server") {
		//fun = MyServer();
		//FrankServer(num);
		fun = CheckCode();
	}
	
	std::cout << "Main to server function successfully called";

	return fun;

}

