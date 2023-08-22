#include <iostream>
#include <cpp_httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
using namespace std;
#define print(x) cout << x

using json = nlohmann::json;

string secret_key = "agekvoslfhfgaye6382m4i201nui32h078hrauipbvluag78e4tg4w3liutbh2q89897wrgh4ui3gh2780gbrwauy";
string url = "http://127.0.0.1:8080"; //LOCAL SERVER
//string url = "http://52.15.115.37"; //PUBLIC SERVER

string GenerateRandomerString() {
	const string allowedCharacters = "123456789ABCDEFGHIJKLMNPQRSTUVWXYZ"; // Removed the O and the 0 cause too similar looking
	string randomString;
	for (int i = 0; i < 4; i++) {
		int randomChar = rand() % 32;
		randomString += allowedCharacters[randomChar];
	}

	return randomString;
}

bool DoesURLExist(string extension) {
	httplib::Client client(url.c_str());

	auto res = client.Head("/" + extension);

	return res && (res->status == 200);
}

vector<string> RefreshMembers(string mainRoomCode) {

	vector<string> membersReturn = { "", "", "", "" };

	vector<string> membersList;

	string roomCode;
	string members;
	string gameStage;

	httplib::Client client(url.c_str());
	auto res = client.Get((url + "/" + secret_key + "/play/members-info/" + mainRoomCode + "?secret_key=" + secret_key).c_str());

	if (res && res->status == 200) {
		json response = json::parse(res->body);

		json members = response["members"];
		roomCode = to_string(response["roomCode"]);
		gameStage = response["gameStage"];
		//print(members);

		string all_names;
		for (const auto& member : members) {
			string name = member["name"];
			all_names += name + ", ";
			membersList.push_back(name);
		}

		// Remove the trailing comma and space
		if (!all_names.empty()) {
			all_names = all_names.substr(0, all_names.size() - 2);
		}

		roomCode.erase(std::remove(roomCode.begin(), roomCode.end(), '"'), roomCode.end());

		membersReturn[0] = "MEMBERS: " + all_names;
		membersReturn[1] = to_string(membersList.size());
		
		if (membersList.size() > 2) {
			membersReturn[2] = "Waiting for " + membersList[0] + " to start the game...";
		}
		else if (membersList.size() == 1) {
			membersReturn[2] = "Need at least 2 more players to begin";
		}
		else if (membersList.size() == 2) {
			membersReturn[2] = "Need at least 1 more players to begin";
		}
		else {
			membersReturn[2] = "No players in room...";
		}

		membersReturn[3] = gameStage;

	}
	else {
		cerr << "\n Play Request Failed.\n" << endl;
	}

	return membersReturn;

}

void TransferClicks(int num) {

	string number = to_string(num); 

	httplib::Client client(url.c_str());
	auto res = client.Get(("/profile?number=" + number).c_str());
}

string CheckCode(string generatedCode) {

	for (int i = 0; i < 1000; i++) {
		httplib::Client client(url.c_str());
		auto res = client.Get((url + "/newroom?roomcode=" + generatedCode).c_str());

		string access;

		if (res && res->status == 200) {
			json response = json::parse(res->body);

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

	httplib::Client client(url.c_str());
	auto res = client.Get((url + "/deleteroom?roomcode=" + deleteCode).c_str());

	if (res && res->status == 200) {
		json response = json::parse(res->body);

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

