#include <iostream>
#include <abc_api.h>
#include <vector>

using namespace abc;

void Abc_NodeMffcConeSuppPrint(Abc_Obj_t* pObj);

int main() {
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
    char* filename = (char*) "./benchmark/div.blif";
    Abc_Ntk_t* blif_ntk = Io_ReadBlif(filename,1);
    blif_ntk = Abc_NtkToLogic(blif_ntk);
    int NodeNum = Abc_NtkNodeNum(blif_ntk);



    std::vector<std::vector<Vec_Ptr_t*>> nodeSet_vec;
    Vec_Ptr_t *vCone, *vSupp;


    int i = 0;
    Abc_Obj_t* pNode;
    Abc_NtkForEachNode(blif_ntk, pNode, i){
        int num = Abc_NodeMffcSize(pNode);

        vCone = Vec_PtrAlloc(100);
        vSupp = Vec_PtrAlloc(100);
        Abc_NodeMffcConeSupp(pNode, vCone, vSupp);
        std::cout << Vec_PtrSize(vCone) << std::endl;
        std::cout << "The supp size is " << vSupp->nSize << std::endl;
        Abc_NodeMffcConeSuppPrint(pNode);
        Vec_PtrFree(vCone);
        Vec_PtrFree(vSupp);
    }

    return 0;
}


void Abc_NodeMffcConeSuppPrint( Abc_Obj_t * pNode )
{
    Vec_Ptr_t * vCone, * vSupp;
    Abc_Obj_t * pObj;
    int i;
    vCone = Vec_PtrAlloc( 100 );
    vSupp = Vec_PtrAlloc( 100 );
    Abc_NodeDeref_rec( pNode );
    Abc_NodeMffcConeSupp( pNode, vCone, vSupp );
    Abc_NodeRef_rec( pNode );
    printf( "Node = %6s : Supp = %3d  Cone = %3d  (",
            Abc_ObjName(pNode), Vec_PtrSize(vSupp), Vec_PtrSize(vCone) );
    Vec_PtrForEachEntry( Abc_Obj_t *, vCone, pObj, i )
        printf( " %s", Abc_ObjName(pObj) );
    printf( " )\n" );
    Vec_PtrFree( vCone );
    Vec_PtrFree( vSupp );
}