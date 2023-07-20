#include "raylib.h"
#include <iostream>
#include <string>
#include <list>
#define GetIntArrayLength(x) sizeof(x) / sizeof(int)
using namespace std;
#define print(x) cout << x

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

string generateRandomString() {
	const string allowedCharacters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string randomString;
	for (int i = 0; i < 4; ++i) {
		int random = rand() % 37;
		randomString += allowedCharacters[random];
	}
	print("\n\n\n" + randomString + "\n\n\n");
	return randomString;
}

int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	const int initialScreenWidth = 1280;
	const int initialScreenHeight = 720;

	int screenWidth = 1280;
	int screenHeight = 720;
	int realScreenWidth = 1280;
	int realScreenHeight = 720;
	int xScreenMargin, yScreenMargin = 0;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(initialScreenWidth, initialScreenHeight, "Conspirators");

	string roomCode1 = generateRandomString();
	const char* roomCode = roomCode1.c_str();

	
	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------

		screenHeight = GetScreenHeight();
		screenWidth = GetScreenWidth();
		realScreenHeight = GetScreenHeight();
		realScreenWidth = GetScreenWidth();

		
		if (screenWidth > (screenHeight * 16) / 9) {
			screenWidth = (screenHeight * 16) / 9;
		}
		else {
			screenHeight = (screenWidth * 9) / 16;
		}

		
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

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		//posx, posy, sizex, sizey
		Rectangle testBG = { xScreenMargin, yScreenMargin, screenWidth, screenHeight};
		DrawRectangleRec(testBG, BLUE);

		//posx, posy, sizex, sizey
		Rectangle testRec = { screenWidth / 11.636364f + xScreenMargin, screenHeight / 1.8f + yScreenMargin, screenWidth / 25.6f, screenHeight / 14.4f };
		Rectangle testRec2 = { screenWidth / 7.111111f + xScreenMargin, screenHeight / 1.800000f + yScreenMargin, screenWidth / 25.600000, screenHeight / 14.400000 };
		DrawRectangleRounded(testRec, 0.2f, 2, RAYWHITE);
		DrawRectangleRounded(testRec2, 0.2f, 2, RED);

		DrawText(roomCode, 400, 400, 50, WHITE);

		if (IsKeyPressed(KEY_R)) {
			float myPosX = screenWidth / testRec2.x;
			float myPosY = screenHeight / testRec2.y;
			float mySizeX = screenWidth / testRec2.width;
			float mySizeY = screenHeight / testRec2.height;

			print("Rectangle testRec = { screenWidth / " + to_string(myPosX) + "f + xScreenMargin, screenHeight / " + to_string(myPosY) + "f + yScreenMargin, screenWidth / " + to_string(mySizeX) + ", screenHeight / " + to_string(mySizeY) + "};");
		}
		
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}