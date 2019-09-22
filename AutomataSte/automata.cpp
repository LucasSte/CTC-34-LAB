//
// Created by lucas on 9/22/19.
//
#include "automata.h"
#include <string>
#include <fstream>
#include <vector>

Automata::Automata(int size) {
    states.resize(size);
    this->size = size;
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

            states[node].keys[letter].emplace_back(next);

            states[node].final = (line[6] == 'f');
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
        if(states[node].keys.find(letter) == states[node].keys.end())
        {
            return nullptr;
        }
        else
        {
            return &states[node].keys[letter];
        }
    }
    else
    {
        return nullptr;
    }

}

int Automata::getSize() {
    return size;
}