#include <iostream>
#include "raylib.h"

int ConspiratorsMenuMain() {
	while(!WindowShouldClose) {
		BeginDrawing();
		DrawText("Yeah", 100, 100, 100, WHITE);
		EndDrawing();
	}
	return 0;
}