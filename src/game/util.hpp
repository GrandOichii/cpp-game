#pragma once

#include "scripting/overseer.hpp"

using std::string;

namespace game {

    class ILoadable {
    public:
        virtual void load(const string parentDir, const string path, game::scripting::ScriptOverseer* so) = 0;
        virtual void print() = 0;
    };

}