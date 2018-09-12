#include <iostream>
#include <abc_api.h>

using namespace abc;

extern ABC_DLL Abc_Ntk_t * Abc_NtkCreateMffc( Abc_Ntk_t * pNtk, Abc_Obj_t * pNode,
        char * PNodeName);

int main() {
    abc::Abc_Obj_t_ niqiu;
    auto* origin_ntk = Io_ReadBlif((char*)"./benchmark/ctrl.blif", 1);
    std::cout << "Hello, World!" << std::endl;
    Abc_Frame_t * pAbc;
    Abc_Start();
    pAbc = Abc_FrameGetGlobalFrame();
    char* command1="read_blif ./benchmark/ctrl.blif";
    char* command2="if -K 4";
    char* command3="write_blif ./new_mapLUT.blif";
    Cmd_CommandExecute(pAbc, command1);
    Cmd_CommandExecute(pAbc, command2);
    Cmd_CommandExecute(pAbc, command3);
    Abc_End();
    return 0;
}
