#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;
#ifndef MYGLOBALS_H
#define MYGLOBALS_H

extern string roomCode;
extern string roomQuestion;
extern string gameStage;
extern int numMembers;
extern int wordFrame;
extern vector<string> membersList;
extern string membersNames;
extern string firstMember;
extern bool threadActive;
extern vector<string> wordsToPresent;

#endif


/*
@views.route('/sse/<enteredRoomCode>')
def sse(enteredRoomCode) :
	room = Room.query.filter_by(code = enteredRoomCode).first()
	# Get list of members to return
	members_list = []
	for member in room.members:
member_dict = {
	'id': member.id,
	'name' : member.name,
	'points' : member.points
}
members_list.append(member_dict)
def event_stream(room) :
	global data_to_send
	while room :
		if data_to_send :
			data_to_send = False
			data = {
				'gameStage': room.gameStage,
				'members' : members_list,
		}
		yield json.dumps(data) + "\n\n"

			return Response(event_stream(room), content_type = 'text/event-stream')


size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
	string received_data(static_cast<char*>(contents), size * nmemb);
	cout << "\n\nReceived data: " << received_data << endl;

	try {
		json response = json::parse(received_data);

		json members = response["members"];

		membersList = {};
		string all_names;
		if (members.size() > 0) {
			for (const auto& member : members) {
				string name = member["name"];
				all_names += name + ", ";
				membersList.push_back(name);
			}
			// Remove the trailing comma and space
			if (!all_names.empty()) {
				all_names = all_names.substr(0, all_names.size() - 2);
			}
			membersNames = "MEMBERS: " + all_names;
			firstMember = membersList[0];
		}
		gameStage = response["gameStage"];

	}
	catch (const json::exception& e) {
		cerr << "Error parsing JSON: " << e.what() << endl;
	}

	// Append the received data as-is to the output string
	output->append(static_cast<char*>(contents), size * nmemb);
	return size * nmemb;
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
	curl_easy_setopt(curl, CURLOPT_URL, (url_get + "/sse/" + roomCode).c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &sseData);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 0L); // Set timeout to zero to keep the connection open

	// Perform the request
	res = curl_easy_perform(curl);

	// Check for errors
	if (res != CURLE_OK) {
		cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
	}

	curl_easy_cleanup(curl);

}


int MainMenu()
{
	thread sseThread(PerformSSE);

	while (currentScene == MAIN_MENU)
	{

		if (IsKeyPressed(KEY_ENTER) && (serverOnline == true)) {
			currentScene = STARTING_ROOM;
		}

		if (WindowShouldClose()) {
			sseThread.join();
			CloseWindow();
			return 0;
		}
	}

	return 0;
}


int StartingRoom()
{
	thread sseThread(PerformSSE);

	while (currentScene ==STARTING_ROOM)
	{

		if (IsKeyPressed(KEY_ENTER) && (serverOnline == true)) {
			currentScene = STARTING_ROOM;
		}

		if (WindowShouldClose()) {
			sseThread.join();
			CloseWindow();
			return 0;
		}
	}

	return 0;
}

int main() {

	InitWindow(initialScreenWidth, initialScreenHeight, "Conspirators"); 

	currentScene = MAIN_MENU; 

	SetTargetFPS(60);


	while (!WindowShouldClose()) {
		switch (currentScene) {
		case MAIN_MENU:
			MainMenu();
			break;
		case STARTING_ROOM:
			StartingRoom();
			break;
	}

	ClosingMaintenance();
	CloseWindow();
	return 0;
}
*/