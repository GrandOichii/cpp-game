#include "script.hpp"

#include <vector>

#include "overseer.hpp"
#include "command.hpp"
#include "../../util.hpp"

namespace game {
namespace scripting {

static std::string getMacroDeclaration(std::string line) {
    auto index = line.find('{');
    if (index == -1) {
        return "";
    }
    return str::trim_copy(line.substr(0, index));
}

Script::Script(const char* path, ScriptOverseer* so) : Script(fs::readAllLines(path), so) {}

Script::Script(std::vector<std::string> lines, ScriptOverseer* so) {
    for (int i = 0; i < lines.size(); i++){
        auto line = lines[i];
        if (line == "") continue;
        auto macroName = getMacroDeclaration(line);
        if (macroName != "") {
            auto endI = i + 1;
            while (lines[endI] != "}") {
                endI++;
            }
            std::vector<string> macroLines = {lines.begin() + i + 1, lines.begin() + endI - 1};
            auto script = new Script(macroLines, so);
            so->addMacro(macroName, script);
            i = endI;
            continue;
        }
        commands.push_back(new Command(line, so));
    }
}

Script::~Script() {
    for (const auto& p : commands) delete p;
}

void Script::exec() {
    for (const auto command : commands) {
        command->exec();
    }
}

}
}