//
// Created by lucas on 9/22/19.
//

#include "automata.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

Automata::Automata() {

}


void Automata::createFromFile(std::string path) {
    std::ifstream file(path);
    std::string line;

    getline(file, line);
    size = std::stoi(line);

    states.resize(size);

    if(file.is_open())
    {
        while(getline(file, line))
        {
            std::vector<std::string> result;
            std::istringstream iss(line);
            for(std::string s; iss >> s; )
            {
                result.push_back(s);
            }
            int node = std::stoi(result[0]);
            char letter = line[2];
            int next = std::stoi(result[2]);

            states[node].keys[letter].emplace_back(next);
            states[next].reverse[letter].emplace_back(node);

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

void Automata::addTransition(int node, char letter, int next, bool final) {
    states[node].keys[letter].emplace_back(next);
    states[node].final = final;
    states[next].reverse[letter].emplace_back(node);
}

std::unordered_map<char, std::vector<int>>& Automata::getReverses (int node) {
    return states[node].reverse;
}

void Automata::printAutomata() {
    int length = states.size();

    for(int i=0; i< length; i++)
    {
        for(auto itr = states[i].keys.begin(); itr != states[i].keys.end(); itr++)
        {
            for(int & j : (itr->second))
            {
                std::cout << i << " " << itr->first << " " << j << (states[i].final?" f":" n") << std::endl;
            }
        }
    }
}

std::unordered_map<char, std::vector<int>> & Automata::getKeys(int node) {
    return states[node].keys;
}

bool Automata::isFinal(int node) {
    return states[node].final;
}

void Automata::setFinal(int node) {
    states[node].final = true;
}


///Only works for epsilon trasitions
void Automata::removeEpsilonTransitions(int node) {

    std::vector<int> & arrives = states[node].keys['.'];
    for (int & i : arrives) {
        states[i].reverse.erase('.');
    }
    states[node].keys.erase('.');
}