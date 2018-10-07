//
// Created by niqiu on 18-9-24.
//

#include "util.h"


map<int ,int> getNtkSetInfo(vector<Abc_Ntk_t*> pNtkSet){
    vector<Abc_Ntk_t*>::size_type ix;
    map<int, int> pNtkSetInfo;
    int ConeSize;
    for(ix = 0; ix != pNtkSet.size(); ix++){
        ConeSize = Abc_NtkNodeNum(pNtkSet[ix]);
        pNtkSetInfo[ConeSize]++;
    }
    return pNtkSetInfo;
}


void NodeMffcConeSupp(Abc_Obj_t* pNode, Vec_Ptr_t* vCone, Vec_Ptr_t* vSupp, bool print){
    Abc_NodeDeref_rec(pNode);
    Abc_NodeMffcConeSupp(pNode, vCone, vSupp);
    Abc_NodeRef_rec(pNode);

    if(print) {
        printf("Printing the MFFC for node %s\n", Abc_ObjName(pNode));
        Abc_Obj_t *pObj;
        int i;
        printf("Node = %6s : Supp = %3d  Cone = %3d  (",
               Abc_ObjName(pNode), Vec_PtrSize(vSupp), Vec_PtrSize(vCone));
        Vec_PtrForEachEntry(Abc_Obj_t *, vCone, pObj, i)printf(" %s", Abc_ObjName(pObj));
        printf(" )\n");
    }
}

Abc_Ntk_t* ReadBlif(char* filename){
    Abc_Ntk_t* blif_ntk = Io_ReadBlif(filename,1);
    blif_ntk = Abc_NtkToLogic(blif_ntk);
    return blif_ntk;
}

vector<Abc_Ntk_t*> getMffcNtk(Abc_Ntk_t* pNtk, int ConeSize){
    Vec_Ptr_t *vCone, *vSupp;
    vector<Abc_Ntk_t*> pNtkSet;

    int i = 0;
    Abc_Obj_t* pNode;
    Abc_NtkForEachNode(pNtk, pNode, i){

            vCone = Vec_PtrAlloc(100);
            vSupp = Vec_PtrAlloc(100);
            NodeMffcConeSupp(pNode, vCone, vSupp, false);
            //cout << "Mffc size is: " << Abc_NodeMffcSize(pNode) << endl;

            if(vCone->nSize >= ConeSize) {
                auto temp = (Abc_Obj_t *) vCone->pArray[vCone->nSize - 1];
                Abc_Ntk_t *pNtkNew = Abc_NtkCreateMffc(pNtk, temp, Abc_ObjName(pNode));
                //std::cout << Vec_PtrSize(vCone) << " " << Abc_NtkNodeNum(pNtkNew) << endl;
                pNtkSet.push_back(pNtkNew);
            }

            Vec_PtrFree(vCone);
            Vec_PtrFree(vSupp);
    }
    return pNtkSet;
}

/*void map_LUT(char* old_filename, char* new_filename){
    Abc_Frame_t * pAbc;
    Abc_Start();
    pAbc = Abc_FrameGetGlobalFrame();
    char command[1000];
    sprintf(command,"read_blif %s",old_filename);
    Cmd_CommandExecute(pAbc, command);
    char* command2="if -K 4";
    Cmd_CommandExecute(pAbc, command2);
    sprintf(command,"write_blif %s",new_filename);
    Cmd_CommandExecute(pAbc, command);
    Abc_Stop();
}*/
void truth_table::print_truth_table() {
    for(int i=0; i<column; i++){
        std::cout<<" |"<<i;
    }
    std::cout<<" |"<<std::endl;
    for(int i=0; i<row; i++){
        for(int j=0; j<=column; j++){
            std::cout<<"---";
        }
        std::cout<<std::endl;
        std::cout<<i<<"| ";
        for(int j=0; j<column; j++){
            std::cout<<truthtable[j][i]<<" |";
        }
        std::cout<<std::endl;
    }
}

//inverse
int inverse(int num){
    if(num==1) return 0;
    else if(num==0) return 1;
    else std::cout<<"the input can only be 0 or 1!"<<std::endl;
    return 0;
}

//base on the size of the bound set or free set, it can change an integer into an array, which is the binary form of the integer
//with this function we can convert an integer into the real input case of the bound set or free set
//for example, if the bound set has 4 elements, they are i1, i2, i3, i4, and now I give a value 7, which means i1=i2=i3=1, i4=0
//then the parameter size should be 4, and num should be 7, and the elements stored in array will become array[0]=array[1]=array[2]=1, array[3]=0
void binary(int* array, int size, int num){
    for(int i=0;i<size;i++){
        array[i]=num%2;
        num=num/2;
    }
    return;
}

//simulate one specific node, base on the total network, the value of the Fanin of the node, and the pData, I can get the value of the output of the node, and store it in the dTemp
void simulate_node(Abc_Ntk_t* LUT_network, Abc_Obj_t* pnode){
    char* read=(char*)pnode->pData;
    int output=0;                         //the value of the output
    int check=0;                          //used to check whether the pData reach its end or not
    while(1){
        int save=1;                       //use to temporarily store the value of one line, cause we know sometimes pData may have two or more lines
        for(int i=0;i<pnode->vFanins.nSize;i++){
            if(check==1){
                break;
            }
            auto faninnode=Abc_NtkObj(LUT_network, pnode->vFanins.pArray[i]);
            switch((int)*(read+i)){
                case 49: {
                    save *= faninnode->dTemp;
                    break;
                }
                case 48: {
                    save *= inverse(faninnode->dTemp);
                    break;
                }
                case 45:
                    break;
                case 0: {                 //when it's 0, it means the pData is finished
                    check = 1;
                    break;
                }
            }
        }
        if(check==1){
            pnode->dTemp=output%2;
            break;
        }
        if((int)*(read+pnode->vFanins.nSize+1)==48){                 //if the number represent the output is 0, that means you should inverse the result
            output+=inverse(save);
        }
        else if((int)*(read+pnode->vFanins.nSize+1)==49){
            output+=save;
        }
        read+=pnode->vFanins.nSize+3;
    }
}

//simulate all the network, input_row and input_column is the integer that represent the input case of free set and bound set
int simulation(Abc_Ntk_t* LUT_network, int* bound_set, int* free_set, int bound_size, int free_size, int input_row, int input_column){
    int level_max=Abc_NtkLevel(LUT_network);            //record the whole level of the network
    int output;
    Vec_Ptr_t** Nodes=new Vec_Ptr_t*[level_max];        //base on the level of the network, I use a 2d Vector to store all the nodes with different level
    for(int k=0;k<level_max;k++){
        Nodes[k]=Vec_PtrAlloc(16);
    }
    Abc_Obj_t* pNode;
    int j;
    Abc_NtkForEachNode(LUT_network, pNode, j){
            int level=pNode->Level-1;
            Vec_PtrPush(Nodes[level], pNode);
        }
    int binary_row[free_size];                          //change the integer into the real input case
    binary(binary_row,free_size, input_row);
    int binary_column[bound_size];
    binary(binary_column,bound_size, input_column);
    for(int n=0; n<bound_size; n++){                    //set the dTemp of the PI to be same as the input case
        auto pnode=Abc_NtkObj(LUT_network, bound_set[n]);
        pnode->dTemp=binary_column[n];
    }
    for(int n=0; n<free_size; n++){
        auto pnode=Abc_NtkObj(LUT_network, free_set[n]);
        pnode->dTemp=binary_row[n];
    }
    for(int n=0; n<level_max; n++){                      //simulate all the nodes base on their levels, we should simulate the low-level nodes first, because the high-level nodes may take the low-level nodes as the Fanins
        int size=Nodes[n]->nSize;
        for(int i=0; i<size; i++){
            Abc_Obj_t* output_node=(Abc_Obj_t*)Nodes[n]->pArray[i];
            simulate_node(LUT_network, output_node);
        }
    }
    output=((Abc_Obj_t*)Nodes[level_max-1]->pArray[0])->dTemp;         //since we only concern about he MFFC, so for the MFFC circuit, there will be only one output, that is the node with the highest level, and the dTemp of the node is the output of the whole circuit
    for(int k=0;k<level_max;k++){
        Vec_PtrFree(Nodes[k]);
    }
    delete[] Nodes;
    return output;
}

//create a truth table for a specific circuit
truth_table create_truth_table(Abc_Ntk_t* LUT_network, int* bound_set, int* free_set, int num){
    int row=pow(2,num-4);
    int column=pow(2,4);
    int** truthtable=new int*[column];
    for(int n=0; n<column; n++){
        truthtable[n]=new int[row];
    }
    for(int i=0; i<column; i++){
        for(int j=0; j<row; j++){
            truthtable[i][j]=simulation(LUT_network, bound_set, free_set, 4, num-4, i, j);
        }
    }
    truth_table truetable=truth_table(truthtable, row, column);
    return truetable;
}
