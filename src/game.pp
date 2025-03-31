{$mode objfpc}{$H+}

program ray;
{$linklib libs/lib/raylib.dll}
procedure InitWindow(width, height: LongInt; title: PChar); cdecl; external;
function WindowShouldClose(): Boolean; cdecl; external;
procedure BeginDrawing(); cdecl; external;
procedure EndDrawing(); cdecl; external;
procedure CloseWindow(); cdecl; external;
begin
	InitWindow(800, 600, 'Test');
	while not WindowShouldClose() do
	begin
		BeginDrawing();
		EndDrawing();
	end;
	CloseWindow();
end.