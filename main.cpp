#include <iostream>
#include <abc_api.h>
#include <util.h>
#include <vector>
#include <map>
#include <network.h>

using namespace abc;
using namespace std;

void sta();
void nodetest();
void simu_test();
void print_node_info(Abc_Ntk_t* blif_ntk);
void transform(char* filename);

int main() {

    //char* filename = (char*) "./benchmark/pm1.blif";
    char* newfilename = (char*) "./benchmark/z4ml.blif";        //the name of the file which we use to store the new LUT network
    //map_LUT(filename, newfilename);
    auto blif_ntk = ReadBlif(newfilename);
    int first[4]={1, 2, 3, 4};
    int second[12]={5,6,7};
    truth_table truthTable=create_truth_table(blif_ntk,first,second,7);
    truthTable.print_truth_table();


}

void transform(char* filename){
    Abc_Start();
    char Command[1000];
    abc::Abc_Frame_t * pAbc = abc::Abc_FrameGetGlobalFrame();
    sprintf( Command, "read_genlib -v %s", "./abc/lib/mcnc.genlib" );
    if ( abc::Cmd_CommandExecute( pAbc, Command ) )
    {
        fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
        assert( 0 );
    }
    sprintf( Command, "read_blif ./benchmark/%s", filename);
    if ( abc::Cmd_CommandExecute( pAbc, Command ) )
    {
        fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
        assert( 0 );
    }
    sprintf( Command, "map") ;
    if ( abc::Cmd_CommandExecute( pAbc, Command ) )
    {
        fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
        assert( 0 );
    }
    sprintf( Command, "write_blif ./benchmark/new%s", filename) ;
    if ( abc::Cmd_CommandExecute( pAbc, Command ) )
    {
        fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
        assert( 0 );
    }
    Abc_Stop();
}


void simu_test(){
    Abc_Start();
    abc::Abc_Frame_t * pAbc = abc::Abc_FrameGetGlobalFrame();
    char Command[1000];
    sprintf( Command, "read_genlib -v %s", "./abc/lib/mcnc.genlib" );
    if ( abc::Cmd_CommandExecute( pAbc, Command ) )
    {
        fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
        assert( 0 );
    }
    // input the original circuit
    //sprintf( Command, "read ./benchmark/c6288.blif");
    sprintf( Command, "read ./benchmark/ectl2.blif");
    if ( abc::Cmd_CommandExecute( pAbc, Command ) )
    {
        fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
        assert( 0 );
    }
    auto pNtk = Abc_FrameReadNtk(pAbc);
    //char* in_filename = (char*) "./in/1000/c6288.in";
    char* in_filename = (char*) "./in/ectl.in";

    user::network_t ntk(pNtk, 10, in_filename);
    clock_t t = clock();
    ntk.GetAllValue(1);
    cout << "Simulation time: "<<clock() - t << endl;


    abc::Abc_Stop();
}


void nodetest(){
    char* filename = (char*) "./benchmark/ectl.blif";
    auto blif_ntk = ReadBlif(filename);

    auto i = Abc_NtkObjNum(blif_ntk);
    auto node0 = Abc_NtkObj(blif_ntk, 1);
    auto node1 = Abc_NtkObj(blif_ntk, 2);
    auto node2 = Abc_NtkObj(blif_ntk, 3);
    //auto muxNode = Abc_NtkCreateNodeMux(blif_ntk, node0, node1, node2);
    //Abc_NtkDeleteObj(node0);

    //cout << Abc_ObjFanoutNum(victim) << endl;
    //print_node_info(blif_ntk);
    Abc_Obj_t* victim = Abc_NtkObj(blif_ntk, 21);
    /*
    Vec_Ptr_t* vNodes;
    vNodes = Vec_PtrAlloc( 100 );
    Abc_NodeCollectFanouts( victim, vNodes );
    for ( i = 0; i < vNodes->nSize; i++ )
        Abc_ObjDeleteFanin( (Abc_Obj_t *)vNodes->pArray[i], victim );
    Vec_PtrFree( vNodes );
     */

    //Abc_NtkDeleteAll_rec(victim);
    cout << Abc_ObjName(victim) << endl;
    victim->pData = (char*)"1011 1\n";
    Abc_Obj_t* pNode;
    int j;
    Abc_NtkForEachNode(blif_ntk, pNode, j){
            cout << "Node " << Abc_ObjName(pNode) << " has pData: " << endl;
            cout << "Node level " << pNode->Level << endl;
            cout << (char*)pNode->pData << endl;
        }
    Io_WriteBlifLogic(blif_ntk, (char*) "out.blif", 1);
    Abc_NtkDelete(blif_ntk);
}



void sta(){
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
}

void print_node_info(Abc_Ntk_t* blif_ntk){
    int j;
    Abc_Obj_t* pNode;
    Abc_NtkForEachNode(blif_ntk, pNode, j){
            cout << "The " << j << "th object has ID: " << Abc_ObjId(pNode)\
        << " and Name: " << Abc_ObjName(pNode) << endl;
            cout << "It has fanout number" << Abc_ObjFanoutNum(pNode) << endl;
        }

}


