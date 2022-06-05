#pragma once

#include <string>

namespace game{
namespace scripting{

class SObject {
public:
    virtual std::string str() = 0;
};

class SInt : public SObject {
private:
    int value;
public:
    SInt(const int value);
    std::string str();
};

class SString : public SObject {
private:
    std::string value;
public:
    SString(const std::string value);
    std::string str();
};

}
}