#include "window/wrapper.hpp"

int main() {
	auto w = new Wrapper("demo", "assets");
	w->start();
	delete w;
}

// #include <iostream>
// #include "util.hpp"

// template<class T>
// void print(CircularBuffer<T>* b) {
// 	auto v = b->getV();
// 	for (const auto& e : v) std::cout << e << "|";
// 	std::cout << std::endl;
// }

// int main() {
// 	auto buffer = new CircularBuffer<std::string>(5);
// 	buffer->add("as");
// 	buffer->add("54");
// 	buffer->add("eee");
// 	buffer->add("1");
// 	buffer->add("2");
// 	print(buffer);
// 	buffer->add("89");
// 	print(buffer);
// 	delete buffer;
// }