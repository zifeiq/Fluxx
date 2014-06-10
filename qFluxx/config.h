#ifndef CONFIG_H
#define CONFIG_H

const int BUFF_MAX = 512;
const int CLIENT_MAX = 4;
const int PORT = 1234; //默认PORT NUMBER = 1234
enum MsgType{
    NACK,//错误信号, 客户端和服务器共用
    //服务器发送的消息类型
    ADD_PLAYER,//增加玩家
    GAME_START,//游戏开始
    //DRAW,//摸牌信息
    ROUND_BEGIN,//回合开始
    CARD_UPDATE,//手牌更新信息
    PLAY_C, //出牌命令
    //ACK,//确认信号
    CARD_NUM,//其他玩家手牌数量因摸牌或行动牌而改变的通知
    CARD_PLAYED,//当前出牌信息
    CARD_DROPED,  //其他玩家弃牌信息
    RULE,//规则信息
    KEEPER_UPDATE,//玩家所有物更新
    DROP_CARD_C,//弃牌命令
    DROP_KEEPER_C,//弃所有物命令
    GAME_OVER,//游戏结束
    //CARD_STOLEN,//抽取的其他玩家卡牌（需要立刻打出），用于行动牌“取而用之”
    CHOOSE_PLAYER_C, //选择一位玩家的命令，用于行动牌“交换手牌”“取而用之”
    CHOOSE_KEEPER_C, //选择一张其他玩家的行动牌命令，用于行动牌“丢弃”“偷窃”
    EXCHANGE_KEEPER_C, //选择交换一张所有物，用于行动牌“交换所有物”
    DROP_RULE_C,//丢弃规则牌的命令，用于行动牌“取消规则”“简化一下”
    CHOOSE_GOAL_C, //选择目标牌的命令，用于双重目标时对某目标的替换

    //客户端发送的消息类型
    REGISTER,//注册消息
    PLAY_I,//出牌信息
    DROP_CARD_I, //弃牌信息
    DROP_KEEPER_I, //弃所有物信息
    CHOOSE_KEEPER_I,//选择的所有物信息，用于行动牌“丢弃”“偷窃”“交换所有物”
    DROP_RULE_I,//选择的规则牌信息，用于行动牌“取消规则”“简化一下”
    CHOOSE_PLAYER_I, //选择的玩家编号， 用于行动牌“交换手牌”“取而用之”
    CHOOSE_GOAL_I
};


#endif // CONFIG_H
