#include "MyMsg.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
int main(int argc, char const *argv[]) {
    key_t mykey = ftok("mymsgkey", 1);
    int msqid = msgget(mykey, IPC_CREAT);
    int x = 0;
    int y[5] = {1, 0, 1, 0, 1};
    char op = 0;
    MsgCalc msgCalc;
    MsgRslt msgRslt;
    printf("Number : ");
    while (1) {
        scanf("%d", &x);
        fflush(stdout);
        fflush(stdin);
        memset(&msgCalc, 0x00, sizeof(MsgCalc));
        msgCalc.mtype = MSG_TYPE_CALC;
        msgCalc.calc.x = x;
        if (0 < x && x < 6) {
            if (x % 2 == 1) {
                printf(">> Answer : CORRECT!\n");
            } else {
                printf(">> Answer : WRONG!\n");
            }

        } else if (0 >= x || x >= 6) {
            printf(">> Answer : WRONG!\n");
        }

        msgsnd(msqid, &msgCalc, MSG_SIZE_CALC, 0);
        memset(&msgRslt, 0x00, sizeof(MsgRslt));
        msgrcv(msqid, &msgRslt, MSG_SIZE_RSLT, MSG_TYPE_RSLT, 0);
        // printf(">>> %d\n", msgRslt.rslt);
    }
    return 0;
}
