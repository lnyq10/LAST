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


#endif //ABC_LAST_UTIL_H
