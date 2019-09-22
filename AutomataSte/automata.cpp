//
// Created by lucas on 9/22/19.
//
#include "automata.h"
#include <string>
#include <fstream>
#include <vector>

Automata::Automata(int size) {
    states.resize(size);
}


void Automata::createFromFile(std::string path) {
    std::ifstream file(path);
    std::string line;

    if(file.is_open())
    {
        while(getline(file, line))
        {
            int node = line[0] - '0';
            char letter = line[2];
            int next = line[4] - '0';

            states[node][letter].emplace_back(next);
        }
    }
    else
    {
        std::cout << "File not found" << std::endl;
    }
}

std::vector<int> * Automata::getNextSates(int node, char letter) {
    int length = states.size();
    if(node < length)
    {
        if(states[node].find(letter) == states[node].end())
        {
            return nullptr;
        }
        else
        {
            return &states[node][letter];
        }
    }
    else
    {
        return nullptr;
    }

}