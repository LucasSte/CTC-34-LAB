#include <string>
#include <vector>
#include <unordered_map>
#ifndef CTC_34_LAB_AUTOMATA_H
#define CTC_34_LAB_AUTOMATA_H

class Automata{
public:
    Automata();
    void createFromFile(std::string path);
    std::vector<int> * getNextSates(int node, char letter);
    int getSize();
    void addTransition(int node, char letter, int next, bool final);
    std::unordered_map<char, std::vector<int>> & getReverses(int node);
    void printAutomata();
    std::unordered_map<char, std::vector<int>> & getKeys(int node);
    bool isFinal(int node);
    void setFinal(int node);
    void removeEpsilonTransitions(int node);

private:
    struct nodes{
        std::unordered_map<char, std::vector<int>> keys;
        std::unordered_map<char, std::vector<int>> reverse;
        bool final;
    };
    std::vector<nodes> states;
    int size;

};

#endif CTC_34_LAB_AUTOMATA_H