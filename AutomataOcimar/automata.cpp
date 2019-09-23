#include "automata.h"
#include <stack>
#include <iostream>

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

Graph::Graph() {
    size = 0;
}

bool Graph::add_edge(int index1, int index2, string reg_expr){
    if(index1< size && index2 < size){
        Edge aux(index1, index2, reg_expr);
        edge_list.push_front(aux);
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

Operation Automata::expr_splitter(string orig_expr) {
    string reg_expr1, reg_expr2;
    char lang_operator;
    int num_open_parentheses = 0;
    bool there_is_union = false,
            there_is_cat = false;

    for (int i = 0; i < orig_expr.size() && !there_is_union; ++i) {
        if(num_open_parentheses == 0 && i!=0){
            if(orig_expr[i] == '+'){ // União
                reg_expr1 = string(orig_expr.begin(), orig_expr.begin()+i);
                reg_expr2 = string(orig_expr.begin()+i+1, orig_expr.end());
                lang_operator = '+';
                there_is_union = true;
            }
            else if(isalpha(orig_expr[i])){ // Concatenação com um char
                reg_expr1 = string(orig_expr.begin(), orig_expr.begin()+i);
                reg_expr2 = string(orig_expr.begin()+i, orig_expr.end());
                lang_operator = 'c';
                there_is_cat = true;
            }
            else if(orig_expr[i] == '('){ // Concatenação com uma expressão entre parenteses
                reg_expr1 = string(orig_expr.begin(), orig_expr.begin()+i);
                reg_expr2 = string(orig_expr.begin()+i+1, orig_expr.end()-1);
                lang_operator = 'c';
                there_is_cat = true;
            }
            else if(orig_expr[i] == '*' && !there_is_cat){  // Feixo de Kleene
                if (isalpha(orig_expr[i-1])){ // a expressão é um único char
                    reg_expr1 = string(orig_expr.begin(), orig_expr.end()-1);
                    lang_operator = '*';
                }
                else{// a expressão é limitado por parenteses
                    reg_expr1 = string(orig_expr.begin()+1, orig_expr.end()-2); // não considera os parenreses inicial e final
                    lang_operator = '*';
                }

            }
//            else {//error
//                lang_operator = 'x';
//            }
        }

        if(orig_expr[i]=='('){
            num_open_parentheses++;
        }
        else if(orig_expr[i]==')'){
            num_open_parentheses--;
        }

    }
    Operation answer(reg_expr1, reg_expr2, lang_operator);
    return  answer;
}

Automata::Automata(string reg_expr) {
    // Construtor
    size = 2;

    automata_graph.new_node(false);//nó inicial (nó de num 0)
    automata_graph.new_node(true);//nó final (nó de num 1)
    automata_graph.add_edge(0, 1, reg_expr);

    stack<list<Edge>::iterator> remaining_edges; // armazena o ponteiro das aresas que podem ser reduzidas
    remaining_edges.push(automata_graph.edge_list.begin()); // adiciona a única aresta que o grafo tem por enquanto

    while (!remaining_edges.empty()){ //enquanto houver arestas que podem ser reduzidas
        auto local_edge_ptr = remaining_edges.top();
        Operation aux_op = expr_splitter(local_edge_ptr->reg_expr);

        if (aux_op.lang_operator == '+'){ // União
            local_edge_ptr->reg_expr = aux_op.reg_expr1; //atualiza a expr reg da aresta já existente para uma das da união

            // e adiciona uma nova aresta com a outra expr reg:
            automata_graph.add_edge(local_edge_ptr->node1, local_edge_ptr->node2, aux_op.reg_expr2);
            remaining_edges.push(automata_graph.edge_list.begin());
        }
        else if (aux_op.lang_operator == 'c') {// concatenação
            int local_new_node = automata_graph.new_node(false); // cria um novo nó (que ficará entre os dois analisados)

            // deleta a anterior da pilha:
            remaining_edges.pop();

            // e add as arestas:
            automata_graph.add_edge(local_edge_ptr->node1, local_new_node, aux_op.reg_expr1);
            remaining_edges.push(automata_graph.edge_list.begin());
            automata_graph.add_edge(local_new_node, local_edge_ptr->node2, aux_op.reg_expr2);
            remaining_edges.push(automata_graph.edge_list.begin());

            // e deleta a anterior do grafo
            automata_graph.edge_list.erase(local_edge_ptr);
        }
        else if (aux_op.lang_operator == '*'){// feixo de Kleene
            int local_new_node = automata_graph.new_node(false); // cria um novo nó (que ficará entre os dois analisados)

            // deleta a anterior da pilha:
            remaining_edges.pop();

            // add as arestas:
            automata_graph.add_edge(local_edge_ptr->node1, local_new_node, "&");
            remaining_edges.push(automata_graph.edge_list.begin());
            automata_graph.add_edge(local_new_node, local_edge_ptr->node2, "&");
            remaining_edges.push(automata_graph.edge_list.begin());
            automata_graph.add_edge(local_new_node, local_new_node, aux_op.reg_expr1);
            remaining_edges.push(automata_graph.edge_list.begin());

            // e deleta a anterior do grafo
            automata_graph.edge_list.erase(local_edge_ptr);
        }
        else
            remaining_edges.pop(); // não há nenhuma operação na expressão da aresta, logo não precisa mais ser reduzida


    }
}

void Automata::show_graph() {
    for (auto edge : automata_graph.edge_list) {
        cout<<"("<<edge.node1<<")--- "<<edge.reg_expr<<" ---("<<edge.node2<<")"<<endl;
    }
}
