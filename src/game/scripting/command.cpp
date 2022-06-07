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
    return result;
}

const std::map<string, std::function<bool(vector<string>, ScriptOverseer*)>> IF_EVALS = {
    {"set", [](vector<string> argv, ScriptOverseer* so) {
        // for (auto e : argv) std::cout << e << " ";
        // std::cout << std::endl;
        return so->isSet(argv[1]);
    } }
};

const std::map<string, func> FUNC_MAP = {
    { "print", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 1) throw std::runtime_error("bad argument count for <print>");
        std::cout << args[0]->str() << std::endl;
    } },
    { "log", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 1) throw std::runtime_error("bad argument count for <log>");
        std::cout << "Logging: " << args[0]->str() << std::endl;
        // throw std::runtime_error("log not implemented");
    } },
    { "mb", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 2) throw std::runtime_error("bad argument count for <mb>");
        auto text = args[0]->str();
        auto choices = args[1]->str();
        std::cout << "Requesting player choice (text: " << text << ") (choices: " << choices << ")\n";
        // TODO set _mbresult
        throw std::runtime_error("mb not implemented");
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
        std::cout << "Setting tile at (" << y << "; " << x << ") to: " << tileName << std::endl;
        throw std::runtime_error("tset not implemented");
    } },
    { "sleep", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 1) throw std::runtime_error("bad argument count for <sleep>");
        auto amount = ((SInt*)args[0])->getValue();
        std::cout << "Sleeping for " << amount << std::endl;
        throw std::runtime_error("sleep not implemented");
    } },
    { "run", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 1) throw std::runtime_error("bad argument count for <run>");
        auto macroName = ((SRaw*)args[0])->get();
        auto macro = so->getMacro(macroName);
        macro->exec();
    } },
    { "opencontainer", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 2) throw std::runtime_error("bad argument count for <opencontainer>");
        auto key = ((SRaw*)args[0])->get();
        auto top = args[1]->str();
        std::cout << "Opening container with key: " << key << " (top: " << top << ")\n";
        throw std::runtime_error("opencontainer not implemented");
    } },
    { "if", [](ScriptOverseer* so, SObject** args, int argc) {
        vector<string> argv;
        for (int i = 0; i < argc; i++)
            argv.push_back(((SRaw*)args[i])->get());
        // for (auto v : argv)
        //     std::cout << v << " ";
        // std::cout << std::endl;

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
            std::cout << j << std::endl;
            auto c = new Command(j, so);
            c->exec();
            delete c;
        }
    } },
    { "warp", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 1) throw std::runtime_error("bad argument count for <warp>");
        auto code = ((SRaw*)args[0])->get();
        so->getGame()->useWarpCode(code);
        // std::cout << "Warping to " << code << std::endl;
        // throw std::runtime_error("warp not implemented");
    } },
    { "give", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 1) throw std::runtime_error("bad argument count for <give>");
        auto name = args[0]->str();
        std::cout << "Giving item " << name << std::endl;
        throw std::runtime_error("give not implemented");
    } },
    { "take", [](ScriptOverseer* so, SObject** args, int argc) {
        if (argc != 1) throw std::runtime_error("bad argument count for <take>");
        auto name = args[0]->str();
        std::cout << "Taking item " << name << std::endl;
        throw std::runtime_error("take not implemented");
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
