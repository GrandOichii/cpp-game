#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <iterator>

#include "nlohmann/json.hpp"

using std::string;
using std::vector;

namespace str {
    template<typename ... Args>
    std::string format( const std::string& format, Args ... args )
    {
        int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
        if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
        auto size = static_cast<size_t>( size_s );
        std::unique_ptr<char[]> buf( new char[ size ] );
        std::snprintf( buf.get(), size, format.c_str(), args ... );
        return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
    }

    template <class InputIt>
    string join(InputIt begin, const InputIt end, const string delimiter);

    vector<string> split(string s, const string delimiter);
}


namespace fs {

    nlohmann::json readJS(const string path);

    string join(const vector<string> dirs);

    string readFile(const char* path);

    vector<string> readAllLines(const char* path);

}