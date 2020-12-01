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
    char op = 0;
    MsgCalc msgCalc;
    MsgRslt msgRslt;

    while (1) {
        printf("Number : ");
        scanf("%d", &x);
        fflush(stdout); //버퍼를 비워줌
        fflush(stdin);
        memset(&msgCalc, 0x00, sizeof(MsgCalc)); //초기화
        msgCalc.mtype = MSG_TYPE_CALC;
        msgCalc.calc.x = x; //메인문에서 받은걸 구조체에 넣어주고
        msgsnd(msqid, &msgCalc, MSG_SIZE_CALC, 0);
        msgrcv(msqid, &msgRslt, MSG_SIZE_RSLT, MSG_TYPE_RSLT,
               0); //서버에서 데이터를 받을 때 까지 기다림
        if (msgRslt.rslt == 1) {
            printf(">> Answer : CORRECT! \n");
        } else {
            printf(">> Answer : WRONG! \n");
        }
        // printf(">>> %d\n", msgRslt.rslt);
    }

    return 0;
}
