//
// Created by niqiu on 18-9-24.
//

#ifndef ABC_LAST_UTIL_H
#define ABC_LAST_UTIL_H

#include "abc_api.h"

using namespace abc;

//void Abc_NodeMffcConeSuppPrint( Abc_Obj_t * pNode );
void NodeMffcConeSupp(Abc_Obj_t* pNode, Vec_Ptr_t* vCone, Vec_Ptr_t* vSupp, bool print);
Abc_Ntk_t* ReadBlif(char* filename);



#endif //ABC_LAST_UTIL_H