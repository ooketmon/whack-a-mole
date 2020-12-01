#include "MyMsg.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

void signalHandler(int signum);

key_t mykey = 0;
int msqid = 0;

int main(int argc, char const *argv[]) {

    int y[5] = {1, 0, 1, 0, 1};

    MsgCalc msgCalc;
    MsgRslt msgRslt;

    mykey = ftok("mymsgkey", 1);             // 키 생성
    msqid = msgget(mykey, IPC_CREAT | 0600); // 메시지 큐 생성

    signal(SIGINT, signalHandler);

    while (1) {
        puts("Wait ...");
        memset(&msgCalc, 0x00, sizeof(MsgCalc));
        msgrcv(msqid, &msgCalc, MSG_SIZE_CALC, MSG_TYPE_CALC, 0);

        printf("Receive: %d\n", msgCalc.calc.x);
        memset(&msgRslt, 0x00, sizeof(MsgRslt));
        msgRslt.mtype = MSG_TYPE_RSLT;

        if (msgCalc.calc.x > -1 && msgCalc.calc.x < 5) {
            msgRslt.rslt = y[msgCalc.calc.x];
        } else {
            msgRslt.rslt = 0;
        }
        printf("Send : %d\n", msgRslt.rslt);
        msgsnd(msqid, &msgRslt, MSG_SIZE_RSLT, 0);
        fflush(stdout);
    }
    return 0;
}

void signalHandler(int signum) {
    if (signum == SIGINT) {
        msgctl(msqid, IPC_RMID, NULL);
        exit(0);
    }
}
