#include <iostream>
#include <abc_api.h>

using namespace abc;

extern ABC_DLL Abc_Ntk_t * Abc_NtkCreateMffc( Abc_Ntk_t * pNtk, Abc_Obj_t * pNode,
        char * PNodeName);

int main() {
    abc::Abc_Obj_t_ niqiu;
    auto* origin_ntk = Io_ReadBlif((char*)"./benchmark/ctrl.blif", 1);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}