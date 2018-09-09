#include <iostream>
#include <abc_api.h>

using namespace abc;


int main() {
    //abc::Abc_Obj_t_ niqiu;
    //auto* origin_ntk = Io_ReadBlif((char*)"./benchmark/test.blif", 1);
    //origin_ntk = Abc_NtkToLogic(origin_ntk);

    //auto Pins = origin_ntk->vPios;
    //auto Pins_deR1 = *Pins->pArray;
    //Io_WriteBlifLogic(origin_ntk, (char*)"./FastAccess/out.blif", 0);

    auto Par = (If_Par_t*) malloc(sizeof(If_Par_t));
    If_ManSetDefaultPars(Par);
    auto Man = If_ManStart(Par);
    //auto i = If_ManPerformMappingComb(Man);


    free(Par);


    auto bar = new If_Obj_t_;
    bar->EstRefs = 2.3;
    delete bar;


    std::cout << "Hello, World!" << std::endl;
    return 0;
}