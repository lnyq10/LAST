//
// Created by niqiu on 18-10-20.
//

#ifndef ABC_LAST_APPROX_H
#define ABC_LAST_APPROX_H

#include "simulation.h"
#include "util.h"
#include "algorithm"
#include <sstream>

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


Mffc* local_approx(Abc_Ntk_t* pNtk, Mffc* mffc, int PiSize);
int* choose_four(const int* arr, int size, int index);
void modify_mffc(Abc_Ntk_t* mffc, pattern* pt, int* bset, int* fset, int fSetSize);

#endif //ABC_LAST_APPROX_H
static inline void Vec_IntPushMem( Mem_Step_t * pMemMan, Vec_Int_t * p, int Entry )
{
    if ( p->nSize == p->nCap )
    {
        int * pArray;
        int i;

        if ( p->nSize == 0 )
            p->nCap = 1;
        if ( pMemMan )
            pArray = (int *)Mem_StepEntryFetch( pMemMan, p->nCap * 8 );
        else
            pArray = ABC_ALLOC( int, p->nCap * 2 );
        if ( p->pArray )
        {
            for ( i = 0; i < p->nSize; i++ )
                pArray[i] = p->pArray[i];
            if ( pMemMan )
                Mem_StepEntryRecycle( pMemMan, (char *)p->pArray, p->nCap * 4 );
            else
                ABC_FREE( p->pArray );
        }
        p->nCap *= 2;
        p->pArray = pArray;
    }
    p->pArray[p->nSize++] = Entry;
}