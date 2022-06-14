#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <iterator>
#include <math.h>

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

    int getSize() {
        return size;
    } 
    
    void add(T item)  {
        ++i;
        i %= size;
        this->b[i] = item;
    }
    
    vector<T> getV() {
        vector<T> result;
        for (int ii = 0; ii < size; ii++) { 
            result.push_back(b[(i+ii+1)%size]);
        }
        return result;
    }

    vector<T> getLast(int count) {
        auto v = this->getV();
        if (count <= v.size()) return v;
        vector<T> result;
        for (int i = 0; i < count; i++)
            result.push_back(v[i]);
        return result;
    }

    void clear() {
        delete[] b;
        this->b = new T[size];
        this->i = 0;
    }
};

const std::string ALLOWED_CHARS = "_,./<>?\" :";

class LineEdit {
private:
    int maxWidth;
    int cursor;
    std::string text;
public:
    LineEdit(std::string text, int maxWidth) : maxWidth(maxWidth) {
        this->setText(text);
    }

    void setText(std::string text) {
        auto size = text.size();
        if (size > maxWidth) throw std::runtime_error("can't set line edit: str length is bigger than max width");
        this->text = text;
        cursor = size;
    }

    std::string getText() {
        return text;
    }

    void cursorLeft() {
        cursor--;
        if (cursor < 0) {
            cursor = 0;
        }
    }

    void cursorRight() {
        cursor++;
        auto size = text.size();
        if (cursor >= size) {
            cursor = size;
        }
    }

    void eraseAtCursor() {
        if (cursor == 0) {
            return;
        }
        text.erase(cursor-1, 1);
        cursor--;
    }

    int getCursor() {
        return cursor;
    }

    bool addCharacter(char ch) {
        if (text.size() == maxWidth) return false;
        if ((ch >= 'a' && ch <= 'z') ||
            (ch >= 'A' && ch <= 'Z') ||
            (ch >= '0' && ch <= '9')) 
        {
            text.insert(text.begin()+cursor, ch);
            cursor++;
            return true;
        }
        for (int i = 0; i < ALLOWED_CHARS.size(); i++) {
            if (ch == ALLOWED_CHARS[i]) {
                text.insert(text.begin()+cursor, ch);
                cursor++;
                return true;
            }
        }
        return false;
    }

    void clear() {
        this->text = "";
        this->cursor = 0;
    }
};

template<class T>
class ListTemplate {
private:
    int cursor = 0;
    int choice = 0;
    int page = 0;
    int maxAmount;
    vector<T> items;
public:
    ListTemplate(int maxAmount) : maxAmount(maxAmount) {

    }

    int size() {
        return items.size();
    }

    void add(T item) { items.push_back(item); }

    void moveUp() {
        choice--;
        cursor--;
        auto size = items.size();
        if (cursor < 0) {
            if (size > maxAmount) {
                if (page == 0) {
                    cursor = maxAmount - 1;
                    choice = size - 1;
                    page = size - maxAmount;
                } else {
                    page--;
                    cursor++;
                }
            } else {
                cursor = size - 1;
                choice = cursor;
            }
        }
    }

    void moveDown() {
        choice++;
        cursor++;
        auto size = items.size();
        if (size > maxAmount) {
            if (cursor >= maxAmount) {
                cursor--;
                page++;
                if (choice == size) {
                    choice = 0;
                    cursor = 0;
                    page = 0;
                }
            }
        } else {
            if (cursor >= size) {
                cursor = 0;
                choice = 0;
            }
        }
    }

    void scrollUp() {
        if (maxAmount > items.size()) return;
        page--;
        if (page < 0) {
            page = items.size() - maxAmount;
        }
    }

    void scrollDown() {
        if (maxAmount > items.size()) return;
        page++;
        if (page > items.size() - maxAmount) {
            page = 0;
        }
    }

    vector<T> getVisible() { 
        auto begin = items.begin() + page;
        auto a = items.size();
        if (maxAmount < a) a = maxAmount;
        auto end = items.begin() + a;
        return {begin, end+page}; 
    }

    vector<T> getAll() const { return items; }

    bool hasMoreTop() { return page != 0; }

    bool hasMoreBottom() { return false; }

    int getCursor() { return cursor; }

    T getCurrent() { return items[choice]; }
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