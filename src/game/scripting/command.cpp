#include "command.hpp"

#include <iostream>
#include <map>
#include <vector>

#include "overseer.hpp"
#include "script.hpp"
#include "../core.hpp"
#include "../../util.hpp"

using std::string;
using std::vector;

namespace game {
namespace scripting {

static vector<string> parseLine(string line) {
    vector<string> result;
    auto sb = false;
    string l = "";
    auto words = str::split(line, " ");
    for (auto word : words) {
        auto first = word[0];
        auto last = word[word.size() - 1];
        if (first == '"') sb = true;
        if (sb) {
            l += " " + word;
            if (last == '"') {
                result.push_back(l.substr(1));
                l = "";
                sb = false;
            }
        } else {
            result.push_back(word);
        }
    }
    if (l != "") throw std::runtime_error("parsing error: no closing string quote");
    // for (const auto& e : result) std::cout << e << "|";
    // std::cout << std::endl;
    return result;
}

const std::map<string, std::function<bool(vector<string>, ScriptOverseer*)>> IF_EVALS = {
    {"set", [](vector<string> argv, ScriptOverseer* so) {
        // std::cout << "Checking whether " << argv[1] << " is set" << std::endl;
        return so->isSet(argv[1]);
    } },
    {"=", [](vector<string> argv, ScriptOverseer* so) {
        // TODO separate string and ints
        return so->parseArg(argv[0])->str() == so->parseArg(argv[2])->str();
    } },
    {"has_item", [](vector<string> argv, ScriptOverseer* so) {
        auto name = so->parseArg(argv[1])->str();
        auto count = so->getGame()->getPlayer()->getInventory()->count(name);
        return count > 0;
    } }
};

const std::map<string, func> FUNC_MAP = {
    { "print", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 1) throw std::runtime_error("bad argument count for <print>");
        std::cout << args[0]->str() << std::endl;
    } },
    { "log", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 1) throw std::runtime_error("bad argument count for <log>");
        so->getGame()->addToLog(args[0]->str());
    } },
    { "mb", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 2) throw std::runtime_error("bad argument count for <mb>");
        auto text = args[0]->str();
        auto choices = args[1]->str();
        std::string res = so->getGame()->requestChoice(text, choices);
        so->set("_mbresult", new SString("\"" + res + "\""));
    } },
    { "set", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 2) throw std::runtime_error("bad argument count for <set>");
        auto varName = ((SRaw*)args[0])->get();
        auto copy = args[1]->copy();
        so->set(varName, copy);
    } },
    { "sadd", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 2) throw std::runtime_error("bad argument count for <sadd>");
        auto var = (SString*)((SRaw*)args[0])->getObject();
        auto value = args[1]->str();
        var->add(value);
    } },
    { "tset", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 3) throw std::runtime_error("bad argument count for <tset>");
        auto y = ((SInt*)args[0])->getValue();
        auto x = ((SInt*)args[1])->getValue();
        auto tileName = ((SRaw*)args[2])->get();        
        auto split = str::split(tileName, ":");
        if (split.size() != 2) throw std::runtime_error("can't set tile " + tileName);
        auto roomName = split[0];
        tileName = split[1];
        auto roomMap = so->getGame()->getMap()->getRoomMap();
        auto it = roomMap.find(roomName);
        if (it == roomMap.end()) throw std::runtime_error("no room with name " + roomName);
        auto room = it->second;
        auto tileSetSize = room->getTileSetSize();
        auto tileSet = room->getTileSet();
        map::Tile* tile = nullptr;
        for (int i = 0; i < tileSetSize; i++) {
            if (tileSet[i]->getName() == tileName) {
                tile = tileSet[i];
                break;
            }
        }
        if (tile == nullptr) throw std::runtime_error("not tile with name " + tileName + " in room " + roomName);
        room->getLayout()[y][x] = tile;
    } },
    { "sleep", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 1) throw std::runtime_error("bad argument count for <sleep>");
        auto amount = ((SInt*)args[0])->getValue();
        so->getGame()->sleep(amount);
    } },
    { "run", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 1) throw std::runtime_error("bad argument count for <run>");
        auto macroName = ((SRaw*)args[0])->get();
        auto macro = so->getMacro(macroName);
        macro->exec();
    } },
    { "opencontainer", [](ScriptOverseer* so, SObject** args, int argc) {
        // std::cout << "Opening container" << std::endl;
        if (argc != 2) throw std::runtime_error("bad argument count for <opencontainer>");
        auto key = ((SRaw*)args[0])->get();
        auto top = args[1]->str();
        auto formatted = formatContainer(key);
        if (so->isSet(formatted) && ((SInt*)so->get(formatted))->getValue()) {
            so->getGame()->requestChoice(top + " is already looted.", "Ok");
            return;
        }
        auto cm = so->getGame()->getContainerManager();
        auto container = cm->get(key);
        auto looted = so->getGame()->accessContainer(container, top);
        so->set(formatted, new SInt(looted ? 1 : 0));
        if (!looted) return;
        auto player = so->getGame()->getPlayer();
        player->getInventory()->addAll(container->getPairs());
    } },
    { "if", [](ScriptOverseer* so, SObject** args, int argc) {
        vector<string> argv;
        for (int i = 0; i < argc; i++) {
            argv.push_back(args[i]->getRaw());
        }
        auto thenI = -1;
        for (int i = 0; i < argc; i++) {
            auto arg = argv[i];
            if (arg == "then") {
                thenI = i;
                break;
            }
        }
        if (thenI == -1) throw std::runtime_error("no then in if statement");
        auto reverse = false;
        auto doIf = false;
        vector<string> ifArgs = {argv.begin(), argv.begin() + thenI};
        vector<string> actionArgs = {argv.begin()+thenI+1, argv.end()};
        if (ifArgs[0] == "not") {
            reverse = true;
            ifArgs = {ifArgs.begin()+1, ifArgs.end()};
        }
        string ifOp = "";
        for (auto it = IF_EVALS.begin(); it != IF_EVALS.end(); it++) {
            if (it->first == ifArgs[1]) {
                ifOp = it->first;
                break;
            }
        }
        if (ifOp == "") {
            vector<string> exceptions{"set", "has_item"};
            auto ifArgFirst = ifArgs[0];
            if (std::find(exceptions.begin(), exceptions.end(), ifArgFirst) != exceptions.end()) {
                ifOp = ifArgFirst;
            } else throw std::runtime_error("unknown evaluator " + ifArgFirst);
        }
        auto it = IF_EVALS.find(ifOp);
        if (it == IF_EVALS.end()) throw std::runtime_error("unknown evaluator " + ifOp);
        doIf = it->second(ifArgs, so);
        doIf = (doIf || reverse) && !(doIf && reverse);
        if (doIf) {
            auto j = str::join(actionArgs.begin(), actionArgs.end(), " ");
            auto c = new Command(j, so);
            c->exec();
            delete c;
        }
    } },
    { "warp", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 1) throw std::runtime_error("bad argument count for <warp>");
        auto code = ((SRaw*)args[0])->get();
        so->getGame()->useWarpCode(code);
    } },
    { "give", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 1) throw std::runtime_error("bad argument count for <give>");
        auto name = args[0]->str();
        auto game = so->getGame();
        auto item = game->getItemManager()->getItem(name);
        auto inventory = game->getPlayer()->getInventory();
        inventory->add(item);
    } },
    { "take", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 1) throw std::runtime_error("bad argument count for <take>");
        auto name = args[0]->str();
        auto game = so->getGame();
        auto itemManager = game->getItemManager();
        auto item = itemManager->getItem(name);
        game->getPlayer()->getInventory()->take(item);
    } },
};

Command::Command(string line, ScriptOverseer* so) : so(so) {
    auto words = parseLine(line);
    auto funcName = words[0];
    this->argc = words.size() - 1;
    this->args = new SObject*[this->argc];
    for (auto i = 1; i < this->argc + 1; i++) {
        this->args[i-1] = so->parseArg(words[i]);
    }
    // if (words[0] == "set") {
    //     for (int i = 0; i < this->argc; i++) std::cout << words[i+1] << "|";
    //     std::cout << std::endl;
    // }
    auto it = FUNC_MAP.find(funcName);
    if (it == FUNC_MAP.end()) throw std::runtime_error("no function " + funcName);
    this->f = it->second;
}

Command::~Command() {
    for (int i = 0; i < argc; i++) delete args[i];
    delete[] args;
}

void Command::exec() {
    this->f(this->so, this->args, this->argc);
}

}
}
