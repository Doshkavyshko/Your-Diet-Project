#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

#include "parser_class.h"

bool Information::title_find(std::string& begin) {
    size_t position = begin.find("Химический состав продукта: ");
    if (position == (size_t) -1) {
        return false;
    }
    begin.erase(0, position + 52);
    return true;
}

void Information::get_name(std::string& begin) {
    size_t end = begin.find('>');
    dish_name = begin.substr(0, end - 1);
}

void Information::calories_find(std::string& begin) {
    size_t position = begin.find("</tr>");
    std::stringstream str(begin.substr(0, position));
    char c = '\0';
    double m[4];
    for (double & i : m) {
        while (c < '0' || c > '9') {
            str.get(c);
        }
        str.seekg((int)str.tellg() - 1, std::ios::beg);
        std::string temp;
        str >> temp;
        int check = temp.find(',');
        if (check != -1) {
            temp[temp.find(',')] = '.';
        }
        i = atof(temp.c_str());
        str.get(c);
    }
    calories = m[0];
    proteins = m[1];
    fats = m[2];
    carbohydrates = m[3];
    begin.erase(0, position);
}

int main() {
    Information KASHA;
    std::ifstream in("in.txt");
    in.seekg(0, std::ios::end);
    size_t size = in.tellg();
    std::string TEXT(size, ' ');
    in.seekg(0, std::ios::beg);
    in.read(&TEXT[0], (int) size);
    while (KASHA.title_find(TEXT)) {
        KASHA.get_name(TEXT);
        KASHA.calories_find(TEXT);
        std::cout << KASHA.getNAME() << ' ' << KASHA.getCAL() << ' ' << KASHA.getPROT() << ' ' << KASHA.getFAT() << ' ' << KASHA.getCARB() << std::endl;
    }
}