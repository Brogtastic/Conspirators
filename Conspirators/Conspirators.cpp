#include "raylib.h"
#include "MiddleMan.h"
#include <iostream>
#include <string>
#include <list>
#define GetIntArrayLength(x) sizeof(x) / sizeof(int)
using namespace std;
#define print(x) cout << x

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

// Enumeration to represent different game scenes
enum GameScene {
	SCENE_MENU,
	SCENE_OTHER_MENU,
	// Add more scenes here as needed
};

GameScene currentScene = SCENE_MENU; // Start with the Menu scene
const int initialScreenWidth = 1280;
const int initialScreenHeight = 720;
int screenWidth = 1280, realScreenWidth = 1280, rememberScreenWidth = 1280;
int screenHeight = 720, realScreenHeight = 720, rememberScreenHeight = 720;
int xScreenMargin, yScreenMargin = 0;

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
}

string GenerateRandomString() {
	const string allowedCharacters = "123456789ABCDEFGHIJKLMNPQRSTUVWXYZ"; // Removed the O and the 0 cause too similar looking
	string randomString;
	for (int i = 0; i < 4; i++) {
		int randomChar = rand() % 32;
		randomString += allowedCharacters[randomChar];
	}
	print("\n" + randomString + "\n");
	return randomString;
}

void ToggleFullScreenWindow(int windowWidth, int windowHeight) {
	if (!IsWindowFullscreen) {
		//SetWindowSize(1280, 720);
		int monitor = GetCurrentMonitor();
		SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
		ToggleFullscreen();
	}
	else {
		ToggleFullscreen();
		SetWindowSize(windowWidth, windowHeight);
	}
}

int Menu()
{

	// Initialization
	//--------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------

	// Main game loop
	while (currentScene == SCENE_MENU)    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------

		AdjustScreenWithSize();


		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		//posx, posy, sizex, sizey
		Rectangle testBG = { xScreenMargin, yScreenMargin, screenWidth, screenHeight };
		DrawRectangleRec(testBG, ORANGE);

		DrawText("MENU", screenWidth / 3.200000 + xScreenMargin, screenHeight / 1.800000 + yScreenMargin, screenWidth / 25.600000, WHITE);

		if (IsKeyPressed(KEY_ENTER)) {
			currentScene = SCENE_OTHER_MENU;
		}

		if (WindowShouldClose()) {
			CloseWindow();
			return 0;
		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	return 0;
}


int OtherMenu()
{

	// Initialization
	//--------------------------------------------------------------------------------------

	int num = 0;

	string generatedCode = GenerateRandomString();

	generatedCode = MainToServer("code", "1234", 0);
	
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (currentScene == SCENE_OTHER_MENU)
	{
		// Update
		//----------------------------------------------------------------------------------

		AdjustScreenWithSize();

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

		//DrawText(roomCode, 400, 400, 50, WHITE);
		DrawText(generatedCode.c_str(), screenWidth / 3.200000f + xScreenMargin, screenHeight / 1.800000f + yScreenMargin, screenWidth / 25.600000f, WHITE);
		//DrawText(access.c_str(), screenWidth / 12.800000f + xScreenMargin, screenHeight / 7.200000f + yScreenMargin, screenWidth / 25.600000f, WHITE);


		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			num += 1;
			string access = MainToServer("number", "1234", num);
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

		if (IsKeyPressed(KEY_N)) {
			generatedCode = GenerateRandomString();
		}

		if (IsKeyPressed(KEY_ENTER)) {
			currentScene = SCENE_MENU;
		}

		if (WindowShouldClose()) {
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

	currentScene = SCENE_MENU; // Start with the Menu scene

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		switch (currentScene) {
			case SCENE_MENU:
				Menu();
				break;
			case SCENE_OTHER_MENU:
				OtherMenu();
				break;
		}
	}

	CloseWindow();
	return 0;
}