#include <iostream>
#include <abc_api.h>
#include <util.h>
#include <vector>

using namespace abc;
using namespace std;

int main() {
    //Abc_Obj_t
    /*
    abc::Abc_Obj_t_ niqiu;
    auto* origin_ntk = Io_ReadBlif((char*)"./benchmark/ctrl.blif", 1);
    std::cout << "Hello, World!" << std::endl;
    Abc_Frame_t * pAbc;
    Abc_Start();
    pAbc = Abc_FrameGetGlobalFrame();
    auto read=(char*)"read_blif ./benchmark/ctrl.blif";
    auto convert=(char*)"if -K 4";
    auto write=(char*)"write_blif ./Lut_circuit/ctrl.blif";
    Cmd_CommandExecute(pAbc, read);
    Cmd_CommandExecute(pAbc, convert);
    Cmd_CommandExecute(pAbc, write);
    Abc_Stop();
    return 0;
    */
    char* filename = (char*) "./benchmark/ectl.blif";
    //Abc_Ntk_t* blif_ntk = Io_ReadBlif(filename,1);
    auto blif_ntk = ReadBlif(filename);


    Vec_Ptr_t *vCone, *vSupp;

    int i = 0;
    Abc_Obj_t* pNode;
    Abc_NtkForEachNode(blif_ntk, pNode, i){

        vCone = Vec_PtrAlloc(100);
        vSupp = Vec_PtrAlloc(100);
        cout << "Node name is " << Abc_ObjName(pNode) << endl;
        cout << "Fanin is " << Abc_ObjName(Abc_ObjFanin(pNode, 1)) << endl;
        NodeMffcConeSupp(pNode, vCone, vSupp, true);
        Vec_PtrFree(vCone);
        Vec_PtrFree(vSupp);
    }

    return 0;
}


