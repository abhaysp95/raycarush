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
			.pos = { (float)screen_width / 2, screen_height * 0.88 },
			.state = CurrentHealth::High,
		}
	};

	while (!WindowShouldClose()) {
		// update
		if (IsKeyDown(KEY_RIGHT)) {
			car.pos.x += 5.0f;
			car.health.pos.x += 5.0f;
		}
		if (IsKeyDown(KEY_LEFT)) {
			car.pos.x -= 5.0f;
			car.health.pos.x -= 5.0f;
		}
		if (IsKeyDown(KEY_UP)) {
			car.pos.y -= 5.0f;
			car.health.pos.y -= 5.0f;
		}
		if (IsKeyDown(KEY_DOWN)) {
			car.pos.y += 5.0f;
			car.health.pos.y += 5.0f;
		}

		BeginDrawing();
		ClearBackground(GOLD);
		DrawLineEx({screen_width * 0.2, 0}, {screen_width * 0.2, screen_height}, 5.0, BLACK);
		DrawLineEx({screen_width * 0.8, 0}, {screen_width * 0.8, screen_height}, 5.0, BLACK);
		DrawRectangleV(car.health.pos, { 50, 10 }, GREEN);
		DrawRectangleV(car.pos, { 50, 50 }, MAROON);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
