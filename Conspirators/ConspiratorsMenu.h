#pragma once
#include "Conspirators.cpp"
#include "raylib.h"
#include <iostream>
#include <string>
#include <list>
#define GetIntArrayLength(x) sizeof(x) / sizeof(int)
using namespace std;
#define print(x) cout << x


void Menu()
{

	// Initialization
	//--------------------------------------------------------------------------------------
	const int initialScreenWidth = 1280;
	const int initialScreenHeight = 720;

	int screenWidth = 1280, realScreenWidth = 1280, rememberScreenWidth = 1280;
	int screenHeight = 720, realScreenHeight = 720, rememberScreenHeight = 720;
	int xScreenMargin, yScreenMargin = 0;

	// Set window resizable flag
	SetWindowState(FLAG_WINDOW_RESIZABLE);

	//print("\n\nYour name must be " + MainToServer("server") + "\n\n");

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


		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		//posx, posy, sizex, sizey
		Rectangle testBG = { xScreenMargin, yScreenMargin, screenWidth, screenHeight };
		DrawRectangleRec(testBG, BLUE);

		Rectangle testRec3 = { 50, 50, 50, 50 };
		DrawRectangleRec(testRec3, RED);

		if (IsKeyPressed(KEY_R)) {
			float myPosX = screenWidth / testRec3.x;
			float myPosY = screenHeight / testRec3.y;
			float mySizeX = screenWidth / testRec3.width;
			float mySizeY = screenHeight / testRec3.height;

			print("Rectangle testRec = { screenWidth / " + to_string(myPosX) + "f + xScreenMargin, screenHeight / " + to_string(myPosY) + "f + yScreenMargin, screenWidth / " + to_string(mySizeX) + ", screenHeight / " + to_string(mySizeY) + "};");
		}

		if (IsKeyPressed(KEY_T)) {
			float myTextPosX = screenWidth / 100.0f;
			float myTextPosY = screenHeight / 100.0f;
			float myFontSize = screenWidth / 50.0f;

			print("DrawText(MainToServer(server).c_str(), screenWidth / " + to_string(myTextPosX) + "f + xScreenMargin, screenHeight / " + to_string(myTextPosY) + "f + yScreenMargin, screenWidth / " + to_string(myFontSize) + "f, WHITE);");
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

		EndDrawing();
		//----------------------------------------------------------------------------------
	}