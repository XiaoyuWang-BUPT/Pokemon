#ifndef HELPER_H
#define HELPER_H

enum Nature {FIRE, WATER, BUSH, ELECTRICITY};
const std::string natureOfString[] = {"Fire", "Water", "Bush", "Electricity"};
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
const std::string kindOfString[] = {"Charamander", "Charmeleon", "Charizard",
                         "Torchic", "Combusken", "Blaziken",
                         "Chimchar", "Monferno", "Infernape",
                        "Squirtle", "Watortle", "Blastoise",
                         "Mudkip", "Marshtomp", "Swampert",
                         "Piplup", "Prinplup", "Empoleon",
                        "Bulbasaur", "Ivysaur", "Venusaur",
                         "Treecko", "Grovyle", "Sceptile",
                         "Turtwig", "Grotle", "Torterra",
                        "Pichu", "Pikachu", "Raichu",
                         "Shinx", "Luxio", "Luxray",
                         "Marrep", "Flaaffy", "Ampharos"};
//在对局时使用下列数组对应获得 Kind 中的枚举类型 例如(Kind1) == CHARMELEON
const int EmbryoPokemon[] = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33}; //对战难度--简单 创建初级口袋妖怪
const int IntermediatePokemon[] = {1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31, 34}; //对战难度--普通 创建中级口袋妖怪
const int LastPokemon[]= {2, 5, 8, 11, 14, 17, 20, 23, 26, 29, 32, 35}; //对战难度--困难 创建高级口袋妖怪
enum State {HEALTHY, DROWNED, BURNED, POISONED, FROZEN};
const std::string stateOfString[] = {"Healthy", "Drowned", "Burned", "Poisoned", "Frozen"};
enum Character {AGGRESIVE, DEFENSIVE, TANK, QUICK};
const std::string characterOfString[] = {"Aggresive", "Denfensive", "Tank", "Quick"};
enum evoLevel {EL1 = 6, EL2 = 13};
const int ExpGrade[17] = {0, 0, 50, 120, 210, 320, 500, 720,
                   1000, 1320, 1800, 2400, 3160, 3900,
                   4900, 6200};
const bool ALIVE = true, DEAD = false;
const std::string aliveOfString[] = {"Dead", "Alive"};
const int UpGradeAttrIncStrong = 10;
const int UpGradeAttrIncNormal = 7;
const int UpGradeAttrIncWeak = 4;
const int UpGradeCriticalInc = 5;

class Helper
{
public:
    Helper() {}
    ~Helper() {}
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
