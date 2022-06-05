#pragma once 

#include <fstream>

#include "scripting/overseer.hpp"
#include "util.hpp"
#include "../util.hpp"

using std::string;

namespace game {
    class GameInfo : public ILoadable {
    private:
        string name;
    public:
        string getName() { return name; }

        GameInfo() {
            name = "";
        }

        ~GameInfo() {

        }

        void load(const string parentDir, const string path, const scripting::ScriptOverseer* so) {
            auto j = fs::readJS(fs::join(vector<string>{parentDir, path}));
            try {
                // assign values
                this->name = j["name"];
            }
            catch (nlohmann::detail::type_error er) {
                throw std::runtime_error(er.what());
            }
        }

        void print() {
            std::cout << "-- Game info --\n\tName: " + this->name << std::endl;
        }
    };
}