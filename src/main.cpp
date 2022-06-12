#include <iostream>
#include "window/wrapper.hpp"

// http://opengameart.org/content/dungeon-crawl-32x32-tiles

int main() {
	auto w = new Wrapper("demo", "assets");
	w->start();
	delete w;
	// try {
	// 	w->start();
	// 	delete w;
	// } catch (const std::exception& ex) {
	// 	delete w;
	// 	throw ex;
	// }
}