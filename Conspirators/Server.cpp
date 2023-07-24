#include <iostream>
#include <cpp_httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <string>
using namespace std;
#define print(x) cout << x

using json = nlohmann::json;

string GenerateRandomerString() {
	const string allowedCharacters = "123456789ABCDEFGHIJKLMNPQRSTUVWXYZ"; // Removed the O and the 0 cause too similar looking
	string randomString;
	for (int i = 0; i < 4; i++) {
		int randomChar = rand() % 32;
		randomString += allowedCharacters[randomChar];
	}

	return randomString;
}

string MyServer() {

	cout << "MyServer function successfully called";

	// Define the URL of your Flask web page
	string url = "http://127.0.0.1:8080"; // Replace with the correct URL

	string name = "No Name";
	string test = "No Test";
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
		cout << "Name: " << name << endl;
		cout << "Coolness: " << coolness << endl;
		cout << "Test: " << test << endl;
	}
	else {
		cerr << "\nRequest failed.\n" << endl;
	}

	return test;
}

void FrankServer(int num) {
	cout << "MyServer function successfully called";

	// Define the URL of your Flask web page
	string url = "http://127.0.0.1:8080"; // Replace with the correct URL
	string name = "Billy Bob" + to_string(num); // Set the desired name

	// Make a GET request to the Flask web page with the name as a query parameter
	httplib::Client client(url.c_str());
	auto res = client.Get(("/profile?name=" + name).c_str());
}

string CheckCode(string generatedCode) {
	// Define the URL of your Flask web page
	string url = "http://127.0.0.1:8080";

	for (int i = 0; i < 1000; i++) {
		// Make a GET request to the Flask web page with the roomCode as a query parameter
		httplib::Client client(url.c_str());
		auto res = client.Get((url + "/newroom?roomcode=" + generatedCode).c_str());

		string access;

		if (res && res->status == 200) {
			// Parse the JSON response
			json response = json::parse(res->body);

			// Access the JSON data
			access = response["access"];

			if (access == "granted") {
				print("ACCESS GRANTED");
				return generatedCode;
			}
			else {
				print("ACCESS DENIED");
				generatedCode = GenerateRandomerString();
			}

		}
		else {
			return "request failed";
		}
	}
	return "Something went wrong";

}

string MainToServer(string function, string code, int num) {

	string fun = "failed";
	if (function == "json") {
		fun = MyServer(); // Gets the Json from the /json address
	}
	else if (function == "code") {
		//fun = CheckCode(code); // Checks to see if the generated code is in the website or not. 'Granted' or 'Denied'
	}
	else if (function == "number") {
		FrankServer(num); // Sends variable name to website
	}

	return fun;

}

