#include "automata.h"
#include <stack>
#include <iostream>
#include <sstream>

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
    num_nodes = 0;
}

bool Graph::add_edge(int index1, int index2, string reg_expr){
    if(index1< num_nodes && index2 < num_nodes){
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
    num_nodes++;
    return num_nodes-1;
}

Operation Automata::expr_splitter(string orig_expr) {
    string reg_expr1, reg_expr2;
    char lang_operator;
    int num_open_parentheses = 0;
    bool there_is_union = false,
         there_is_cat = false,
         first_parentheses_closed = false;

    for (int i = 0; i < orig_expr.size() && !there_is_union; ++i) {
        if(num_open_parentheses == 0 && i!=0){
            if(orig_expr[i] == '+'){ // União
                reg_expr1 = string(orig_expr.begin(), orig_expr.begin()+i);
                reg_expr2 = string(orig_expr.begin()+i+1, orig_expr.end());
                lang_operator = '+';
                there_is_union = true;
            }
            else if(isalpha(orig_expr[i]) || orig_expr[i] == '(') {
                reg_expr1 = string(orig_expr.begin(), orig_expr.begin() + i);
                reg_expr2 = string(orig_expr.begin() + i, orig_expr.end());
                lang_operator = 'c';
                there_is_cat = true;
            }
            else if(orig_expr[i] == '*' && !there_is_cat){  // Feixo de Kleene
                lang_operator = '*';
                if (isalpha(orig_expr[i-1])){ // a expressão é um único char
                    reg_expr1 = string(orig_expr.begin(), orig_expr.end()-1);
                }
                else{// a expressão é limitado por parenteses
                    reg_expr1 = string(orig_expr.begin()+1, orig_expr.end()-2); // não considera os parenreses inicial e final
                }

            }

        }

        if(orig_expr[i]=='('){
            num_open_parentheses++;
        }
        else if(orig_expr[i]==')'){
            num_open_parentheses--;
            if(num_open_parentheses == 0 && !first_parentheses_closed){ // se o primeiro parenteses é fechado só agora
                first_parentheses_closed = true;
                if(i == orig_expr.size()-1){// e agora é o final da expressão, então ele pode ser removido
                    reg_expr1 = string(orig_expr.begin()+1, orig_expr.end()-1);
                    lang_operator = 'p'; // remoção de parenteses
                }
            }
        }

    }
    Operation answer(reg_expr1, reg_expr2, lang_operator);
    return  answer;
}

Automata::Automata(string reg_expr) {
    // Construtor

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
        else if(aux_op.lang_operator == 'p'){ // remoção de parenteses
            local_edge_ptr->reg_expr = aux_op.reg_expr1; //atualiza a expr reg da aresta já existente, removendo os parenteses
        }
        else
            remaining_edges.pop(); // não há nenhuma operação na expressão da aresta, logo não precisa mais ser reduzida


    }
}

void Automata::show_graph() {
    cout<<"digraph {"<<endl;
    for (auto edge : automata_graph.edge_list) {
        cout<<"\t"<<edge.node1<<" -> "<<edge.node2<<" [label=\""<<edge.reg_expr<<"\"]"<<endl;
    }
    for(int i = 0; i< automata_graph.num_nodes; i++){
        if(automata_graph.is_final_node(i)){
            cout<<"\t"<<i<<" [peripheries=2]"<<endl;
        }
    }
    cout<<"}"<<endl;
}
vector<int> Automata::getNextSates(int node, char letter){ //valores negativos indica que o estado é alcançado por meio de uma transição &
    vector<int> next_states;
    for(auto edge : automata_graph.edge_list){
        if(edge.node1 == node){
            if(edge.reg_expr[0] == letter){
                next_states.push_back(edge.node2);
            }
            if(edge.reg_expr[0] == '&'){
                next_states.push_back(-edge.node2);
            }
        }
    }
    return next_states;
}
string Automata::states_after_computation(int start_node, string str) {
    ostringstream output;

    if (str.size() == 0){ // o estado atual é o estado de fim da computação
        if(automata_graph.is_final_node(start_node)){
            output<<start_node<<" - Final state (String accepted)"<<endl;
        }
        else{
            output<<start_node<<endl;
        }
    }

    vector<int> next_states = getNextSates(start_node, str[0]);
    for(auto next_node : next_states){
        string aux;
        if(next_node > 0){
            aux = string(str.begin()+1, str.end());
        }
        else{ // o nó é alcançado por meio de uma transição epsilon
            aux = str;
        }
        output<<states_after_computation(abs(next_node), aux);
    }
    return output.str();

}
