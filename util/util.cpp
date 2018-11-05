//
// Created by niqiu on 18-9-24.
//

#include "util.h"
/**Function*************************************************************

  Synopsis    []

  Description [Print information from the vector containing all the Mffcs of pNtk]

***********************************************************************/
void printMffcSetInfo(vector<Mffc*> pNtkSet, bool isNode){
    map<int, int> pNtkSetInfo = isNode?
            getMffcSetNodeInfo(pNtkSet): getMffcSetPiInfo(pNtkSet);
    auto jx = pNtkSetInfo.begin();
    for(jx = jx; jx != pNtkSetInfo.end(); jx++ ){
        if(isNode) cout << "The cone size ";
        else cout << "The Pi size ";
        cout << jx->first << " appears "\
             << jx->second << " Times" << endl;
    }
}
/**Function*************************************************************

  Synopsis    []

  Description [Obtain information from the vector containing all the Mffcs of pNtk]

***********************************************************************/

map<int ,int> getMffcSetNodeInfo(vector<Mffc*> pNtkSet){
    vector<Abc_Ntk_t*>::size_type ix;
    map<int, int> pNtkSetInfo;
    int ConeSize;
    for(ix = 0; ix != pNtkSet.size(); ix++){
        ConeSize = Abc_NtkNodeNum(pNtkSet[ix]->ref);
        pNtkSetInfo[ConeSize]++;
    }
    return pNtkSetInfo;
}
/**Function*************************************************************

  Synopsis    []

  Description [Obtain information from the vector containing all the Mffcs of pNtk]

***********************************************************************/

map<int ,int> getMffcSetPiInfo(vector<Mffc*> pNtkSet){
    vector<Abc_Ntk_t*>::size_type ix;
    map<int, int> pMffcSetInfo;
    int PiSize;
    for(ix = 0; ix != pNtkSet.size(); ix++){
        PiSize = Abc_NtkPiNum(pNtkSet[ix]->ref);
        pMffcSetInfo[PiSize]++;
    }
    return pMffcSetInfo;
}


/**Function*************************************************************

  Synopsis    []

  Description []

***********************************************************************/
void NodeMffcConeSupp(Abc_Obj_t* pNode, Vec_Ptr_t* vCone, Vec_Ptr_t* vSupp, bool print){
    Abc_NodeDeref_rec(pNode);
    Abc_NodeMffcConeSupp(pNode, vCone, vSupp);
    Abc_NodeRef_rec(pNode);

    if(print) {
        printf("Printing the MFFC for node %s\n", Abc_ObjName(pNode));
        Abc_Obj_t *pObj;
        int i;
        printf("Node = %6s : Supp = %3d  Cone = %3d  (",
               Abc_ObjName(pNode), Vec_PtrSize(vSupp), Vec_PtrSize(vCone));
        Vec_PtrForEachEntry(Abc_Obj_t *, vCone, pObj, i)printf(" %s", Abc_ObjName(pObj));
        printf(" )\n");
    }
}
/**Function*************************************************************

  Synopsis    []

  Description []

***********************************************************************/
Abc_Ntk_t* ReadBlif(char* filename){
    Abc_Ntk_t* blif_ntk = Io_ReadBlif(filename,1);
    blif_ntk = Abc_NtkToLogic(blif_ntk);
    return blif_ntk;
}
/**Function*************************************************************

  Synopsis    []

  Description [Retrieve all the Mffcs in pNtk]

***********************************************************************/
vector<Mffc*> getMffcNtk(Abc_Ntk_t* pNtk, int minPiSize, int maxPiSize){
    Vec_Ptr_t *vCone, *vSupp;
    vector<Mffc*> pNtkSet;

    int i = 0;
    Abc_Obj_t* pNode;
    Abc_NtkForEachNode(pNtk, pNode, i){
        auto pMffc = new Mffc;
        pMffc->root = pNode;
        //cout << "GetMffcNtk fron node " << Abc_ObjName(pNode) << endl;
        vCone = Vec_PtrAlloc(100);
        vSupp = Vec_PtrAlloc(100);
        NodeMffcConeSupp(pNode, vCone, vSupp, false);
        Abc_Obj_t* qNode;
        int j;
        //cout << "Mffc size is: " << Abc_NodeMffcSize(pNode) << endl;

        if(vSupp->nSize >= minPiSize && vSupp->nSize <= maxPiSize) {
            auto temp = (Abc_Obj_t *) vCone->pArray[vCone->nSize - 1];
            Abc_Ntk_t *pNtkNew = Abc_NtkCreateMffc(pNtk, temp, Abc_ObjName(pNode));
            //std::cout << Vec_PtrSize(vCone) << " " << Abc_NtkNodeNum(pNtkNew) << endl;
            pMffc->ref = pNtkNew;
            Vec_PtrForEachEntry(Abc_Obj_t*, vSupp, qNode, j){
                pMffc->Pi_origin.push_back(Abc_NtkObj(pNtk,qNode->Id));
            }
            pNtkSet.push_back(pMffc);

        }
        else{
            delete pMffc;
        }

        Vec_PtrFree(vCone);
        Vec_PtrFree(vSupp);
    }
    return pNtkSet;
}



/*void map_LUT(char* old_filename, char* new_filename){
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
}*/
Mffc::~Mffc(){
    Abc_NtkDelete(this->ref);
}