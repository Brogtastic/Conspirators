#include "MyGlobals.h"
string roomCode = "None";
string roomQuestion = "None";
string gameStage = "None";
int numMembers = 0;
int wordFrame = 0;
vector<string> membersList = { "", "" };
string membersNames = "No members in room";
string firstMember = "Need at least 3 players to start the game...";
bool threadActive = false;

#include <raylib/raylib.h>
#include "MiddleMan.h"
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <thread>
#include <future>
#include <random>
#define GetIntArrayLength(x) sizeof(x) / sizeof(int)
#define to_float(X) static_cast<float>(X)
using namespace std;
#define print(x) cout << x

enum GameScene {
	LOADING,
	REFRESH,
	MAIN_MENU,
	STARTING_ROOM,
	ROUND_1,
	ROUND_2,
	ROUND_3,
	TEST_ROUND
};

GameScene currentScene;
const int initialScreenWidth = 1280;
const int initialScreenHeight = 720;
int screenWidth = 1280, realScreenWidth = 1280, rememberScreenWidth = 1280;
int screenHeight = 720, realScreenHeight = 720, rememberScreenHeight = 720;
int xScreenMargin, yScreenMargin = 0;
float fontspacing = screenWidth / 175.0f;
float fontsize = screenWidth / 25.600000f;
Font font;
bool serverOnline;
vector<string> allGeneratedCodes;
thread sseThread;

#define DRAWTEXT(Text, X, Y, Size, Color) DrawTextEx(font, Text.c_str(), { screenWidth / X + xScreenMargin, screenHeight / Y + yScreenMargin }, fontsize * Size, fontspacing, Color)
#define PrintRelMousePos if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) print("\n\nX: " + to_string(to_float(screenWidth) / to_float(GetMouseX())) + "\nY: " + to_string(to_float(screenHeight) / to_float(GetMouseY())) + "\n")



void DrawTextBreak(string text, float x, float y, float size, int maxWidth, Color color) {
	if (text.length() < maxWidth) {
		DRAWTEXT(text, x, y, size, color);
	}
	else if(text.length() < maxWidth*2){
		string text1, text2;
		string cutInHalf = text.substr(0, text.length() / 2);

		size_t lastSpacePos = cutInHalf.find_last_of(' ');

		if (lastSpacePos != string::npos) {
			text1 = text.substr(0, lastSpacePos);
			text2 = text.substr(lastSpacePos + 1);
		}
		else {
			cout << "No space found in the input string." << endl;
		}

		DRAWTEXT(text1, x, y, size, color);
		DRAWTEXT(text2, x, y + (fontsize * size), size, color);

	}
}

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

	fontspacing = screenWidth / 175.0f;
	fontsize = screenWidth / 25.600000f;
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
	sseThread.join();
	CloseWindow();
}

int Loading() {

	int frame = 0;
	float loadingBarWidth = 0.0f;

	future<bool> result = async(launch::async, DoesURLExist);

	while (currentScene == LOADING) {

		AdjustScreenWithSize();

		frame += 1;
		if(loadingBarWidth < 1.0f) loadingBarWidth += 0.01f;
		else loadingBarWidth = 1.0f;
		
		BeginDrawing();
		ClearBackground(BLACK);

		//posx, posy, sizex, sizey
		Rectangle testBG = { xScreenMargin, yScreenMargin, screenWidth, screenHeight };
		DrawRectangleRec(testBG, DARKGREEN);

		//Rectangle loadingRect = { 450, 350, 400, 50 };
		Rectangle loadingRect1 = { screenWidth / 2.844445f + xScreenMargin, screenHeight / 2.057143f + yScreenMargin, (screenWidth / 3.2000f)*loadingBarWidth, screenHeight / 14.400000 };
		DrawRectangleRec(loadingRect1, WHITE);
		Rectangle loadingRect = { screenWidth / 2.844445f + xScreenMargin, screenHeight / 2.057143f + yScreenMargin, screenWidth / 3.2000f, screenHeight / 14.400000 };
		DrawRectangleLinesEx(loadingRect, screenWidth/300.0f, ORANGE);
		

		string percentText = to_string(static_cast<int>(loadingBarWidth * 100)) + "%";
		DrawText(percentText.c_str(), screenWidth / 2.100000f + xScreenMargin, screenHeight / 1.750000f + yScreenMargin, screenWidth / 25.600000f, WHITE);
		EndDrawing();

		if (loadingBarWidth == 0.25) {
			if (!DoesURLExist()) {
				serverOnline = false;
			}
			else {
				serverOnline = true;
				CreateRoom();
				allGeneratedCodes.push_back(roomCode);
				LimitRoomCodes(1);
			}
		}

		if (IsKeyPressed(KEY_T)) {
			float myRectPosX = screenWidth / 450.0f;
			float myRectPosY = screenHeight / 350.0f;
			float myRectWidth = screenWidth / 400.0f;
			float myRectHeight = screenHeight / 50.0f;

			print("Rectangle loadingRect = { screenWidth / " + to_string(myRectPosX) + "f + xScreenMargin, screenHeight / " + to_string(myRectPosY) + "f + yScreenMargin, screenWidth / " + to_string(myRectWidth) + "f, screenHeight / " + to_string(myRectHeight) + ", WHITE);");
		}

		if (loadingBarWidth == 1.0f) {
			currentScene = MAIN_MENU;
		}

		if (WindowShouldClose()) {
			ClosingMaintenance();
			CloseWindow();
			return 0;
		}

	}
	serverOnline = result.get();
	currentScene = MAIN_MENU;
	return 0;
}

int Refresh() {
	currentScene = MAIN_MENU;
	return 0;
}

int MainMenu()
{
	threadActive = false;
	int frame = 0;
	string statusOfServer = "";

	future<bool> result = async(launch::async, DoesURLExist);
	while (1) {
		frame += 1;
		BeginDrawing();
		ClearBackground(BLACK);
		Rectangle testBG = { xScreenMargin, yScreenMargin, screenWidth, screenHeight };
		DrawRectangleRec(testBG, ORANGE);
		DrawText("MENU", screenWidth / 3.200000f + xScreenMargin, screenHeight / 1.800000f + yScreenMargin, screenWidth / 25.600000f, WHITE);
		DrawText(statusOfServer.c_str(), screenWidth / 7.200000f + xScreenMargin, screenHeight / 1.1000f + yScreenMargin, screenWidth / 60.600000f, WHITE);
		EndDrawing();
		if (frame % 15 == 0) {
			if(statusOfServer.length()>=10 && statusOfServer.length() < 14) statusOfServer = statusOfServer += ".";
			else statusOfServer = "connecting";
		}
		if (result.wait_for(chrono::milliseconds(0)) == future_status::ready) {
			serverOnline = result.get();
			break; 
		}
	}

	if (serverOnline){
		CreateRoom();
		allGeneratedCodes.push_back(roomCode);
		LimitRoomCodes(1);
	}

	if (sseThread.joinable()) {
		print("\nThread joinable!");
		sseThread.join();
		print("\nThread joined!");
	}
	threadActive = true;
	sseThread = thread(PerformSSE);

	int urlCheckInterval = 0;

	while (currentScene == MAIN_MENU)
	{
		// Update
		//----------------------------------------------------------------------------------

		AdjustScreenWithSize();
		
		if (IsKeyPressed(KEY_R)) {
			currentScene = REFRESH;
		}

		if (!serverOnline) statusOfServer = "Servers are down. Cannot create room. Press 'R' to refresh.";
		else statusOfServer = "";

		PrintRelMousePos;

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


	string numberOfMembers = "0/8";
	
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

		if (gameStage == "round1") {
			currentScene = ROUND_1;
		}

		numberOfMembers = to_string(numMembers) + "/8";

		PrintRelMousePos;

		DRAWTEXT(numberOfMembers, 41.29f, 30.0f, 1.0f, WHITE);
		DRAWTEXT(membersNames, 12.8f, 2.21f, 1.0f, WHITE);
		DRAWTEXT(roomCode, 4.53f, 1.76f, 1.2f, WHITE);
		DRAWTEXT(firstMember, 19.555f, 1.125f, 0.85f, WHITE);


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

			print("\n\nDrawText(MainToServer(server).c_str(), screenWidth / " + to_string(myTextPosX) + "f + xScreenMargin, screenHeight / " + to_string(myTextPosY) + "f + yScreenMargin, screenWidth / " + to_string(myFontSize) + "f, WHITE);");
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

	float secondFrame = 15.0f;
	float minuteFrame = 75.0f;
	int secondInt = 30;

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
			SetRound("round2");
			gameStage = "round2";
			currentScene = ROUND_2;
		}

		// Check twice a second if the rounds have changed
		if (gameStage == "round2") {
			print("\ntime for round 2");
			currentScene = ROUND_2;
		}

		frame += 1;
		if (frame > 60) frame = 0;

		PrintRelMousePos;

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		//Artificial Background
		Rectangle testBG = { xScreenMargin, yScreenMargin, screenWidth, screenHeight };
		DrawRectangleRec(testBG, MAROON);

		string round1Text = "ROUND 1!!!";

		DrawTextEx(font, roomQuestion.c_str(), { screenWidth / 16.200000f + xScreenMargin, screenHeight / 8.00000f + yScreenMargin }, screenWidth / 30.600000f, fontspacing, WHITE);
		DrawTextEx(font, timeText.c_str(), { screenWidth / 2.200000f + xScreenMargin, screenHeight / 5.00000f + yScreenMargin }, screenWidth / 30.600000f, fontspacing, WHITE);


		if (WindowShouldClose()) {
			ClosingMaintenance();
			return 0;
		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}
	return 0;
}

int Round2()
{
	// Initialization
	//--------------------------------------------------------------------------------------
	SetRound("round2");

	int num = 0;
	int frame = 0;

	string minute = "1";
	string seconds = "30";

	float secondFrame = 15.0f;
	float minuteFrame = 75.0f;
	int secondInt = 30;

	string timeText = "1:15";

	string promptText = "Write some words to mess with their theory!";

	PrintRelMousePos;

	//--------------------------------------------------------------------------------------

	// game loop
	while (currentScene == ROUND_2)
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
			SetRound("round3");
			gameStage = "round3";
			currentScene = ROUND_3;
		}

		// Check twice a second if the rounds have changed
		if (gameStage == "round3") {
			currentScene = ROUND_3;
		}

		frame += 1;
		if (frame > 60) frame = 0;

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		//Artificial Background
		Rectangle testBG = { xScreenMargin, yScreenMargin, screenWidth, screenHeight };
		DrawRectangleRec(testBG, GREEN);

		string round2Text = "ROUND 2!!!";

		DrawTextEx(font, round2Text.c_str(), { screenWidth / 16.200000f + xScreenMargin, screenHeight / 8.00000f + yScreenMargin }, screenWidth / 30.600000f, fontspacing, WHITE);
		DrawTextEx(font, promptText.c_str(), { screenWidth / 16.200000f + xScreenMargin, screenHeight / 5.00000f + yScreenMargin }, screenWidth / 30.600000f, fontspacing, WHITE);
		DrawTextEx(font, timeText.c_str(), { screenWidth / 16.200000f + xScreenMargin, screenHeight / 3.00000f + yScreenMargin }, screenWidth / 15.600000f, fontspacing, WHITE);

		if (WindowShouldClose()) {
			ClosingMaintenance();
			return 0;
		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}
}

void DisplayWord(string word) {
	if (wordFrame < 100) {
		DrawTextEx(font, word.c_str(), { screenWidth / 8.200000f + xScreenMargin - wordFrame, screenHeight / 2.00000f + yScreenMargin - wordFrame }, screenWidth / 15.600000f + wordFrame, fontspacing, Fade(WHITE, (100.0f / wordFrame - 1)));
	}
}

int Round3() {
	// Initialization
	//--------------------------------------------------------------------------------------
	SetRound("round3");


	int num = 0;
	int frame = 0;

	// Create a vector of strings
	string minute = "1";
	string seconds = "30";

	//--------------------------------------------------------------------------------------

	// game loop
	while (currentScene == ROUND_3)
	{
		// Update
		//----------------------------------------------------------------------------------

		Vector2 mousePos = GetMousePosition();

		AdjustScreenWithSize();

		frame += 1;
		if (frame > 60) {
			frame = 0;
		}

		if (IsKeyPressed(KEY_R)) {
			wordFrame = 0;
			wordsToPresent.push_back("TEST");
		}

		PrintRelMousePos;

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		//Artificial Background
		Rectangle testBG = { xScreenMargin, yScreenMargin, screenWidth, screenHeight };
		DrawRectangleRec(testBG, RED);

		string round3Text = "ROUND 3!!!";

		DrawTextEx(font, round3Text.c_str(), { screenWidth / 16.200000f + xScreenMargin, screenHeight / 5.00000f + yScreenMargin }, screenWidth / 15.600000f, fontspacing, WHITE);

		if (wordsToPresent.size() > 1) {
			wordsToPresent.erase(wordsToPresent.begin());
			wordFrame = 0;
		}
		if (wordsToPresent.size() == 1) {
			DisplayWord(wordsToPresent[0]);
		}
		if (wordFrame > 100) {
			wordFrame = 0;
			if (wordsToPresent.size() > 0) {
				wordsToPresent.erase(wordsToPresent.begin());
			}
		}
		wordFrame++;

		if (WindowShouldClose()) {
			ClosingMaintenance();
			return 0;
		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}
}


int TestRound() {
	// Initialization
	//--------------------------------------------------------------------------------------

	int num = 0;
	int frame = 0;
	int wordFrame = 0;

	string minute = "1";
	string seconds = "30";

	string testRoundText = "Test round";

	//--------------------------------------------------------------------------------------

	// game loop
	while (currentScene == TEST_ROUND)
	{
		// Update
		//----------------------------------------------------------------------------------

		AdjustScreenWithSize();

		PrintRelMousePos;

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		//Artificial Background
		Rectangle testBG = { xScreenMargin, yScreenMargin, screenWidth, screenHeight };
		DrawRectangleRec(testBG, RED);


		if (IsKeyPressed(KEY_SPACE)) {
			testRoundText.insert(0, 1, 'i');
		}

		//DrawTextEx(font, testRoundText.c_str(), { screenWidth / 16.200000f + xScreenMargin, screenHeight / 5.00000f + yScreenMargin }, screenWidth / 15.600000f, fontspacing, WHITE);

		DrawTextBreak("What an amazing day to go out shopping with my friends!", 16.2f, 5.0f, 1.0f, 30, WHITE);

		if (WindowShouldClose()) {
			//ClosingMaintenance();
			CloseWindow();
			return 0;
		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}
	return 0;
}

int main() {

	InitWindow(initialScreenWidth, initialScreenHeight, "Conspirators"); // Initialize the window

	// Set window resizable flag
	SetWindowState(FLAG_WINDOW_RESIZABLE);

	font = LoadFont("resources/fonts/romulus.png");

	currentScene = TEST_ROUND; 

	SetTargetFPS(60);

	SetExitKey(0);

	while (!WindowShouldClose()) {
		switch (currentScene) {
			case LOADING:
				Loading();
				break;
			case REFRESH:
				Refresh();
				break;
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
			case ROUND_3:
				Round3();
				break;
			case TEST_ROUND:
				TestRound();
				break;
		}
	}

	ClosingMaintenance();
	return 0;
}