#include "AutomataOcimar/automata.h"
#include "AutomataOcimar/automata.cpp"
#include <fstream>

int main() {
    ofstream file_out1;

    //Exercise 1:
    file_out1.open("Exercise1_test1.dot", ios::trunc);
    Automata my_automata1("(a+b)*bb(b+a)*");
    file_out1<<my_automata1.show_graph().str();
    file_out1.close();

    file_out1.open("Exercise1_test2.dot", ios::trunc);
    Automata my_automata2("(a(b+c))*");
    file_out1<<my_automata2.show_graph().str();
    file_out1.close();

    file_out1.open("Exercise1_test3.dot");
    Automata my_automata3("a*b+b*a");
    file_out1<<my_automata3.show_graph().str();
    file_out1.close();

    file_out1.open("Exercise1_test4.dot");
    Automata my_automata4("a*b*c*");
    file_out1<<my_automata4.show_graph().str();
    file_out1.close();

    // Exercise 2:
    ifstream file_in("../Exercise2_tests.txt");
    ofstream file_out2("Exercise2_test2.txt"),
             file_out3("Exercise2_test3.txt"),
             file_out4("Exercise2_test4.txt");
    file_out1.open("Exercise2_test1.txt");
    string str;

    while (std::getline(file_in, str)) {
        file_out1<<str<<":"<<endl;
        file_out2<<str<<":"<<endl;
        file_out3<<str<<":"<<endl;
        file_out4<<str<<":"<<endl;


        file_out1<<my_automata1.print_states_after_computation(str).str()<<endl<<endl;
        file_out2<<my_automata2.print_states_after_computation(str).str()<<endl<<endl;
        file_out3<<my_automata3.print_states_after_computation(str).str()<<endl<<endl;
        file_out4<<my_automata4.print_states_after_computation(str).str()<<endl<<endl;

    }
    file_out1.close();
    file_out2.close();
    file_out3.close();
    file_out4.close();


    return 0;
}