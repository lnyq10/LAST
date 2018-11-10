//
// Created by niqiu on 18-10-20.
//

#include "approx.h"
#define PRINT

inline void swap(int& a, int& b){
    int temp = a;
    a = b;
    b = temp;
}

inline void inst(int *arr, int sz){
    for(int i = 1; i < sz; i++){
        for(int j = 0; j < i; j++){
            if(arr[j] > arr[i])
                swap(arr[j], arr[i]);
        }
    }
}

inline int factorial(int i){
    int ans = 1;
    for(int j = 1; j <= i; j++) ans *= j;
    return ans;
}

inline int NChooseR(int n, int r){
    return factorial(n)/factorial(r)/factorial(n-r);
}

inline int NPR(int n, int r){
    return factorial(n)/factorial(n-r);
}

inline int inverse(int num){
    if(num) return 0;
    else return 1;
}

pattern::pattern(int l, int r_size) {
    this->length = l;
    this->row_size = r_size;
    this->val = new int[length];
    this->row = new pattern_type[row_size];
    this->error = 0;
}

pattern::~pattern() {
    delete[] val;
    delete[] row;
}



void pattern::print_info(bool detail){
    for(int i = 0; i < length; i++) cout << val[i] << " ";
    cout << endl;
    cout << "Error for this pattern is " << error << endl;
    if(detail) {

        for (int i = 0; i < row_size; i++) {
            cout << "Line " << i << "'s pattern type is "
                 << pattern_name[row[i]] << endl;
        }
    }
}
/**Function*************************************************************

  Synopsis    []

  Description [Find the optimized pattern]

***********************************************************************/

void pattern::pattern_cracker(int** dc, double** wt){
    int isOne;
    int isZero;

    for(int i = 0; i < row_size; i++){
        isOne = 1;
        isZero = 0;
        row[i] = UNDECIDED;
        for(int j = 0; j < length; j++){
            isOne *= dc[j][i];
            isZero += dc[j][i];
        }
        if(isOne) row[i] = ALL_ONE;
        if(!isZero) row[i] = ALL_ZERO;
    }
    //Pre-compute to determine the case of ALL_ONEs and ALL_ZEROs

    int* pt = p_cracker_helper(dc, wt, 0, length-1);
    this->error = local_error(dc, wt, 0, length-1, pt);//Calculate error
    for(int i = 0; i < length; i++) val[i] = pt[i];//Pass the result to the class attribute
    delete[] pt;

    double err1, err2;
    for(int i = 0; i < row_size; i++){
        if(row[i] == ALL_ONE || row[i] == ALL_ZERO) continue;
        err1 = err2 = 0;
        for(int j = 0; j < length; j++){
            err1 += abs(val[j] - dc[j][i]) * wt[j][i];
            err2 += abs(val[j] - inverse(dc[j][i])) * wt[j][i];
        }
        row[i] = err1 < err2? PATTERN: COMPLTD_PATTERN;
    }
    //Decide the row_pattern_type

}
vector<int> pattern::decoder(){
    vector<int> ans;
    for(int i = 0; i < this->length; i++){
        if(this->val[i] == 1) ans.push_back(i);
    }
    return ans;
}

double pattern::local_error(int ** dc, double** wt, int row_head, int row_tail, int* candidate){
    double ans = 0;
    for(int i = row_head; i <= row_tail; i++){
        if(row[i] == ALL_ONE || row[i] == ALL_ZERO) continue;

        double error1 = 0;
        double error2 = 0;
        for(int j = 0; j < length; j++){
            error1 += abs(candidate[j] - dc[j][i]) * wt[j][i];
            error2 += abs(candidate[j] - inverse(dc[j][i])) * wt[j][i];
        }
        ans += error1 < error2? error1: error2;
    }

    return ans;
}


int* pattern::merge(int ** dc, double** wt, int row_head, int row_tail, int* candidates[], int candidate_size){
    auto error = new double[candidate_size];
    for(int ctr = 0; ctr < candidate_size; ctr++){
        error[ctr] = local_error(dc, wt, row_head, row_tail, candidates[ctr]);
    }

    int minAt = 0;
    for(int i = 0; i < candidate_size; i++){
        if(error[i] < error[minAt]) minAt = i;
    }

    int* ans = new int[length];
    for(int i = 0; i < length; i++) ans[i] = candidates[minAt][i];
    delete[] error;
    return ans;
}

int* pattern::p_cracker_helper(int ** dc, double** wt, int row_head, int row_tail){
    //assert(row_tail >= row_head);
    if(row_tail <= row_head){
        int* ans = new int[length];
        for(int i = 0; i < length; i++) ans[i] = dc[i][row_head];
        return ans;
    }
    else{
        int mid1 = row_head + (row_tail - row_head)/3;
        int mid2 = row_tail - (row_tail - row_head)/3;
        //int mid2 = mid1 +
        int* candidate1 = pattern::p_cracker_helper(dc, wt, row_head, mid1);
        int* candidate2 = pattern::p_cracker_helper(dc, wt, mid1+1, mid2);
        int* candidate3 = pattern::p_cracker_helper(dc, wt, mid2+1, row_tail);

        int* candidates[3] = {candidate1, candidate2, candidate3};
        const int candidate_size = 3;

        int* ans = merge(dc, wt, row_head, row_tail, candidates, candidate_size);
        for(int i = 0; i != candidate_size; i++) delete[] candidates[i];
        return ans;
    }
}

/**Function*************************************************************

  Synopsis    []

  Description [output all the possible combinations size choose four,
  ordered by index. The first four elements in the output are the Tian Xuan ZHi Zi]

***********************************************************************/
int* choose_four(const int* arr, int size, int index){
    int* ans = new int[size];
    for(int i = 0; i < size; i++){
        ans[i] = arr[i];
    }
    sort(ans, ans+size);
    int offset = factorial(size)/NChooseR(size, 4);

    for(int i = 0; i < offset*index; i++){
        next_permutation(ans, ans+size);
    }
    reverse(ans, ans+size);
    return ans;
}

string dec2bin(unsigned n, int num_of_digit)
{
    string res;

    for(int i = 0; i != num_of_digit; i++){
        res.push_back((n & 1) + '0');
        n >>= 1;
    }

    /*
    while (n)
    {
        res.push_back((n & 1) + '0');
        n >>= 1;
    }
     */

    if (res.empty())
        res = "0";
    else
        std::reverse(res.begin(), res.end());

    return res;
}



/**Function*************************************************************

  Synopsis    []

  Description [Core function for approximation]

***********************************************************************/
Mffc* local_approx(Abc_Ntk_t* pNtk, Mffc* mffc, int PiSize, int min_level){

    //Choose a mffc

    int* PiSet = new int[PiSize];
    int i, j = 0;
    Abc_Obj_t* pNode;
    Abc_NtkForEachPi(mffc->ref, pNode, i){
        pNode->iTemp = 0;
        PiSet[j] = Abc_ObjId(pNode);
        j++;
    }

    pattern *pt, *pt_min = nullptr;
    vector<pattern *> pt_set;
    vector<pattern *>::size_type ix_pt;
    int *nC4 = nullptr;
    int index;
    decomposed_chart *dc, *dc_min = nullptr;
    vector<decomposed_chart *> dc_set;
    vector<decomposed_chart *>::size_type ix_dc;

    int rounds = PiSize/4;
    for(int ii = 0; ii < rounds; ii++) {
        if(ii > 0) break;
        int *bSet = new int[4]{0};
        int *fSet = new int[PiSize - 4]{0};
        int *bSet_min = new int[4];
        int *fSet_min = new int[PiSize - 4];

        int loop_size = 10;
        //int loop_size = NChooseR(PiSize, 4);
        for (i = 0; i < loop_size; i++) {
            index = i;//3
            //nC4 = PiSet;
            nC4 = choose_four(PiSet, PiSize, index);
            for (j = 0; j < PiSize; j++) {
                if (j < 4) bSet[j] = nC4[j];
                else fSet[j - 4] = nC4[j];
            }
            //Assigning Pis to bound set and free set
            delete[] nC4;

            dc = create_decomposed_chart(mffc->ref, bSet, fSet, PiSize, min_level);
            dc_set.push_back(dc);
            //int lvl = Abc_NtkLevel(pNtk);
            //cout <<  << endl;
            //simulate_whole(pNtk);
            create_weight(dc, mffc->ref, pNtk, mffc->Pi_origin);
            //Create_Weight(dc, *mffc, bSet, fSet);

            pt = new pattern(16, (int) pow(2, PiSize - 4));
            pt_set.push_back(pt);
            pt->pattern_cracker(dc->decompose_chart, dc->weight);
            if (i == 0) {
                pt_min = pt;
                dc_min = dc;
                for (int k = 0; k < PiSize; k++) {
                    if (k < 4) bSet_min[k] = bSet[k];
                    else fSet_min[k - 4] = fSet[k - 4];
                }
            } else {
                if (pt->get_error() < pt_min->get_error()) {
                    //delete dc_min;
                    //delete pt_min;
                    dc_min = dc;
                    pt_min = pt;
                    for (int k = 0; k < PiSize; k++) {
                        if (k < 4) bSet_min[k] = bSet[k];
                        else fSet_min[k - 4] = fSet[k - 4];
                    }
                }
            }
            if (pt_min->get_error() == 0) break;


        }

        assert(pt_min && dc_min);
        //Find the minimum pattern
        mffc->error += pt_min->get_error();
        dc_min->print_decomposed_chart();
        //dc_min->print_weight();
        pt_min->print_info(false);

        modify_mffc(mffc->ref, pt_min, bSet_min, fSet_min, PiSize - 4);
        /*
        for(i = 0; i < 4; i++){
            Abc_NtkObj(mffc->ref, bSet_min[i])->iTemp = 1;
        }
        */
        for (ix_dc = 0; ix_dc != dc_set.size(); ix_dc++) {
            if (dc_set[ix_dc] == dc_min) continue;
            else delete dc_set[ix_dc];
        }
        for (ix_pt = 0; ix_pt != pt_set.size(); ix_pt++) {
            if (pt_set[ix_pt] == pt_min) continue;
            else delete pt_set[ix_pt];
        }
        delete[] PiSet;
        delete[] bSet_min;
        delete[] fSet_min;
        delete[] bSet;
        delete[] fSet;
        delete dc_min;
        delete pt_min;

    }

    return mffc;
}

void modify_mffc(Abc_Ntk_t* mffc, pattern* pt, int* bset, int* fset, int fSetSize, bool initial){
    Abc_Obj_t *pNode, *qNode, *vicNode;
    int j, max = 0;
    if(initial) {
        Abc_NtkForEachNode(mffc, pNode, j) {
                if (Abc_ObjId(pNode) > max) max = Abc_ObjId(pNode);
            }
        Abc_NtkForEachNode(mffc, pNode, j) {
                if (Abc_ObjId(pNode) == max) continue;
                Abc_NtkDeleteObj(pNode);
            }
    }else{
        vicNode = Abc_NtkObj(mffc, Abc_NtkPo(mffc, 0)->vFanins.pArray[0]);
        Abc_NtkDeleteObj(vicNode);
        //
    }
    //Delete old node

    vector<int> numerical_pattern = pt->decoder();
    if(numerical_pattern.empty()){
        pNode = Abc_NtkCreateNodeConst0(mffc);
    }
    else if(numerical_pattern.size() == 16){
        pNode = Abc_NtkCreateNodeConst1(mffc);
    }
    else {
        pNode = Abc_NtkCreateNode(mffc);
        for (int i = 0; i < 4; i++) {
            qNode = Abc_NtkObj(mffc, bset[i]);
            Abc_ObjAddFanin(pNode, qNode);
        }
    }
    //Create bound set node
    string temp = std::to_string(Abc_ObjId(pNode));
    Nm_ManStoreIdName(mffc->pManName, Abc_ObjId(pNode), ABC_OBJ_NODE, (char *) "n", (char *) temp.c_str());
    //Asign name
    //cout << "New node has name " << Abc_ObjName(pNode) << endl;

    vector<int>::size_type ix;
    static char buffer[2000] = {0};
    string s;
    for(ix = 0; ix != numerical_pattern.size(); ix++){
        s = dec2bin((unsigned)numerical_pattern[ix], 4);
        //cout << s << endl;
        sprintf(buffer, "%s%s 1\n", buffer, s.c_str());
    }

    pNode->pData = buffer;
    //Assign pData

    Abc_Obj_t* endNode = Abc_NtkCreateNode(mffc);
    //Create auxiliary node(used to connected to Po)
    temp = std::to_string(Abc_ObjId(endNode));
    Nm_ManStoreIdName(mffc->pManName, Abc_ObjId(endNode), ABC_OBJ_NODE, (char *) "n", (char *) temp.c_str());
    //Create name
    Abc_ObjAddFanin(endNode, pNode);
    for(int i = 0; i < fSetSize; i++) {
        Abc_ObjAddFanin(endNode, Abc_NtkObj(mffc, fset[i]));
    }
    //Assign free set as endNodes's fanin
    endNode->pData = (char*)"10 1\n";

    static char buffer2[20000] = {0};
    auto pattern_type = pt->get_pattern_type();
    for(int i = 0; i != pt->get_row_size(); i++){
        s = dec2bin((unsigned)i, fSetSize);
        switch(pattern_type[i]){
            case(ALL_ONE):{
                sprintf(buffer2, "%s-%s 1\n", buffer2, s.c_str());
                break;
            }
            case(ALL_ZERO):{
                break;
            }
            case(PATTERN):{
                //s = dec2bin((unsigned)i, fSetSize);
                sprintf(buffer2, "%s1%s 1\n", buffer2, s.c_str());
                break;
            }
            case(COMPLTD_PATTERN):{
                //s = dec2bin((unsigned)i, fSetSize);
                sprintf(buffer2, "%s0%s 1\n", buffer2, s.c_str());
                break;
            }
            default: assert(0);
        }
    }

    //cout << buffer << endl;
    endNode->pData = buffer2;
    //Endnode's pData


    qNode = Abc_NtkPo(mffc, 0);
    Abc_ObjRemoveFanins(qNode);
    Abc_ObjAddFanin(qNode, endNode);
    //Connect to Po


    //vicNode = Abc_NtkObj(mffc, Abc_NtkPo(mffc, 0)->vFanins.pArray[0]);
    //cout << "Victim has node name: " << Abc_ObjName(vicNode) << endl;
}

