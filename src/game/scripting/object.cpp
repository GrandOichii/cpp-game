#include "object.hpp"

game::scripting::SInt::SInt(const int value) : value(value) {

}

std::string game::scripting::SInt::str() { 
    return std::to_string(value); 
}

game::scripting::SString::SString(const std::string value) : value(value) {

}

std::string game::scripting::SString::str() { 
    return value; 
}
