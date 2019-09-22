//
// Created by lucas on 9/22/19.
//
#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>
#include "AutomataSte/automata.cpp"
#include "AutomataSte/automata.h"

void findEClosure(Automata & automato, std::vector<std::unordered_set<int>> & eclosure)
{
    int lenght = automato.getSize();
    std::vector<int> * nextNodes;
    for(int i=0; i<lenght; i++)
    {
        nextNodes = automato.getNextSates(i, '.');
        if(nextNodes != nullptr)
        {
            for(int & j : (*nextNodes))
            {
                eclosure[i].insert(j);
            }
        }
    }
}


int main()
{
    int n = 5;
    std::string path = "../teste.txt";

    Automata automato(n);
    automato.createFromFile(path);

    std::vector<std::unordered_set<int>> eclosure;
    eclosure.resize(n);

    findEClosure(automato, eclosure);

    for(int i=0; i<n; i++)
    {
        std::cout << "E-closure: " << i << std::endl;
        if(eclosure[i].empty())
        {
            std::cout << " Empty" << std::endl;
        }
        else
        {
            for(auto itr = eclosure[i].begin(); itr != eclosure[i].end(); itr++)
            {
                std::cout << " " << (*itr) << std::endl;
            }
        }
    }


    return 0;
}