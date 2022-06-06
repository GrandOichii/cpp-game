#include "util.hpp"

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <iterator>

#include "nlohmann/json.hpp"

using std::string;
using std::vector;

namespace str {

    template <class InputIt>
    string join(InputIt begin, const InputIt end, const string delimiter) {
        string result = "";
        while (begin != end-1) {
            result += *begin++ + delimiter;
        }
        result += *begin;
        return result;
    }


    vector<string> split(string s, const string delimiter) {
        size_t pos = 0;
        string token;
        vector<string> result;
        while ((pos = s.find(delimiter)) != string::npos) {
            token = s.substr(0, pos);
            result.push_back(token);
            s.erase(0, pos + delimiter.length());
        }
        result.push_back(s);
        return result;
    }
}


namespace fs {

    nlohmann::json readJS(const string path) {
        std::ifstream in(path);
        if (!in) throw std::runtime_error("path " + string(path) + " doesn't exist");
        nlohmann::json j;
        in >> j;
        in.close();
        return j;
    }

    string join(const vector<string> dirs) {
        return str::join(dirs.begin(), dirs.end(), "/");
    }

    string join(const string d1, const string d2) {
        return join(vector<string>{d1, d2});
    }


    string readFile(const char* path) {
        string line;
        std::ifstream in(path);
        if (!in) {
            throw std::runtime_error("no file " + string(path));
        }
        vector<string> result;
        while (std::getline(in, line)) {
            result.push_back(line);
        }
        in.close();
        return str::join(result.begin(), result.end(), "\n");
    }

    vector<string> readAllLines(const char* path) {
        auto text = readFile(path);
        return str::split(text, "\n");
    }

}