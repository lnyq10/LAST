//
// Created by frank on 18-10-17.
//

#ifndef ABC_LAST_SIMULATION_H
#define ABC_LAST_SIMULATION_H
#include "abc_api.h"
#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <cmath>
#include <iomanip>
#include "util.h"

using namespace abc;
using namespace std;

int inverse(int num);

void binary(int* array, int size, int num);

void simulate_node(Abc_Ntk_t* LUT_network, Abc_Obj_t* pnode);

int simulation(Abc_Ntk_t* LUT_network, int* bound_set, int* free_set, int bound_size, int free_size, int input_row, int input_column);

decomposed_chart create_decomposed_chart(Abc_Ntk_t* LUT_network, int* bound_set, int* free_set, int num);

int binary_inverse(int* array, int size);

void simulate_weight(decomposed_chart* decomposedChart, Abc_Ntk_t* MFFC_network, vector<Abc_Obj_t*> input);

void create_weight(decomposed_chart* decomposedChart, Abc_Ntk_t* MFFC_network, Abc_Ntk_t* LUT_network, vector<Abc_Obj_t*> input);

#endif //ABC_LAST_SIMULATION_H
