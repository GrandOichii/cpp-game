#include "game/core.hpp"

int main() {
	auto game = new game::Game("demo");
	game->print();
	delete game;
}