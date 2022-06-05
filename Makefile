build:
	@g++ src/*.cpp src/game/map/*.cpp src/game/scripting/*.cpp -o bin/main -std=c++17

run: build
	@./bin/main

.DEFAULT_GOAL=run