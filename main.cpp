#include <random>
#include <vector>

#include "raylib.h"
#include "raymath.h"

auto main() -> int
{
    constexpr int SCREEN_WIDTH = 800;
    constexpr int SCREEN_HEIGHT = 600;
    constexpr int CELL_SIZE = 50;

    Vector2 position = Vector2Zero();
    int frame_counter = 0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 20.0);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Marching Squares Algorithm");
    SetTargetFPS(60);

    const int MARGIN = 50;
    bool bottom_reached = false;

    const int grid_width = (SCREEN_WIDTH - 2 * MARGIN) / CELL_SIZE + 1;
    const int grid_height = (SCREEN_HEIGHT - 2 * MARGIN) / CELL_SIZE + 1;

    int scalar_values[grid_width][grid_height];
    std::vector<std::tuple<int, int, int>> values;

    for (int i = 0; i < grid_width; ++i) {
        for (int j = 0; j < grid_height; ++j) {
            scalar_values[i][j] = dist(gen);
        }
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

	for (int i = 0; i <= grid_width; i++) {
	  if (i == grid_width) continue;
	  DrawLine(MARGIN + i * CELL_SIZE, MARGIN,
		   MARGIN + i * CELL_SIZE, MARGIN + (grid_height - 1) * CELL_SIZE, BLACK);
	}
	DrawLine(MARGIN + grid_width * CELL_SIZE, MARGIN,
		 MARGIN + grid_width * CELL_SIZE, MARGIN + (grid_height - 1) * CELL_SIZE, BLACK);

	for (int j = 0; j <= grid_height; j++) {
	  if (j == grid_height) continue;
	  DrawLine(MARGIN, MARGIN + j * CELL_SIZE,
		   MARGIN + (grid_width - 1) * CELL_SIZE, MARGIN + j * CELL_SIZE, BLACK);
	}
	DrawLine(MARGIN, MARGIN + grid_height * CELL_SIZE,
		 MARGIN + (grid_width - 1) * CELL_SIZE, MARGIN + grid_height * CELL_SIZE, BLACK);

        for (int i = 0; i < grid_width; ++i) {
            for (int j = 0; j < grid_height; ++j) {
                int x = i * CELL_SIZE;
                int y = j * CELL_SIZE;
                Color color = (scalar_values[i][j] >= 10) ? GRAY : BLACK;
                DrawCircle(MARGIN + x, MARGIN + y, 5, color);
            }
        }

        for (auto& [value, gx, gy] : values) {
            float x = MARGIN + gx * CELL_SIZE;
            float y = MARGIN + gy * CELL_SIZE;

            switch (value) {
                case 0: case 15: break;
                case 1: case 14:
                    DrawLine(x, y + 0.5f * CELL_SIZE, x + 0.5f * CELL_SIZE, y, RED);
                    break;
                case 2: case 13:
                    DrawLine(x + 0.5f * CELL_SIZE, y, x + CELL_SIZE, y + 0.5f * CELL_SIZE, RED);
                    break;
                case 3: case 12:
                    DrawLine(x, y + 0.5f * CELL_SIZE, x + CELL_SIZE, y + 0.5f * CELL_SIZE, RED);
                    break;
                case 4: case 11:
                    DrawLine(x + CELL_SIZE, y + 0.5f * CELL_SIZE, x + 0.5f * CELL_SIZE, y + CELL_SIZE, RED);
                    break;
                case 5:
                    DrawLine(x, y + 0.5f * CELL_SIZE, x + 0.5f * CELL_SIZE, y, RED);
                    DrawLine(x + CELL_SIZE, y + 0.5f * CELL_SIZE, x + 0.5f * CELL_SIZE, y + CELL_SIZE, RED);
                    break;
                case 6: case 9:
                    DrawLine(x + 0.5f * CELL_SIZE, y, x + 0.5f * CELL_SIZE, y + CELL_SIZE, RED);
                    break;
                case 7: case 8:
                    DrawLine(x, y + 0.5f * CELL_SIZE, x + 0.5f * CELL_SIZE, y + CELL_SIZE, RED);
                    break;
                case 10:
                    DrawLine(x + 0.5f * CELL_SIZE, y, x + CELL_SIZE, y + 0.5f * CELL_SIZE, RED);
                    DrawLine(x, y + 0.5f * CELL_SIZE, x + 0.5f * CELL_SIZE, y + CELL_SIZE, RED);
                    break;
            }
        }

	if (!bottom_reached && frame_counter >= 5) {

	  position.x += CELL_SIZE;

	  if (position.x >= (grid_width - 1) * CELL_SIZE && position.y >= (grid_height - 1) * CELL_SIZE) {
	    bottom_reached = true;
	  }

	  int gx = static_cast<int>(position.x) / CELL_SIZE;
	  int gy = static_cast<int>(position.y) / CELL_SIZE;
    
	  if (gx < grid_width - 1 && gy < grid_height - 1) {
	    bool bl = scalar_values[gx][gy] >= 10;
	    bool br = scalar_values[gx + 1][gy] >= 10;
	    bool tr = scalar_values[gx + 1][gy + 1] >= 10;
	    bool tl = scalar_values[gx][gy + 1] >= 10;

	    int value = (bl << 0) | (br << 1) | (tr << 2) | (tl << 3);
	    values.emplace_back(value, gx, gy);
	  }

	  if (position.x >= (grid_width - 1) * CELL_SIZE) {
	    position.x = 0;
	    position.y += CELL_SIZE;

	    if (position.y >= (grid_height - 1) * CELL_SIZE) {
	      position.y = 0;
	    }
	  }

	  frame_counter = 0;
	}

        frame_counter++;
	DrawRectangle(position.x + MARGIN, position.y + MARGIN, CELL_SIZE, CELL_SIZE, BLUE);
	EndDrawing();
    }

    CloseWindow();
}
