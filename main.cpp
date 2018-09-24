#include <iostream>
#include <abc_api.h>
#include <util.h>
#include <vector>

using namespace abc;
using namespace std;

int main() {
    char* filename = (char*) "./benchmark/ectl.blif";
    auto blif_ntk = ReadBlif(filename);

    Vec_Ptr_t *vCone, *vSupp;
    vector<Abc_Ntk_t*> pNtkSet;

    int i = 0;
    Abc_Obj_t* pNode;
    Abc_NtkForEachNode(blif_ntk, pNode, i){

            vCone = Vec_PtrAlloc(100);
            vSupp = Vec_PtrAlloc(100);
            NodeMffcConeSupp(pNode, vCone, vSupp, true);

            if(vCone->nSize >= 4) {
                auto temp = (Abc_Obj_t *) vCone->pArray[0];
                Abc_Ntk_t *pNtkNew = Abc_NtkCreateMffc(blif_ntk, temp, Abc_ObjName(pNode));
                pNtkSet.push_back(pNtkNew);
            }

            Vec_PtrFree(vCone);
            Vec_PtrFree(vSupp);
    }


    vector<Abc_Ntk_t*>::size_type ix;
    for(ix = 0; ix != pNtkSet.size(); ix++)
        Abc_NtkDelete(pNtkSet[ix]);

    Abc_NtkDelete(blif_ntk);
    return 0;
}



