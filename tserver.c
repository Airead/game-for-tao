#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <taocard.h>
 
#define TEST

int errorNo=0;
int clearCard = 0;

int splitStrByChar(char *source, char *dest[], char ch){
    /* 将source按ch分开储存在dest[]中,返回有几个字符串 */
    int k=0;
    while(*source){
	while(*source == ch)	/* 跳过首ch */
	    source++;

	if(!*source) return 0;	/* 跳完ch结束了 */

	dest[k++] = source;
	
	while(*source != ch && *source != '\0')	/* 找到词尾ch 或 串尾 */
	    source++;
	if(*source)
	    *source++ = '\0';
    }
    return k;
}

int openFifo(char *fifoname, int mode){
    /* simple open fifo, return the num of pipe */
    int fd;
    fd = open(fifoname, mode);
    if(fd == -1){
	printf("Open %s  error\n",fifoname);
    }
    return fd;
}


int qsort_compar(ServerCard *card1, ServerCard *card2){
    /* qsort 回调函数 */
    return card1->randomNum - card2->randomNum;
}

int initClientId(int clientId[]){
    /* 初始化客户端ID */
    clientId[0] = 1;
    clientId[1] = 2;
    return 0;
}

int initCurPokerString(char curPokerString[][POKERLISTSIZE*3+1]){
    /* 初始化玩家当前牌 */
    ServerCard scard[29];
    char buf[10];
    int i,j,k = 0;

    /* 随机洗牌 */
    srand(time(NULL));
    for(i = 1; i <= 3; i++){
	for(j = 1; j <= 9; j++){
	    sprintf(buf,"%c%d ","TDR"[i - 1], j);
	    strcpy(scard[k].name, buf);
	    scard[k].randomNum = rand();
	    k++;
	}
    }
    strcpy(scard[k].name, "XT ");
    scard[k].randomNum = rand();
    k++;
    strcpy(scard[k].name, "XW ");
    scard[k].randomNum = rand();
    qsort(scard, 29, sizeof(ServerCard), (void *)qsort_compar);
    
    /* 顺序发牌 */
    memset(curPokerString[0], 0, POKERLISTSIZE*3+1);
    memset(curPokerString[1], 0, POKERLISTSIZE*3+1);
    for(i = 0; i < INIT_CURNUM * 2;){
	strcat(curPokerString[0], scard[i++].name);
	strcat(curPokerString[1], scard[i++].name);
    }
    /* 测试 */
    //    printf("player1: %s\n",curPokerString[0]);
    //    printf("player2: %s\n",curPokerString[1]);
    return 0;
}

int initPuttedPokerString(char puttedPokerString[][POKERLISTSIZE*3+1]){
    //初始化出过的牌
    memset(puttedPokerString[0], 0, POKERLISTSIZE*3+1);
    memset(puttedPokerString[1], 0, POKERLISTSIZE*3+1);
    return 0;
}

int initScore(int score[]){
    //初始化玩家得分
    int i;
    for(i = 0; i < 2; i++)
	score[i] = 0;
    return 0;
}

int initPuttingPokerName(char puttingPokerString[][6]){
    /* 初始化出过的牌 */
    memset(puttingPokerString[0], 0, 6);
    memset(puttingPokerString[1], 0, 6);
    return 0;
}

int initStat(int stat[]){
    /* 玩家开始状态，决定谁先出牌 */
    srand(time(NULL));
    if( rand() > rand() ){
	stat[0] = 1;
	stat[1] = 2;
    }else{
	stat[0] = 2;
	stat[1] = 1;
    }
    return 0;
}

int Stat(int stat[]){
    /* 初始化玩家状态 */
    stat[0] = -1;
    stat[1] = -1;
    return 0;
}

int initErrorStr(char errorStr[][MAX_MESSAGE+1]){
    /* init error string */
    memset(errorStr[0], 0, MAX_MESSAGE+1);
    memset(errorStr[1], 0, MAX_MESSAGE+1);
    return 0;
}

int initAllGameInfo(AllGameInfo *agi){
    /* 初始化游戏数据 */
    initClientId(agi->clientId);
    initCurPokerString(agi->playerCurPokerString);
    initPuttedPokerString(agi->playerPuttedPokerString);
    initScore(agi->playerScore);
    initPuttingPokerName(agi->playerPuttingPokerName);
    initStat(agi->playerStat);
    initErrorStr(agi->errorStr);
    return 0;
}
/* 以上初始化模块 */

int getClientId(char *cmd0){
    if(cmd0[1] == '1') return 1;
    if(cmd0[1] == '2') return 2;
    printf("client message <p1|p2> error\n");
}

int getOtherId(int clientId){
    /* 取得另一个ID */
    if(clientId == 1)
	return 2;
    else
	return 1;
}

int countPokerStringNum(char * pokerString){
    /* change pokerName to XI */
    char *p;
    int i=0;
    p = pokerString;
    while(*p){
	if(*p++ == ' ')
	    i++;
    }
    return i;
}

int sendMessageToClient(AllGameInfo agi, int clientId){
    /* 向客户端发送消息 */
    ServerMessage sm;
    int fd;
    int res;
    char *fifoPath;

    if(clientId == 1)
	fifoPath = FIFO_CLIENT_1;
    else
	fifoPath = FIFO_CLIENT_2;

    /* 生成消息 */
    gameInfoToServerMessage(agi, &sm, clientId);
     
    /* 打开端口 */
    if((fd = openFifo(fifoPath, O_WRONLY)) == -1){
	printf("open %s error\n", fifoPath);
	exit(0);
    }else{
	printf("open %s over\n", fifoPath);
    }

    /* 发送消息 */
    res = write(fd, &sm, sizeof(ServerMessage));
    if(res == -1){
	printf("write %s error\n",FIFO_CLIENT_1);
	exit(-1);
    }else{
	printf("write bytes: %d\n", res);
    }
    /* 关闭端口 */
    close(fd);
    return 0;
}

int initServerMessage(ServerMessage *sm){
    /* init ServerMessage */
    sm->clientId = 0;
    memset(sm->playerCurPokerString, 0, POKERLISTSIZE*3 + 1);
    memset(sm->playerPuttedPokerString, 0, POKERLISTSIZE*3 + 1);
    memset(sm->playerPuttingPokerName, 0, 6);
    sm->playerScore[0] = 0;
    sm->playerStat = 0;
    memset(sm->errorStr, 0, MAX_MESSAGE+1);
    /* other */
    sm->rivalCurPokerNum = 0;
    memset(sm->rivalPuttedPokerString, 0, POKERLISTSIZE*3 + 1);
    memset(sm->rivalPuttingPokerName, 0, 6);
    sm->playerScore[1] = 0;
}


int gameInfoToServerMessage(AllGameInfo agi, ServerMessage *sm, int clientId){
    /* 将游戏信息转换为特定的服务器消息 */
    int num,otherNum;

    initServerMessage(sm);
    
    num = clientId -1;
    otherNum = getOtherId(clientId) -1;

    sm->clientId = clientId;
    strcpy(sm->playerCurPokerString, agi.playerCurPokerString[num]);
    strcpy(sm->playerPuttedPokerString, agi.playerPuttedPokerString[num]);
    strncpy(sm->playerPuttingPokerName, agi.playerPuttingPokerName[num], 2);
    sm->playerScore[0] = agi.playerScore[num];
    sm->playerStat = agi.playerStat[num];
    strcpy(sm->errorStr, agi.errorStr[num]);
    //otherCurNum 
    strcpy(sm->rivalPuttedPokerString, agi.playerPuttedPokerString[otherNum]);
    sm->rivalCurPokerNum = countPokerStringNum(agi.playerCurPokerString[otherNum]);
    strncpy(sm->rivalPuttingPokerName, agi.playerPuttingPokerName[otherNum], 2);
    sm->playerScore[1] = agi.playerScore[otherNum];

    return 0;
}



int receiveClientMsg(char *fifoPath, char *clientMsg){
    /* receive client msg */
    int fd;
    int res;
    char buf[MAX_MESSAGE+1];
    fd = open(fifoPath, O_RDONLY);
    if(fd == -1){
	printf("open %s to read error\n", fifoPath);
	exit(-1);
    }else{
	printf("open %s to read over\n", fifoPath);
    }
    res = read(fd, clientMsg, MAX_MESSAGE+1);
    if(res == -1){
	printf("read %s error\n", fifoPath);
	exit(-1);
    }else{
	printf("receive msg: %s\n", clientMsg);
    }
    close(fd);
    return 0;
}

int changePlayerStat(int playerStat[]){
    int temp;
    temp = playerStat[0];
    playerStat[0] = playerStat[1];
    playerStat[1] = temp;
    return 0;
}

int pkResult(AllGameInfo *agi){
    if(!strcmp(agi->playerCurPokerString[0], "")
       && !strcmp(agi->playerCurPokerString[1], "")){
	if(agi->playerScore[0] > agi->playerScore[1]){
	    agi->playerStat[0] = 3;
	    agi->playerStat[1] = 4;
	    clearCard = 3;
	}else if(agi->playerScore[0] == agi->playerScore[1]){
	    agi->playerStat[0] = 5;
	    agi->playerStat[1] = 5;
	    clearCard = 3;
	}else{
	    agi->playerStat[0] = 4;
	    agi->playerStat[1] = 3;
	    clearCard = 3;
	}
    }
    return 0;
}

int playerPutPoker(AllGameInfo *agi, char *cmd[]){
    /* player put poker */
    char *p,*q,*s;
    char buf[10];
    char errorStr[MAX_MESSAGE+1];
    int clientId;
    int num,otherNum;
    char *pokerName;
    pokerName = cmd[2];		/* 在这个函数里不知道怎么会把agi->errorStr改变 */
    clientId = getClientId(cmd[0]);
    num = clientId - 1;
    /* 检测是否轮到 clientId 出牌 */
    if(agi->playerStat[num] != 1){
	errorNo = CLIENT_PLA_TURN_ERROR;
	strcpy(agi->errorStr[num], "没有轮到你出牌");
	return -1;
    }
    /* 检测clientId 有没有 pokerName */
    s = strstr(agi->playerCurPokerString[num], pokerName);
    if(s == NULL){
	errorNo = CLIENT_POK_NON_ERROR;
	sprintf(errorStr, "你没有‘%2s’牌",pokerName);
	strcpy(agi->errorStr[num], errorStr);
	return -1;
    }
    /* 储存要出的牌 */
    q = buf; p = s;
    while(*p != ' '){
	*q++ = *p++;
    }
    *q++ = ' ';			/* 保持序列的一致性:t1 t3 d3 */
    *q = '\0';
    otherNum = getOtherId(clientId) - 1;
    /* 出过两张牌后,清空 */
    if(clearCard == 2){
	clearCard =0;
	memset(agi->playerPuttingPokerName[otherNum], 0, 6);
    }
    if(!strcmp(agi->playerPuttingPokerName[otherNum], "")){ /* 对方没出牌 */
	strcpy(agi->playerPuttingPokerName[num], buf);
	strcat(agi->playerPuttingPokerName[num], "1");
	clearCard++;
    }else{			/* 对方已出牌 */
	strcpy(agi->playerPuttingPokerName[num], buf);
	strcat(agi->playerPuttingPokerName[num], "2");
	clearCard++;
    }
    /* 比较牌的大小 */
    if(clearCard == 2){
	comparePutingCard(agi);
    }
    /* 删除打出的牌 */
    while(*s){			/* 可能有问题,证明没问题 */
	*s = *(s+3);
	s++;
    }
    /* 将删除的牌加入出过的牌中 */
    strcat(agi->playerPuttedPokerString[num],buf);
    /* 改变玩家状态 */
    if(clearCard != 2){
	changePlayerStat(agi->playerStat);
    }
    if(clearCard == 2)
	pkResult(agi);
    strcpy(agi->errorStr[num], "");
    return 0;
}

int upStatAndScore(AllGameInfo *agi, int num){
    /* 更新玩家分数和出牌顺序 */
    if(num == 0){
	agi->playerScore[0]++;
	agi->playerStat[0] = 1;
	agi->playerStat[1] = 2;
    }
    if(num == 1){
	agi->playerScore[1]++;
	agi->playerStat[0] = 2;
	agi->playerStat[1] = 1;
    }
}

int comparePutingCard(AllGameInfo *agi){
    /* 比较出牌的大小 */
    int res;
    int wasfirst;
    
    if(!strcmp(agi->playerPuttingPokerName[0],"") || !strcmp(agi->playerPuttingPokerName[1],"")) /* 有一方还没出牌 */
	return 0;
    /* 判断先手 */
    if(agi->playerPuttingPokerName[0][3] < agi->playerPuttingPokerName[1][3])
	wasfirst = 0;
    else
	wasfirst = 1;
    /* 都出牌后比较大小 */
    if(agi->playerPuttingPokerName[0][0] == agi->playerPuttingPokerName[1][0]){ /* 花色相同 */
	if(agi->playerPuttingPokerName[0][1] > agi->playerPuttingPokerName[1][1]) /* 比较数字 */
	    upStatAndScore(agi,0);
	else
	    upStatAndScore(agi,1);
	
    }else{			/* 花色不同 */
	if(agi->playerPuttingPokerName[0][0] == 'X' || agi->playerPuttingPokerName[1][0] == 'X'){ /* 有王 */
	    if(agi->playerPuttingPokerName[0][0] == 'X')
		upStatAndScore(agi,0);
	    else
		upStatAndScore(agi,1);
	    
	}else{			/* 没王 */
	    if(wasfirst == 0)
		upStatAndScore(agi,0);
	    else
		upStatAndScore(agi,1);
	}
    }

    return 0;
}

int playerQuit(){
    /* player quit */
}

int updateAllGameInfo(AllGameInfo *agi, char *cmd[]){
    /* update all game info */
    
    if(!strcmp(cmd[1], "PUT")){	
	printf("into playerPutPoker\n");
	playerPutPoker(agi, cmd);
    }else if(!strcmp(cmd[1], "QUIT")){
	playerQuit(agi, cmd);
    }else{
	printf("update gameinfo error: %s\n",cmd[1]);
    }
    return 0;
}

int startServer(){
    /* server start */
    AllGameInfo agi;
    char clientMsg[MAX_MESSAGE+1];
    char *cmd[20];
    int n;
    ServerMessage sm;
    int clientId;		/* 1 or 2 */
    int fd;
    int res;
#if defined TEST
    
#endif
    /* Create fifo file */
    if((mkfifo(FIFO_SERVER, 0777)) < 0)
	printf("Cannot create fifoServe : %s\n", FIFO_SERVER);
    
    /* iit all info */
    initAllGameInfo(&agi);
    while(1){
	/* receive client msg */
	receiveClientMsg(FIFO_SERVER, clientMsg);    
        n = splitStrByChar(clientMsg, cmd, ' ');
	if(n < 1 || n > 3){
	    printf("******************************\n");
	    printf("%s clientMsg error: %s %s %s\n", cmd[0], cmd[0], cmd[1], cmd[2]);
	    printf("******************************\n");
	}
	if(n >= 3){
	       if(!strcmp(cmd[2],"TJ")) strcpy(cmd[2],"XT");
	       if(!strcmp(cmd[2],"WJ")) strcpy(cmd[2],"XW");
	}
	clientId = getClientId(cmd[0]);
	updateAllGameInfo(&agi, cmd);
	printf("gameInfoToServerMessage for %d\n", clientId);
	sendMessageToClient(agi, clientId);
    }
	
#if defined TEST
	
#endif
    return 0;
}

int main(void){
    startServer();
    return 0;
}

