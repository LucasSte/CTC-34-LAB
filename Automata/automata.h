#ifndef LAB01_AUTOMATA_H
#define LAB01_AUTOMATA_H

#include <vector>
#include <string>

using namespace std;

class Edge{
public:
    Edge(int index1, int index2, string reg_expr);
    int node1, node2;
    string reg_expr;

};

class Operation{
public:
    Operation(string reg_exprsn1, string reg_exprsn2, char language_operator);

    string reg_expr1, reg_expr2;
    char lang_operator;
};

class Graph{ //implementado por lista de arco
private:
    vector<bool> final_node;
public:
    vector<Edge> edge_list;
    int size;// numero de nó's
    Graph(int num_initial_nodes);
    bool add_edge(int index1, int index2, string reg_expr); // retorna se conseguiu incerir a aresta (não consegue
                                                            // incerir se um dos indices não se refere a um existente)
    bool is_final_node(int node_index);
    int new_node(bool is_final); //retorna o indice do nó
};

class Automata{
private:
    int size;
    Graph automata_graph = Graph(0);
public:
    Automata(string reg_expr);
    Operation expr_splitter(string reg_expr); // retorna uma ou duas expressoes regulares e
                                              // a operação que ocorre com/entre elas
    bool all_edge_ok;

};

#endif //LAB01_AUTOMATA_H
