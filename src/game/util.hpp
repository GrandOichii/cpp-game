#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <iterator>

#include "../nlohmann/json.hpp"

using std::string;
using std::vector;

template <class InputIt>
string join(InputIt begin, const InputIt end, const string delimiter) {
    string result = "";
    while (begin != end-1) {
        result += *begin++ + delimiter;
    }
    result += *begin;
    return result;
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

    string pathJoin(const vector<string> dirs) {
        return join(dirs.begin(), dirs.end(), "/");
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
        return join(result.begin(), result.end(), "\n");
    }

}

vector<string> splitString(string s, const string delimiter) {
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


class ILoadable {
public:
    virtual void load(string parentDir, string path) = 0;
    virtual void print() = 0;
};