#include <iostream>
#include "MyGlobals.h"
#include "MiddleMan.h"
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

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
	string received_data(static_cast<char*>(contents), size * nmemb);
	cout << "\n\nReceived data: " << received_data << endl;

	try {
		json response = json::parse(received_data);

		json data = response["data"];

		if (data == "UpdateMembers") {
			print("\nCalling UpdateMembers");
			UpdateMembers();
		}
		else if (data == "UpdateGameStage") {
			print("\nCalling UpdateGameStage");
			UpdateGameStage();
		}
		else if (data == "SSE Connected") {
			print("\nSSE Connected");
		}
		else {
			print("\nSSE data not recognized... ");
			print("\nData: " + to_string(data));
		}

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
	print("\n\nURL: " + (url_get + "/sse/" + roomCode) + "\n");
	curl_easy_setopt(curl, CURLOPT_URL, (url_get + "/sse/" + roomCode).c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &sseData);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 0L); // Set timeout to zero to keep the connection open

	// Check the SSE thread flag
	// Perform the request
	res = curl_easy_perform(curl);

	// Check for errors
	if (res != CURLE_OK) {
		cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
	}

	curl_easy_cleanup(curl);
}
