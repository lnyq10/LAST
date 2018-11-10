//
// Created by niqiu on 18-10-20.
//

#ifndef ABC_LAST_APPROX_H
#define ABC_LAST_APPROX_H

#include "simulation.h"
#include "util.h"
#include "algorithm"
#include <sstream>
#include <vector>

enum pattern_type{
    ALL_ONE, ALL_ZERO, PATTERN, COMPLTD_PATTERN, UNDECIDED
};

const string pattern_name[] = {
        "ALL_ONE", "ALL_ZERO", "PATTERN", "COMPLTD_PATTERN", "UNDECIDED"
};

class pattern{
public:
    pattern(int l, int r_size);
    ~pattern();
    void pattern_cracker(int** dc, double** wt);
    int* p_cracker_helper(int ** dc, double** wt, int row_head, int row_tail);
    double local_error(int ** dc, double** wt, int row_head, int row_tail, int* candidate);
    int* merge(int ** dc, double** wt, int row_head, int row_tail, int* candidates[], int candidate_size);
    void print_info(bool detail);
    vector<int> decoder();
    double get_error(){return error;}
    int get_row_size(){return row_size;}
    pattern_type* get_pattern_type(){return row;};
private:
    int length = 16;
    int row_size;
    int* val;
    double error;
    pattern_type* row;
};


Mffc* local_approx(Abc_Ntk_t* pNtk, Mffc* mffc, int PiSize, int min_level = 0);
int* choose_four(const int* arr, int size, int index);
void modify_mffc(Abc_Ntk_t* mffc, pattern* pt, int* bset, int* fset, int fSetSize, bool initial=true);

#endif //ABC_LAST_APPROX_H
