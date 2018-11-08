#include <iostream>
#include <abc_api.h>
#include <util.h>
#include <vector>
#include <map>

using namespace abc;
using namespace std;

void sta();
void nodetest();
void simu_test();
void print_node_info(Abc_Ntk_t* blif_ntk);
void transform(char* filename);

int main() {
    char* filename = (char*) "./benchmark/i2c.blif";
    char* newfilename = (char*) "out.blif";        //the name of the file which we use to store the new LUT network
    //map_LUT(filename, newfilename);
    Abc_Ntk_t* blif_ntk = ReadBlif(filename);
    Abc_Obj_t* pnode=Abc_NtkObj(blif_ntk, 4);
    //cout<<pnode->Id<<pnode->Type<<blif_ntk->nObjCounts[3]<<endl;    //these cout are just used for testing
    //cout<<Vec_PtrSize((blif_ntk)->vObjs)<<endl;
    //cout<<Abc_ObjName(pnode)<<endl;
    auto mffc = getMffcNtk(blif_ntk, 10);
    cout<<mffc.size()<<endl;
    auto mffc1 = mffc[0];
    int a[4]={1,2,3,4,};         //bound set
    //int b[mffc1.Pi_origin.size()-4];          //free set
    //for(int i=0; i<mffc1.Pi_origin.size()-4; i++){              //set the original value of the free set
        //b[i]=5+i;
    //}
    int b[5]={5,6,7,8,9};
    /*cout<<"Name in Pi_origin"<<endl;
    for(int i=0; i<mffc1.Pi_origin.size(); i++){
        cout<<Abc_ObjName(mffc1.Pi_origin[i])<<endl;             //used for testing, ignore this
    }
    cout<<"Name in ref->vPis"<<endl;
    for(int i=0; i<mffc1.ref->vPis->nSize; i++){
        Abc_Obj_t* PNOde=Abc_NtkObj(mffc1.ref,i+1);
        cout<<Abc_ObjName(PNOde)<<" "<<PNOde->Id<<endl;           //used for testing, ignore this
    }
    cout<<"--------------------------"<<endl;*/
    decomposed_chart decomposedChart=create_decomposed_chart(mffc1.ref,a,b,mffc1.Pi_origin.size());
    decomposed_chart* address=&decomposedChart;               //remember the input of the create_weight is the pointer to a decomposed chart
    simulate_whole(blif_ntk);
    Create_Weight(address,mffc1 ,a,b);
    //create_weight(address,mffc1.ref,blif_ntk,mffc1.Pi_origin);
    decomposedChart.print_weight();
    int c[4]={2,3,4,5};
    int d[5]={1,6,7,8,9};
    //decomposedChart.change_set(c,d);
    //simulation(mffc1.ref,a,b,4,mffc1.Pi_origin.size()-4,126, 0);
    //pnode=Abc_NtkObj(mffc1.ref,22);
    Io_WriteBlifLogic(mffc1.ref, newfilename, 1);
    Abc_NtkDelete(blif_ntk);
    return 0;
}
