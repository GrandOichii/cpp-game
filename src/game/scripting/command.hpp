#pragma once

#include "object.hpp"
#include "overseer.hpp"

namespace game {
namespace scripting {

using func = std::function<void(ScriptOverseer*, SObject**, int)>;

class Command {
private:
    func f;
    SObject** args;
    int argc;
    ScriptOverseer* so;
public:
    Command(std::string line, ScriptOverseer* so);
    ~Command();
    void exec();
};

}
}
