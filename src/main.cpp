#include "terminal/wrapper.hpp"

int main() {
	auto wrapper = new Wrapper("demo");
	wrapper->start();
	delete wrapper;
}