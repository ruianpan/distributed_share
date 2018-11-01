#include <iostream>
#include <stdlib.h>
#include <string>

#define ASC_ZERO 48

using namespace std;
/**
 * [main description: This function takes in the target vm_id and
 * send the locally generated grep file back]
 * @param  argc [description]
 * @param  argv [description]
 * @return      [description: 0]
 */
int main(int argc, char *argv[])
{
    // Take in the second argument as the id for the vm
    if(argc!=2){
        cout<<"no vm num arg"<<endl;
        return 0;
    }
    int arg = (int)(*argv[1]) - ASC_ZERO;
    if(arg == 1){
        if(*(argv[1] + 1) == ASC_ZERO){
            arg = 10;
        }
        else{
            arg = 1;
        }
    }
    int vm_id = arg;

    // send generated grep file to the client
    string cmd("scp /home/hz5/CS-425-MP/actual.log hz5@fa18-cs425-g23-");
    if(vm_id == 10)
        cmd += "10.cs.illinois.edu:CS-425-MP/actual.log";
    else{
        cmd += "0";
        cmd += vm_id + ASC_ZERO;
        cmd += ".cs.illinois.edu:CS-425-MP/actual.log";
    }
    cout<<cmd;
    system(&cmd[0]);

    return 0;
}
