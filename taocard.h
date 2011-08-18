#define SCREEN_WIDTH 132//主屏幕宽度
#define SCREEN_HEIGHT 40//主屏幕高度
#define POKERLISTSIZE 31//扑克列表最大容量
#define INIT_CURNUM 12//玩家手牌初始数量

#define CLIENT_PLA_TURN_ERROR 1001;
#define CLIENT_POK_NON_ERROR 1002;

#define MAX_MESSAGE 20
#define FIFO_SERVER "serverfifo"
#define FIFO_CLIENT_1 "clientfifo1"
#define FIFO_CLIENT_2 "clientfifo2"
//显示控制
#define	    ESC_BOLD_ON   "\33[1m"
#define	    ESC_ITALICS_ON   "\33[3m"
#define	    ESC_UNDERLINE_ON   "\33[4m"
#define	    ESC_INVERSE_ON   "\33[7m"
#define	    ESC_STRIKETHROUGH_ON   "\33[9m"
#define	    ESC_BOLD_OFF   "\33[22m"
#define	    ESC_ITALICS_OFF   "\33[23m"
#define	    ESC_UNDERLINE_OFF   "\33[24m"
#define	    ESC_INVERSE_OFF   "\33[27m"
#define	    ESC_STRIKETHROUGH_OFF   "\33[29m"
//文本颜色
#define	    ESC_FG_BLACK   "\33[30m"
#define	    ESC_FG_RED   "\33[31m"
#define	    ESC_FG_GREEN   "\33[32m"
#define	    ESC_FG_YELLOW   "\33[33m"
#define	    ESC_FG_BLUE   "\33[34m"
#define	    ESC_FG_MAGENTA   "\33[35m"
#define	    ESC_FG_CYAN   "\33[36m"
#define	    ESC_FG_WHITE   "\33[37m"
#define	    ESC_FG_DEFAULT   "\33[39m"
#define	    ESC_BG_BLACK   "\33[40m"
#define	    ESC_BG_RED   "\33[41m"
#define	    ESC_BG_GREEN   "\33[42m"
#define	    ESC_BG_YELLOW   "\33[43m"
#define	    ESC_BG_BLUE   "\33[44m"
#define	    ESC_BG_MAGENTA   "\33[45m"
#define	    ESC_BG_CYAN   "\33[46m"
#define	    ESC_BG_WHITE   "\33[47m"
#define	    ESC_BG_DEFAULT   "\33[49m"

typedef int Rank;
typedef char Attribute;
typedef char Color[9];

typedef struct{
    char name[10];
    int width;
    int height;
    int guix;
    int guiy;
    int opaque;//不透明为1，透明为0
    Color color;
}CommonGUI;//一般UI

typedef struct{
    char name[10];
    Rank ranknum;
    Attribute attribute;
    CommonGUI pokergui;
}Poker;//扑克类

typedef struct{
    Poker poker[POKERLISTSIZE+1];
    int length;
}PokerList;//扑克列表

typedef struct{
    int playerScore[2];
    PokerList playerCurPokerList[2];
    PokerList playerPuttedPokerList[2];
    Poker puttingPoker[2];
    char errorStr[MAX_MESSAGE+1];
    int playerStat;//1出牌，2等待,3win,4lose
}GameData;//游戏数据

typedef struct{
    int clientId[2];//分别为1和2
    char playerCurPokerString[2][POKERLISTSIZE*3+1];
    char playerPuttedPokerString[2][POKERLISTSIZE*3+1];
    int playerScore[2];
    char playerPuttingPokerName[2][6];
    int playerStat[2];//0准备，1出牌，2等待,3win,4lose
    char errorStr[2][MAX_MESSAGE+1];
}AllGameInfo;

typedef struct{
    int clientId;
    char playerCurPokerString[POKERLISTSIZE*3+1];
    char playerPuttedPokerString[POKERLISTSIZE*3+1];
    int rivalCurPokerNum;
    char rivalPuttedPokerString[POKERLISTSIZE*3+1];
    int playerScore[2];
    char playerPuttingPokerName[6];
    char rivalPuttingPokerName[6];
    int playerStat;//1出牌，2等待,3win,4lose
    char errorStr[MAX_MESSAGE+1];
}ServerMessage;//服务器消息

typedef struct{
    char name[4];
    int randomNum;
}ServerCard;
