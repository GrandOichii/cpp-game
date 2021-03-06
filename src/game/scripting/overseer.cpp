#include "overseer.hpp"

#include "object.hpp"
#include "script.hpp"
#include "../core.hpp"
#include "../player/player.hpp"
#include "../../util.hpp"

namespace game {
namespace scripting {


ScriptOverseer::ScriptOverseer(Game * game) : game(game) {
}

ScriptOverseer::~ScriptOverseer() {
    for (auto it = vars.begin(); it != vars.end(); it++) delete it->second;
    for (auto it = macros.begin(); it != macros.end(); it++) delete it->second;
}

Game * ScriptOverseer::getGame() {
    return this->game;
}

SObject * ScriptOverseer::get(std::string key) {
    auto it1 = vars.find(key);
    if (it1 != vars.end()) return it1->second;
    auto it2 = immutableVars.find(key);
    if (it2 != immutableVars.end()) return it2->second;
    throw std::runtime_error("don't know what " + key + " is");
}

void ScriptOverseer::set(std::string name, SObject * object) {
    auto it = this->vars.find(name);
    if (it != this->vars.end()) delete it->second;
    this->vars[name] = object;
}

bool ScriptOverseer::isSet(std::string name) {
    // return false;
    return this->vars.find(name) != this->vars.end();
}

SObject * ScriptOverseer::parseArg(std::string word) {
    if (word[0] == '"' && word[word.size()-1] == '"') {
        return new SString(word);
    }
    if (isNumber(word)) {
        return new SInt(std::atoi(word.c_str()));
    }
    // failed to find anything that matches
    return new SRaw(word, this);
}

SObject * ScriptOverseer::getValueOf(SRaw * object) {
    auto name = object->get();
    auto it1 = this->vars.find(name);
    if (it1 != this->vars.end()) {
        return it1->second;
    }    
    auto it2 = this->immutableVars.find(name);
    if (it2 != this->immutableVars.end()) {
        return it2->second;
    }
    // check if is warp code
    throw std::runtime_error("overseer doesn't know what " + name + " is");
}

void ScriptOverseer::addMacro(std::string name, Script * macro) {
    auto it = this->macros.find(name);
    if (it != this->macros.end()) throw std::runtime_error("redefining macro " + name);
    this->macros[name] = macro;
}

Script * ScriptOverseer::getMacro(std::string name) {
    auto it = this->macros.find(name);
    if (it == this->macros.end()) throw std::runtime_error("unknown macro: " + name);
    return it->second;
}

void ScriptOverseer::bindPlayer(player::Player * player) {
    this->immutableVars["player.name"] = new SCustom("player.name", [player]() {
        return player->getName();
    });
    this->immutableVars["player.class"] = new SCustom("player.class", [player]() {
        return player->getClassName();
    });
    this->immutableVars["player.currency"] = new SCustom("player.currency", [player]() {
        return std::to_string(player->getCurrency());
    },
    [player](){
        return player->getCurrency();
    });
    this->immutableVars["player.chealth"] = new SCustom("player.chealth", 
    [player]() {
        return std::to_string(player->getCurrentHealth());
    }, 
    [player]() {
        return player->getCurrentHealth();
    });
    this->immutableVars["player.mhealth"] = new SCustom("player.mhealth", 
    [player]() {
        return std::to_string(player->getMaxHealth());
    }, 
    [player]() {
        return player->getMaxHealth();
    });
    this->immutableVars["player.cmana"] = new SCustom("player.cmana", 
    [player]() {
        return std::to_string(player->getCurrentMana());
    }, 
    [player]() {
        return player->getCurrentMana();
    });
    this->immutableVars["player.mmana"] = new SCustom("player.mmana", 
    [player]() {
        return std::to_string(player->getMaxMana());
    }, 
    [player]() {
        return player->getMaxMana();
    });
}

// void ScriptOverseer::addContainerKey(std::string key) {
//     this->set(formatContainer(key), new SInt(0));
// }


}
}
