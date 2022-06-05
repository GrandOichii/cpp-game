#pragma once

#include "object.hpp"

namespace game {
namespace scripting {

class Command {
private:
    // func f;
    SObject** args;
    unsigned int argc;
public:
    Command();
    ~Command();
    void run();
};

}
}
