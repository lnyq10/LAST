//
// Created by niqiu on 18-9-24.
//

#include <cstdio>
#include "util.h"
using namespace abc;

void NodeMffcConeSupp(Abc_Obj_t* pNode, Vec_Ptr_t* vCone, Vec_Ptr_t* vSupp, bool print){
    Abc_NodeDeref_rec(pNode);
    Abc_NodeMffcConeSupp(pNode, vCone, vSupp);
    Abc_NodeRef_rec(pNode);

    if(print) {
        printf("Printing the MFFC for node %s", Abc_ObjName(pNode));
        Abc_Obj_t *pObj;
        int i;
        printf("Node = %6s : Supp = %3d  Cone = %3d  (",
               Abc_ObjName(pNode), Vec_PtrSize(vSupp), Vec_PtrSize(vCone));
        Vec_PtrForEachEntry(Abc_Obj_t *, vCone, pObj, i)printf(" %s", Abc_ObjName(pObj));
        printf(" )\n");
    }
}

Abc_Ntk_t* ReadBlif(char* filename){
    Abc_Ntk_t* blif_ntk = Io_ReadBlif(filename,1);
    blif_ntk = Abc_NtkToLogic(blif_ntk);
    return blif_ntk;
}

void map_LUT(char* old_filename, char* new_filename){
    Abc_Frame_t * pAbc;
    Abc_Start();
    pAbc = Abc_FrameGetGlobalFrame();
    char command[1000];
    sprintf(command,"read_blif %s",old_filename);
    Cmd_CommandExecute(pAbc, command);
    char* command2="if -K 4";
    Cmd_CommandExecute(pAbc, command2);
    sprintf(command,"write_blif %s",new_filename);
    Cmd_CommandExecute(pAbc, command);
    Abc_Stop();
}

int Optimization_Num(Abc_Ntk_t* old_Ntk, bool print){
    int Ideal_Lut;
    int Real_Lut=old_Ntk->nObjCounts[7];
    if(old_Ntk->vCis->nSize<=4){
        Ideal_Lut=1;
    }
    else Ideal_Lut=(old_Ntk->vCis->nSize+1)/3;
    if(print){
        std::cout<<"The MFFC now uses "<<Real_Lut<<" Luts, after the optimization, ideally we can reduce it to "<<Ideal_Lut<<" Luts."<<std::endl;
    }
    return Real_Lut-Ideal_Lut;//the number of LUTs that we can reduce by the optimization.
}
