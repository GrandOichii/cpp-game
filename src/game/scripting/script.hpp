#pragma once

#include <vector>

#include "command.hpp"
#include "overseer.hpp"

namespace game {
namespace scripting {

class Script {
private:
    std::vector<Command*> commands;
public:
    Script(const char* path, const ScriptOverseer* so);
    ~Script();
};

}
}