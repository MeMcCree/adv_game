#include "raylib.h"
#include "raymath.h"
#include "stdlib.h"
#include "stdio.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "math.h"

const int TILE_SIZE = 32;
const float cam_speed = 4.0f;

typedef enum {
  TILES_AIR = 0,
  TILES_GROUND,
  TILES_SIZE
} tile_e;

int read_level(char* filepath, tile_e* buffer, int* sizex, int* sizey) {
  FILE* fd = fopen(filepath, "r");

  if (!fd) return 1;

  if (!fread(sizex, sizeof(int), 1, fd)) return 1;
  if (!fread(sizey, sizeof(int), 1, fd)) return 1;

  fread((void*)buffer, sizeof(tile_e) * *sizey * *sizex, 1, fd);

  return 0;
}


int write_level(char* filepath, tile_e* buffer, int sizex, int sizey) {
  FILE* fd = fopen(filepath, "w");

  if (!fd) return 1;

  if (!fwrite((void*)&sizex, sizeof(int), 1, fd)) return 1;
  if (!fwrite((void*)&sizey, sizeof(int), 1, fd)) return 1;

  if (!fwrite((void*)buffer, sizeof(tile_e) * sizey * sizex, 1, fd)) return 1;

  return 0;
}

void create_level(int sizex, int sizey, void* buffer) {
  buffer = malloc(sizeof(tile_e) * sizey * sizex);
  memset((void*)buffer, 0, sizeof(tile_e) * sizey * sizex);
}

#define LEVEL_SIZE 256*128
tile_e level[2][LEVEL_SIZE] = {0};

double pulse(double offset) {
  double alpha = sin(GetTime() + offset);
  alpha = alpha < 0 ? -alpha : alpha;
  alpha = alpha < 0.15 ? 0.15 : alpha;
  alpha = alpha * 0.5 * 255;
  return alpha;
}

double min(double a, double b) {
  return a < b ? a : b;
}

double max(double a, double b) {
  return a > b ? a : b;
}

int main(void) {
  const int screen_width = 768;
  const int screen_height = 512;
  const int screen_tiles_x = screen_width / TILE_SIZE;

  InitWindow(screen_width, screen_height, "");
  SetTargetFPS(60);

  int sizex = 40;
  int sizey = 20;

  for (int x = 0; x < sizex; x++) {
    level[1][x] = 1;
  }

  char buffer[128];

  Camera2D main_camera;

  Image tiles_img = LoadImage("tiles.png");
  int tile_amount_x = tiles_img.width / TILE_SIZE;
  int tile_amount_y = tiles_img.height / TILE_SIZE;
  Texture2D tiles_tex = LoadTextureFromImage(tiles_img);

  Vector2 move;
  Rectangle draw_region;
  while (!WindowShouldClose()) {

    BeginDrawing();
      ClearBackground(BLACK);
      BeginMode2D(main_camera);

      // Draw tiles
      // Bg layer
      for (int y = 0; y < sizey; y++) {
        for (int x = 0; x < sizex; x++) {
          if (level[0][y*sizex + x]) {
            draw_region.x = level[0][y*sizex + x] % tile_amount_x * TILE_SIZE;
            draw_region.y = level[0][y*sizex + x] / tile_amount_x * TILE_SIZE;
            draw_region.width = TILE_SIZE;
            draw_region.height = TILE_SIZE;
            DrawTextureRec(tiles_tex, draw_region, (Vector2){x*TILE_SIZE, y*-TILE_SIZE}, WHITE);
          }
        }
      }
      // Main layer
      for (int y = 0; y < sizey; y++) {
        for (int x = 0; x < sizex; x++) {
          if (level[1][y*sizex + x]) {
            draw_region.x = level[1][y*sizex + x] % tile_amount_x * TILE_SIZE;
            draw_region.y = level[1][y*sizex + x] / tile_amount_x * TILE_SIZE;
            draw_region.width = TILE_SIZE;
            draw_region.height = TILE_SIZE;
            DrawTextureRec(tiles_tex, draw_region, (Vector2){x*TILE_SIZE, y*-TILE_SIZE}, WHITE);
          }
        }
      }

      EndMode2D();
      
      // HUD
      snprintf(buffer, sizeof(buffer), "Speed: %.0f%% | Layer: %d", editor.mul * 100.0f, editor.draw_layer);
      DrawText(buffer, 16, screen_height - 16, 16, LIGHTGRAY);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
