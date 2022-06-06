#include "window/wrapper.hpp"

int main() {
	auto w = new Window("demo");
	w->start("assets");
	delete w;
}