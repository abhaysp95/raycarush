#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>

#define ROADBORDERSZ 2
#define ROADSTRIPSZ 4
#define MAXENEMYCARSZ 10

void update_score(char *str, size_t score);

typedef enum CurrentHealth { Low, Medium, High } CurrentHealth;
typedef enum CarType { Player, Enemy } CarType;

typedef struct Health {
	Rectangle body;
	CurrentHealth state;
} Health;

typedef struct Car {
	Rectangle body;
	Health health;
	size_t score;
	size_t speed;
	CarType car_type;
	bool active;
	Color color;
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

	Car player = {
		.body = { (float)screen_width / 2 - 25, screen_height * 0.9, 50, 50 },
		.health = {
			.body = { (float)screen_width / 2 - 25, screen_height * 0.88, 50, 10 },
			.state = High
		},
		.score = 0,
		.speed = 5,  // initial speed
		.car_type = Player,
		.color = SKYBLUE,
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

	Car enemies[MAXENEMYCARSZ];
	for (int i = 0; i < MAXENEMYCARSZ; i++) {
		int xpos = GetRandomValue(screen_width * 0.2, screen_width * 0.8 - 50);
		Car enemy = {
			.body = { xpos, 0, 50, 50 },
			.score = 0,
			.speed = 5,
			.car_type = Enemy,
			.active = false,
			.color = RED,
		};
		enemies[i] = enemy;
	}

	float strip_scrolling = 0.0;
	int framecount = 0;

	while (!WindowShouldClose()) {
		// update
		if (IsKeyDown(KEY_RIGHT)) {
			player.body.x += 5.0f;
			player.health.body.x += 5.0f;
			if (player.body.x + player.body.width > screen_width * 0.8) {
				player.body.x = screen_width * 0.8 - player.body.width;
				player.health.body.x = player.body.x;
			}
		}
		if (IsKeyDown(KEY_LEFT)) {
			player.body.x -= 5.0f;
			player.health.body.x -= 5.0f;
			if (player.body.x < screen_width * 0.2) {
				player.body.x = screen_width * 0.2;
				player.health.body.x = player.body.x;
			}
		}
		if (IsKeyDown(KEY_UP)) {
			player.body.y -= 5.0f;
			player.health.body.y -= 5.0f;
			if (player.health.body.y < 0) {
				player.body.y = screen_height * 0.02;
				player.health.body.y = 0;
			}
		}
		if (IsKeyDown(KEY_DOWN)) {
			player.body.y += 5.0f;
			player.health.body.y += 5.0f;
			if (player.body.y + player.body.height > screen_height) {
				player.body.y = screen_height - player.body.height;
				player.health.body.y = player.body.y - (screen_height * 0.02);
			}
		}


		update_score(score_buf, player.score);

		// make the strip move
		// TODO: make it smooth (if half the strip moves out of frame,
		// that much strip should come in frame from opposite side)
		for (size_t i = 0; i < ROADSTRIPSZ; i++) {
			road.road_strips[i].body.y += player.speed;
			if (road.road_strips[i].body.y >= screen_height) {
				road.road_strips[i].body.y = 0;
			}
		}

		// update enemy cars
		if (framecount >= 40) {
			framecount = 0;
			for (size_t i = 0; i < MAXENEMYCARSZ; i++) {
				if (!enemies[i].active) {
					enemies[i].active = true;
					break;
				}
			}
		}

		// make active enemy move
		for (size_t i = 0; i < MAXENEMYCARSZ; i++) {
			if (enemies[i].active) {
				enemies[i].body.y += 5;
				if (enemies[i].body.y > screen_height) {
					enemies[i].active = false;
					enemies[i].body.y = 0;
				}
			}
		}

		framecount++;
		printf("framecount: %d\n", framecount);

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
		DrawRectangleRec(player.health.body, GREEN);
		DrawRectangleRec(player.body, player.color);

		// draw enemies
		for (size_t i = 0; i < MAXENEMYCARSZ; i++) {
			if (enemies[i].active) {
				DrawRectangleRec(enemies[i].body, enemies[i].color);
			}
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}

void update_score(char *str, size_t score) {
	snprintf(str, 32, "Score: %zu", score);
}
