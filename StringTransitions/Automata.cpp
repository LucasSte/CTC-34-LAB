//
// Created by lucas on 9/23/19.
//


#include "Automata.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
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
            std::string letters = result[1];
            int next = std::stoi(result[2]);

            states[node].keys[letters].push_back(next);
            states[next].reverse[letters].push_back(node);

            states[node].final = (line[6] == 'f');
            states[node].forget = false;
        }
    }
    else
    {
        std::cout << "File not found" << std::endl;
    }
}

std::list<int> * Automata::getNextSates(int node, std::string letters) {
    int length = states.size();
    if(node < length)
    {
        if(states[node].keys.find(letters) == states[node].keys.end())
        {
            return nullptr;
        }
        else
        {
            return &states[node].keys[letters];
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

void Automata::addTransition(int node, std::string letters, int next, bool final) {
    states[node].keys[letters].push_back(next);
    states[node].final = final;
    states[next].reverse[letters].push_back(node);
}

std::unordered_map<std::string, std::list<int>>& Automata::getReverses (int node) {
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

std::unordered_map<std::string, std::list<int>> & Automata::getKeys(int node) {
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

    std::list<int> & arrives = states[node].keys["."];
    for (int & i : arrives) {
        states[i].reverse.erase(".");
    }
    states[node].keys.erase(".");
}

void Automata::getFinalStates(std::vector<int> & finalStates) {

    for(int i=0; i<size; i++)
    {
        if(states[i].final)
        {
            finalStates.emplace_back(i);
        }
    }
}

void Automata::removeFinal(int node) {
    states[node].final = false;
}

void Automata::addEpsilionBeginning() {

    for(nodes & node : states)
    {
        for(auto itr = node.keys.begin(); itr != node.keys.end(); itr++) {
            for (auto it = itr->second.begin(); it != itr->second.end(); it++) {
                (*it)++;
            }
        }
        for(auto itr = node.reverse.begin(); itr != node.reverse.end(); itr++)
        {
            for(auto it = itr->second.begin(); it != itr->second.end(); it++)
            {
                (*it)++;
            }
        }
    }
    size++;
    states.resize(size);
    std::rotate(states.begin(), states.begin()+states.size()-1, states.end());

    Automata::addTransition(0, ".", 1, false);
    //states[0].keys["."].emplace_back(1);
    //states[0].final = false;

    }

void Automata::concatenateEdges() {
    std::unordered_map<int, std::vector<std::string>> transitions;
    std::vector<int> repeatedTransitions;


    for(int i=0; i<size; i++)
    {
        for(auto itr = states[i].keys.begin(); itr!= states[i].keys.end(); itr++)
        {
            for(int & j : (itr->second))
            {
                transitions[j].emplace_back(itr->first);
            }
        }

        for(auto itr = transitions.begin(); itr != transitions.end(); itr++)
        {
            if(itr->second.size() > 1)
            {
                repeatedTransitions.emplace_back(itr->first);
            }
        }

        for(int & j : repeatedTransitions)
        {
            std::ostringstream newTransition;
            newTransition << "(";
            int lenght = transitions[j].size();
            for(int k=0; k<lenght; k++)
            {
                if(i==j && transitions[j][k] != ".")
                {
                    newTransition << "(";
                    newTransition << transitions[j][k];
                    newTransition << ")*";
                }
                else if(transitions[j][k] != ".")
                {
                    newTransition << transitions[j][k];
                }

                if(k != lenght-1)
                {
                    newTransition << "+";
                }
                else
                {
                    newTransition << ")";
                }
                states[i].keys[transitions[j][k]].remove(j);
                states[j].reverse[transitions[j][k]].remove(i);
            }
            Automata::addTransition(i, newTransition.str(), j, states[i].final);
            //states[i].keys[newTransition.str()].push_back(j);
        }
        transitions.clear();
        repeatedTransitions.clear();

    }
}


void Automata::removeState(int node) {
    bool hasLoopExpression = false;
    std::string loopExpression;

    if(node==0)
    {
        std::cout << "Cannot remove starting node" << std::endl;
    }
    else if(states[node].final)
    {
        std::cout << "Cannot remove final state" << std::endl;
    }
    else {
        std::ostringstream regex;
        regex << "(";
        for (auto itr = states[node].keys.begin(); itr != states[node].keys.end(); itr++) {
            for (auto it = itr->second.begin(); it != itr->second.end(); it++) {
                if (*it == node) {
                    hasLoopExpression = true;
                    if(itr->first != ".")
                        regex << itr->first;
                    if(std::next(it) != itr->second.end())
                        regex << "+";
                }
            }
        }
        regex << ")*";
        loopExpression = regex.str();
        for (auto itr = states[node].reverse.begin(); itr != states[node].reverse.end(); itr++) {
            for (int &i : (itr->second)) {
                for (auto itr2 = states[node].keys.begin(); itr2 != states[node].keys.end(); itr2++) {
                    for (int &j : (itr2->second)) {
                        if (hasLoopExpression)
                        {
                            if(itr->first == "." && itr2->first == ".")
                            {
                                Automata::addTransition(i, loopExpression, j, states[i].final);
                            }
                            else if(itr->first == ".")
                            {
                                Automata::addTransition(i, loopExpression + itr2->first, j, states[i].final);
                            }
                            else if(itr2->first == ".")
                            {
                                Automata::addTransition(i, itr->first + loopExpression, j, states[i].final);
                            }
                            else
                            {
                                Automata::addTransition(i, itr->first + loopExpression + itr2->first, j, states[i].final);
                            }
                        }
                        else
                        {
                            if(itr->first == "." && itr2->first == ".")
                            {
                                Automata::addTransition(i, "&", j, states[i].final);
                            }
                            else if(itr->first == ".")
                            {
                                Automata::addTransition(i, itr2->first, j, states[i].final);
                            }
                            else if(itr2->first == ".")
                            {
                                Automata::addTransition(i, itr->first, j, states[i].final);
                            }
                            else
                            {
                                Automata::addTransition(i, itr->first + itr2->first, j, states[i].final);
                            }
                        }
                    }
                }
            }
        }

        states[node].keys.clear();
        states[node].reverse.clear();
        for (int i = node; i < size-1; i++) {
            states[i] = states[i + 1];
        }
        size--;
        states.resize(size);


        for (nodes &state : states) {
            std::vector<std::string> keysToDelete;
            for (auto itr = state.keys.begin(); itr != state.keys.end(); itr++) {
                for (auto it = itr->second.begin(); it != itr->second.end(); it++) {
                    if (*it == node) {
                        itr->second.erase(it);
                        if(itr->second.empty())
                        {
                            keysToDelete.emplace_back(itr->first);
                        }
                        break;
                    }
                }
            }
            for(std::string & key : keysToDelete)
            {
                state.keys.erase(key);
            }
            keysToDelete.clear();
            for(auto itr = state.keys.begin(); itr != state.keys.end(); itr++)
            {
                for(auto it = itr->second.begin(); it != itr->second.end(); it++)
                {
                    if(*it > node)
                    {
                        (*it)--;
                    }
                }
            }
            for(auto itr = state.reverse.begin(); itr != state.reverse.end(); itr++)
            {
                for(auto it = itr->second.begin(); it != itr->second.end(); it++)
                {
                    if(*it == node)
                    {
                        itr->second.erase(it);
                        if(itr->second.empty())
                        {
                            keysToDelete.emplace_back(itr->first);
                        }
                        break;
                    }
                }
            }
            for(std::string & key : keysToDelete)
            {
                state.reverse.erase(key);
            }
            for(auto itr = state.reverse.begin(); itr != state.reverse.end(); itr++)
            {
                for(auto it = itr->second.begin(); it != itr->second.end(); it++)
                {
                    if(*it > node)
                    {
                        (*it)--;
                    }
                }
            }



        }
    }
}