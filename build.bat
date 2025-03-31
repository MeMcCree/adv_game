::gcc -fshort-enums -Wall -I./libs/include -o bin/editor.exe src/editor.c -L./libs/lib -lraylib -lopengl32 -lgdi32 -lwinmm -lm
fpc -Px86_64 src/game.pp -o"bin/game.exe"