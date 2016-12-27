#ifndef HELPER_H
#define HELPER_H

#include <string>

//enumeration of 4 nature
enum Nature { FIRE, WATER, BUSH, ELECTRICITY };
const std::string natureOfString[] = {"Fire", "Water", "Bush", "Electricity"};

//enumeration of 36 detailed kind
enum Kind{
            CHARMANDER, CHARMELEON, CHARIZARD, //小火龙 火恐龙 喷火龙
            TORCHIC, COMBUSKEN, BLAZIKEN, //火稚鸡 力壮鸡 火焰鸡
            CHIMCHAR, MONFERNO, INFERNAPE, //小火焰猴 猛火猴 烈焰猴
            SQUIRTLE, WARTORTLE, BLASTOISE, //杰尼龟 卡咪龟 水箭龟
            MUDKIP, MARSHTOMP, SWAMPERT, //水跃鱼 沼跃鱼 巨沼怪
            PIPLUP, PRINPLUP, EMPOLEON, //波加曼 波皇子 帝王拿波
            BULBASAUR, IVYSAUR, VENUSAUR, //秒蛙种子 秒蛙草 妙蛙花
            TREECKO, GROVYLE, SCEPTILE, //木守宫 森林蜥蜴 蜥蜴王
            TURTWIG, GROTLE, TORTERRA, //草苗龟 树林龟 土台龟
            PICHU, PIKACHU, RAICHU, //皮丘 皮卡丘 雷丘
            SHINX, LUXIO, LUXRAY, //小猫怪 勒克猫 伦琴猫
            MARREP, FLAAFFY, AMPHAROS //咩利羊 绵绵 电龙
            };
const std::string kindOfString[] = {"Charmander", "Charmeleon", "Charizard",
                         "Torchic", "Combusken", "Blaziken",
                         "Chimchar", "Monferno", "Infernape",
                        "Squirtle", "Wartortle", "Blastoise",
                         "Mudkip", "Marshtomp", "Swampert",
                         "Piplup", "Prinplup", "Empoleon",
                        "Bulbasaur", "Ivysaur", "Venusaur",
                         "Treecko", "Grovyle", "Sceptile",
                         "Turtwig", "Grotle", "Torterra",
                        "Pichu", "Pikachu", "Raichu",
                         "Shinx", "Luxio", "Luxray",
                         "Mareep", "Flaaffy", "Ampharos"};
//在对局时使用下列数组对应获得 Kind 中的枚举类型 例如(Kind1) == CHARMELEON
const int EmbryoPokemon[] = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33}; //对战难度--简单 创建初级口袋妖怪
const int IntermediatePokemon[] = {1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31, 34}; //对战难度--普通 创建中级口袋妖怪
const int LastPokemon[]= {2, 5, 8, 11, 14, 17, 20, 23, 26, 29, 32, 35}; //对战难度--困难 创建高级口袋妖怪

//embryo fire pokemon
const int EmbryoFire[] = {0, 3, 6};
//embryo water pokemon
const int EmbryoWater[] = {9, 12, 15};
//embryo bush pokemon
const int EmbryoBush[] = {18, 21, 24};
//embryo electricity pokemon
const int EmbryoElec[] = {27, 30, 33};
//intermediate fire pokemon
const int InterFire[] = {1, 4, 7};
//intermediate water pokemon
const int InterWater[] = {10, 13, 16};
//intermediate bush pokemon
const int InterBush[] = {19, 22, 25};
//intermediate electricity pokemon
const int InterElec[] = {28, 31, 34};
//last fire pokemon
const int LastFire[] = {2, 5, 8};
//last water pokemon
const int LastWater[] = {11, 14, 17};
//last bush pokemon
const int LastBush[] = {20, 23, 26};
//last electricity pokemon
const int LastElec[] = {29, 32, 35};

//enumeration of health state
enum State {HEALTHY, DROWNED, BURNED, POISONED, FROZEN};
const std::string stateOfString[] = {"Healthy", "Drowned", "Burned", "Poisoned", "Frozen"};

//enumeration of character
enum Character {AGGRESIVE, DEFENSIVE, TANK, QUICK};
const std::string characterOfString[] = {"Aggresive", "Denfensive", "Tank", "Quick"};

//enumeration of evolution level
enum evoLevel {EL1 = 6, EL2 = 13};
const int ExpGrade[17] = {0, 0, 50, 120, 210, 320, 500, 720,
                   1000, 1320, 1800, 2400, 3160, 3900,
                   4900, 6200};

const bool ALIVE = true, DEAD = false;
const std::string aliveOfString[] = {"Dead", "Alive"};

//attribute point increasing
const int UpGradeAttrIncStrong = 10;
const int UpGradeAttrIncNormal = 7;
const int UpGradeAttrIncWeak = 4;
const int UpGradeCriticalInc = 5;

//length of intiative bar
const int InitiativeBar = 100;

//boot pokemon levels
const int GoldPokeLevel = 15;
const int SilverPokeLevel = 9;
const int BronzePokeLevel = 3;

class Helper
{
private:
    //std::string recved from client
    std::string recvStrHelper = "";

    //std::string sent to client
    std::string sendStrHelper = "";

public:
    Helper() {}
    ~Helper() {}

    /**
       Function : setRecvStrHelper
       Description : set recv string helper
       Input : string
       Return : None
       Other : None
    **/
    void setRecvStrHelper(std::string str) { recvStrHelper = str; }

    /**
       Function : setSendStrHelper
       Description : set send string helper
       Input : string
       Return : None
       Other : None
    **/
    void setSendStrHelper(std::string str) { sendStrHelper = str; }

    /**
       Function : getRecvStrHelper
       Description : get recv string helper
       Input : None
       Return : recved string
       Other : None
    **/
    std::string getRecvStrHelper() { return this->recvStrHelper; }

    /**
       Function : getSendStrHelper
       Description : get send string helper
       Input : None
       Return : send string
       Other : None
    **/
    std::string getSendStrHelper() { return this->sendStrHelper; }

    /**
       Function : SplitStr
       Description : split string bt ','
       Input : string
       Return : first part of string between ','
       Other : previous string erase the part before ','
    **/
    static std::string SplitStr(std::string &inputStr)
    {
        size_t pos = 0;
        if ((pos = inputStr.find(',', 0)) != std::string::npos)
        {
            auto ret = inputStr.substr(0, pos);
            inputStr.erase(0, pos + 1);
            return std::move(ret);
        }
        return inputStr;
    }

    /**
       Function : getPrime
       Description : get the field which is seperated by ','
       Input : string
       Return : string
       Other : previous string dose not change
    **/
    static std::string getPrime(std::string inputStr)
    {
        size_t pos = 0;
        if ((pos = inputStr.find(',', 0)) != std::string::npos)
        {
            auto ret = inputStr.substr(0, pos);
            return std::move(ret);
        }
        return inputStr;
    }

    //判断Kind是否在口袋妖怪形态分级数组(初级形态 中级形态 高级形态)
    static bool isInStandard(Kind kind, const int standard[]) {
        for (int i = 0; i < 12; i++) {
            if (kind == standard[i])
                return true;
        }
        return false;
    }
    //用于判断create时等级和形态是否对应
    static bool isKindMeetLevel(Kind kind, int level) {
        if ((level < 6 && isInStandard(kind, EmbryoPokemon))||
                (level >= 6 && level < 13 && isInStandard(kind, IntermediatePokemon))||
                (level >= 13 && isInStandard(kind, LastPokemon)))
            return true;
        return false;
    }
};

#endif // HELPER_H
