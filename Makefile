SRC = $(wildcard src/*.c)

all:
	gcc -Iinclude -Llib $(SRC) -lmingw32 -lSDL2main -lSDL2 -lopengl32 -O3 -flto -o pv1000.exe

emcc:
	emcc -Iinclude -sUSE_SDL=2 $(SRC) -O2 -o website/emulator.js \
	-sNO_INVOKE_RUN \