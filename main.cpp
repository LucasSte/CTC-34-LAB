#include <iostream>
#include "AutomataOcimar/automata.h"
#include <vector>

int main() {

    Automata my_automata("ab+(b+c)*");
    my_automata.show_graph();

    return 0;
}