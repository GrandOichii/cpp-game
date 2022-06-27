#pragma once

#include <string>
#include <iostream>

// #include "overseer.hpp"

namespace game {
namespace scripting {

class ScriptOverseer;

class SObject {
protected:
    std::string raw;
public:
    SObject(std::string raw);
    virtual std::string str() = 0;
    virtual int digit() = 0;
    virtual std::string getRaw();
    virtual SObject * copy() = 0;
    virtual ~SObject() = default;
};

class SInt : public SObject {
private:
    int value;
public:
    SInt(int value);
    SObject * copy();
    std::string str();
    int digit();
    int getValue();
    void setValue(int value);
};

class SString : public SObject {
protected:
    std::string value;
public:
    SString(const std::string value);
    SObject * copy();
    void add(std::string value);
    std::string str();
    int digit();
};

class SCustom : public SObject {
private:
    std::function<std::string(void)> strFunc;
    std::function<int(void)> digitFunc;
public:
    SCustom(std::string name, std::function<std::string(void)> strFunc, std::function<int(void)> digitFunc);
    SCustom(std::string name, std::function<std::string(void)> strFunc);
    SObject * copy();
    std::string str();
    int digit();
};

class SRaw : public SObject {
private:
    ScriptOverseer * so;
public:
    SRaw(std::string word, ScriptOverseer * so);
    SObject * copy();
    std::string get();
    SObject * getObject();
    std::string str();
    int digit();
};

}
}