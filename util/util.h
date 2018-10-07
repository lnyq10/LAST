//
// Created by niqiu on 18-9-24.
//

#ifndef ABC_LAST_UTIL_H
#define ABC_LAST_UTIL_H

#include "abc_api.h"
#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <cmath>

using namespace abc;
using namespace std;

//void Abc_NodeMffcConeSuppPrint( Abc_Obj_t * pNode );
void NodeMffcConeSupp(Abc_Obj_t* pNode, Vec_Ptr_t* vCone, Vec_Ptr_t* vSupp, bool print);

Abc_Ntk_t* ReadBlif(char* filename);

vector<Abc_Ntk_t*> getMffcNtk(Abc_Ntk_t* pNtk, int ConeSize);
//EFFECT: Get all the sub-Mffc-network with specified size

map<int ,int> getNtkSetInfo(vector<Abc_Ntk_t*> pNtkSet);
//EFFECT: ix->first represents the size of the cone. ix->second represents
//Home many times the cone of this size appear in pNtkSet

void map_LUT(char* old_filename, char* new_filename);
class truth_table{
private:
    int** truthtable;
    int row;
    int column;
public:
    void print_truth_table();
    truth_table(int** truth_table, int row, int column){
        this->truthtable=truth_table;
        this->column=column;
        this->row=row;
    }
    ~truth_table(){
        for(int i=0; i<column; i++){
            delete[] truthtable[i];
        }
        delete[] truthtable;
    }
};

int inverse(int num);

void binary(int* array, int size, int num);

void simulate_node(Abc_Ntk_t* LUT_network, Abc_Obj_t* pnode);

int simulation(Abc_Ntk_t* LUT_network, int* bound_set, int* free_set, int bound_size, int free_size, int input_row, int input_column);

truth_table create_truth_table(Abc_Ntk_t* LUT_network, int* bound_set, int* free_set, int num);

#endif //ABC_LAST_UTIL_H
