@REM criação da pasta build e suas subpastas

md .\build\deps .\build\bin .\build\logs

@REM compilação da libs externas

@REM compila glad como lib: -> gl.o
@REM obs: alterei a importação no gl.c [<glad/gl.h>] para ["gl.h"]
g++ -c -Iextern\glad\include\glad .\extern\glad\src\gl.c -o build\deps\gl.o

@REM compila glm\glm como lib: -> libglm.o
g++ -c -Iextern\glm .\extern\glm\glm\detail\glm.cpp -o build\deps\libglm.o

@REM copia a lib statica do glfw
copy .\extern\glfw\lib-static-ucrt\glfw3.dll build\bin\

@REM compilação do source
g++ -Wall -c .\src\main.cpp -Iextern\glfw\include -Iextern\glm -Iinclude -Iextern\glad\include -Iextern\stb\include -o build\deps\cube_test.o
g++ -fPIC .\build\deps\cube_test.o .\build\deps\libglm.o .\build\deps\gl.o -Lextern\glfw\lib-static-ucrt -lglfw3dll -o build\bin\cube_test

echo "Compilação finalizada"