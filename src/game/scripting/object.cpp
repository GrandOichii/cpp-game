#include "object.hpp"

#include "overseer.hpp"

namespace game {
namespace scripting {


SInt::SInt(const int value) : value(value) {

}

std::string SInt::str() { 
    return std::to_string(value); 
}

SObject * SInt::copy() {
    return new SInt(this->value);
}

int SInt::getValue() {
    return this->value;
}

SString::SString(const std::string value) : value(value) {

}

SObject * SString::copy() {
    return new SString(this->value);
}

std::string SString::str() { 
    return value; 
}

void SString::add(std::string value) {
    this->value += value;
}

SCustom::SCustom(std::function<std::string(void)> strFunc) : strFunc(strFunc) {

}

SObject * SCustom::copy() {
    throw std::runtime_error("can't make copy of custom script object");
}

std::string SCustom::str() {
    return this->strFunc();
}

SRaw::SRaw(std::string word, ScriptOverseer * so) : word(word), so(so) {

}

SObject * SRaw::copy() {
    throw std::runtime_error("can't make copy of raw object (don't know what " + this->word + " is)");
}

std::string SRaw::get() {
    return word;
}

SObject * SRaw::getObject() {
    return this->so->getValueOf(this);
}

std::string SRaw::str() {
    return this->getObject()->str();;
}

}
}

