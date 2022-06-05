#pragma once

#include <regex>
#include <ncurses.h>
#include <map>
#include "../util.hpp"

using std::string;

namespace nc {
    const std::regex colorRegex("\\$\\{([\\w|-]+)\\}([^\\$]*)");

    const std::map<string, int> colorMap = {
        {"black",   COLOR_BLACK   },
        {"red",     COLOR_RED     },
        {"green",   COLOR_GREEN   },
        {"yellow",  COLOR_YELLOW  },
        {"blue",    COLOR_BLUE    },
        {"magenta", COLOR_MAGENTA },
        {"cyan",    COLOR_CYAN    },
        {"white",   COLOR_WHITE   }
    };
    const string BG_DEFAULT_COLOR = "black";
    const string FG_DEFAULT_COLOR = "white";

    std::pair<int, int> parseColor(string color) {
        auto split = str::split(color, "-");
        if (split.size() != 1 && split.size() != 2) throw std::runtime_error("can't parse color " + color);
        auto fg = split[0];
        auto bg = BG_DEFAULT_COLOR;
        if (split.size() == 2) {
            bg = split[1];
        }
        auto it = colorMap.find(fg);
        if (it == colorMap.end()) throw std::runtime_error("don't know color " + fg);
        int fgc = it->second;
        it = colorMap.find(bg);
        if (it == colorMap.end()) throw std::runtime_error("don't know color " + bg);
        int bgc = it->second;
        return std::make_pair(fgc, bgc);
    }

    class CCTMessage {
    private:
        string raw;
        int count;
        std::pair<int, int>* pairs;
        string* lines;

    public:
        CCTMessage(string line) {
            this->raw = line;    

            std::smatch m;

            const vector<std::smatch> matches{
                std::sregex_iterator{line.begin(), line.end(), colorRegex},
                std::sregex_iterator{}
            };

            auto size = matches.size();

            this->pairs = new std::pair<int, int>[size];
            this->lines = new string[size];

            for (int i = 0; i < size; i++) {
                auto pair = parseColor(matches[i].str(1));
                this->pairs[i] = pair;
                auto line = matches[i].str(2);
                this->lines[i] = line;
            }
        }

        ~CCTMessage(){
            delete[] pairs;
            delete[] lines;
        }

        void draw(WINDOW* w, int y, int x, bool reverse) {
            
        }

        void draw(WINDOW* w, int y, int x) {
            this->draw(w, y, x, false);
        }
    };

    void resetColors(WINDOW* w) {
        auto pair = parseColor(FG_DEFAULT_COLOR + "-" + BG_DEFAULT_COLOR);
    }
}
