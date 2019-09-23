//
// Created by lucas on 9/23/19.
//

#include "StringTransitions/Automata.h"
#include "StringTransitions/Automata.cpp"

void DecomposeAutomata(Automata & automato, std::vector<Automata> & automatos)
{
    std::vector<int> finalStates;
    automato.getFinalStates(finalStates);

    Automata newAutomata = automato;

    for(int & i : finalStates)
    {
        newAutomata.removeFinal(i);
    }

    for(int & i : finalStates)
    {
        newAutomata.setFinal(i);
        automatos.emplace_back(newAutomata);
        newAutomata.removeFinal(i);
    }
}

int main()
{
    Automata automato;

    std::string path = "../exemplo2m.txt";

    automato.createFromFile(path);

    std::vector<Automata> automatos;

    DecomposeAutomata(automato, automatos);

    //std::cout << automatos.size() << std::endl;

    std::ostringstream regularExpressions;

    int size = automato.getSize();
    int regexqtde = automatos.size();
    std::cout << regexqtde << std::endl;
    std::string regex;
    for(int i=0; i<regexqtde; i++)
    {
        automatos[i].addEpsilionBeginning();
        automatos[i].concatenateEdges();
        int k=1;
        for(int j=0; j<size-1; j++)
        {
            do{
                int new_size = automatos[i].getSize();
                k = (k + 1)% new_size;
                if(k==0)
                {
                    k++;
                }
            }while(automatos[i].isFinal(k));
            automatos[i].removeState(k);
   //         automatos[i].printAutomata();

        }
        automatos[i].concatenateEdges();
        automatos[i].getRegularExpression(regex);
        regularExpressions << regex;
        regex.clear();
        if(i != regexqtde - 1)
        {
            regularExpressions << " + ";
        }
    }

    regex = regularExpressions.str();

    std::cout << "Expressao regular encontrada:" << std::endl;
    std::cout << regex << std::endl;

    return 0;

}
