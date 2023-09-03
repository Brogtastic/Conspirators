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

string RequestRoomCode() {
	httplib::Client client(url.c_str());
	auto res = client.Get((url + "/request-new-room-code?secret_key=" + secret_key).c_str());
	if (res && res->status == 200) {
		json response = json::parse(res->body);
		string round = response["gameStage"];
		return round;
	}
	else {
		return "server-full";
	}
}

bool DoesURLExist(string extension) {
	httplib::Client client(url.c_str());

	auto res = client.Head("/" + extension);

	return res && (res->status == 200);
}

void SetRound(string roomCode, string roundSet) {
	httplib::Client client(url.c_str());
	auto res = client.Get((url + "/set-round?roundSet=" + roundSet + "&roomCode=" + roomCode + "&key=" + secret_key).c_str());
}

string GetRound(string roomCode) {
	httplib::Client client(url.c_str());
	auto res = client.Get((url + "/game-stage/" + roomCode).c_str());
	if (res && res->status == 200) {
		json response = json::parse(res->body);
		string round = response["gameStage"];
		return round;
	}
	else {
		return "disconnected";
	}
}

vector<string> RefreshMembers(string mainRoomCode) {

	vector<string> membersReturn = { "", "", "", "", "" };

	vector<string> membersList;

	string roomCode;
	string members;
	string gameStage;
	string roomQuestion;

	httplib::Client client(url.c_str());
	auto res = client.Get((url + "/" + secret_key + "/play/members-info/" + mainRoomCode + "?secret_key=" + secret_key).c_str());

	if (res && res->status == 200) {
		json response = json::parse(res->body);

		json members = response["members"];
		roomCode = to_string(response["roomCode"]);
		gameStage = response["gameStage"];
		roomQuestion = response["roomQuestion"];
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
		membersReturn[4] = roomQuestion;

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

string CreateRoom() {

	httplib::Client client(url.c_str());
	auto res = client.Get((url + "/newroom?secret_key=" + secret_key).c_str());

	string access;
	string newRoomCode;

	if (res && res->status == 200) {
		json response = json::parse(res->body);

		access = response["access"];
		newRoomCode = response["newRoomCode"];


		if ((access == "granted") && (newRoomCode.length() == 4)) {
			print("\nRoom Code added to server\n");
			return newRoomCode;
		}
		else {
			print("\nRoom code not added to server\n");
			return "Access Denied";
		}

	}
	else {
		return "Check Code request failed";
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

