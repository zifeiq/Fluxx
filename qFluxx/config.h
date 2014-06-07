#ifndef CONFIG_H
#define CONFIG_H

#define PLAYER_NUM 4

enum MsgType{
    //服务器发送的消息类型
    ADD_PLAYER = 0,//增加玩家
    GAME_START,//游戏开始
    DRAW,//摸牌信息
    ROUND_BEGIN,//回合开始
    ACK,//确认信号
    NACK,//错误信号
    DROP_CARD_C,//弃牌命令
    DROP_KEEPER_C,//弃所有物命令
    CARD_NUM,//其他玩家手牌数量
    CARD_PLAYED,//当前出牌信息
    GAME_OVER,//游戏结束
    RULE,//规则信息
    KEEPER_UPDATE,//其他玩家所有物更新
    CARD_DROPED,  //其他玩家弃牌信息
    //客户端发送的消息类型
    REGISTER,//注册消息
    PLAY,//出牌信息
    DROP_CARD_I, //弃牌信息
    DROP_KEEPER_I //弃所有物信息
    //另外，客户端发的ACK,NACK与服务器的消息类型相同
};

#endif // CONFIG_H
