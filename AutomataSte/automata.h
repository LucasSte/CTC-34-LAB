#include <string>
#include <vector>
#include <unordered_map>
#ifndef CTC_34_LAB_AUTOMATA_H
#define CTC_34_LAB_AUTOMATA_H

class Automata{
public:
    explicit Automata(int size);
    void createFromFile(std::string path);
    std::vector<int> * getNextSates(int node, char letter);

private:
    std::vector<std::unordered_map<char, std::vector<int>>> states;

};

#endif CTC_34_LAB_AUTOMATA_H