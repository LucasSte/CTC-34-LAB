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
    int getSize();

private:
    struct nodes{
        std::unordered_map<char, std::vector<int>> keys;
        bool final;
    };
    std::vector<nodes> states;
    int size;

};

#endif CTC_34_LAB_AUTOMATA_H