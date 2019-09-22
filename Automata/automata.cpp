#include "automata.h"
#include <stack>

Edge::Edge(int index1, int index2, string reg_exprsn) {
    node1 = index1;
    node2 = index2;
    reg_expr = reg_exprsn;
}

Operation::Operation(string reg_exprsn1, string reg_exprsn2, char language_operator) {
    reg_expr1 = reg_exprsn1;
    reg_expr2 = reg_exprsn2;
    lang_operator = language_operator;

}

Graph::Graph(int num_initial_nodes) {
    size = num_initial_nodes;
}

bool Graph::add_edge(int index1, int index2, string reg_expr){
    if(index1< size && index2 < size){
        Edge aux(index1, index2, reg_expr);
        edge_list.push_back(aux);
        return true;
    }
    else
        return false;

}

bool Graph::is_final_node(int node_index){
    return final_node[node_index];
}

int Graph::new_node(bool is_final) { //retorna o indice do nó
    final_node.push_back(is_final);
    size++;
    return size-1;
}

Operation Automata::expr_splitter(string reg_expr) {

}

Automata::Automata(string reg_expr) {
    // Construtor
    size = 2;

    automata_graph = Graph(2);
    automata_graph.add_edge(0, 1, reg_expr);

    stack<Edge> remaining_edges; // armazena as aresas que podem ser reduzidas
    remaining_edges.push(automata_graph.edge_list[0]); // adiciona a única aresta que o grafo tem por enquanto

    while (!remaining_edges.empty()){ //enquanto houver arestas que podem ser reduzidas
        Edge local_edge = remaining_edges.top();
        Operation aux_op = expr_splitter(local_edge.reg_expr);

        if (aux_op.lang_operator == '+'){ // União
            modifica_aresta_correspondente // vou ter que usar lista ligada


        }
        else if (aux_op.lang_operator == '*'){// feixo de Kleene

        }
        else if (aux_op.lang_operator == 'c') {// concatenação

        }

    }
}
