//
// Created by frank on 18-10-17.
//
#include "simulation.h"

void decomposed_chart::print_weight() {
    for(int i=0; i<column_size; i++){
        std::cout<<" |"<<i;
    }
    std::cout<<" |"<<std::endl;
    for(int i=0; i<row_size; i++){
        for(int j=0; j<=column_size; j++){
            std::cout<<"---";
        }
        std::cout<<std::endl;
        std::cout<<i<<"| ";
        for(int j=0; j<column_size; j++){
            std::cout<<setiosflags(ios::fixed)<<setprecision(4)<<weight[j][i]<<" |";
        }
        std::cout<<std::endl;
    }
}
void decomposed_chart::print_decomposed_chart() {
    for(int i=0; i<column_size; i++){
        std::cout<<" |"<<i;
    }
    std::cout<<" |"<<std::endl;
    for(int i=0; i<row_size; i++){
        for(int j=0; j<=column_size; j++){
            std::cout<<"---";
        }
        std::cout<<std::endl;
        std::cout<<i<<"| ";
        for(int j=0; j<column_size; j++){
            std::cout<<decomposedchart[j][i]<<" |";
        }
        std::cout<<std::endl;
    }
}

void decomposed_chart::clear_weight() {
    for(int i=0; i<this->column; i++){
        for(int j=0; j<this->row; j++){
            this->weight[i][j]=0;
        }
    }
}

int inverse(int num){
    if(num==1) return 0;
    else if(num==0) return 1;
    else std::cout<<"the input can only be 0 or 1!"<<std::endl;
    return 0;
}

int binary_inverse(int* array, int size){
    int result=0;
    for(int i=0; i<size; i++){
        result+=array[i]*pow(2,i);
    }
    return result;
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
            pnode->iTemp+=pnode->dTemp;
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
        if(level>=0) {
            Vec_PtrPush(Nodes[level], pNode);
        }
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

decomposed_chart create_decomposed_chart(Abc_Ntk_t* LUT_network, int* bound_set, int* free_set, int num){
    int row=pow(2,num-4);
    int column=pow(2,4);
    int** decomposedchart=new int*[column];
    for(int n=0; n<column; n++){
        decomposedchart[n]=new int[row];
    }
    for(int i=0; i<column; i++){
        for(int j=0; j<row; j++){
            decomposedchart[i][j]=simulation(LUT_network, bound_set, free_set, 4, num-4, j, i);
        }
    }
    decomposed_chart decomposedChart=decomposed_chart(decomposedchart, num-4, 4, free_set, bound_set);
    return decomposedChart;
}

void simulate_weight(decomposed_chart* decomposedChart, Abc_Ntk_t* MFFC_network, vector<Abc_Obj_t*> input){
    int row[decomposedChart->row];
    int column[decomposedChart->column];
    int x, y;
    Abc_Obj_t* PNode;
    for(int m=0; m<decomposedChart->row; m++){
        row[m]=input[decomposedChart->array_row[m]-1]->dTemp;
    }
    for(int m=0; m<decomposedChart->column; m++){
        column[m]=input[decomposedChart->array_column[m]-1]->dTemp;
    }
    x=binary_inverse(column, decomposedChart->column);
    y=binary_inverse(row, decomposedChart->row);
    decomposedChart->weight[x][y]++;
}

//create a weight for the given MFFC circuit and the old LUT circuit, remember the weight is stored in the class decomposed_chart, the weight is a 2d double array, just like the decomposed chart
//and you can use print_weight to print it
void create_weight(decomposed_chart* decomposedChart, Abc_Ntk_t* MFFC_network, Abc_Ntk_t* LUT_network, vector<Abc_Obj_t*> input){
    srand(time(NULL));
    decomposedChart->clear_weight();
    int inputsize=LUT_network->vPis->nSize;
    int max_level=0;
    int count=100000;
    Abc_Obj_t* pnode;
    Abc_Obj_t* pin;
    for(int i=0; i<input.size(); i++){                    //find the max level of the Vins of the MFFC circuit
        pnode=Abc_NtkObj(LUT_network,input[i]->Id);
        if(pnode->Level>max_level){
            max_level=pnode->Level;
        }
    }
    Vec_Ptr_t** Nodes=new Vec_Ptr_t*[max_level];
    for(int k=0;k<max_level;k++){
        Nodes[k]=Vec_PtrAlloc(16);
    }
    Abc_Obj_t* pNode;
    int j;
    Abc_NtkForEachNode(LUT_network, pNode, j){            //store the nodes in LUT circuit base on their level, remember here we only need to store the nodes with lower level than the max level
        int level=pNode->Level-1;
        if(level<max_level && level>=0)
            Vec_PtrPush(Nodes[level], pNode);
    }
    for(int n=0; n<count; n++){                           //I'll use 100000 random input to calculate the weight
        int *input_condition=new int[inputsize];
        for(int u=0; u<inputsize; u++){
            input_condition[u]=rand()%2;
        }
        for(j=0; j<LUT_network->vPis->nSize; j++){
            pin=(Abc_Obj_t*)(LUT_network->vPis->pArray[j]);
            pin->dTemp=input_condition[j];
            pin->iTemp+=pin->dTemp;
        }
        for(int n=0; n<max_level; n++){
            int size=Nodes[n]->nSize;
            for(int i=0; i<size; i++){
                Abc_Obj_t* output_node=(Abc_Obj_t*)Nodes[n]->pArray[i];
                simulate_node(LUT_network, output_node);
            }
        }
        simulate_weight(decomposedChart,MFFC_network,input);
        delete[] input_condition;
    }
    for(int o=0; o<decomposedChart->column_size; o++){
        for(int p=0; p<decomposedChart->row_size; p++){
            decomposedChart->weight[o][p]=(double)((decomposedChart->weight[o][p])/100000);
        }
    }
    for(int k=0;k<max_level;k++){
        Vec_PtrFree(Nodes[k]);
    }
    delete[] Nodes;
}
