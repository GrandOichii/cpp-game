#include "object.hpp"

#include "overseer.hpp"

namespace game {
namespace scripting {

SObject::SObject(std::string raw) : raw(raw) {

}

std::string SObject::getRaw() {
    return this->raw;
}

SInt::SInt(const int value) : SObject(std::to_string(value)), value(value) {

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

SString::SString(const std::string value) : SObject(value) {
    this->value = value.substr(1, value.size() - 2);
}

SObject * SString::copy() {
    return new SString("\"" + this->value + "\"");
}

std::string SString::str() { 
    return value; 
}

void SString::add(std::string value) {
    this->value += value;
}

SCustom::SCustom(std::string name, std::function<std::string(void)> strFunc) : SObject(name), strFunc(strFunc) {

}

SObject * SCustom::copy() {
    throw std::runtime_error("can't make copy of custom script object");
}

std::string SCustom::str() {
    return this->strFunc();
}

SRaw::SRaw(std::string word, ScriptOverseer * so) : SObject(word), so(so) {

}

SObject * SRaw::copy() {
    // TODO
    return this->getObject()->copy();
}

std::string SRaw::get() {
    return raw;
}

SObject * SRaw::getObject() {
    return this->so->getValueOf(this);
}

std::string SRaw::str() {
    return this->getObject()->str();;
}

}
}
