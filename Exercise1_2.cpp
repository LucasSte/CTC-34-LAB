#include "AutomataOcimar/automata.h"
#include "AutomataOcimar/automata.cpp"

int main() {

    Automata my_automata1("(a+b)*bb(b+a)*");
    my_automata1.show_graph();

    Automata my_automata2("(a(b+c))*");
    my_automata2.show_graph();

    Automata my_automata3("a*b+b*a");
    my_automata3.show_graph();

    Automata my_automata4("a*b*c*");
    my_automata4.show_graph();

    return 0;
}