#pragma once

#include <vector>

#include "command.hpp"
#include "overseer.hpp"

namespace game {
namespace scripting {

// class ScriptOverseer;

class Script {
private:
    std::vector<Command*> commands;
public:
    Script(const char* path, ScriptOverseer* so);
    Script(std::vector<std::string>, ScriptOverseer* so);
    ~Script();
    void exec();
};

}
}