//
// Created by lucas on 9/22/19.
//

#ifndef CTC_34_LAB_AUTOMATA_H
#define CTC_34_LAB_AUTOMATA_H

class Automata {
public:
    Automata();
    void createFromFile();
    std::vector & getNextSates();

private:
    std::unordered_map<char, std::vector<int>> states;
};

#endif //CTC_34_LAB_AUTOMATA_H
