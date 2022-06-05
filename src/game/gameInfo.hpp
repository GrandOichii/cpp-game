#pragma once 

#include <fstream>

#include "util.hpp"

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

        void load(const string parentDir, const string path) {
            auto j = fs::readJS(fs::pathJoin(vector<string>{parentDir, path}));
            try {
                // assign values
                this->name = j["name"];
            }
            catch (nlohmann::detail::type_error er) {
                throw std::runtime_error("failed loading game info");
            }
        }

        void print() {
            std::cout << "-- Game info --\n\tName: " + this->name << std::endl;
        }
    };
}