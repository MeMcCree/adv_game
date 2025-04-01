{$mode objfpc}{$H+}

program ray;
{$linklib libs/lib/raylib.dll}
{$include raylib.inc}
var
	img: TImage;
	col: TColor;
begin
	InitWindow(800, 600, 'Test');
	SetWindowState(dWord(FLAG_WINDOW_RESIZABLE));
	img := LoadImage('icon.png');
	SetWindowIcon(img);
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
			ClearBackground(col);
		EndDrawing();
	end;
	CloseWindow();
end.