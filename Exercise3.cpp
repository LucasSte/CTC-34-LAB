//
// Created by lucas on 9/22/19.
//
#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>
#include "StringTransitions/Automata.cpp"
#include "StringTransitions/Automata.h"


//Funcao recursiva para encontrar e-fecho
void findEpsilonTrasitions(int insertion_node, int verification_node, Automata & automato, std::vector<std::unordered_set<int>> & eclosure)
{
    std::list<int> * nextNodes;
    nextNodes = automato.getNextSates(verification_node, ".");
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

//Funcao auxiliar para encontrar e-fecho
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
    ///Alterar aqui para utilizar outro arquivo de entrada
    std::string path = "../exemplo4m.txt";

    //Criacao do automato
    Automata automato;
    automato.createFromFile(path);
    int n = automato.getSize();

    std::vector<std::unordered_set<int>> eclosure;
    eclosure.resize(n);

    eclosure.clear();

    //Encontro do e-fecho
    findEClosure(automato, eclosure);


    struct toAdd{
        int depart;
        int arrive;
        std::string key;
    };

    std::vector<toAdd> transitionstoAdd;
    for(int i=0; i<n; i++) //Procura de transicoes para serem adicionadas
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
                        if(itr2->first != ".")
                        {
                            transitionstoAdd.push_back({depart, *itr, itr2->first});
                        }

                    }
                }

                auto keys = automato.getKeys(*itr);
                for(auto itr2 = keys.begin(); itr2 != keys.end(); itr2++)
                {
                    for(int & nextNode : (itr2->second))
                    {
                        if(itr2->first != ".") {
                            transitionstoAdd.push_back({i, nextNode, itr2->first});
                        }
                    }
                }
                if(automato.isFinal(*itr)) //Configuracao de novos estado finais
                {
                    automato.setFinal(i);
                }
                automato.removeEpsilonTransitions(i);

            }

        }
    }

    //Adicao de novas transicoes
    for(toAdd & transitions : transitionstoAdd)
    {
        automato.addTransition(transitions.depart, transitions.key, transitions.arrive, automato.isFinal(transitions.depart));
    }

    automato.printAsGraphViz();


    return 0;
}