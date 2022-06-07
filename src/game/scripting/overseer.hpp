#pragma once

#include <map>
#include <iostream>

#include "object.hpp"
// #include "../core.hpp"

using std::string;

namespace game {
class Game;
namespace scripting {

class Script;

class ScriptOverseer {
private:
    Game *game;
    std::map<string, Script*> macros;
    std::map<string, SObject*> vars;
    std::map<string, std::function<SObject*()>> immutableVars;
public:
    ScriptOverseer(Game * game);
    ~ScriptOverseer();
    Game * getGame();
    void set(std::string name, SObject * object);
    bool isSet(std::string name);
    SObject * parseArg(std::string word);
    SObject * getValueOf(SRaw * object);
    void addMacro(std::string name, Script * macro);
    Script * getMacro(std::string name);
};

}
}