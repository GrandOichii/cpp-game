build:
	@g++ src/main.cpp -o bin/main -std=c++17

run: build
	@./bin/main

.DEFAULT_GOAL=run