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


void Automata::createFromFile(std::string path) { //Cria automato de arquivo
    std::ifstream file(path);
    std::string line;

    getline(file, line);
    size = std::stoi(line); //Le tamannho

    states.resize(size);

    if(file.is_open())
    {
        getline(file, line);        //Encontra estado finais
        std::vector<std::string> result1;
        std::istringstream iss1(line);
        for(std::string s; iss1 >> s;)
        {
            result1.emplace_back(s);
        }
        for(std::string & state : result1)
        {
            int state_int = std::stoi(state);
            states[state_int].final = true;
        }

        while(getline(file, line))      //Salva transicoes
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


        }

    }
    else
    {
        std::cout << "File not found" << std::endl;
    }
}

//Retorna proximos estados, a partir de um noh e um
//carater de transicao
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

//Retorna quantidade de estados do automato
int Automata::getSize() {
    return size;
}

//Adiciona uma transicao ao automato
void Automata::addTransition(int node, std::string letters, int next, bool final) {
    states[node].keys[letters].push_back(next);
    states[node].final = final;
    states[next].reverse[letters].push_back(node);
}

//Retorna mapa de transicoes que chegam a
//determinado estado
std::unordered_map<std::string, std::list<int>>& Automata::getReverses (int node) {
    return states[node].reverse;
}


//Imprime automato na formatacao do arquivo de entrada
void Automata::printAutomata() {
    int length = states.size();

    for(int i=0; i< length; i++)
    {
        for(auto itr = states[i].keys.begin(); itr != states[i].keys.end(); itr++)
        {
            for(int & j : (itr->second))
            {
                std::cout << i << " " << itr->first << " " << j << std::endl;
            }
        }
    }

    std::cout << "Final states" << std::endl;
    for(int i=0; i<size; i++)
    {
        if(states[i].final)
        {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
}

//Retorna mapa de transicoes que parter de um
//determinado estado
std::unordered_map<std::string, std::list<int>> & Automata::getKeys(int node) {
    return states[node].keys;
}

//Verifica se estado eh final
bool Automata::isFinal(int node) {
    return states[node].final;
}

//Faz com que estado seja final
void Automata::setFinal(int node) {
    states[node].final = true;
}


//Remove transicoes epsilon
void Automata::removeEpsilonTransitions(int node) {

    std::list<int> & arrives = states[node].keys["."];
    for (int & i : arrives) {
        states[i].reverse.erase(".");
    }
    states[node].keys.erase(".");
}

//Retonra estados finais do automato
void Automata::getFinalStates(std::vector<int> & finalStates) {

    for(int i=0; i<size; i++)
    {
        if(states[i].final)
        {
            finalStates.emplace_back(i);
        }
    }
}

//Faz que que estado deixe de ser final
void Automata::removeFinal(int node) {
    states[node].final = false;
}

//Adiciona um estado inicial ligado com uma trasicao epsilon ao estado inicial original
void Automata::addEpsilionBeginning() {

    for(nodes & node : states)
    {
        for(auto itr = node.keys.begin(); itr != node.keys.end(); itr++) { //Alterar mapa de partidas
            for (auto it = itr->second.begin(); it != itr->second.end(); it++) {
                (*it)++;
            }
        }
        for(auto itr = node.reverse.begin(); itr != node.reverse.end(); itr++) //Alterar mapa de chegadas
        {
            for(auto it = itr->second.begin(); it != itr->second.end(); it++)
            {
                (*it)++;
            }
        }
    }
    size++;
    states.resize(size);    //Diminuir tamanho do vetor
    std::rotate(states.begin(), states.begin()+states.size()-1, states.end());

    Automata::addTransition(0, ".", 1, false);

}

//Contatena transicoes que partem de um mesmo estado e chegam a um mesmo estado
void Automata::concatenateEdges() {
    std::unordered_map<int, std::vector<std::string>> transitions;
    std::vector<int> repeatedTransitions;


    for(int i=0; i<size; i++)
    {
        for(auto itr = states[i].keys.begin(); itr!= states[i].keys.end(); itr++) //Criar mapa de transicoes
        {
            for(int & j : (itr->second))
            {
                transitions[j].emplace_back(itr->first);
            }
        }

        for(auto itr = transitions.begin(); itr != transitions.end(); itr++) //Encontra transicoes com mesma origem e
            //mesmo destino
        {
            if(itr->second.size() > 1)
            {
                repeatedTransitions.emplace_back(itr->first);
            }
        }

        for(int & j : repeatedTransitions) //Retira transicoes com mesma origem e mesmo destino, criando outra com
            //uma expressao regualar concatenada a partir das duas outras transicoes
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

                if(states[i].keys[transitions[j][k]].empty())
                {
                    states[i].keys.erase(transitions[j][k]);
                }

                if(states[j].reverse[transitions[j][k]].empty())
                {
                    states[j].reverse.erase(transitions[j][k]);
                }
            }
            Automata::addTransition(i, newTransition.str(), j, states[i].final);
        }
        transitions.clear();
        repeatedTransitions.clear();

    }
}

//Remove estado do automato
void Automata::removeState(int node) {
    bool hasLoopExpression = false;
    std::string loopExpression;

    //Nao se pode remover estados inicial e final
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
        regex << "("; //Encontra se ha alguma transicao em loop
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
        loopExpression = regex.str(); //Adiciona novas transicoes para remover o estado
        for (auto itr = states[node].reverse.begin(); itr != states[node].reverse.end(); itr++) {
            for (int &i : (itr->second)) {
                for (auto itr2 = states[node].keys.begin(); itr2 != states[node].keys.end(); itr2++) {
                    for (int &j : (itr2->second)) {
                        if(i!= node && j!= node) {
                            if (hasLoopExpression) {
                                if (itr->first == "." && itr2->first == ".") {
                                    Automata::addTransition(i, loopExpression, j, states[i].final);
                                } else if (itr->first == ".") {
                                    Automata::addTransition(i, loopExpression + itr2->first, j, states[i].final);
                                } else if (itr2->first == ".") {
                                    Automata::addTransition(i, itr->first + loopExpression, j, states[i].final);
                                } else {
                                    Automata::addTransition(i, itr->first + loopExpression + itr2->first, j,
                                                            states[i].final);
                                }
                            } else {
                                if (itr->first == "." && itr2->first == ".") {
                                    Automata::addTransition(i, "&", j, states[i].final);
                                } else if (itr->first == ".") {
                                    Automata::addTransition(i, itr2->first, j, states[i].final);
                                } else if (itr2->first == ".") {
                                    Automata::addTransition(i, itr->first, j, states[i].final);
                                } else {
                                    Automata::addTransition(i, itr->first + itr2->first, j, states[i].final);
                                }
                            }
                        }
                    }
                }
            }
        }
         //Remocao do estado
        states[node].keys.clear();
        states[node].reverse.clear();
        for (int i = node; i < size-1; i++) {
            states[i] = states[i + 1];
        }
        size--;
        states.resize(size);


        //Ajuste dos mapas para a remocao do estado
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

//Retira expressao reuglar do automato, com tamanho no maximo 2
void Automata::getRegularExpression(std::string & answer) {
    std::ostringstream regex;
    if(size > 2)
    {
        std::cout << "Impossible to get regular expression. Simplify automata first!";
    }
    else
    {
        std::string regex_ante = "None";
        for(nodes & node : states)
        {
            for(auto itr = node.keys.begin(); itr != node.keys.end(); itr++)
            {
                if(itr->first != regex_ante)
                    regex << itr->first;
                regex_ante = itr->first;
            }
        }
        answer = regex.str();
    }
}

//Imprime grafo no formato GraphViz
void Automata::printAsGraphViz() {
    std::cout << "digraph fsm {" << std::endl;
    std::cout << "node [shape = doublecircle];";

    for(int i=0; i<size; i++)
    {
        if(states[i].final)
            std::cout << " " << i;
    }
    std::cout << ";" << std::endl;
    std::cout << "node [shape = circle];" << std::endl;

    for(int i=0; i< size; i++)
    {
        for(auto itr = states[i].keys.begin(); itr != states[i].keys.end(); itr++)
        {
            for(int & j : (itr->second))
            {
                std::cout << i << " -> " << j  << " [ label = \""<< itr->first << "\" ];" <<  std::endl;
            }
        }
    }

    std::cout << "}" << std::endl;

}