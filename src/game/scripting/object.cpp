#include "object.hpp"

#include "overseer.hpp"

namespace game {
namespace scripting {

SObject::SObject(std::string raw) : raw(raw) {

}

std::string SObject::getRaw() {
    return this->raw;
}

SInt::SInt(int value) : SObject(std::to_string(value)), value(value) {
    // std::cout << getRaw() << "\t" << value << " set" << std::endl;
}

std::string SInt::str() {
    return std::to_string(value); 
}

int SInt::digit() {
    return value;
}

SObject * SInt::copy() {
    return new SInt(this->value);
}

int SInt::getValue() {
    return value;
}

void SInt::setValue(int value) {
    this->value = value;
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

int SString::digit() {
    throw std::runtime_error("cannot convert str to digit");
}

void SString::add(std::string value) {
    this->value += value;
}

SCustom::SCustom(std::string name, std::function<std::string(void)> strFunc, std::function<int(void)> digitFunc) : 
    SObject(name),
    strFunc(strFunc),
    digitFunc(digitFunc)
{}

SCustom::SCustom(std::string name, std::function<std::string(void)> strFunc) : 
    SObject(name),
    strFunc(strFunc),
    digitFunc([](){
        throw std::runtime_error("int function not defined");
        return 0;
    })
{}

SObject * SCustom::copy() {
    throw std::runtime_error("can't make copy of custom script object");
}

std::string SCustom::str() {
    return strFunc();
}

int SCustom::digit() {
    return digitFunc();
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

int SRaw::digit() {
    return this->getObject()->digit();
}

}
}
