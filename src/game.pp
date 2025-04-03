{$mode objfpc}{$H+}

program ray;
uses raylib;
type
    cols = (COL_R=0, COR_G, COL_B, COL_Y, COL_M, COL_CY);
    squaresIdx = 0..19;
var
    sqCols: Array[squaresIdx] of TColor;
    sqVecs: Array[squaresIdx] of TVector2;
    sqVels: Array[squaresIdx] of TVector2;
    prVec: TVector2;
    i: squaresIdx; 
const
    squareSize: Integer = 64;
    screenWidth: Integer = 800;
    screenHeight: Integer = 600;
    title: PChar = 'Example';
    squareVelMulX: Single = 15.0;
    squareVelMulY: Single = 32.0;
    g: Single = 0.7;

procedure ResetSquares();
var
    i: squaresIdx;
begin
    for i := Low(squaresIdx) to High(squaresIdx) do
    begin
        sqVecs[i].x := screenWidth / 2 - squareSize / 2;
        sqVecs[i].y := screenHeight / 2 - squareSize / 2;
        sqVels[i].x := (Random() - 0.5) * squareVelMulX;
        sqVels[i].y := (Random() - 0.5) * squareVelMulY;

        case Random(Integer(High(cols))) of
            0: sqCols[i] := Color(255, 0, 0, 255);
            1: sqCols[i] := Color(0, 255, 0, 255);
            2: sqCols[i] := Color(0, 0, 255, 255);
            3: sqCols[i] := Color(255, 255, 0, 255);
            4: sqCols[i] := Color(255, 0, 255, 255);
            5: sqCols[i] := Color(0, 255, 255, 255);
        end;
    end;
end;

begin
    InitWindow(screenWidth, screenHeight, title);

    SetTargetFPS(60);
    ResetSquares();
    while not WindowShouldClose() do
    begin
        if IsKeyPressed(Integer(KEY_R)) then
        begin
            ResetSquares();
        end;

        for i := Low(squaresIdx) to High(squaresIdx) do
        begin
            prVec := sqVecs[i];
            sqVels[i].y -= g;

            sqVecs[i].x += sqVels[i].x;
            sqVecs[i].y -= sqVels[i].y;

            if (sqVecs[i].x < 0) or (sqVecs[i].x > screenWidth - squareSize) then
            begin
                sqVecs[i] := prVec;
                sqVels[i] := Vector2Multiply(sqVels[i], Vector2(-1, 1));
            end;

            if (sqVecs[i].y < 0) or (sqVecs[i].y > screenHeight - squareSize) then
            begin
                sqVecs[i] := prVec;
                sqVels[i] := Vector2Multiply(sqVels[i], Vector2(1, -1));
            end;
        end;

        BeginDrawing();
            ClearBackground(Color(18, 18, 18, 255));
            for i := Low(squaresIdx) to High(squaresIdx) do
            begin
                DrawRectangle(Trunc(sqVecs[i].x), Trunc(sqVecs[i].y), squareSize, squareSize, sqCols[i]);
            end;
        EndDrawing();
    end;
    CloseWindow();
end.