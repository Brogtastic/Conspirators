#include <iostream>
#include <cpp_httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <string>
using namespace std;
#define print(x) cout << x

using json = nlohmann::json;

string secret_key = "agekvoslfhfgaye6382m4i201nui32h078hrauipbvluag78e4tg4w3liutbh2q89897wrgh4ui3gh2780gbrwauy";

string GenerateRandomerString() {
	const string allowedCharacters = "123456789ABCDEFGHIJKLMNPQRSTUVWXYZ"; // Removed the O and the 0 cause too similar looking
	string randomString;
	for (int i = 0; i < 4; i++) {
		int randomChar = rand() % 32;
		randomString += allowedCharacters[randomChar];
	}

	return randomString;
}

bool doesURLExist(const string& url, string roomCode) {
	httplib::Client client(url.c_str());

	auto res = client.Head("/play/" + roomCode);

	return res && (res->status == 200);
}

string MyServer(string mainRoomCode) {

	// Define the URL of your Flask web page
	string url = "http://127.0.0.1:8080"; // Replace with the correct URL

	if (!doesURLExist(url, mainRoomCode)) {
		return("URL Does not exist.");
	}

	string roomCode;
	string members;

	// Make a GET request to the Flask web page
	httplib::Client client(url.c_str());
	auto res = client.Get((url + "/play/" + mainRoomCode + "?secret_key=" + secret_key).c_str());

	// Check if the request was successful
	if (res && res->status == 200) {
		// Parse the JSON response
		json response = json::parse(res->body);

		// Access the JSON data
		// Extract the "members" array
		json members = response["members"];
		print(members);

		string all_names;
		for (const auto& member : members) {
			string name = member["name"];
			all_names += name + ", ";
		}

		// Remove the trailing comma and space
		if (!all_names.empty()) {
			all_names = all_names.substr(0, all_names.size() - 2);
		}

		roomCode = to_string(response["roomCode"]);
		roomCode.erase(std::remove(roomCode.begin(), roomCode.end(), '"'), roomCode.end());

		if (roomCode == mainRoomCode) {
			return "MEMBERS: " + all_names;
		}
		else {
			return "Wrong Room Code" + roomCode;
		}
	}
	else {
		cerr << "\n Play Request Failed.\n" << endl;
		return "Request Failed";
	}
}

void FrankServer(int num) {

	// Define the URL of your Flask web page
	string url = "http://127.0.0.1:8080"; // Replace with the correct URL
	string number = to_string(num); // Set the desired name

	// Make a GET request to the Flask web page with the name as a query parameter
	httplib::Client client(url.c_str());
	auto res = client.Get(("/profile?number=" + number).c_str());
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
				print("\nRoom Code added to server\n");
				return generatedCode;
			}
			else {
				print("\nRoom code not added to server\n");
				generatedCode = GenerateRandomerString();
			}

		}
		else {
			return "Check Code request failed";
		}
	}
	return "Something went wrong";

}

void DeleteCodeOffServer(string deleteCode) {
	string url = "http://127.0.0.1:8080";

	httplib::Client client(url.c_str());
	auto res = client.Get((url + "/deleteroom?roomcode=" + deleteCode).c_str());

	if (res && res->status == 200) {
		// Parse the JSON response
		json response = json::parse(res->body);

		// Access the JSON data
		string status = response["status"];

		if (status == "Code Deleted and Room Removed") {
			print("\nCode Deleted and Room Removed");
		}
		else {
			print("\nCode Not Present");
		}

	}
	else {
		print("request failed");
	}
}

