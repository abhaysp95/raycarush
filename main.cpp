#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <raylib.h>
#include <iostream>

void update_score(char *str, size_t score);

enum CurrentHealth { Low, Medium, High };

struct Health {
	Vector2 pos;
	CurrentHealth state;
};

struct Car {
	Vector2 pos;
	Health health;
	size_t score;
};

int main(void) {
	const int screen_height = 720;
	const int screen_width = 1280;

	InitWindow(screen_width, screen_height, "RAYCARUSH");

	SetTargetFPS(60);

	Car car = {
		.pos = { (float)screen_width / 2, screen_height * 0.9 },
		.health = {
			.pos = { (float)screen_width / 2, screen_height * 0.88 },
			.state = CurrentHealth::High,
		},
		.score = 0,
	};

	// score buffer
	char score_buf[32] = { '\0' };

	// road strip
	/* Rectangle road_strips[5] = {
		{ (float)screen_width / 2 - 15, screen_height * 0.1, 30, 100 },
		{ (float)screen_width / 2 - 15, screen_height * 0.3, 30, 100 },
		{ (float)screen_width / 2 - 15, screen_height * 0.5, 30, 100 },
		{ (float)screen_width / 2 - 15, screen_height * 0.7, 30, 100 },
		{ (float)screen_width / 2 - 15, screen_height * 0.9, 30, 100 },
	}; */

	float strip_scrolling = 0.0;

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

		update_score(score_buf, car.score);

		strip_scrolling += 10;
		if (strip_scrolling >= screen_height) {
			strip_scrolling = 0;
		}

		BeginDrawing();
		ClearBackground(GOLD);
		DrawText("RAYCARUSH", screen_width * 0.03, screen_height * 0.03, 25, DARKGRAY);
		DrawText(score_buf, screen_width * 0.85, screen_height * 0.03, 25, DARKGRAY);
		DrawLineEx({screen_width * 0.2, 0}, {screen_width * 0.2, screen_height}, 10.0, BLACK);
		DrawLineEx({screen_width * 0.8, 0}, {screen_width * 0.8, screen_height}, 10.0, BLACK);
		DrawRectangleV({screen_width * 0.2, 0}, { screen_width * 0.6, screen_height }, DARKGRAY);
		// DrawRectangleRec({ (float)screen_width / 2 - 15, screen_height * 0.9, 30, 100 }, WHITE);
		/* for (size_t i{}; i < 5; i++) {
			DrawRectangleRec(road_strips[i], WHITE);
		} */

		DrawRectangleRec({ (float)screen_width / 2 - 15, strip_scrolling, 30,  100}, WHITE);


		DrawRectangleV(car.health.pos, { 50, 10 }, GREEN);
		DrawRectangleV(car.pos, { 50, 50 }, MAROON);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}

void update_score(char *str, size_t score) {
	snprintf(str, 32, "Score: %zu", score);
}
