#include <iostream>
#include "Automata/automata.h"

int main() {

    string reg_expr = "ab+(b+c)*";
    Automata my_automata(reg_expr);

    return 0;
}