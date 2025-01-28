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

#define LEVEL_SIZE 256*128
tile_e level[2][LEVEL_SIZE] = {0};

int read_level(char* filepath, int* sizex, int* sizey) {
  FILE* fd = fopen(filepath, "r");

  if (!fd) return 1;
  printf("[INFO] (read_level) Opened file successfuly\n");

  if (!fread(sizex, sizeof(int), 1, fd)) return 1;
  printf("[INFO] (read_level) Read sizex successfuly\n");
  if (!fread(sizey, sizeof(int), 1, fd)) return 1;
  printf("[INFO] (read_level) Read sizey successfuly\n");

  fread((void*)level[0], sizeof(tile_e) * (*sizey) * (*sizex), 1, fd);
  printf("[INFO] (read_level) Read first layer successfuly\n");
  fread((void*)level[1], sizeof(tile_e) * (*sizey) * (*sizex), 1, fd);
  printf("[INFO] (read_level) Read second layer successfuly\n");

  return 0;
}

int write_level(char* filepath, int sizex, int sizey) {
  FILE* fd = fopen(filepath, "wb");

  if (!fd) return 1;
  printf("[INFO] (write_level) Opened file successfuly\n");

  if (!fwrite(&sizex, sizeof(int), 1, fd)) return 1;
  printf("[INFO] (write_level) Wrote sizex successfuly\n");
  if (!fwrite(&sizey, sizeof(int), 1, fd)) return 1;
  printf("[INFO] (write_level) Wrote sizey successfuly\n");

  if (!fwrite(level[0], sizeof(tile_e), sizey * sizex, fd)) return 1;
  printf("[INFO] (write_level) Wrote first layer successfuly\n");
  if (!fwrite(level[1], sizeof(tile_e), sizey * sizex, fd)) return 1;
  printf("[INFO] (write_level) Wrote second layer successfuly\n");

  return 0;
}


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
  
  Vector2 cursor;
  
  Vector2 start_select;

  float mul;

  int draw_tile;
  int draw_layer;

  bool tile_selection;
  bool is_selecting;
  bool selection;

  bool io_confirm;
} editor_t;

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
  //read_level("level0.bin", level, &sizex, &sizey);

  editor_t editor = {0};
  editor.main_camera.target = (Vector2){0, 0};
  editor.main_camera.rotation = 0.0f;
  editor.main_camera.zoom = 1.0f;
  editor.mul = 1.0f;
  editor.draw_tile = 1;
  editor.draw_layer = 1;
  editor.io_confirm = false;

  char buffer[128];

  Image tiles_img = LoadImage("tiles.png");
  int tile_amount_x = tiles_img.width / TILE_SIZE;
  int tile_amount_y = tiles_img.height / TILE_SIZE;
  Texture2D tiles_tex = LoadTextureFromImage(tiles_img);

  Vector2 move;
  Rectangle draw_region;
  while (!WindowShouldClose()) {
    // Open tile selection
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
      editor.tile_selection = !editor.tile_selection;
    }

    if (editor.tile_selection) {
      float mousex = GetMouseX();
      float mousey = GetMouseY();
      int tilex = floor(mousex / TILE_SIZE);
      int tiley = floor(mousey / TILE_SIZE);

      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        editor.draw_tile = tiley*screen_tiles_x + tilex;
        editor.tile_selection = false;
        continue;
      }

      BeginDrawing();
      ClearBackground(BLACK);
        for (int i = 0; i < tile_amount_x * tile_amount_y; i++) {
          draw_region.x = i % tile_amount_x * TILE_SIZE;
          draw_region.y = i / tile_amount_x * TILE_SIZE;
          draw_region.width = TILE_SIZE;
          draw_region.height = TILE_SIZE;
          DrawTextureRec(tiles_tex, draw_region, (Vector2){(i % screen_tiles_x)*TILE_SIZE, (i / screen_tiles_x)*TILE_SIZE}, WHITE);
        }
        DrawRectangle(tilex * TILE_SIZE, tiley * TILE_SIZE, TILE_SIZE, TILE_SIZE, (Color){255, 255, 255, pulse(0)});
      EndDrawing();
      continue;
    }

    // Camera move
    move = (Vector2){0, 0};

    if      (IsKeyDown(KEY_D)) move.x = 1;
    else if (IsKeyDown(KEY_A)) move.x = -1;
    if (IsKeyDown(KEY_S))      move.y = 1;
    else if (IsKeyDown(KEY_W)) move.y = -1;

    move = Vector2Scale(Vector2Normalize(move), cam_speed * editor.mul);
    editor.main_camera.target.x += move.x;
    editor.main_camera.target.y += move.y;

    // Cursor move
    if      (IsKeyPressed(KEY_RIGHT)) editor.cursor.x++;
    else if (IsKeyPressed(KEY_LEFT))  editor.cursor.x--;
    if      (IsKeyPressed(KEY_DOWN))  editor.cursor.y--;
    else if (IsKeyPressed(KEY_UP))    editor.cursor.y++;

    float mousex = GetMouseX() + editor.main_camera.target.x;
    float mousey = GetMouseY() + editor.main_camera.target.y;
    int tilex = floor(mousex / TILE_SIZE);
    int tiley = -floor(mousey / TILE_SIZE);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      editor.cursor.x = tilex;
      editor.cursor.y = tiley;
    }

    editor.cursor.x = editor.cursor.x < 0 ? 0 : editor.cursor.x;
    editor.cursor.x = editor.cursor.x > sizex - 1 ? sizex - 1 : editor.cursor.x;

    editor.cursor.y = editor.cursor.y < 0 ? 0 : editor.cursor.y;
    editor.cursor.y = editor.cursor.y > sizey - 1 ? sizey - 1 : editor.cursor.y;

    // Selection
    if (IsKeyPressed(KEY_V)) {
      if (editor.is_selecting) {
        editor.is_selecting = false;
      } else {
        editor.is_selecting = true;
        editor.start_select = editor.cursor;
      }
    }

    if (IsKeyPressed(KEY_Q)) {
      editor.draw_tile = level[editor.draw_layer][(int)editor.cursor.y*sizex + (int)editor.cursor.x];
    }

    // Switching layer
    if (IsKeyPressed(KEY_C)) {
      editor.draw_layer = !editor.draw_layer;
    }

     // Fill selection
    if (IsKeyPressed(KEY_F)) {
      if (editor.is_selecting) {
        Vector2 mins, maxs;
        mins.x = min(editor.start_select.x, editor.cursor.x);
        mins.y = min(editor.start_select.y, editor.cursor.y);
        maxs.x = max(editor.start_select.x, editor.cursor.x);
        maxs.y = max(editor.start_select.y, editor.cursor.y);
        for (int y = mins.y; y <= maxs.y; y++) {
          for (int x = mins.x; x <= maxs.x; x++) {
            level[editor.draw_layer][y*sizex + x] = editor.draw_tile;
          }
        }
      } else {
        level[editor.draw_layer][(int)editor.cursor.y*sizex + (int)editor.cursor.x] = editor.draw_tile;
      }
    }

    if (IsKeyPressed(KEY_KP_ADD)) {
      editor.draw_tile = (editor.draw_tile + 1) % (tile_amount_x * tile_amount_y);
    }

    if (IsKeyPressed(KEY_KP_SUBTRACT)) {
      editor.draw_tile = editor.draw_tile - 1;
      editor.draw_tile = editor.draw_tile < 0 ? 0 : editor.draw_tile;
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
      editor.io_confirm = false;
    }

    if (IsKeyPressed(KEY_COMMA)) {
      if (!editor.io_confirm) {
        editor.io_confirm = true;
      } else {
        write_level("level0.bin", sizex, sizey);
        editor.io_confirm = false;
      }
    }

    if (IsKeyPressed(KEY_PERIOD)) {
      if (!editor.io_confirm) {
        editor.io_confirm = true;
      } else {
        read_level("level0.bin", &sizex, &sizey);
        editor.io_confirm = false;
      }
    }

    // Change speed
    if (GetMouseWheelMove() < 0) {
      editor.mul -= 0.1f;
      editor.mul = editor.mul >= 0.1f ? editor.mul : 0.1f;
    } else if (GetMouseWheelMove() > 0) {
      editor.mul += 0.1f;
      editor.mul = editor.mul <= 8.0f ? editor.mul : 8.0f;
    }

    BeginDrawing();
      ClearBackground(BLACK);
      BeginMode2D(editor.main_camera);

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


      // Draw selection
      if (editor.is_selecting) {
        Vector2 mins, maxs;
        mins.x = min(editor.start_select.x, editor.cursor.x);
        mins.y = min(editor.start_select.y, editor.cursor.y);
        maxs.x = max(editor.start_select.x, editor.cursor.x);
        maxs.y = max(editor.start_select.y, editor.cursor.y);
        DrawRectangle(mins.x * TILE_SIZE, maxs.y * -TILE_SIZE, (maxs.x - mins.x + 1) * TILE_SIZE, (maxs.y - mins.y + 1) * TILE_SIZE, (Color){255, 255, 255, pulse(0.1)});
      }

      // Draw cursor
      DrawRectangle(editor.cursor.x * TILE_SIZE, editor.cursor.y * -TILE_SIZE, TILE_SIZE, TILE_SIZE, (Color){255, 255, 255, pulse(0)});

      EndMode2D();
      
      // HUD
      snprintf(buffer, sizeof(buffer), "Speed: %.0f%% | Layer: %d", editor.mul * 100.0f, editor.draw_layer);
      DrawText(buffer, 16, screen_height - 16, 16, LIGHTGRAY);

      draw_region.x = editor.draw_tile % tile_amount_x * TILE_SIZE;
      draw_region.y = editor.draw_tile / tile_amount_x * TILE_SIZE;
      draw_region.width = TILE_SIZE;
      draw_region.height = TILE_SIZE;
      DrawTextureRec(tiles_tex, draw_region, (Vector2){16, screen_height - 2*TILE_SIZE}, WHITE);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
