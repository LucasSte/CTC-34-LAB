//
// Created by lucas on 9/22/19.
//
#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>
#include "AutomataSte/automata.cpp"
#include "AutomataSte/automata.h"

void findEpsilonTrasitions(int insertion_node, int verification_node, Automata & automato, std::vector<std::unordered_set<int>> & eclosure)
{
    std::vector<int> * nextNodes;
    nextNodes = automato.getNextSates(verification_node, '.');
    if(nextNodes != nullptr)
    {
        for(int & j : (*nextNodes))
        {
            if(insertion_node != j) {
                eclosure[insertion_node].insert(j);
                findEpsilonTrasitions(insertion_node, j, automato, eclosure);
            }
        }
    }

}


void findEClosure(Automata & automato, std::vector<std::unordered_set<int>> & eclosure)
{
    int lenght = automato.getSize();
    for(int i=0; i<lenght; i++)
    {
        findEpsilonTrasitions(i, i, automato, eclosure);
    }
}


int main()
{
    std::string path = "../teste.txt";

    Automata automato;
    automato.createFromFile(path);
    int n = automato.getSize();

    std::vector<std::unordered_set<int>> eclosure;
    eclosure.resize(n);


    eclosure.clear();
    findEClosure(automato, eclosure);

    for(int i=0; i<n; i++)
    {
        if(!eclosure[i].empty())
        {
            for(auto itr = eclosure[i].begin(); itr != eclosure[i].end(); itr++)
            {
                auto reverses = automato.getReverses(i);
                for(auto itr2 = reverses.begin(); itr2 != reverses.end(); itr2++)
                {
                    for(int & depart : (itr2->second))
                    {

                        automato.addTransition(depart, itr2->first, (*itr), false);
                    }
                }

                auto keys = automato.getKeys(*itr);
                for(auto itr2 = keys.begin(); itr2 != keys.end(); itr2++)
                {
                    for(int & nextNode : (itr2->second))
                    {
                        automato.addTransition(i, itr2->first, nextNode, false);
                    }
                }
                if(automato.isFinal(*itr))
                {
                    automato.setFinal(i);
                }
                automato.removeEpsilonTransitions(i);

            }

        }
    }

    automato.printAutomata();


    return 0;
}