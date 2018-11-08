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


using namespace abc;
using namespace std;

class decomposed_chart{
public:
    int** decompose_chart;
    double ** weight;
    int* array_row;
    int* array_column;
    int row;
    int column;
    int row_size;
    int column_size;

    void print_decomposed_chart();
    void print_weight();
    void clear_weight();
    void change_set(int* bound_set, int* free_set);
    decomposed_chart(int** truth_table, int row, int column, int* array_row, int* array_column){
        this->decompose_chart=truth_table;
        this->column=column;
        this->row=row;
        this->array_column=array_column;
        this->array_row=array_row;
        this->row_size=pow(2, row);
        this->column_size=pow(2, column);
        weight=new double*[column_size];
        for(int i=0; i<column_size; i++){
            weight[i]=new double[row_size];
        }
        for(int i=0; i<column_size; i++){
            for(int j=0; j<row_size; j++){
                weight[i][j]=0;
            }
        }
    }
    ~decomposed_chart(){
        for(int i=0; i<column; i++){
            delete[] decompose_chart[i];
        }
        delete[] decompose_chart;
        //delete[] array_row;
        //delete[] array_column;
        for(int i=0; i<column; i++){
            delete[] weight[i];
        }
        delete[] weight;
    }
};

int inverse(int num);

void binary(int* array, int size, int num);

void simulate_node(Abc_Ntk_t* LUT_network, Abc_Obj_t* pnode);

int simulation(Abc_Ntk_t* LUT_network, int* bound_set, int* free_set,\
        int bound_size, int free_size, int input_row, int input_column);

decomposed_chart* create_decomposed_chart(Abc_Ntk_t* LUT_network, int* bound_set,\
        int* free_set, int num);

int binary_inverse(int* array, int size);

void simulate_weight(decomposed_chart* decomposedChart, \
        Abc_Ntk_t* MFFC_network, vector<Abc_Obj_t*> input);

void create_weight(decomposed_chart* decomposedChart, \
        Abc_Ntk_t* MFFC_network, Abc_Ntk_t* LUT_network, vector<Abc_Obj_t*> input);

void simulate_whole(Abc_Ntk_t* LUT_network);

void Create_Weight(decomposed_chart* decomposedChart, Mffc MFFC_network, int* bound_set, int* free_set);

#endif //ABC_LAST_SIMULATION_H
