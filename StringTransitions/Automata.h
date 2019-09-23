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
    void createFromFile(std::string path);
    std::list<int> * getNextSates(int node, std::string letters);
    int getSize();
    void addTransition(int node, std::string letters, int next, bool final);
    std::unordered_map<std::string, std::list<int>> & getReverses(int node);
    void printAutomata();
    std::unordered_map<std::string, std::list<int>> & getKeys(int node);
    bool isFinal(int node);
    void setFinal(int node);
    void removeEpsilonTransitions(int node);
    void getFinalStates(std::vector<int> & finalStates);
    void removeFinal(int node);
    void addEpsilionBeginning();
    void concatenateEdges();
    void removeState(int node);
    void getRegularExpression(std::string & answer);

private:
    struct nodes{
        std::unordered_map<std::string, std::list<int>> keys;
        std::unordered_map<std::string, std::list<int>> reverse;
        bool final;
        bool forget;
    };
    std::vector<nodes> states;
    int size;

};


#endif //CTC_34_LAB_AUTOMATA_H
