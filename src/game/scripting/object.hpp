#pragma once

#include <string>
#include <iostream>

// #include "overseer.hpp"

namespace game {
namespace scripting {

class ScriptOverseer;

class SObject {
public:
    virtual std::string str() = 0;
    virtual SObject * copy() = 0;
    virtual ~SObject() = default;
};

class SInt : public SObject {
private:
    int value;
public:
    SInt(const int value);
    SObject * copy();
    std::string str();
    int getValue();
};

class SString : public SObject {
private:
    std::string value;
public:
    SString(const std::string value);
    SObject * copy();
    void add(std::string value);
    std::string str();
};

class SCustom : public SObject {
private:
    std::function<std::string(void)> strFunc;
public:
    SCustom(std::function<std::string(void)> strFunc);
    SObject * copy();
    std::string str();
};

class SRaw : public SObject {
private:
    std::string word;
    ScriptOverseer * so;
public:
    SRaw(std::string word, ScriptOverseer * so);
    SObject * copy();
    std::string get();
    SObject * getObject();
    std::string str();
};

}
}