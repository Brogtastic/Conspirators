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

void TerminateRoomThread() {
	httplib::Client client(url.c_str());
	auto res = client.Get((url + "/terminate-room-thread/" + roomCode + "?secret_key=" + secret_key).c_str());

	if (res && res->status == 200) {
		json response = json::parse(res->body);

		string access = response["access"];

		if (access == "granted") {
			print("\nRoom thread terminated.");
		}
		else {
			print("\nRoomThread access not granted.");
		}
	}
	else {
		print("\nRoomThread function did not connect.");
	}
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
		print("\nrequest failed, room code not deleted");
	}
}

