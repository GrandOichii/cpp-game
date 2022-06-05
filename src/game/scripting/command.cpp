#include "command.hpp"

#include <iostream>
#include <map>

using std::string;

namespace game {
    namespace scripting {
        using func = std::function<void(SObject**, int)>;
        const std::map<string, func> FUNC_MAP = {
            {"print", [](SObject** args, int argc) {
                if (argc != 1) throw std::runtime_error("bad argument count for <print>");
                std::cout << args[0] << std::endl;
            }}
        };
    }
}

game::scripting::Command::Command() {

}

game::scripting::Command::~Command() {

}

void game::scripting::Command::run() {

}