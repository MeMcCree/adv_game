{$mode objfpc}{$H+}

program ray;
{$linklib c}
{$linklib m}
{$linklib raylib}
{$include raylib.inc}
var
    img: TImage;
    col: TColor;
    tex: TTexture2D;
begin
    InitWindow(800, 600, 'Test');
    SetWindowState(dWord(FLAG_WINDOW_RESIZABLE));
    img := LoadImage('icon.png');
    SetWindowIcon(img);
    tex := LoadTextureFromImage(img);
    while not WindowShouldClose() do
    begin
        if IsKeyDown(Integer(KEY_R)) then
        begin
            col := Color(255, 0, 255, 255);
        end
        else
        begin
            col := Color(0, 255, 0, 255);
        end;
        BeginDrawing();
            ClearBackground(Color(128, 128, 128, 255));
            //DrawRectangle(0, 0, 32, 32, col);
            DrawTexture(tex, 0, 0, Color(255, 255, 255, 255));
        EndDrawing();
    end;
    CloseWindow();
end.