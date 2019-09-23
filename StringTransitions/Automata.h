//
// Created by lucas on 9/23/19.
//

#ifndef CTC_34_LAB_AUTOMATA_H
#define CTC_34_LAB_AUTOMATA_H

#include <vector>
#include <list>
#include <unordered_map>
#include <string>

class Automata{
public:
    Automata();
    void createFromFile(std::string path); //Cria automato de arquivo txt
    std::list<int> * getNextSates(int node, std::string letters); //Retorna proximos estados, a partir de um noh e um
    //carater de transicao
    int getSize(); //Retorna quantidade de estados do automato
    void addTransition(int node, std::string letters, int next, bool final); //Adiciona transicao ao automato
    std::unordered_map<std::string, std::list<int>> & getReverses(int node); //Retorna mapa de transicoes que chegam a
    //determinado estado
    void printAutomata(); //Imprime o automato
    std::unordered_map<std::string, std::list<int>> & getKeys(int node); //Retorna mapa de transicoes que parter de um
    //determinado estado
    bool isFinal(int node); //Verifica se eh estado final
    void setFinal(int node); //Faz com que estado seja final
    void removeEpsilonTransitions(int node); //Remove transicoes epsilon de determinado estado
    void getFinalStates(std::vector<int> & finalStates); //Retorna os estados final do automato
    void removeFinal(int node); //Faz com que estado deixe de ser final
    void addEpsilionBeginning(); //Adiciona um estado inicial ligado com uma trasicao epsilon ao estado inicial original
    void concatenateEdges(); //Contatena transicoes que partem de um mesmo estado e chegam a um mesmo estado
    void removeState(int node); //Remove estado do automato
    void getRegularExpression(std::string & answer); //Retorna expressao regular do automato simplificado
    void printAsGraphViz(); //Imprime o automato no formato do graphviz

private:
    struct nodes{
        std::unordered_map<std::string, std::list<int>> keys;
        std::unordered_map<std::string, std::list<int>> reverse;
        bool final;
    }; //Estrutra na qual ficam salvos os estados
    std::vector<nodes> states; //Vetor que representa o automato
    int size; //Tamanho do automato

};


#endif //CTC_34_LAB_AUTOMATA_H
