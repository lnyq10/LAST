#include "abc_api.h"

using namespace abc;

void map_LUT(char* old_filename, char* new_filename){
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
}
