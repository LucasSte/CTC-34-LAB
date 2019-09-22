#include <iostream>
#include "AutomataSte/automata.h"
#include "AutomataSte/automata.cpp"
#include <vector>

int main() {

    Automata automato;
    automato.createFromFile("../teste.txt");

    std::vector<int> * nodes;
    nodes = automato.getNextSates(1, '.');
    //std::cout << (*nodes)[0] << std::endl;

    return 0;
}