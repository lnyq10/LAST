#include <iostream>
#include <abc_api.h>
#include <util.h>
#include <vector>
#include <map>

using namespace abc;
using namespace std;


int main() {
    char* filename = (char*) "./benchmark/hyp.blif";
    auto blif_ntk = ReadBlif(filename);

    vector<Abc_Ntk_t*> pNtkSet = getMffcNtk(blif_ntk, 7);
    map<int, int> pNtkSetInfo = getNtkSetInfo(pNtkSet);
    auto jx = pNtkSetInfo.begin();
    for(jx = jx; jx != pNtkSetInfo.end(); jx++ ){
        cout << "The cone size " << jx->first << " appears "\
             << jx->second << " Times" << endl;
    }


    vector<Abc_Ntk_t*>::size_type ix;
    for(ix = 0; ix != pNtkSet.size(); ix++) {
        //Io_WriteBlifLogic(pNtkSet[ix], (char *) "out.blif", 0);
        Abc_NtkDelete(pNtkSet[ix]);
    }

    Abc_NtkDelete(blif_ntk);
    return 0;
}


