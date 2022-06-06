#include "window/wrapper.hpp"

int main() {
	auto w = new Wrapper("demo", "assets");
	w->start();
	delete w;
}