#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <iterator>

#include "nlohmann/json.hpp"

using std::string;
using std::vector;

struct Double {
    int a[2];
};

bool isNumber(string word);

template<class T>
class CircularBuffer {
private:
    int size;
    int i;
    T* b;
public:
    CircularBuffer(int size) : size(size), i(0) {
        this->b = new T[size];
    }
    
    ~CircularBuffer(){
        delete[] b;
    }   
    
    void add(T item)  {
        ++i;
        i %= size;
        this->b[i] = item;
    }
    
    vector<T> getV() {
        vector<T> result;
        for (int ii = 0; ii < size; ii++) {
            result.push_back(b[(i+ii)%size]);
        }
        return result;
    }
};

namespace str {
    vector<string> widthSplit(string text, int maxWidth);

    void ltrim(std::string &s);

    void rtrim(std::string &s);

    void trim(std::string &s);

    std::string ltrim_copy(std::string s);

    std::string rtrim_copy(std::string s);

    std::string trim_copy(std::string s);

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

    string join(const string d1, const string d2);

    string readFile(const char* path);

    vector<string> readAllLines(const char* path);

    string dirOf(const string path);

}