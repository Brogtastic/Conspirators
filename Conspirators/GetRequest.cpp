#include <iostream>
#include <cpp_httplib/httplib.h>
#include <thread>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#define print(x) cout << x

using json = nlohmann::json;

using namespace std;

string secret_key_get = "agekvoslfhfgaye6382m4i201nui32h078hrauipbvluag78e4tg4w3liutbh2q89897wrgh4ui3gh2780gbrwauy";
string url_get = "http://127.0.0.1:8080"; //LOCAL SERVER
//string url_get = "http://52.15.115.37"; //PUBLIC SERVER

// Global variable to store SSE data
string sseData;
CURL* curl;

// Callback function for handling SSE data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
	print("\nWRITECALLBACK successfully\n");
	size_t total_size = size * nmemb;
	output->append(static_cast<char*>(contents), total_size);
	return total_size;
}

// Function to perform the SSE request in a separate thread
void PerformSSE() {
	print("\nPerform SSE called successfully\n");
	CURLcode res;

	curl = curl_easy_init();
	if (!curl) {
		cerr << "Error initializing libcurl." << endl;
		return;
	}
	else {
		print("\nCurl initialized!\n");
	}

	// Configure libcurl
	curl_easy_setopt(curl, CURLOPT_URL, (url_get + "/trigger_sse").c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &sseData);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 0L); // Set timeout to zero to keep the connection open

	// Perform the request
	res = curl_easy_perform(curl);

	// Check for errors
	if (res != CURLE_OK) {
		cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
	}
}

void CleanCurl() {
	curl_easy_cleanup(curl);
}


string GetRound(string roomCode) {
	httplib::Client client(url_get.c_str());
	auto res = client.Get((url_get + "/game-stage/" + roomCode).c_str());
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

	httplib::Client client(url_get.c_str());
	auto res = client.Get((url_get + "/" + secret_key_get + "/play/members-info/" + mainRoomCode + "?secret_key=" + secret_key_get).c_str());

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

//sseThread.join();