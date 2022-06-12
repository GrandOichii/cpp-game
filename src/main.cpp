#include <iostream>
#include "window/wrapper.hpp"

int main() {
	auto w = new Wrapper("demo", "assets");
	try {
		w->start();
		delete w;
	} catch (std::exception ex) {
		delete w;
		throw ex;
	}
}