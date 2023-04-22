#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>

#define ROADBORDERSZ 2
#define ROADSTRIPSZ 4

void update_score(char *str, size_t score);

typedef enum CurrentHealth { Low, Medium, High } CurrentHealth;

typedef struct Health {
	Rectangle body;
	CurrentHealth state;
} Health;

typedef struct Car {
	Rectangle body;
	Health health;
	size_t score;
	size_t speed;
} Car;

typedef struct RoadStrip {
	Rectangle body;
	Color color;
} RoadStrip;

typedef struct RoadBorder {
	Vector2 start;
	Vector2 end;
	float width;
	Color color;
} RoadBorder;

typedef struct Road {
	RoadStrip road_strips[ROADSTRIPSZ];
	RoadBorder road_borders[ROADBORDERSZ];
	Rectangle body;
	Color color;
} Road;

int main(void) {
	const int screen_height = 720;
	const int screen_width = 1280;

	InitWindow(screen_width, screen_height, "RAYCARUSH");

	SetTargetFPS(60);

	Car car = {
		.body = { (float)screen_width / 2 - 25, screen_height * 0.9, 50, 50 },
		.health = {
			.body = { (float)screen_width / 2 - 25, screen_height * 0.88, 50, 10 },
			.state = High
		},
		.score = 0,
		.speed = 5,  // initial speed
	};

	// score buffer
	char score_buf[32] = { '\0' };

	Road road = {
		.body = { (float)screen_width * 0.2, 0, (float)screen_width * 0.6, screen_height },
		.color = DARKGRAY,
		.road_borders =  {
			{
				.start ={ .x = screen_width * 0.2, .y = 0 },
				.end = { .x = screen_width * 0.2, .y = screen_height },
				.width = 10,
			},
			{
				.start ={ .x = screen_width * 0.8, .y = 0 },
				.end ={ .x = screen_width * 0.8, .y = screen_height },
				.width = 10,
			}
		},
	};

	// road strip
	for (size_t i = 0; i < ROADSTRIPSZ; i++) {
		RoadStrip road_strip = { .body = { (float)screen_width / 2 - 15, (float)(screen_height * (0.1 + (float)i / 4)), 30, 100 } };
		road.road_strips[i] = road_strip;
		road.road_strips[i].color = WHITE;
	}

	float strip_scrolling = 0.0;

	while (!WindowShouldClose()) {
		// update
		if (IsKeyDown(KEY_RIGHT)) {
			car.body.x += 5.0f;
			car.health.body.x += 5.0f;
			if (car.body.x + car.body.width > screen_width * 0.8) {
				car.body.x = screen_width * 0.8 - car.body.width;
				car.health.body.x = car.body.x;
			}
		}
		if (IsKeyDown(KEY_LEFT)) {
			car.body.x -= 5.0f;
			car.health.body.x -= 5.0f;
			if (car.body.x < screen_width * 0.2) {
				car.body.x = screen_width * 0.2;
				car.health.body.x = car.body.x;
			}
		}
		if (IsKeyDown(KEY_UP)) {
			car.body.y -= 5.0f;
			car.health.body.y -= 5.0f;
			if (car.health.body.y < 0) {
				car.body.y = screen_height * 0.02;
				car.health.body.y = 0;
			}
		}
		if (IsKeyDown(KEY_DOWN)) {
			car.body.y += 5.0f;
			car.health.body.y += 5.0f;
			if (car.body.y + car.body.height > screen_height) {
				car.body.y = screen_height - car.body.height;
				car.health.body.y = car.body.y - (screen_height * 0.02);
			}
		}


		update_score(score_buf, car.score);

		// make the strip move
		// TODO: make it smooth (if half the strip moves out of frame,
		// that much strip should come in frame from opposite side)
		for (size_t i = 0; i < ROADSTRIPSZ; i++) {
			road.road_strips[i].body.y += car.speed;
			if (road.road_strips[i].body.y >= screen_height) {
				road.road_strips[i].body.y = 0;
			}
		}

		BeginDrawing();
		ClearBackground(GOLD);

		// draw game name and score
		DrawText("RAYCARUSH", screen_width * 0.03, screen_height * 0.03, 25, DARKGRAY);
		DrawText(score_buf, screen_width * 0.85, screen_height * 0.03, 25, DARKGRAY);

		// draw road border
		for (int i = 0; i < ROADBORDERSZ; i++) {
			DrawLineEx(road.road_borders[i].start, road.road_borders[i].end, road.road_borders[i].width, BLACK);
		}

		// draw road
		DrawRectangleRec(road.body, road.color);

		// draw road strips
		for (size_t i = 0; i < 5; i++) {
			DrawRectangleRec(road.road_strips[i].body, road.road_strips[i].color);
		}


		// draw car & health
		DrawRectangleRec(car.health.body, GREEN);
		DrawRectangleRec(car.body, MAROON);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}

void update_score(char *str, size_t score) {
	snprintf(str, 32, "Score: %zu", score);
}
