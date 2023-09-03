#include "raylib.h"
#include "MiddleMan.h"
#include <iostream>
#include <string>
#include <list>
#include <vector>
#define GetIntArrayLength(x) sizeof(x) / sizeof(int)
using namespace std;
#define print(x) cout << x

enum GameScene {
	MAIN_MENU,
	STARTING_ROOM,
	ROUND_1,
	ROUND_2
};

GameScene currentScene;
const int initialScreenWidth = 1280;
const int initialScreenHeight = 720;
int screenWidth = 1280, realScreenWidth = 1280, rememberScreenWidth = 1280;
int screenHeight = 720, realScreenHeight = 720, rememberScreenHeight = 720;
int xScreenMargin, yScreenMargin = 0;
Font font;
vector<string> allGeneratedCodes;
string roomCode;


void AdjustScreenWithSize() {
	screenHeight = GetScreenHeight();
	screenWidth = GetScreenWidth();
	realScreenHeight = GetScreenHeight();
	realScreenWidth = GetScreenWidth();

	// Makes sure background is always in a 16:9 ratio
	if (screenWidth > (screenHeight * 16) / 9) {
		screenWidth = (screenHeight * 16) / 9;
	}
	else {
		screenHeight = (screenWidth * 9) / 16;
	}

	// Finding measurements to keep the game centered in the window
	if (realScreenHeight > screenHeight) {
		yScreenMargin = (realScreenHeight - screenHeight) / 2;
	}
	else {
		yScreenMargin = 0;
	}
	if (realScreenWidth > screenWidth) {
		xScreenMargin = (realScreenWidth - screenWidth) / 2;
	}
	else {
		xScreenMargin = 0;
	}

	if (IsKeyPressed(KEY_F)) {
		if (!IsWindowFullscreen()) {
			rememberScreenWidth = GetScreenWidth();
			rememberScreenHeight = GetScreenHeight();
			int monitor = GetCurrentMonitor();
			screenWidth = GetMonitorWidth(monitor);
			screenHeight = GetMonitorHeight(monitor);
			SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
			SetWindowSize(1600, 900);
			ToggleFullscreen();
		}
		else {
			ToggleFullscreen();
			SetWindowSize(rememberScreenWidth, rememberScreenHeight);
		}
	}
	if (IsKeyPressed(KEY_ESCAPE) && IsWindowFullscreen()) {
		ToggleFullscreen();
		SetWindowSize(rememberScreenWidth, rememberScreenHeight);
	}
}

string GenerateRandomString() {
	const string allowedCharacters = "23456789ABCDEFGHJKLMNPQRSTUVWXYZ"; // Removed the O and the 0  and then I and 1 cause too similar looking
	string randomString;
	for (int i = 0; i < 4; i++) {
		int randomChar = rand() % 32;
		randomString += allowedCharacters[randomChar];
	}
	return randomString;
}

void LimitRoomCodes(int limit) {
	while (allGeneratedCodes.size() > limit) {
		DeleteCodeOffServer(allGeneratedCodes[0]);
		allGeneratedCodes.erase(allGeneratedCodes.begin());
	}
}

void ToggleFullScreenWindow(int windowWidth, int windowHeight) {
	if (!IsWindowFullscreen) {
		int monitor = GetCurrentMonitor();
		SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
		ToggleFullscreen();
	}
	else {
		ToggleFullscreen();
		SetWindowSize(windowWidth, windowHeight);
	}
}

void ClosingMaintenance() {
	UnloadFont(font);
	LimitRoomCodes(0);
}

int MainMenu()
{
	LimitRoomCodes(0);

	string statusOfServer = "";
	bool serverOnline;

	int urlCheckInterval = 0;

	if (!DoesURLExist("")) {
		statusOfServer = "Servers are down. Cannot create room.";
		serverOnline = false;
	}
	else {
		serverOnline = true;
	}

	while (currentScene == MAIN_MENU)
	{
		// Update
		//----------------------------------------------------------------------------------

		AdjustScreenWithSize();
		
		if (IsKeyPressed(KEY_R)) {
			serverOnline = DoesURLExist("");
		}

		if (!serverOnline) {
			statusOfServer = "Servers are down. Cannot create room. Press 'R' to refresh.";
		}
		else {
			statusOfServer = "";
		}

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		//posx, posy, sizex, sizey
		Rectangle testBG = { xScreenMargin, yScreenMargin, screenWidth, screenHeight };
		DrawRectangleRec(testBG, ORANGE);

		DrawText("MENU", screenWidth / 3.200000f + xScreenMargin, screenHeight / 1.800000f + yScreenMargin, screenWidth / 25.600000f, WHITE);
		DrawText(statusOfServer.c_str(), screenWidth / 7.200000f + xScreenMargin, screenHeight / 1.1000f + yScreenMargin, screenWidth / 60.600000f, WHITE);

		if (IsKeyPressed(KEY_ENTER) && (serverOnline == true)) {
			currentScene = STARTING_ROOM;
		}

		if (WindowShouldClose()) {
			ClosingMaintenance();
			CloseWindow();
			return 0;
		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	return 0;
}


int StartingRoom()
{

	// Initialization
	//--------------------------------------------------------------------------------------

	int num = 0;
	int frame = 0;
	bool onRed = false;
	bool onRedClick = false;

	roomCode = CreateRoom();
	allGeneratedCodes.push_back(roomCode);

	LimitRoomCodes(1);

	vector<string> membersVector = RefreshMembers(roomCode);

	string membersNames = membersVector[0];
	string numberOfMembers = "0/8";
	string firstMember = membersVector[2];
	
	//--------------------------------------------------------------------------------------

	// game loop
	while (currentScene == STARTING_ROOM)
	{
		// Update
		//----------------------------------------------------------------------------------

		Vector2 mousePos = GetMousePosition();

		AdjustScreenWithSize();

		frame += 1;
		if (frame > 60) {
			frame = 0;
		}

		float fontspacing = screenWidth / 175.0f;
		float fontsize = screenWidth / 25.600000f;

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		//posx, posy, sizex, sizey
		Rectangle testBG = { xScreenMargin, yScreenMargin, screenWidth, screenHeight };
		DrawRectangleRec(testBG, BLUE);

		//posx, posy, sizex, sizey
		Rectangle testRec = { screenWidth / 11.636364f + xScreenMargin, screenHeight / 1.8f + yScreenMargin, screenWidth / 25.6f, screenHeight / 14.4f };
		Rectangle testRec2 = { screenWidth / 7.111111f + xScreenMargin, screenHeight / 1.800000f + yScreenMargin, screenWidth / 25.600000f, screenHeight / 14.400000f };
		DrawRectangleRounded(testRec, 0.2f, 2, RAYWHITE);
		DrawRectangleRounded(testRec2, 0.2f, 2, RED);

		//DrawTextEx(font, string, vector2position, fontsize, fontspacing, color);
		DrawTextEx(font, roomCode.c_str(), { screenWidth / 3.200000f + xScreenMargin, screenHeight / 1.800000f + yScreenMargin }, fontsize, fontspacing, WHITE);

		if ((frame%9 == 0) && !IsKeyPressed(KEY_ENTER)) {
			vector<string> membersVector = RefreshMembers(roomCode);

			membersNames = membersVector[0];
			numberOfMembers = membersVector[1] + "/8";
			firstMember = membersVector[2];

			if (membersVector[3] == "round1") {
				currentScene = ROUND_1;
			}
		}


		DrawTextEx(font, membersNames.c_str(), { screenWidth / 3.200000f + xScreenMargin, screenHeight / 2.100000f + yScreenMargin }, screenWidth / 25.600000f, fontspacing, WHITE);
		DrawTextEx(font, firstMember.c_str(), { screenWidth / 4.200000f + xScreenMargin, screenHeight / 1.100000f + yScreenMargin }, screenWidth / 25.600000f, fontspacing, WHITE);
		DrawTextEx(font, numberOfMembers.c_str(), { screenWidth / 100.200000f + xScreenMargin, screenHeight / 100.100000f + yScreenMargin }, screenWidth / 25.600000f, fontspacing, WHITE);
		//DrawText(access.c_str(), screenWidth / 12.800000f + xScreenMargin, screenHeight / 7.200000f + yScreenMargin, screenWidth / 25.600000f, WHITE);


		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			num += 1;
			TransferClicks(num);
		}

		if (IsKeyPressed(KEY_R)) {
			float myPosX = screenWidth / testRec2.x;
			float myPosY = screenHeight / testRec2.y;
			float mySizeX = screenWidth / testRec2.width;
			float mySizeY = screenHeight / testRec2.height;

			print("Rectangle testRec = { screenWidth / " + to_string(myPosX) + "f + xScreenMargin, screenHeight / " + to_string(myPosY) + "f + yScreenMargin, screenWidth / " + to_string(mySizeX) + ", screenHeight / " + to_string(mySizeY) + "};");
		}

		if (IsKeyPressed(KEY_T)) {
			float myTextPosX = screenWidth / 100.0f;
			float myTextPosY = screenHeight / 100.0f;
			float myFontSize = screenWidth / 50.0f;

			print("DrawText(MainToServer(server).c_str(), screenWidth / " + to_string(myTextPosX) + "f + xScreenMargin, screenHeight / " + to_string(myTextPosY) + "f + yScreenMargin, screenWidth / " + to_string(myFontSize) + "f, WHITE);");
		}

		if (IsKeyPressed(KEY_ENTER)){
			currentScene = MAIN_MENU;
		}

		if (CheckCollisionPointRec(mousePos, testRec2)) onRed = true;
		else onRed = false;
		
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && onRed == true) onRedClick = true;
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && onRed == false) onRedClick = false;
		if (IsMouseButtonUp(MOUSE_BUTTON_LEFT) && (onRedClick == true)) currentScene = MAIN_MENU;


		if (WindowShouldClose()) {
			ClosingMaintenance();
			CloseWindow();
			return 0;
		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}
}


int Round1()
{

	// Initialization
	//--------------------------------------------------------------------------------------

	int num = 0;
	int frame = 0;

	string minute = "1";
	string seconds = "30";

	vector<string> membersVector = RefreshMembers(roomCode);
	string questionText = membersVector[4];

	float secondFrame = 15.0f;
	float minuteFrame = 75.0f;
	int secondInt = 30;

	string currentRound;

	string timeText = "1:15";

	//--------------------------------------------------------------------------------------

	// game loop
	while (currentScene == ROUND_1)
	{
		// Update
		//----------------------------------------------------------------------------------

		Vector2 mousePos = GetMousePosition();

		AdjustScreenWithSize();

		secondFrame -= 0.017f;
		minuteFrame -= 0.017f;

		if (secondFrame < 0) secondFrame = 60.0f;

		secondInt = static_cast<int>(secondFrame);
		seconds = to_string(secondInt);

		// Track minute num
		if (minuteFrame < 60.0f) minute = "0";
		else minute = "1";

		// Add zero if seconds are less than 2 digits
		if (secondFrame > 10.0f) timeText = minute + ":" + seconds;
		else timeText = minute + ":0" + seconds;
		

		// Display text when time is up
		if (minuteFrame < 0) timeText = "Time's Up!";

		// If time runs out, round advances
		if ((minuteFrame < -3) || (IsKeyPressed(KEY_N))) {
			SetRound(roomCode, "round2");
			currentScene = ROUND_2;
		}

		// Check twice a second if the rounds have changed
		if (frame == 30 || frame == 60) {
			currentRound = GetRound(roomCode);
			if(currentRound == "round2") currentScene = ROUND_2;
		}

		frame += 1;
		if (frame > 60) frame = 0;

		float fontspacing = screenWidth / 175.0f;
		float fontsize = screenWidth / 25.600000f;

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		//Artificial Background
		Rectangle testBG = { xScreenMargin, yScreenMargin, screenWidth, screenHeight };
		DrawRectangleRec(testBG, MAROON);

		string round1Text = "ROUND 1!!!";

		DrawTextEx(font, questionText.c_str(), { screenWidth / 16.200000f + xScreenMargin, screenHeight / 8.00000f + yScreenMargin }, screenWidth / 30.600000f, fontspacing, WHITE);
		DrawTextEx(font, timeText.c_str(), { screenWidth / 2.200000f + xScreenMargin, screenHeight / 5.00000f + yScreenMargin }, screenWidth / 30.600000f, fontspacing, WHITE);


		if (WindowShouldClose()) {
			ClosingMaintenance();
			CloseWindow();
			return 0;
		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}
}

int Round2()
{

	// Initialization
	//--------------------------------------------------------------------------------------

	int num = 0;
	int frame = 0;

	string minute = "1";
	string seconds = "30";

	//--------------------------------------------------------------------------------------

	// game loop
	while (currentScene == ROUND_2)
	{
		// Update
		//----------------------------------------------------------------------------------

		Vector2 mousePos = GetMousePosition();

		AdjustScreenWithSize();

		frame += 1;
		if (frame > 60) {
			frame = 0;
		}

		float fontspacing = screenWidth / 175.0f;
		float fontsize = screenWidth / 25.600000f;

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		//Artificial Background
		Rectangle testBG = { xScreenMargin, yScreenMargin, screenWidth, screenHeight };
		DrawRectangleRec(testBG, MAROON);

		string round2Text = "ROUND 2!!!";

		DrawTextEx(font, round2Text.c_str(), { screenWidth / 16.200000f + xScreenMargin, screenHeight / 5.00000f + yScreenMargin }, screenWidth / 15.600000f, fontspacing, WHITE);

		if (WindowShouldClose()) {
			ClosingMaintenance();
			CloseWindow();
			return 0;
		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}
}


int main() {

	InitWindow(initialScreenWidth, initialScreenHeight, "Conspirators"); // Initialize the window

	// Set window resizable flag
	SetWindowState(FLAG_WINDOW_RESIZABLE);

	currentScene = MAIN_MENU; // Start with the Menu scene

	SetTargetFPS(60);

	SetExitKey(0);

	font = LoadFont("resources/fonts/romulus.png");

	while (!WindowShouldClose()) {
		switch (currentScene) {
			case MAIN_MENU:
				MainMenu();
				break;
			case STARTING_ROOM:
				StartingRoom();
				break;
			case ROUND_1:
				Round1();
				break;
			case ROUND_2:
				Round2();
				break;
		}
	}

	ClosingMaintenance();
	CloseWindow();
	return 0;
}


/*

60 numbers divisible by 1
30 numbers divisible by 2
20 numbers divisible by 3
12 numbers divisible by 4
12 numbers divisible by 5
10 numbers divisible by 6
8 numbers divisible by 7
7 numbers divisible by 8

*/