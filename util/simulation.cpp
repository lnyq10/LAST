//
// Created by frank on 18-10-17.
//
#include "simulation.h"

void decomposed_chart::change_set(int* bound_set, int* free_set) {
    int new_dc[column_size][row_size];
    double new_weight[column_size][row_size];
    for(int n=0; n<column_size; n++){
        for(int m=0; m<row_size;m++){
            new_dc[n][m]=decompose_chart[n][m];
            new_weight[n][m]=weight[n][m];
        }
    }
    int array[column+row];
    int input_column[column];
    int input_row[row];
    int new_column, new_row;
    for(int n=0; n<column_size; n++){
        for(int m=0; m<row_size; m++){
            binary(input_column, column, n);
            binary(input_row, row, m);
            for(int j=0; j<column; j++){
                array[array_column[j]]=input_column[j];
            }
            for(int j=0; j<row; j++){
                array[array_row[j]]=input_row[j];
            }
            for(int j=0; j<column; j++){
                input_column[j]=array[bound_set[j]];
            }
            for(int j=0; j<row; j++){
                input_row[j]=array[free_set[j]];
            }
            new_column=binary_inverse(input_column, column);
            new_row=binary_inverse(input_row, row);
            weight[new_column][new_row]=new_weight[n][m];
            decompose_chart[new_column][new_row]=new_dc[n][m];
        }
    }
    array_row=free_set;
    array_column=bound_set;
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

//base on the size of the bound set or free set, it can change an integer into an array,
// which is the binary form of the integer with this function we can convert
// an integer into the real input case of the bound set or free set. for example,
// if the bound set has 4 elements, they are i1, i2, i3, i4, and now I give a value 7,
// which means i1=i2=i3=1, i4=0 then the parameter size should be 4,
// and num should be 7, and the elements stored in array
// will become array[0]=array[1]=array[2]=1, array[3]=0
void binary(int* array, int size, int num){
    for(int i=0;i<size;i++){
        array[i]=num%2;
        num=num/2;
    }
}

//simulate one specific node, base on the total network, the value of
// the Fanin of the node, and the pData, I can get the value of the output of the node,
// and store it in the dTemp
void simulate_node(Abc_Ntk_t* LUT_network, Abc_Obj_t* pnode){
    char* read=(char*)pnode->pData;
    int output=0;
    //the value of the output
    int check=0;
    //used to check whether the pData reach its end or not
    while(1){
        int save=1;
        //use to temporarily store the value of one line,
        // cause we know sometimes pData may have two or more lines
        for(int i=0;i<pnode->vFanins.nSize;i++){
            if(check==1){
                break;
            }
            auto faninnode = Abc_NtkObj(LUT_network, pnode->vFanins.pArray[i]);
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
                default: break;
            }
        }
        if(check==1){
            pnode->dTemp=output%2;
            pnode->iTemp+=pnode->dTemp;
            break;
        }
        if((int)*(read+pnode->vFanins.nSize+1)==48){
            //if the number represent the output is 0,
            // that means you should inverse the result
            output+=inverse(save);
        }
        else if((int)*(read+pnode->vFanins.nSize+1)==49){
            output+=save;
        }
        read+=pnode->vFanins.nSize+3;
    }
}

//simulate all the network, input_row and input_column
// is the integer that represent the input case of free set and bound set
int simulation(Abc_Ntk_t* LUT_network, int* bound_set, \
        int* free_set, int bound_size, int free_size, int input_row, int input_column){
    int level_max=Abc_NtkLevel(LUT_network);
    //record the whole level of the network
    int output;
    Vec_Ptr_t** Nodes=new Vec_Ptr_t*[level_max];
    //base on the level of the network, I use a 2d Vector to store
    // all the nodes with different level
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
    int binary_row[free_size];
    //change the integer into the real input case
    binary(binary_row,free_size, input_row);
    int binary_column[bound_size];
    binary(binary_column,bound_size, input_column);
    for(int n=0; n<bound_size; n++){
        //set the dTemp of the PI to be same as the input case
        auto pnode=Abc_NtkObj(LUT_network, bound_set[n]);
        pnode->dTemp=binary_column[n];
    }
    for(int n=0; n<free_size; n++){
        auto pnode=Abc_NtkObj(LUT_network, free_set[n]);
        pnode->dTemp=binary_row[n];
    }
    for(int n=0; n<level_max; n++){
        //simulate all the nodes base on their levels, we should simulate the low-level nodes first, because the high-level nodes may take the low-level nodes as the Fanins
        int size=Nodes[n]->nSize;
        for(int i=0; i<size; i++){
            Abc_Obj_t* output_node=(Abc_Obj_t*)Nodes[n]->pArray[i];
            simulate_node(LUT_network, output_node);
        }
    }
    output=((Abc_Obj_t*)Nodes[level_max-1]->pArray[0])->dTemp;
    //since we only concern about he MFFC, so for the MFFC circuit,
    // there will be only one output, that is the node with the highest level,
    // and the dTemp of the node is the output of the whole circuit
    for(int k=0;k<level_max;k++){
        Vec_PtrFree(Nodes[k]);
    }
    delete[] Nodes;
    return output;
}



decomposed_chart* create_decomposed_chart(Abc_Ntk_t* LUT_network, int* bound_set,\
        int* free_set, int num){
    int row=pow(2,num-4);
    int column=pow(2,4);
    int** bin_arr=new int*[column];
    for(int n=0; n<column; n++){
        bin_arr[n]=new int[row];
    }
    for(int i=0; i<column; i++){
        for(int j=0; j<row; j++){
            bin_arr[i][j]=simulation(LUT_network, bound_set, free_set, 4, num-4, j, i);
        }
    }
    decomposed_chart* decomposedChart =
             new decomposed_chart(bin_arr, num-4, 4, free_set, bound_set);
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
//create a weight for the given MFFC circuit and the old LUT circuit,
// remember the weight is stored in the class decomposed_chart,
// the weight is a 2d double array, just like the decomposed chart
//and you can use print_weight to print it
void create_weight(decomposed_chart* decomposedChart, Abc_Ntk_t* MFFC_network, Abc_Ntk_t* LUT_network, vector<Abc_Obj_t*> input){
    srand(time(NULL));
    decomposedChart->clear_weight();
    int inputsize=LUT_network->vPis->nSize;
    int max_level=0;
    int count=100000;
    Abc_Obj_t* pnode;
    Abc_Obj_t* pin;
    for(int i=0; i<input.size(); i++){
        pnode=Abc_NtkObj(LUT_network,input[i]->Id);
        if(pnode->Level>max_level){
            max_level=pnode->Level;
        }
    }
    Vec_Ptr_t** Nodes=new Vec_Ptr_t*[max_level];        //base on the level of the network, I use a 2d Vector to store all the nodes with different level
    for(int k=0;k<max_level;k++){
        Nodes[k]=Vec_PtrAlloc(16);
    }
    Abc_Obj_t* pNode;
    int j;
    Abc_NtkForEachNode(LUT_network, pNode, j){
            int level=pNode->Level-1;
            if(level<max_level && level>=0)
                Vec_PtrPush(Nodes[level], pNode);
        }
    for(int ic=0; ic<count; ic++){
        //if(n%1000 == 0) cout << "Iteration: " << n << endl;
        int *input_condition=new int[inputsize];
        for(int u=0; u<inputsize; u++){
            input_condition[u]=rand()%2;
            //cout<<input_condition[u];
        }
        //cout<<endl;
        for(j=0; j<LUT_network->vPis->nSize; j++){
            pin=(Abc_Obj_t*)(LUT_network->vPis->pArray[j]);
            pin->dTemp=input_condition[j];
            pin->iTemp+=pin->dTemp;
        }
        for(int n=0; n<max_level; n++){                      //simulate all the nodes base on their levels, we should simulate the low-level nodes first, because the high-level nodes may take the low-level nodes as the Fanins
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
            std::cout<<decompose_chart[j][i]<<" |";
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

vector<vector<int>> simulate_whole(Abc_Ntk_t* LUT_network){
    srand(time(NULL));
    vector<vector<int>> array(1);
    for(int i=0; i<(Abc_NtkNodeNum(LUT_network)+Abc_NtkPiNum(LUT_network)+Abc_NtkPoNum(LUT_network)+1); i++){
        vector<int> temp;
        temp.resize(100000);
        array.push_back(temp);
    }
    cout<<"!!!"<<array.size()<<endl;
    int level_max=Abc_NtkLevel(LUT_network);            //record the whole level of the network
    int count=100000;
    Abc_Obj_t* pin;
    Vec_Ptr_t** Nodes=new Vec_Ptr_t*[level_max];        //base on the level of the network, I use a 2d Vector to store all the nodes with different level
    for(int k=0;k<level_max;k++){
        Nodes[k]=Vec_PtrAlloc(16);
    }
    Abc_Obj_t* pNode;
    int j;
    Abc_NtkForEachNode(LUT_network, pNode, j) {
            int level = pNode->Level - 1;
            if (level >= 0) {
                Vec_PtrPush(Nodes[level], pNode);
            }
        }
    /*for(int g=0; g<LUT_network->vPis->nSize; g++){
        pin=(Abc_Obj_t*)(LUT_network->vPis->pArray[g]);
        pin->sample=new int[count];
    }*/
    /*for (int h = 0; h < level_max; h++) {                      //simulate all the nodes base on their levels, we should simulate the low-level nodes first, because the high-level nodes may take the low-level nodes as the Fanins
        int size = Nodes[h]->nSize;
        for (int j = 0; j < size; j++) {
            pin = (Abc_Obj_t *) Nodes[h]->pArray[j];
            pin->sample=new int[count];
        }
    }*/
    for(int i=0; i<count; i++) {
        for (int k = 0; k < LUT_network->vPis->nSize; k++) {
            pin = (Abc_Obj_t *) (LUT_network->vPis->pArray[k]);
            pin->dTemp = rand() % 2;
            //*((int*)(pin->sample)+i) = pin->dTemp;
            array[pin->Id][i]=pin->dTemp;
        }
        for (int n = 0; n < level_max; n++) {                      //simulate all the nodes base on their levels, we should simulate the low-level nodes first, because the high-level nodes may take the low-level nodes as the Fanins
            int size = Nodes[n]->nSize;
            for (int l = 0; l < size; l++) {
                pin = (Abc_Obj_t *) Nodes[n]->pArray[l];
                simulate_node(LUT_network, pin);
                //*((int*)(pin->sample)+i) = pin->dTemp;
                array[pin->Id][i]=pin->dTemp;
            }
        }
    }
    for(int k=0;k<level_max;k++){
            Vec_PtrFree(Nodes[k]);
    }
    delete[] Nodes;
    return array;
}

void Create_Weight(decomposed_chart* decomposedChart, Mffc MFFC_network, int* bound_set, int* free_set, vector<vector<int>>& array){
    int count=100000;
    int x,y;
    int bound_input[decomposedChart->column];
    int free_input[decomposedChart->row];
    Abc_Obj_t* pnode;
    Abc_Ntk_t* MFFC=MFFC_network.ref;
    int level_max=Abc_NtkLevel(MFFC);
    /*for(int i=0; i<input.size(); i++){
        pnode=((Abc_Obj_t*)MFFC->vPis->pArray[i]);
        pnode->sample=input[i]->sample;
    }*/
    Vec_Ptr_t** Nodes=new Vec_Ptr_t*[level_max];        //base on the level of the network, I use a 2d Vector to store all the nodes with different level
    for(int k=0;k<level_max;k++){
        Nodes[k]=Vec_PtrAlloc(16);
    }
    Abc_Obj_t* pNode;
    int j;
    Abc_NtkForEachNode(MFFC, pNode, j) {
            int level = pNode->Level - 1;
            if (level >= 0) {
                Vec_PtrPush(Nodes[level], pNode);
            }
        }
    for(int i=0; i<count; i++){
        for(int n=0; n<MFFC_network.Pi_origin.size(); n++){
            pnode=((Abc_Obj_t*)MFFC->vPis->pArray[n]);
            //pnode->dTemp=*((int*)MFFC_network.Pi_origin[n]->sample+i);
            pnode->dTemp=array[MFFC_network.Pi_origin[n]->Id][i];
        }
        for (int n = 0; n < level_max; n++) {                      //simulate all the nodes base on their levels, we should simulate the low-level nodes first, because the high-level nodes may take the low-level nodes as the Fanins
            int size = Nodes[n]->nSize;
            for (int l = 0; l < size; l++) {
                pnode = (Abc_Obj_t *) Nodes[n]->pArray[l];
                simulate_node(MFFC, pnode);
            }
        }
        for(int j=0; j<decomposedChart->column; j++){
            pnode=Abc_NtkObj(MFFC, decomposedChart->array_column[j]);
            bound_input[j]=pnode->dTemp;
        }
        for(int j=0; j<decomposedChart->row; j++){
            pnode=Abc_NtkObj(MFFC, decomposedChart->array_row[j]);
            free_input[j]=pnode->dTemp;
        }
        x=binary_inverse(bound_input, decomposedChart->column);
        y=binary_inverse(free_input, decomposedChart->row);
        //delete[] bound_input;
        //delete[] free_input;
        decomposedChart->weight[x][y]++;
    }
    for(int i=0; i<decomposedChart->column_size; i++){
        for(int j=0; j<decomposedChart->row_size; j++){
            decomposedChart->weight[i][j]=decomposedChart->weight[i][j]/count;
        }
    }
    for(int k=0;k<level_max;k++){
            Vec_PtrFree(Nodes[k]);
    }
    delete[] Nodes;
}
