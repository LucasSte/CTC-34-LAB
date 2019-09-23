#include <iostream>
#include "AutomataOcimar/automata.h"

int main() {

    string reg_expr = "ab+(b+c)*";
    Automata my_automata(reg_expr);
    my_automata.show_graph();

    return 0;
}