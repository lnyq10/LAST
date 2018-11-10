#include <iostream>
#include <cassert>
#include <abc_api.h>
#include <util.h>
#include <simulation.h>
#include <approx.h>
#include <vector>
#include <map>

using namespace abc;
using namespace std;


void nodetest();
void simu_test();
void print_node_info(Abc_Ntk_t* blif_ntk);
void transform(char* filename);


void printMffcInfo(Abc_Ntk_t* pNtk){
    cout << "Level = " << Abc_NtkLevel(pNtk) << endl;
    cout << "Number of internal nodes = " << Abc_NtkNodeNum(pNtk) << endl;
    cout << "Number of Pi = " << Abc_NtkPiNum(pNtk) << endl;
    assert(Abc_NtkPoNum(pNtk) == 1);
}



int main() {
    Abc_Obj_t* pNode;
    int j;

    char* filename = (char*) "./LUT_benchmark/C880_new.blif";
    char* newfilename = (char*) "out.blif";//the name of the file which we use to store the new LUT network
    char* old = (char*) "origin_mffc.blif";
    //map_LUT(filename, newfilename);
    Abc_Ntk_t* origin_Ntk = ReadBlif(filename);

    //cout << Abc_NtkNodeNum(origin_Ntk) << endl;

    int PiSize = 12;//Change this to change the PiSize you want to approximate

    vector<Mffc*> mffc_set = getMffcNtk(origin_Ntk, PiSize, PiSize);
    //printMffcSetInfo(mffc_set, false);
    vector<Mffc*>::size_type ix = 0;
    Mffc* mffc = nullptr;
    int ctr = 0;
    const int target = 0;

    for(ix = 0; ix != mffc_set.size(); ix++){
        if(Abc_NtkPiNum(mffc_set[ix]->ref) == PiSize){
            mffc = mffc_set[ix];
            ctr++;
            if(ctr == target) break;
        }
    }
    assert(mffc != nullptr);
    //simulate_whole(origin_Ntk);
    //simulate_whole(origin_Ntk);
    Io_WriteBlifLogic(mffc->ref, old, 1);

    auto mffc2 = local_approx(origin_Ntk, mffc, PiSize);

   // int a = 1;
    Io_WriteBlifLogic(mffc->ref, newfilename, 1);
    Abc_NtkDelete(origin_Ntk);
    delete mffc;
    for(ix = 0; ix != mffc_set.size(); ix++){
        if(mffc_set[ix] == mffc) continue;
        delete mffc_set[ix];
    }
    return 0;
}

void Modify_Mffc(Mffc pMffc, Abc_Ntk_t* pNtk){
    //Try to delete a node
    Vec_Ptr_t *vCone, *vSupp;
    Abc_Obj_t* pNode;
    int i,j;

    vCone = Vec_PtrAlloc(100);
    vSupp = Vec_PtrAlloc(100);
    NodeMffcConeSupp(pMffc.root, vCone, vSupp, false);

    Vec_PtrForEachEntry(Abc_Obj_t*, vSupp, pNode, i){
        assert(pNode->pNtk == pNtk);
        cout << "Object name is " << Abc_ObjName(pNode) << endl;
        cout << "It has ID " << Abc_ObjId(pNode) << endl;
        char* victim = (char*) "eee";
        if(strcmp(Abc_ObjName(pNode), victim) == 0) {
            cout << "Deleteing Node " << victim << endl;
            Abc_NtkDeleteObj(pNode);
        }
    }
    Vec_PtrFree(vCone);
    Vec_PtrFree(vSupp);
}


void testMffc(){
    char* filename = (char*) "./benchmark/ctrl.blif";
    char* newfilename = (char*) "out.blif";        //the name of the file which we use to store the new LUT network
    //map_LUT(filename, newfilename);
    Abc_Ntk_t* blif_ntk = ReadBlif(filename);

    auto mffc = getMffcNtk(blif_ntk, 10);
    auto mffc1 = mffc[0];

    //cout << "Root name is " << Abc_ObjName(mffc1.root) << endl;

    //Modify_Mffc(mffc1, blif_ntk);

    Abc_Obj_t* pNode;
    int i,j;
    cout << "Printing information for the reference network" << endl;
    vector<Abc_Obj_t*>::size_type ix = 0;
    for(ix = 0; ix!= mffc1->Pi_origin.size(); ix++){
        cout << "Object name: " << Abc_ObjName(mffc1->Pi_origin[ix]) << endl;
        cout << "Object ID: " << Abc_ObjId(mffc1->Pi_origin[ix]) << endl;
    }
    cout << "*************" << endl;
    Abc_NtkForEachPi(mffc1->ref, pNode, i){
        cout << "Object name: " << Abc_ObjName(pNode) << endl;
        cout << "Object ID: " << Abc_ObjId(pNode) << endl;
    }


    Io_WriteBlifLogic(blif_ntk, newfilename, 1);
    Abc_NtkDelete(blif_ntk);
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


void print_node_info(Abc_Ntk_t* blif_ntk){
    int j;
    Abc_Obj_t* pNode;
    Abc_NtkForEachNode(blif_ntk, pNode, j){
            cout << "The " << j << "th object has ID: " << Abc_ObjId(pNode)\
        << " and Name: " << Abc_ObjName(pNode) << endl;
            cout << "It has fanout number" << Abc_ObjFanoutNum(pNode) << endl;
        }

}

