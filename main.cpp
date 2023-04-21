#include <raylib.h>
#include <iostream>

enum CurrentHealth { Low, Medium, High };

struct Health {
	Vector2 pos;
	CurrentHealth state;
};

struct Car {
	Vector2 pos;
	Health health;
};

int main(void) {
	const int screen_height = 720;
	const int screen_width = 1280;

	InitWindow(screen_width, screen_height, "RayCarRush");

	SetTargetFPS(60);

	Car car = {
		.pos = { (float)screen_width / 2, screen_height * 0.9 },
		.health = {
			.pos = { screen_width * 0.05, screen_height * 0.1 },
			.state = CurrentHealth::High,
		}
	};

	while (!WindowShouldClose()) {
		// update
		if (IsKeyDown(KEY_RIGHT)) car.pos.x += 5.0f;
		if (IsKeyDown(KEY_LEFT)) car.pos.x -= 5.0f;
		if (IsKeyDown(KEY_UP)) car.pos.y -= 5.0f;
		if (IsKeyDown(KEY_DOWN)) car.pos.y += 5.0f;

		BeginDrawing();
		ClearBackground(DARKGRAY);
		DrawText("Health: ", 190, 200, 20, PINK);
		DrawRectangleV(car.pos, { 50, 50 }, MAROON);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
