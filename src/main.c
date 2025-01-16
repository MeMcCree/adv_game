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
tile_e level[LEVEL_SIZE] = {0};

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

typedef struct {
  Camera2D main_camera;
  Camera2D tile_camera;
  
  Vector2 cursor;
  
  Vector2 start_select;
  Vector2 end_select;

  float scroll;
  float mul;

  bool tile_selection;
  bool is_selecting;
  bool selection;
} editor_t;

int main(void) {
  const int screen_width = 800;
  const int screen_height = 600;

  InitWindow(screen_width, screen_height, "");
  SetTargetFPS(60);

  int sizex = 15;
  int sizey = 10;
  create_level(sizex, sizey, level);

  for (int x = 0; x < sizex; x++) {
    level[x] = 1;
  }

  editor_t editor = {0};
  editor.main_camera.target = (Vector2){0, 0};
  editor.main_camera.rotation = 0.0f;
  editor.main_camera.zoom = 1.0f;
  editor.mul = 1.0f;

  char buffer[128];

  Vector2 move;
  while (!WindowShouldClose()) {
    move = (Vector2){0, 0};
    if      (IsKeyDown(KEY_D)) move.x = 1;
    else if (IsKeyDown(KEY_A)) move.x = -1;
    if (IsKeyDown(KEY_S))      move.y = 1;
    else if (IsKeyDown(KEY_W)) move.y = -1;

    if      (IsKeyPressed(KEY_RIGHT)) editor.cursor.x++;
    else if (IsKeyPressed(KEY_LEFT))  editor.cursor.x--;
    if      (IsKeyPressed(KEY_DOWN))  editor.cursor.y--;
    else if (IsKeyPressed(KEY_UP))    editor.cursor.y++;

    if (IsKeyPressed(KEY_V)) {
      if (editor.is_selecting) {
        editor.is_selecting = false;
        editor.end_select = editor.cursor;
        editor.selection = true;
      } else {
        editor.is_selecting = true;
        editor.start_select = editor.cursor;
      }
    }

    editor.cursor.x = editor.cursor.x < 0 ? 0 : editor.cursor.x;
    editor.cursor.x = editor.cursor.x > sizex - 1 ? sizex - 1 : editor.cursor.x;

    editor.cursor.y = editor.cursor.y < 0 ? 0 : editor.cursor.y;
    editor.cursor.y = editor.cursor.y > sizey - 1 ? sizey - 1 : editor.cursor.y;

    if (GetMouseWheelMove() < 0) {
      editor.mul -= 0.1f;
      editor.mul = editor.mul >= 0.1f ? editor.mul : 0.1f;
    } else if (GetMouseWheelMove() > 0) {
      editor.mul += 0.1f;
      editor.mul = editor.mul <= 8.0f ? editor.mul : 8.0f;
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
      editor.tile_selection = !editor.tile_selection;
    }

    move = Vector2Scale(Vector2Normalize(move), cam_speed * editor.mul);

    editor.main_camera.target.x += move.x;
    editor.main_camera.target.y += move.y;

    BeginDrawing();
      ClearBackground(BLACK);
      BeginMode2D(editor.main_camera);
      for (int y = 0; y < sizey; y++) {
        for (int x = 0; x < sizex; x++) {
          if (level[y*sizex + x]) DrawRectangle(x * TILE_SIZE, y * -TILE_SIZE, TILE_SIZE, TILE_SIZE, RED);
        }
      }
      if (editor.is_selecting) {
        Vector2 mins, maxs;
        mins.x = min(editor.start_select.x, editor.cursor.x);
        mins.y = min(editor.start_select.y, editor.cursor.y);
        maxs.x = max(editor.start_select.x, editor.cursor.x);
        maxs.y = max(editor.start_select.y, editor.cursor.y);
        DrawRectangle(mins.x * TILE_SIZE, maxs.y * -TILE_SIZE, (maxs.x - mins.x + 1) * TILE_SIZE, (maxs.y - mins.y + 1) * TILE_SIZE, (Color){255, 255, 255, pulse(0.1)});
      } else {
        if (editor.selection) {
          Vector2 mins, maxs;
          mins.x = min(editor.start_select.x, editor.end_select.x);
          mins.y = min(editor.start_select.y, editor.end_select.y);
          maxs.x = max(editor.start_select.x, editor.end_select.x);
          maxs.y = max(editor.start_select.y, editor.end_select.y);
          DrawRectangle(mins.x * TILE_SIZE, maxs.y * -TILE_SIZE, (maxs.x - mins.x + 1) * TILE_SIZE, (maxs.y - mins.y + 1) * TILE_SIZE, (Color){255, 255, 255, pulse(0.1)});
        }
      }
      DrawRectangle(editor.cursor.x * TILE_SIZE, editor.cursor.y * -TILE_SIZE, TILE_SIZE, TILE_SIZE, (Color){255, 255, 255, pulse(0)});
      EndMode2D();
      snprintf(buffer, sizeof(buffer), "Speed: %.0f%%", editor.mul * 100.0f);
      DrawText(buffer, 16, screen_height - 16, 16, LIGHTGRAY);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
