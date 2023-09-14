#include <iostream>
#include "MyGlobals.h"
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

bool DoesURLExist() {
	httplib::Client client(url.c_str());

	auto res = client.Head("/");

	return res && (res->status == 200);
}

void SetRound(string roundSet) {
	httplib::Client client(url.c_str());
	auto res = client.Get((url + "/set-round?roundSet=" + roundSet + "&roomCode=" + roomCode + "&key=" + secret_key).c_str());
}

void CreateRoom() {

	httplib::Client client(url.c_str());
	auto res = client.Get((url + "/createroom?secret_key=" + secret_key).c_str());

	string access;
	string newRoomCode;

	if (res && res->status == 200) {
		json response = json::parse(res->body);

		access = response["access"];
		newRoomCode = response["newRoomCode"];

		if ((access == "granted") && (newRoomCode.length() == 4)) {
			roomCode = newRoomCode;
			roomQuestion = response["roomQuestion"];
			print("\nRoom created!");
		}
		else {
			print("\nAccess not granted. No room created.");
		}
	}
	else {
		print("\nCreateRoom function did not connect.");
	}

}

void DeleteCodeOffServer(string deleteCode) {

	numMembers = 0;
	membersList = {};
	membersNames = "No members in room";
	firstMember = "Need at least 3 players to start the game...";

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
		print("\nrequest failed, room code not deleted");
	}
}


void UpdateMembers() {
	httplib::Client client(url.c_str());
	auto res = client.Get((url + "/members-names-return/" + roomCode + "?secret_key=" + secret_key).c_str());

	if (res && res->status == 200) {
		json response = json::parse(res->body);

		string access = response["access"];

		if (access == "denied") {
			print("UpdateMembers access denied");
		}
		else {
			json allNames = response["allNames"];
			string names = "";
			if (allNames.is_array()) {
				// Create a vector of strings to store the names
				std::vector<string> namesVector;

				string allNamesStr = "";
				for (const auto& name : allNames) {
					string nameStr = name;
					namesVector.push_back(nameStr);
					allNamesStr += nameStr + ", ";
				}

				membersNames = "MEMBERS: " + allNamesStr.substr(0, allNamesStr.size() - 2);

				numMembers = namesVector.size();

				if (namesVector.size() == 0) {
					firstMember = "Need at least 3 players to start the game...";
				}
				else if (namesVector.size() == 1) {
					firstMember = "Need two more players to start the game";
				}
				else if (namesVector.size() == 2) {
					firstMember = "Need one more player to start the game";
				}
				else if (namesVector.size() > 2) {
					firstMember = "Waiting for " + namesVector[0] + " to start the game...";
				}

			}
			else {
				print("\nUpdateMembers: allNames is not an array");
			}
		}

	}
	else {
		print("\nUpdate Members request failed");
	}
}


void UpdateGameStage() {
	print("\nUpdateGamestage called");
	httplib::Client client(url.c_str());
	auto res = client.Get((url + "/game-stage-return/" + roomCode + "?secret_key=" + secret_key).c_str());

	if (res && res->status == 200) {
		json response = json::parse(res->body);

		string access = response["access"];

		if (access == "denied") {
			print("UpdateGameStage access denied");
		}
		else {
			gameStage = response["gameStage"];
			print("\nNew gameStage: " + gameStage);
		}

	}
	else {
		print("\nUpdate Members request failed");
	}
}
