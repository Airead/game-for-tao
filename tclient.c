#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <taocard.h>

//#define TEST2

static FILE *guiFilep;
static PokerList TP;
static PokerList DP;
static PokerList RP;
static PokerList XP;
static Poker XINGP;
static Poker EMPTYP;

int setCommonGUIName(CommonGUI *cg, char *name){
    strcpy(cg->name,name);
    return 0;
}

int setCommonGUIWidth(CommonGUI *cg, int width){
    cg->width=width;
    return 0;
}

int setCommonGUIHeight(CommonGUI *cg, int height){
    cg->height=height;
    return 0;
}

int setCommonGUIGuiX(CommonGUI *cg, int guix){
    cg->guix=guix;
    return 0;
}

int setCommonGUIGuiY(CommonGUI *cg, int guiy){
    cg->guiy=guiy;
    return 0;
}

int setCommonGUIOpaque(CommonGUI *cg, int opaque){
    //设置透明度，透明为0，不透明为1
    cg->opaque=opaque;
    return 0;
}

int setCommonGUIColor(CommonGUI *cg, char *color){
    strcpy(cg->color,color);
    return 0;
}


int setPokerName(Poker *poker, char * name){
    //设置poker的名称
    strcpy(poker->name,name);
    return 0;
}

int setPokerRank(Poker *poker, Rank rank){
    //设置poker的大小
    poker->ranknum=rank;
    return 0;
}

int setPokerAttribute(Poker *poker, Attribute attribute){
    //设置poker的属性
    poker->attribute=attribute;
    return 0;
}

int setPokerCommonGUI(Poker *poker, CommonGUI cg){
    //设置poker的图象
    poker->pokergui=cg;
    return 0;
}

//下面为PokerList的操作

int initPokerList(PokerList *pl){
    //初始化
    pl->length=0;
}

int addPoker(PokerList *pl, Poker poker){
    //向pl中添加一张poker
    if(pl->length >= POKERLISTSIZE){//溢出
	printf("addPoker error,最大只能放%d张牌\n",POKERLISTSIZE);
	exit(0);
    }
    pl->poker[pl->length+1]=poker;//放在最后
    pl->length++;
    return 0;
}



int delPokerByPos(PokerList *pl, int pos){
    //从列表pl中删除第pos个poker
    int i; 
    if(pos<0 || pos>pl->length){
	printf("delPoker error,删除位置错误\n");
	exit(0);
    }
    for(i=pos;i<pl->length;i++){//循环前移
	pl->poker[i]=pl->poker[i+1];
    }
    pl->length--;
    return 0;
}

int locatePokerByName(PokerList pl, char * name){
    //通过pokername确定poker在列表pl中的位置i,找不到则返回0
    int i;
    for(i=1;i<=pl.length;i++){
	if( !strcmp(pl.poker[i].name,name) ) break;
    }
    if(i>pl.length)//找不到
	return 0;
    else
	return i;
}

int addPokerByName(PokerList *pl, char * name){
    //向pl中添加名为name的扑克
    int n;
    if( n=locatePokerByName(TP,name) ){//查找天牌
	addPoker(pl,TP.poker[n]);
	return 0;
    }
    if( n=locatePokerByName(DP,name) ){//查找地牌
	addPoker(pl,DP.poker[n]);
	return 0;
    }
    if( n=locatePokerByName(RP,name) ){//查找人牌
	addPoker(pl,RP.poker[n]);
	return 0;
    }
    if( n=locatePokerByName(XP,name) ){//查找道自然牌
	addPoker(pl,XP.poker[n]);
	return 0;
    }
    printf("addPokerByName error\n");
    return 1;
}

int delPokerByName(PokerList *pl, char * name){
    //删除列表pl中名为name的扑克
    delPokerByPos(pl, locatePokerByName(*pl,name) );
    return 0;
}

int insertPokerByPos(PokerList *pl, int pos, Poker poker){
    //向pl的pos位置前插入poker
    int i;
    if(pl->length>=pl->length){//溢出
	printf("insertPokerByPos error,溢出\n");
	exit(0);
    }
    if(pos<1||pos>pl->length+1){//插入位置不对
	printf("insertPoderByPos error,插入位置不对\n");
	exit(0);
    }
    for(i=pl->length;i>pos;i--){//元素后移
	pl->poker[i]=pl->poker[i-1];
    }
    pl->poker[pos]=poker;
    pl->length++;
}

int sortPokerListByName(PokerList *pl){
    //按名称，将pl列表中的牌排序。用选择排序
    int i,j;
    int k;
    Poker *p;
    Poker tpoker;//中间变量
    p=pl->poker;
    for(i=1;i<pl->length;i++){
	tpoker=p[i];k=i;
	for(j=i+1;j<=pl->length;j++){
	    if( strcmp(p[k].name,p[j].name)==-1 ){
		k=j;
	    }
	}
	if(k!=i){//最小的name与poker[i]交换
	    p[0]=p[i];
	    p[i]=p[k];
	    p[k]=p[0];
	}
    }
    return 0;
}

int gotoxy(int x, int y){
    //定位(x,y)
    printf("\33[%d;%dH",y,x);
}


int cursorEnd(){
    printf("\33[%d;1H",SCREEN_HEIGHT+2);    
    return 0;
}

int printH(char ch, int row, int column, int width){
    //在第row行打印width个ch
    int i;
    printf("\33[%d;%dH",row,column);
    for(i=0;i<width;i++){
	printf("%c",ch);
    }
    return 0;
}

int printV(char ch, int row, int column, int height){
    //在第column列打印heigth个ch
    int i;
    for(i=1;i<=height;i++){
	printf("\33[%d;%dH",i+row,column);
	printf("%c",ch);
    }
    return 0;
}

int clrscr(){
    //清屏函数
    printf("\33[2J");
    return 0;
}

int paintFrame(int x1, int y1, int x2, int y2, char ch, char * color){
    //以ch字符从(x1,y1)到(x2,y2)打印边框
    int width,height;
    if(!strcmp(color,""))
	color=ESC_FG_DEFAULT;
    width=x2-x1+1;//求宽度
    height=y2-y1;
    printf("%s",color);
    printf(ESC_BOLD_ON);
    printH(ch,y1,x1,width);//打印上框
    printV(ch,y1,x1,height);//打印左框
    printV(ch,y1,x2,height);//打印右框
    printH(ch,y2,x1,width);//打印下框
    printf("\n");
    printf(ESC_BOLD_OFF);
    printf(ESC_FG_DEFAULT);
    cursorEnd();
    return 0;
}
/*
int paintGUIB(int x, int y, CommonGUI cg){
    //在(x,y)处绘制出cg的图形
    int h,v;
    char buf[140]={0};
    puts(cg.color);
    for(v=y;v<y+cg.height;v++){
	gotoxy(x,v);//将光标定位到（x,i)处
	//输出图形
	fseek(guiFilep,cg.guix+(v-y+cg.guiy-1)*HLENGTH-1,SEEK_SET);//定位文件指针
	fread(buf, sizeof(char), cg.width, guiFilep);
	printf("%s",buf);
    }
    fseek(guiFilep,0,SEEK_SET);//恢复文件指针
    puts(ESC_FG_DEFAULT);
    cursorEnd();
}
*/
int paintGUI(int x, int y, CommonGUI cg){
    //在(x,y)处绘制出cg的图形
    int h,v;
    char buf[140]={0};
    unsigned char ch;//判断是否为回车的中间变量
    int enterNum,i;
    puts(cg.color);
    for(v=y;v<y+cg.height;v++){
	gotoxy(x,v);//将光标定位到（x,i)处
	//输出图形
	fseek(guiFilep,0,SEEK_SET);
	enterNum=v-y+cg.guiy-1;//enterNum记录要找到的回车数（打印第i+1行）
	i=0;//i记录找到的空格数
	while(i<enterNum){//找到第guiy行
	    ch=fgetc(guiFilep);
	    if(ch=='\n')  i++;
	}
	fseek(guiFilep,cg.guix-1,SEEK_CUR);//定位最终文件指针
	fread(buf, sizeof(char), cg.width, guiFilep);
	printf("%s",buf);
    }
    fseek(guiFilep,0,SEEK_SET);//恢复文件指针
    puts(ESC_FG_DEFAULT);
    cursorEnd();
}

int paintMainScreen(){
    //绘制主屏幕
    clrscr();
    paintFrame(1,1,SCREEN_WIDTH, SCREEN_HEIGHT,'#',ESC_FG_YELLOW);
    cursorEnd();
    return 0;    
}

int paintGameData(GameData gd){
    //paint current PokerList
    clrscr();
    paintMainScreen();
    paintPokerListCenter(3,gd.playerCurPokerList[1]);
    paintPokerListShort(9,4,gd.playerPuttedPokerList[1]);
    paintPokerListShort(28,4,gd.playerPuttedPokerList[0]);
    paintPokerListCenter(34,gd.playerCurPokerList[0]);
    setCommonGUIColor(&gd.puttingPoker[0].pokergui, ESC_FG_YELLOW);
    paintGUI(63,22,gd.puttingPoker[0].pokergui);
    setCommonGUIColor(&gd.puttingPoker[1].pokergui, ESC_FG_YELLOW);
    paintGUI(63,15,gd.puttingPoker[1].pokergui);
    gotoxy(62,2); printf("还剩%d张",gd.playerCurPokerList[1].length);
    gotoxy(62,SCREEN_HEIGHT-1); printf("还剩%d张",gd.playerCurPokerList[0].length);
    paintHelpScreen();
    paintScore(gd.playerScore);
    paintErrorScreen(gd.errorStr);
    paintStatScreen(gd.playerStat);
    cursorEnd();    
    return 0;
}

int paintErrorScreen(char *errorStr){
    int x,y;
    x = 10; y = 21;
    if(*errorStr){
	gotoxy(x,y);
	printf("出错了：");
	gotoxy(x,y+1);
	printf("    %s",errorStr);
    }
    return 0;
}

int paintStatScreen(int stat){

    if(stat == 2){
	gotoxy(60,30);
	printf("等待对方出牌...");
    }else if(stat == 1){
	gotoxy(62,30);
	printf("请出牌...");
    }else if(stat == 3){
	gotoxy(62,30);
	printf("你赢了!");
	sleep(2);
    }else if(stat == 4){
	gotoxy(62,30);
	printf("你输了!");
	sleep(2);
    }else if(stat == 5){
	gotoxy(62,30);
	printf("平局!");
	sleep(2);
    }
    return 0;
}

int paintQuitScreen(){
    //paint quitScreen
    clrscr();
    paintFrame(1,1,SCREEN_WIDTH, SCREEN_HEIGHT,'W',ESC_FG_BLUE);
    paintFrame(50, 18, 70, 22,'*',ESC_FG_BLUE);
    cursorEnd();
    return 0;
}

int paintScore(int score[]){
    int x,y;
    int i;
    x = 115; y = 28;
    gotoxy(x, y);
    printf("    得分");
    gotoxy(x, y + 1);
    printf("自己 : 对手");
    gotoxy(x-1, y + 2);
    printf("%4d  : %3d",score[0],score[1]);
    return 0;
}

int paintHelpScreen(){
    //paint help
    int x,y;
    char message[][50]={"   操作说明","1.准备：ready","2.出牌：put","3.退出：quit"};
    int i;
    x = 115; y = 8;
    for(i = 0; i < 4; i++){
	gotoxy(x,y+i);
	printf("%s",message[i]);
    }
}
    
int startGui(){
    //引用pokergui内容，配置全局变量guiFilep
    guiFilep = fopen("pokergui","r");
    if(!guiFilep){
        printf("打开配置文件失败，图形未导入!\n");
        exit(0);
    }
    initPoker();
    return 0;
}

int initPoker(){
    //初始化Poker
    int i;
    CommonGUI cg;
    char name[9];
    //初始化天牌
    for(i=1;i<10;i++){
	sprintf(name,"T%d",i);
	setPokerName(&TP.poker[i],name);
	setPokerRank(&TP.poker[i],i);
	setPokerAttribute(&TP.poker[i],'T');
	setCommonGUIName(&cg,name);
	setCommonGUIWidth(&cg,8);
	setCommonGUIHeight(&cg,5);
	setCommonGUIGuiX(&cg,(i-1)*8+1);
	setCommonGUIGuiY(&cg,1);
	setCommonGUIOpaque(&cg,1);
	setCommonGUIColor(&cg,ESC_FG_GREEN);
	setPokerCommonGUI(&TP.poker[i],cg);
    }
    TP.length=9;
    //初始化地牌
    for(i=1;i<10;i++){
	sprintf(name,"D%d",i);
	setPokerName(&DP.poker[i],name);
	setPokerRank(&DP.poker[i],i);
	setPokerAttribute(&DP.poker[i],'D');
	setCommonGUIName(&cg,name);
	setCommonGUIWidth(&cg,8);
	setCommonGUIHeight(&cg,5);
	setCommonGUIGuiX(&cg,(i-1)*8+1);
	setCommonGUIGuiY(&cg,7);
	setCommonGUIOpaque(&cg,1);
	setCommonGUIColor(&cg,ESC_FG_GREEN);
	setPokerCommonGUI(&DP.poker[i],cg);
    }
    DP.length=9;
    //初始化人牌
    for(i=1;i<10;i++){
	sprintf(name,"R%d",i);
	setPokerName(&RP.poker[i],name);
	setPokerRank(&RP.poker[i],i);
	setPokerAttribute(&RP.poker[i],'R');
	setCommonGUIName(&cg,name);
	setCommonGUIWidth(&cg,8);
	setCommonGUIHeight(&cg,5);
	setCommonGUIGuiX(&cg,(i-1)*8+1);
	setCommonGUIGuiY(&cg,13);
	setCommonGUIOpaque(&cg,1);
	setCommonGUIColor(&cg,ESC_FG_GREEN);
	setPokerCommonGUI(&RP.poker[i],cg);
    }
    RP.length=9;
    //初始道牌
    sprintf(name,"XT%s","");
    setPokerName(&XP.poker[1],name);
    setPokerRank(&XP.poker[1],18);
    setPokerAttribute(&XP.poker[1],'X');
    setCommonGUIName(&cg,name);
    setCommonGUIWidth(&cg,8);
    setCommonGUIHeight(&cg,5);
    setCommonGUIGuiX(&cg,1);
    setCommonGUIGuiY(&cg,19);
    setCommonGUIOpaque(&cg,1);
    setCommonGUIColor(&cg,ESC_FG_RED);
    setPokerCommonGUI(&XP.poker[1],cg);
    //初始自然牌
    sprintf(name,"XW%s","");
    setPokerName(&XP.poker[2],name);
    setPokerRank(&XP.poker[2],19);
    setPokerAttribute(&XP.poker[2],'Y');
    setCommonGUIName(&cg,name);
    setCommonGUIWidth(&cg,8);
    setCommonGUIHeight(&cg,5);
    setCommonGUIGuiX(&cg,9);
    setCommonGUIGuiY(&cg,19);
    setCommonGUIOpaque(&cg,1);
    setCommonGUIColor(&cg,ESC_FG_RED);
    setPokerCommonGUI(&XP.poker[2],cg);
    XP.length=2;
    //初始化星牌 
    sprintf(name,"XI%s","");
    setPokerName(&XINGP,name);
    setPokerRank(&XINGP,19);
    setPokerAttribute(&XINGP,'Y');
    setCommonGUIName(&cg,name);
    setCommonGUIWidth(&cg,8);
    setCommonGUIHeight(&cg,5);
    setCommonGUIGuiX(&cg,17);
    setCommonGUIGuiY(&cg,19);
    setCommonGUIOpaque(&cg,1);
    setCommonGUIColor(&cg,ESC_FG_GREEN);
    setPokerCommonGUI(&XINGP,cg);
    XP.length=2;
    //初始化空牌 
    sprintf(name,"EM%s","");
    setPokerName(&EMPTYP,name);
    setPokerRank(&EMPTYP,19);
    setPokerAttribute(&EMPTYP,'E');
    setCommonGUIName(&cg,name);
    setCommonGUIWidth(&cg,8);
    setCommonGUIHeight(&cg,5);
    setCommonGUIGuiX(&cg,25);
    setCommonGUIGuiY(&cg,19);
    setCommonGUIOpaque(&cg,1);
    setCommonGUIColor(&cg,ESC_FG_GREEN);
    setPokerCommonGUI(&EMPTYP,cg);
    XP.length=2;
    return 0;
}

int paintPokerList(int x, int y, PokerList pokerlist){
    //打印出一组牌
    int i;
    for(i=1;i<=pokerlist.length;i++){
	paintGUI(x+(i-1)*8,y,pokerlist.poker[i].pokergui);
    }
    return 0;
}

int paintPokerListCenter(int row, PokerList pl){
    int x;
    x=(SCREEN_WIDTH-pl.length*8+1)/2;
    paintPokerList(x,row,pl);
    return 0;
}

int paintPokerListShort(int row, int column, PokerList pl){
    //紧密的打印一组牌
    int i;
    for(i=1;i<=pl.length;i++){
	setCommonGUIColor(&pl.poker[i].pokergui, ESC_FG_BLUE);
	paintGUI(column+(i-1)*3,row,pl.poker[i].pokergui);
    }
    return 0;
}

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

int strToPokerList(char *str, PokerList *pl){
    /* 将字符串转换为PokerList */
    int n,i;
    char *pokerName[POKERLISTSIZE*3+1];
    
    n = splitStrByChar(str, pokerName, ' ');
    if(n == 0){	
	initPokerList(pl);
	return 0;
    }
    
    for(i = 1; i <= n; i++){
	addPokerByName(pl,pokerName[i-1]);
    }
    return pl->length;
}

Poker nameToPoker(char *name){
    /* 根据牌名返回牌 */
    int n;
    char buf[3];
    int i;
    memset(buf, 0, sizeof(buf));
    strncpy(buf, name, 2);
    if( n=locatePokerByName(TP,name) ){//查找天牌
	return TP.poker[n];
    }
    if( n=locatePokerByName(DP,name) ){//查找地牌
	return DP.poker[n];
    }
    if( n=locatePokerByName(RP,name) ){//查找人牌
	return RP.poker[n];
    }
    if( n=locatePokerByName(XP,name) ){//查找道自然牌
	return XP.poker[n];
    }
    if(!strcmp(name, "XING"))
	return XINGP;
    if(!strcmp(name, ""))
	return EMPTYP;
    printf("nameToPoker error\n");
    return ; 
}

int openFifo(char *fifoname, int mode){
    /* simple open fifo, return the num of pipe */
    int fd;
    fd = open(fifoname, mode);
    if(fd == -1){
	printf("Open %s  error\n",fifoname);
    }else
      printf("Open %s over\n",fifoname);
    return fd;
}

char *getClientFifoPath(char *clientDescribe){
    /* 返回clientfifo 路径 */
    if(!strcmp(clientDescribe, "p1"))
	return FIFO_CLIENT_1;
    if(!strcmp(clientDescribe, "p2"))
	return FIFO_CLIENT_2;
}

int receiveServerMessage(ServerMessage *sm, char *clientDescribe){
    /* 接收服务器消息 */
    int res;
    int fd;
    char *fifoPath;
    fifoPath = getClientFifoPath(clientDescribe);
    /* 打开端口 */
    if((fd = openFifo(fifoPath, O_RDONLY)) == -1){
	printf("open %s error\n", fifoPath);
	exit(-1);
    }else{
	printf("open %s over\n", fifoPath);
    }
    
    /* 接收消息 */
    res = read(fd, sm, sizeof(ServerMessage));
    if(res == -1){
	printf("read %s error\n",fifoPath);
	exit(0);
    }else{
	printf("read bytes: %d\n", res);
    }
    close(fd);
    return 0;
}

int numToPokerList(int num, PokerList *pl){
    /* 将对手的牌变为星 */
    int i;
    for(i = 1; i <= num; i++)
	addPoker(pl, XINGP);
    return pl->length;
}

int clearGameData(GameData *gd){
    //置空游戏数据
    gd->playerCurPokerList[0].length = 0;
    gd->playerCurPokerList[1].length = 0;
    gd->playerPuttedPokerList[0].length = 0;
    gd->playerPuttedPokerList[1].length = 0;
    memset(gd->errorStr, 0, MAX_MESSAGE+1);
}

int serverMessageToGameData(ServerMessage sm, GameData *gd){
    /* 将服务器消息转换为游戏数据 */
    clearGameData(gd);
    
    strToPokerList(sm.playerCurPokerString, &gd->playerCurPokerList[0]);
    sortPokerListByName(&gd->playerCurPokerList[0]);
    strToPokerList(sm.playerPuttedPokerString, &gd->playerPuttedPokerList[0]);
    strToPokerList(sm.rivalPuttedPokerString, &gd->playerPuttedPokerList[1]);
    numToPokerList(sm.rivalCurPokerNum, &gd->playerCurPokerList[1]);
    gd->playerScore[0] = sm.playerScore[0];
    gd->playerScore[1] = sm.playerScore[1];
    gd->puttingPoker[0] = nameToPoker(sm.playerPuttingPokerName);
    gd->puttingPoker[1] = nameToPoker(sm.rivalPuttingPokerName);
    strcpy(gd->errorStr, sm.errorStr);
    gd->playerStat = sm.playerStat;
    return 0;
}

int sendMsgToServer(char *serverFifo, char *clientDescribe, char *bufMsg){
    /* send msg to serverfifo */
    int fd;
    int res;
    char msg[MAX_MESSAGE+1];
    fd = open(serverFifo, O_WRONLY);
    if(fd == -1){
	printf("open %s error\n", serverFifo);
	exit(-1);
    }else{
	printf("open %s over \n", FIFO_SERVER);
    }
    sprintf(msg, "%s %s", clientDescribe, bufMsg);
    res = write(fd, msg, MAX_MESSAGE+1);
    if(res == -1){
	printf("write msg to %s error\n", FIFO_SERVER);
	exit(0);
    }else{
	printf("write bytes: %d\n", res);
    }
    close(fd);
}

int test(){
    //测试函数    
    int i;
    char name[9];
    CommonGUI cg;
    PokerList myPokerList;
    initPokerList(&myPokerList);
    /*    printFrame(20,20,30,30,'*',ESC_FG_RED);
    printFrame(40,10,60,20,'@',ESC_FG_YELLOW);
    printFrame(60,30,70,38,'W',ESC_FG_BLUE);
    gotoxy(21,21);
    printf("hello!");
    gotoxy(21,21);
    printf("you");
    cursorEnd();
    //定义一个CommonGui
    strcpy(cg.name,"T2");
    cg.width=7;
    cg.height=5;
    cg.guix=1;
    cg.guiy=1;
    strcpy(cg.color,ESC_FG_GREEN);
    paintGUI(10,10,cg);*/
    initPoker();
    //sortPokerListByName(&TP);
    paintPokerListCenter(15,TP);
    paintPokerListCenter(23,DP);
    paintPokerListCenter(31,RP);
    paintPokerListCenter(7,XP);
    /*    paintGUI(20,20,TP.poker[2].pokergui); 
    sortPokerListByName(&TP);
    paintPokerList(20,30,TP);*/
    /*    addPokerByName(&myPokerList,"T2");
    addPokerByName(&myPokerList,"D2");
    addPokerByName(&myPokerList,"R5");
    addPokerByName(&myPokerList,"XD");
    addPokerByName(&myPokerList,"XZ");
    addPokerByName(&myPokerList,"T8");
    paintPokerListCenter(23,myPokerList);
    sortPokerListByName(&myPokerList);
    paintPokerListCenter(31,myPokerList);
    delPokerByName(&myPokerList,"XD");
    delPokerByName(&myPokerList,"D2");
    paintPokerListCenter(15,myPokerList);*/
     //测试删除
    /*    for(i=1;i<=9;i++){
	sprintf(name,"D%d",i);
	addPokerByName(&myPokerList,name);
    }
    for(i=1;i<=4;i++){
	sprintf(name,"R%d",10-i);
	addPokerByName(&myPokerList,name);
    }
    addPokerByName(&myPokerList,"XD");
    addPokerByName(&myPokerList,"XZ");
    paintPokerListCenter(15,myPokerList);    
    sortPokerListByName(&myPokerList);
    paintPokerListCenter(23,myPokerList);    
    delPokerByName(&myPokerList,"T2");
    delPokerByName(&myPokerList,"T5");
    delPokerByName(&myPokerList,"T8");
    delPokerByName(&myPokerList,"T3");
    paintPokerListCenter(35,myPokerList);*/
    return 0;
    fclose(guiFilep);
}

int makeClientFifo(char *clientDescribe){
    /* 创建FIFO文件 */
    char *fifoPath;

    fifoPath = getClientFifoPath(clientDescribe);
    if((mkfifo(fifoPath, 0777)) < 0)
      printf("Cannot create fifoServer: %s\n", fifoPath);
    return 0;
}

int startClient(char *clientDescribe){
#if defined TEST2
    GameData testData;
    PokerList myCurPokerList,myPuttedPokerList;
    PokerList yourCurPokerList,yourPuttedPokerList;
    ServerMessage smsg;
#endif
    ServerMessage sm;
    GameData gd;
    char msg[MAX_MESSAGE+1];
    char buf[20];
    int fd;
    int res;

#if defined TEST2
    smsg.clientId = 1;
    strcpy(smsg.playerCurPokerString,"T1 T2 T3 T4 D9 D8 D7 D6 D3 D4 XT YW");
    strcpy(smsg.playerPuttedPokerString,"T1 T2 T3 T4 D9 D8 D7 D6 D3 D4 XT YW");
    smsg.rivalCurPokerNum = 12;
    strcpy(smsg.rivalPuttedPokerString,"T1 T2 T3 T4 D9 D8 D7 D6 D3 D4 XT YW");
    smsg.playerScore[0] = 11;
    smsg.playerScore[1] = 22;
    strcpy(smsg.playerPuttingPokerName, "R2");
    strcpy(smsg.rivalPuttingPokerName, "R8");
    smsg.playerStat = 1;
    strcpy(smsg.errorStr, "没有这张牌");

#endif

#if defined TEST
    //init testData
    int i;
    testData.playerScore[1] = 30;
    // strcpy(testData.errorStr, "出过了");
    strcpy(testData.errorStr, "");
    testData.playerStat = 0;
    testData.puttingPoker[0] = RP.poker[1];
    testData.puttingPoker[1] = RP.poker[2];
    initPokerList(&myCurPokerList);
    initPokerList(&myPuttedPokerList);
    for(i = 1; i < 5; i++){
        addPoker(&myCurPokerList, TP.poker[i]);
        addPoker(&yourCurPokerList, XINGP);
        addPoker(&myPuttedPokerList, TP.poker[i]);
        addPoker(&yourPuttedPokerList, TP.poker[i]);
    }
    for(i = 1; i < 4; i++){
        addPoker(&myCurPokerList, DP.poker[i]);
        addPoker(&yourCurPokerList, XINGP);
        addPoker(&myPuttedPokerList, DP.poker[i]);
        addPoker(&yourPuttedPokerList, DP.poker[i]);
    }
    for(i = 9; i > 6; i--){
        addPoker(&myCurPokerList, RP.poker[i]);
        addPoker(&yourCurPokerList, XINGP);
        addPoker(&myPuttedPokerList, RP.poker[i]);
        addPoker(&yourPuttedPokerList, RP.poker[i]);
    }
    addPoker(&myCurPokerList, XP.poker[1]);
    addPoker(&yourCurPokerList, XINGP);
    addPoker(&myCurPokerList, XP.poker[2]);
    addPoker(&yourCurPokerList, XINGP);
    testData.playerCurPokerList[0] = myCurPokerList;
    testData.playerPuttedPokerList[0] = myPuttedPokerList;
    testData.playerCurPokerList[1] = yourCurPokerList;
    testData.playerPuttedPokerList[1] = yourPuttedPokerList;
#endif

    //start client

#if defined TEST2
    startGui();
    serverMessageToGameData(smsg, &testData); 
    paintGameData(testData);
#endif

    makeClientFifo(clientDescribe);
    startGui();    
    /* send client msg */
    sendMsgToServer(FIFO_SERVER, clientDescribe, "ready");
    while(1){    
	receiveServerMessage(&sm, clientDescribe);
	serverMessageToGameData(sm, &gd);
	paintGameData(gd);
	printf("%s@道法自然> ", clientDescribe);
	gets(msg);
	fflush(stdin);
	sendMsgToServer(FIFO_SERVER, clientDescribe, msg);
    }
    return 0;
}

int main(int argc, char * argv[]){
        if(argc < 2){
	printf("help: client p1, or client p2\n");
	return 0;
    }
    if(strcmp(argv[1], "p1") && strcmp(argv[1], "p2")){
	printf("help: client p1, or client p2\n");
	return 0;
	}
    startClient(argv[1]);
    return 0;
}
