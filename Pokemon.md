# 				Pokemon

##							     					口袋妖怪人机对战游戏

---

### 开发环境

* Visual Studio 2015 (MSVC15)
* SQLite3
* Qt5

### 特性

* 遵循当前游戏架构，逻辑和结构全部在服务器实现，客户端只负责展示和交互，玩家无法魔改。程序设计难度提高，但最大程度地保证了游戏系统的健壮性和公平原则
* 通过ORM(对象关系映射)封装SQLite操作，开发者无需纠结于SQL语句
* 使用JSON作为服务器与客户端的通信载体，提高通信协议的可读性和可拓展性。服务器在接受到后均发送回执，留作拓展之用。
* 支持多个玩家同时在线
* 丰富的小精灵种类，分水，火，草，电4属性，36具体种，并可向高级形态进化
* 小精灵的创建具有很大随机性，所有天赋随机产生。性格在`aggresive` `defensive` `tank` `quick`中随机，分别擅长攻击，防御，血量和敏捷。产生攻击力，防御力，血量和攻击速度时，对应上述四种性格各有偏重，并在升级进化时有格外的增强效果
* 用户在选择对战时不局限于挑选自己的某一精灵，而是背包中的所有(至多三只)精灵都参与战斗。也不局限于挑选服务器固有的小精灵作为对手，而是挑选索要进行的比赛等级，以及对战精灵的属性，分为`火系神奇宝贝训练家对抗赛` `火系道馆主对抗赛` `火系神奇宝贝训练大师对抗赛`...以此类推，难度依次提升，获胜后玩家获得的排位积分和参战小精灵获得的经验也依次提高
* 界面简洁美观，功能明确，响应迅速
* 具备完善的对玩家登陆注册操作的提示，包括`玩家已经登陆，请稍后尝试` `玩家不存在，请注册` `密码不正确，请重新输入` `用户名已注册，请登陆或更换用户名重新注册` `两次输入密码不一致` 等
* 增加捕捉小精灵功能
* 增加玩家背包和仓库，背包中的小精灵用于战斗，与口袋妖怪游戏设定一致。玩家可自由地将背包中的小精灵放回仓库，也可将仓库中的小精灵置入背包
* 增加点赞功能
* 增加排位功能，对战胜利可获得相应难度的排位积分。玩家查看天梯时即可查看到所有玩家的排名分布
* 增加玩家游戏时长和初次登录的时间显示，以尊重初代口袋妖怪的游戏设定
* 增加属性克制，特殊攻击依照属性相克原则造成`效果拔群` `效果抵抗`的伤害
* 增加特殊攻击造成异常状态，可在攻击回合前受到一定伤害
* 增加GitHub项目入口，可供探讨交流和用户反馈之用

### 框架设计

![无标3](C:\Users\wxiaoyu\Desktop\无标3.png)



### 协议设计

**Sign In** `symbol = "signin"`

```json
client -> server
    key = ["symbol"], value = ["signin"];
    key = ["username"], value = ["eg.Lee"];
    key = ["password"], value = ["eg.2014232"];
    key = ["end"], value = ["end"]

server -> client
    key = ["symbol"], value = ["signin"];
    key = ["useravailable"], value = [true]; //user logged
	[
      key = ['useravailable'], value = [false] //user not logged
	]    
	key = ["passwordcorrect"], value = [true];
	key = ["passwordcorrect"], value = [false];
    key = ["end"], value = ["end"]
```

**Sign On** `symbol = "signon"`

```json
client -> server
    key = ["symbol"], value = ["signon"];
    key = ["username"], value = ["eg.Bob"];
    key = ["password"], value = ["eg.123456"];
    key = ["end"], value = ["end"]

server -> client
    key = ["symbol"], value = ["signin"];
    key = ["useravailable"], value = [true]; //user not logged
	key = ["useravailable"], value = [false]; //user logged
    key = ["signonsuccess"], value = [true];
    key = ["end"], value = ["end"]
```

**Acquaintance** `symbol = "acquaintance"`

```json
client -> server
	key = ["symbol"], value = ["acquaintance"];
	key = ["owner"], value = ["eg.Lee"];
	key = ["pokemon"], value = ["eg.charmander"]
	key = ["end"], value = ["end"];

server -> client
	key = ["symbol"], value = ["acquaintance"];
	key = ["end"], value = ["end"];
```

**OnlinePlayer** `symbol = "onlinePlayer"`

```json
client -> server
    key = ["symbol"], value = ["onlinePlayer"];
    key = ["end"], value = ["end"]

server -> client
    key = ["symbol"], value = ["onlinePlayer"];
    key = ["amount"], value = [eg.0];
    [	
      	key = ["amount"], value = [1]; //example with 1 online player
      	key = ["name1"], value = ["Lee"];
    	key = ["rank1"], value = [9999]
    ]
    key = ["end"], value = ["end"]
```

**My Information** `symbol = "myinfo"`

```json
client -> server
	key = ["symbol"], value = ["myinfo"];
	key = ["name"], value = ["eg.Lee"];
	key = ["end"], value = ["end"];

server -> client
	key = ["symbol"], value = ["myinfo"];
	key = ["end"], value = ["end"];
	key = ["name"], value = ["eg.Lee"];
	key = ["pokemonNumber"], value = [eg.1];
	key = ["rate"], value = [eg.1.00];
	key = ["rank"], value = [eg.2];
	key = ["thumb"], value = [eg.10];
	key = ["begintime"], value = ["eg.20161221009"]
	key = ["gametime"], value = ["eg.1202"];
```

**Hunt** `symbol = "hunt"`

```json
client -> server
	key = ["symbol"], value = ["hunt"];
	key = ["kind"], value = ["eg.charmander"];
	key = ["name"], value = ["eg.LeeCharmander"];
	key = ["owner"], value = ["eg.Lee"]
	key = ["end"], value = ["end"];

server -> client
	key = ["symbol"], value = ["hunt"];
	key = ["end"], value = ["end"];
```

**Rank** `symbol = "rank"`

```json
client -> server
	key = ["symbol"], value = ["rank"];
	key = ["name"], value = ["eg.Lee"];
	key = ["end"], value = ["end"];
	
server -> client
	key = ["symbol"], value = ["rank"];
	key = ["amount"], value = [eg.0];
	[
      key = ["amount"], value = [eg.1]; //example with 1 player in total
      key = ["name1"], value = ["bob"];
      key = ["rank1"], value = [2];
      key = ["rate1"], value = [1.00];
	]
	key = ["end"], value = ["end"];
```

**Package** `symbol = "package"`

```json
client -> server
	key = ["symbol"], value = ["package"];
	key = ["name"], value = ["eg.Lee"];
	key = ["end"], value = ["end"];

server -> client
	key = ["symbol"], value = ["package"];
	key = ["amount"], value = [eg.0]
	[
      key = ["amount"], value = [eg.1]; //example with 1 pokemon in package
      key = ["kind1"], value = ["eg.Charmander"];
	  key = ["name1"], value = ["eg.LeeCharmander"];
	  key = ["character1"], value = ["eg.aggresive"];
	  key = ["level1"], value = [eg.1];
	  key = ["exp1"], value = [eg.10];
	  key = ["att1"], value = [eg.25];
	  key = ["def1"], value = [eg.15];
	  key = ["HP1"], value = [eg.30];
	  key = ["speed"], value = [eg.5];
	  key = ["critical"], value = [eg.10];
	]
	key = ["end"], value = ["end"];
```

**Storage** `symbol = "storage"`

```json
client -> server
	key = ["symbol"], value = ["storage"];
	key = ["name"], value = ["eg.Lee"];
	key = ["end"], value = ["end"];

server -> client
	key = ["symbol"], value = ["storage"];
	key = ["amount"], value = [eg.0]
	[
      key = ["amount"], value = [eg.1]; //example with 1 pokemon in storage
      key = ["kind1"], value = ["eg.Charmander"];
	  key = ["name1"], value = ["eg.LeeCharmander"];
	  key = ["character1"], value = ["eg.aggresive"];
	  key = ["level1"], value = [eg.1];
	  key = ["exp1"], value = [eg.10];
	  key = ["att1"], value = [eg.25];
	  key = ["def1"], value = [eg.15];
	  key = ["HP1"], value = [eg.30];
	  key = ["speed"], value = [eg.5];
	  key = ["critical"], value = [eg.10];
	]
	key = ["end"], value = ["end"];
```

**Package Out** `symbol = "packout"`

```json
client -> server
	key = ["symbol"], value = ["packout"];
	key = ["name"], value = ["eg.LeeCharmander"];
	key = ['owner'], value = ["eg.Lee"];
	key = ["end"], value = ["end"];

server -> client
	key = ["symbol"], value = ["packout"];
	key = ["success"], value = true;
	key = ["end"], value = ["end"];
```

**Storage Out** `symbol = "stoout"`

```json
client -> server
	key = ["symbol"], value = ["stoout"];
	key = ["name"], value = ["eg.LeeCharmander"];
	key = ['owner'], value = ["eg.Lee"];
	key = ["end"], value = ["end"];

server -> client
	key = ["symbol"], value = ["stoout"];
	key = ["success"], value = true; //package is not full
	[
      key = ['success'], value = false; //package is full
	]
	key = ["end"], value = ["end"];
```

**Player Pokemon** `symbol = "playerpoke"`

```json
client -> server
	key = ["symbol"], value = ["playerpoke"];
	key = ["name"], value = ["eg.bob"]
	key = ["end"], value = ['end'];
	
server -> client
	key = ["symbol"], value = ["playerpoke"];
	key = ["amount"], value = [eg.0];
	[
      key = ["amount"], value = [eg.1]; //example with that the player poccesses merely 1 pokemon
      key = ["name1"], value = ["eg.bobCharmander"];
      key = ["kind1"], value = ["eg.Charmander"];
      key = ["level1"], value = [eg.2];
	]
	key = ['end'], value = ["end"];
```

**Thumb** `symbol = "thumb"`

```json
client -> server
	key = ["symbol"], value = ["thumb"];
	key = ["name"], value = ["eg.bob"];
	key = ["end"], value = ["end"];
	
server -> client
	key = ["symbol"], value = ["thumb"];
	key = ["end"], value = ["end"];
```

**Battle** `symbol = "battle"`

```json
client -> server
	key = ["symbol"], value = ["battle"]
	key = ["owner"], value = ["eg.Lee"];
	key = ["nature"], value = ["eg.Fire"];
	key = ["standard"], value = ["Gold"];
	key = ["end"], value = ["end"];
	
server -> client
	//example with merely 1 round in total	
	key = ["symbol"], value = ["battle"];
	key = ["amount"], value = [eg.1]; //example with 1 pokemon on both side
	key = ["round1attack"], value = ["eg.mypokemon"];
	key = ["round1myhurt"], value = [false]; //pokemon is healthy
	[
      key = ["round1myhurt"], value = [true]; //pokemon is unhealthy
      key = ["round1myhurtnature"], value = ["eg.Water"]; //pokemon is drown
      key = ["round1myhurthp"], value = [eg.20]; //HP after get drown
	]
	key = ["round1attnature"], value = ["eg.Fire"];
	key = ["round1attway"], value = ["att"]; //normal attack
	[
      key = ["round1attway"], value = ['specialatt']; //special attack
	]
	key = ["round1enemypokemonhp"], value = [eg.2]; //HP of boot after being attacked
	key = ["win"], value = true;
	[
      key = ["win"], value = false;
	]
	key = ["myevokind"], value = ["eg.Charmeleon"]; //kind of pokemon after battle, evoluted
	[
      key = ["myevokind"], value = ["eg.Charmander"] //kind of pokemon after battle, not evoluted
	]
	key = ["expGot"], value = [eg.50];
	key = ["end"], value = ["end"];
	
	//example with empty package, not qualified to have a battle
	[
      key = ["symbol"], value = ["nobattle"];
      key = ["end"], value = ["end"];
	]
```

### 结构说明

### PokemonServer

#### **Helper**(辅助类) `lib/helper.h`

```c++
enum Nature //Fire, Water, Bush, Electricity
const std::string natureOfString[] //"Fire", "Water", "Bush", "Electricity"
enum Kind //Charmander, Charmeleon, Charizard...
const std::string kindOfString[] //"Charmander", "Charmeleon", "Charizard"...
const int EmbryoPokemon[] //初级形态小精灵序号
const int IntermediatePokemon[] //中级形态小精灵序号
const int LastPokemon[] //高级形态小精灵序号
const int EmbryoFire[] //初级形态火属性精灵序号
const int EmbryoWater[] //初级形态水属性精灵序号
const int EmbryoBush[] //初级形态草属性精灵序号
const int EmbryoElec[] //初级形态电属性精灵序号
const int InterFire[] //中级形态火属性精灵序号
const int InterWater[] //中级形态水属性精灵序号
const int InterBush[] //中级形态草属性精灵序号
const int InterElec[] //中级形态电属性精灵序号
const int LastFire[] //高级形态火属性精灵序号
const int LastWater[] //高级形态水属性精灵序号
const int LastBush[] //高级形态草属性精灵序号
const int LastElec[] //高级形态电属性精灵序号
enum State //Healthy, Burned, Drowned, Poisoned, Frozen
const std::string stateOfString[] //"Healthy", "Burned", "Drowned", "Poisoned", "Frozen"
enum Character //aggresive, defensive, tank, quick
const std::string characterOfString[] //"aggresive", "defensive", "tank", "quick"
enum evoLevel //6, 12
const int ExpGrade[17] //小精灵升级的经验阈值
const bool ALIVE = true, DEAD = false
const std::string aliveOfString[] //"DEAD", "ALIVE"
const int UpGradeAttrIncStrong = 10; //长项增长
const int UpGradeAttrIncNormal = 7; //普通增长
const int UpGradeAttrIncWeak = 4; //弱项增长
const int UpGradeCriticalInc = 5; //暴击增长
const int InitiativeBar = 100; //行动条阈值
const int GoldPokeLevel = 15; //高级电脑等级
const int SilverPokeLevel = 9; //中级电脑等级
const int BronzePokeLevel = 3; //初级电脑等级
class Helper
{
  private:
  	std::string recvStrHelper; //接收信息缓存
  	std::string sendStrHelper; //发送信息缓存
  public:
  	Helper() {};
  	~Helper() {};
    void setRecvStrHelper(std::string str);
    void setSendStrHelper(std::string str);
    std::string getRecvStrHelper();
    std::string getSendStrHelper();
    static std::string SplitStr(std::string &inputStr); //分割字符串，修改原字符串
    static std::string getPrime(std::string inputStr); //分割字符串，不修改原字符串
    static bool isInStandard(Kind kind, const int standard[]); //判断是否属于指定形态
    static bool isKindMeetLevel(Kind kind, int level); //判断等级和形态是否匹配
}
```

#### **Player(用户类)** `src/player.h`

```c++
#define CAPACITY 3
struct PlayerInfo
{
    string name; //用户名
    string password; //密码
    int pokemonNumber; //小精灵数目
    int packageCapacity; //背包剩余容量
    double rate; //胜率
    int games; //与战数
    int rank; //天梯积分
    int thumb; //获得赞的数目
    string beginDateTime; //开始游戏时间(eg.201611171230 2016-11-17 12:30)
    string gameTime; //游戏时间(eg.12003 120hours 3minutes)
    /**ORM将struct关联到数据表**/
    ORMAP(PlayerInfo, name, password,
          pokemonNumber, packageCapacity, rate, games, rank, thumb, beginDateTime,
          gameTime)
};
class Player
{
private:
    string _name_;
    string _password_;
    int _pokemonNumber_; 
    int _packageCapacity_;
    double _rate_; 
    int _games_;
    int _rank_; 
    int _thumb_;
    QDateTime _beginDateTime_; 
    string _gameTime_;
    set<Pokemon*> _pokemonGot_; 
public:
    Player();
    Player(struct PlayerInfo playerInfo); //根据struct创建玩家对象
    ~Player() {}
    string getName();
    string getPassword();
    int getPMNumber();
    double getRate();
    int getRank();
    int getThumb();
    int getGames();
    QDateTime getBeginDT();
    string getGameTime();
    set<Pokemon*> getGotPokemon();
    struct PlayerInfo ToPlayerInfo();

    void setName(string name);
    void setPassword(string password);
    void setPMNumber(int pokemonNumber);
    void setRank(int rank);
    void setThumb(int thumb);
    void setBeginDateTime(QDateTime beginDateTime);
    void setGameTime(QDateTime startDateTime);
    void addPokemon(Pokemon *PMtoGet);
};
```

#### **PlayerFactory(用户简单工厂类)** `src/playerfactory.h`

```c++
class PlayerFactory
{
public:
    PlayerFactory(){}
    ~PlayerFactory(){}
    Player *CreatePlayer(struct PlayerInfo playerInfo); //根据struct创建玩家对象
};
```

#### **Pokemon(口袋妖怪类)** `src/pokemon.h`

```c++
struct PokemonInfo
{
    string owner; //神奇宝贝训练家
    string name; //小精灵名字
    int nature; //小精灵属性
    int kind; //小精灵种类
    int character; //小精灵性格
    int level; //小精灵等级
    int experiencePoint; //小精灵经验值
    int attackPoint; //小精灵攻击力
    int defencePoint; //小精灵防御力
    int totalHP; //小精灵总血量
    int currentHP; //小精灵当前血量
    int intervalIncrease; //小精灵攻击频率
    int criticalPoint; //暴击指数
    int state; //健康状态
    int sickCounter; //不健康状态回合计数
    int sickPoint; //不健康状态损失血量
    int alive; //存活与否
    /*ORM将struct关联到数据表*/
    ORMAP(PokemonInfo, name, owner, nature, kind, character, level, experiencePoint,
          attackPoint, defencePoint, totalHP, currentHP, intervalIncrease,
          criticalPoint, state, sickCounter, sickPoint, alive)
};

class Pokemon
{
private:
    string _owner_;
    Nature _nature_;
    Kind _kind_;
    string _name_;
    Character _character_;
    int _level_;
    int _experiencePoint_;
    int _attackPoint_;
    int _defencePoint_;
    int _totalHP_;
    int _currentHP_;
    int _intervalIncrease_;
    int _criticalPoint_;
    vector<Nature> _counter_; //克制的小精灵属性
    State _state_;
    int _sickCounter_;
    int _sickPoint_;
    bool _alive_;
public:
    Pokemon(){}
    ~Pokemon(){}
    struct PokemonInfo ToPokeStruInfo();
    void setOwner(std::string o);
    void setNature(Nature nature);
    void setKind(Kind kind);
    void setName(string name);
    void setCharacter(Character character);
    void setLevel(int level);
    void setExperiencePoint(int experiencePoint);
    void setAttackPoint(int attackPoint);
    void setDefencePoint(int defencePoint) ;
    void setTotalHP(int totalHP);
    void setCurrentHP(int currentHP);
    void setIntervalIncrease(int intervalIncrease);
    void setCriticalPoint(int criticalPoint);
    void setCounter(vector<Nature> counter);
    void setState(State state);
    void setSickCounter(int sickCounter);
    void setSickPoint(int sickPoint);
    void setAlive(bool alive);
    std::string getOwner();
    Nature getNature();
    Kind getKind();
    string getName();
    Character getCharacter();
    int getLevel();
    int getExperiencePoint();
    int getAttackPoint();
    int getDefencePoint();
    int getTotalHP();
    int getCurrentHP();
    vector<Nature> getCounterVec();
    State getState();
    int getCriticalPoint();
    int getSickCounter();
    int getSickPoint();
    bool getAlive();
    int getIntervalIncrease();
    void Attack(Pokemon *dePokemon); //攻击
    virtual void SpecialAttack(Pokemon *dePokemon){} //特殊攻击虚函数
    void SpecialAttackDamage(Pokemon *dePokemon); //特殊攻击造成伤害
    virtual void EnSick(Pokemon *sickPokemon){} //特殊攻击致病虚函数
    bool EnSickPossible(); //致病几率
    bool CriticalStrike(); //暴击几率
    void Hurt(); //不健康状态损失血量
    void DeadJudge(); //判定存活与否
    void Upgrade(); //升级
    void Evolution(int evoLevel); //进化
    /*重载 < 用于存放用户拥有的小精灵*/
    bool operator < (const Pokemon& anotherPokemon) const
    {return _level_ < anotherPokemon._level_;}
};

class Fire : public Pokemon
{
public:
    Fire(Kind kind, int level, string name, string owner);
    Fire(struct PokemonInfo pokemonInfo); //根据struct创建火属性小精灵
    ~Fire() {}
    void EnSick(Pokemon *sickPokemon); //造成烧伤状态
    void SpecialAttack(Pokemon *dePokemon); //火属性特殊攻击
};

class Water : public Pokemon
{
public:
    Water(Kind kind, int level, string name, string owner);
    Water(struct PokemonInfo pokemonInfo); //根据struct创建水属性小精灵
    ~Water() {}
    void EnSick(Pokemon *sickPokemon); //造成溺水状态
    void SpecialAttack(Pokemon *dePokemon); //水属性特殊攻击
};

class Bush : public Pokemon
{
public:
    Bush(Kind kind, int level, string name, string owner); 
    Bush(struct PokemonInfo pokemonInfo); //根据struct创建草属性小精灵
    ~Bush() {}
    void EnSick(Pokemon *sickPokemon); //造成中毒
    void SpecialAttack(Pokemon *dePokemon); //草属性特殊攻击
};

class Electricity : public Pokemon
{
public:
    Electricity(Kind kind, int level, string name, string owner);
    Electricity(struct PokemonInfo pokemonInfo); //根据struct创建电属性小精灵
    ~Electricity() {}
    void EnSick(Pokemon *sickPokemon); //造成触电
    void SpecialAttack(Pokemon *dePokemon); //电属性特殊攻击
};
unsigned int Random(int max);
```

#### **PokemonFactory(口袋妖怪简单工厂类)** `src/pokemonfactory.h`

```c++
class PokemonFactory
{
public:
    PokemonFactory(){}
    ~PokemonFactory(){}
    Pokemon *CreatePokemon(Kind kind, int level, string name, string owner); //根据种类，等级，名字，训练家创建小精灵
    Pokemon *CreatePokemon(struct PokemonInfo pokemonInfo); //根据struct创建小精灵
};
```

#### **Session(会话类)** `src/session.h`

```c++
int AttGenFunc(int MIN, int MAX); //生成攻击方式，普通攻击/特殊攻击
bool SortByRank(const struct PlayerInfo& p1, const struct PlayerInfo& p2); //传入sort函数作为排序依据
void GetSignInSendStr(int pid, json recvJ, json& sendJ); //登录操作
void GetSignOnSendStr(int pid, json recvJ, json& sendJ); //注册操作
void GetAcquaintanceSendStr(json recvJ, json& sendJ); //分配御三家操作
void GetOnlinePlayerSendStr(json recvJ, json& sendJ); //查看在线用户操作
void GetMyinfoSendStr(json recvJ, json& sendJ); //查看本人信息操作
void GetHuntSendStr(int pid, json recvJ, json& sendJ); //捕捉小精灵操作
void GetRankSendStr(json recvJ, json& sendJ); //查看排位操作
void GetMyPokeSendStr(std::string symbol, json recvJ, json& sendJ); //查看背包/仓库操作
void GetExchangeSendStr(std::string symbol, json recvJ, json& sendJ); //放回取出小精灵操作
void GetPlayerPokeSendStr(json recvJ, json& sendJ); //查看其他用户小精灵操作
void GetThumbSendStr(json recvJ, json& sendJ); //点赞操作
void GetBattleSendStr(json recvJ, json& sendJ); //对战操作
std::string GetSendStr(int pid, Helper* helper);
std::vector<Pokemon*> GetMyPokeCrew(std::string owner); //获取玩家小精灵战队
std::vector<Pokemon*> GetEnemyPokeCrew(int pokeNum, std::string nature, std::string standard); //获取电脑战队
void GetCrewInfo(int pokeNum, std::vector<Pokemon*> myPokemon, std::vector<Pokemon*> enemyPokemon, json& sendJ); //获取双方战队信息
void ResolveBarConflict(int& myBar, int& enemyBar, int InitiativeBar); //解决行动条冲突
void Defeat(json& sendJ, std::string owner, std::vector<Pokemon*> myPokemon, std::vector<Pokemon*> enemyPokemon); //失败
void Win(json& sendJ, std::string owner, std::vector<Pokemon*> myPokemon, std::vector<Pokemon*> enemyPokemon); //胜利
```

#### **SocketServer(服务器socket类)** `src/socketServer.h`

```c++
SOCKET cSock[MAXSIZE_POOL] = { INVALID_SOCKET }; //sokcet池用于存放client socket
std::string onlinePlayer[MAXSIZE_POOL] = {""}; //在线用户记录
const std::string nullPlayer = ""; //空玩家
const std::string Permision = "permision"; //连接请求通过
const std::string Deny = "Thread pool is full, please wait"; //连接请求拒绝
class SocketServer
{
public:
	SocketServer(){}
	~SocketServer(){}
	void Prepare(); //创建socket
	void InitWinSock(); //初始化socket
	void InitCheckListenSock(); //初始化监听socket
	void BindSock(); //绑定监听socket
	void ListenSock(); //监听
	void ShutDown(); //关闭socket
	int existingClientCount = 0; //已连接的客户端数
	SOCKET ListenSocket = INVALID_SOCKET;
private:
	WSADATA wsaData;
	int iResult;
	struct addrinfo *result = NULL;
	struct addrinfo hints;
};
```

### PokemonClient

#### **Acquaintance(分配御三家类)** `src/acquaintance.h`

```c++
class Acquaintance : public QWidget
{
    Q_OBJECT

public:
    explicit Acquaintance(SocketClient *sc, QWidget *parent = 0);
    explicit Acquaintance(QWidget *parent = 0);
    ~Acquaintance();
    SocketClient *socketClient; //socket client
private:
    void InitUI(); //初始化UI
    void InitConnect(); //关联signal和slot
    void SetEventFilter(); //设置事件过滤器
    Ui::Acquaintance *ui;
private slots:
    void receiveSwitch(); //接受到切换至此信号
    void onChose(QObject* obj); //选定御三家
    bool eventFilter(QObject *watched, QEvent *event); //事件过滤器
signals:
    void SwitchToMainPage(); //发出切换至主界面信号
    void choseSignal(QObject* obj); //发出选定信号
};
```

#### **Battle(对战类)** `src/battle.h`

```c++
const int CAPACITY = 3; //背包容量为3
class Battle : public QWidget
{
    Q_OBJECT

public:
    SocketClient *socketClient; //client socket
    explicit Battle(SocketClient *sc, QWidget *parent = 0);
    explicit Battle(QWidget *parent = 0);
    ~Battle();
    std::string getNatureStd() { return this->natureStd; } //获取用户指定的电脑属性
    std::string getStandardStd() { return this->standardStd; } //获取用户指定的电脑级别

private:
    void InitConnect();
    void InitUI();
    Ui::Battle *ui;
    QHBoxLayout* endHLayout = new QHBoxLayout(); //对战结果界面水平布局
    QLabel* endPokeLabel[CAPACITY]; //对战结果界面小精灵头像
    QLabel* endInfoLabel[CAPACITY]; //对战结果小精灵信息
    std::vector<std::string> myPokemonName; //用户参战小精灵名字
    std::vector<std::string> myPokemonKind; //用户参站小精灵战前种类
    std::vector<std::string> myEvoKind; //用户参战小精灵战后种类
    std::vector<int> myPokemonTHP; //用户参展小精灵总血量
    std::vector<std::string> enemyPokemonName; //电脑小精灵名字
    std::vector<std::string> enemyPokemonKind; //电脑小精灵种类
    std::vector<int> enemyPokemonTHP; //电脑小精灵总血量
    int myPokeIndex = 0; //用户当前战斗小精灵序号
    int enemyPokeIndex = 0; //电脑当前战斗小精灵序号
    int myPokeCurrentHP = 0; //用户当前战斗小精灵血量
    int enemyPokeCurrentHP = 0; //电脑当前战斗小精灵血量
    std::string natureStd = ""; //用户指定的电脑属性
    std::string standardStd = ""; //用户指定的电脑级别

signals:
    void switchToMainPage(); //转到主界面
    void battleNotQualified(); //背包空，无法战斗
    void WinOrLoseSignal(bool win, int expGot); //对战结果显示
    void MyPokeAttackSignal(QString attway, QString myNature); //用户小精灵攻击
    void EnemyPokeAttackSignal(QString attway, QString enemyNature); //电脑小精灵攻击
    void MyPokeBeAttacked(int mypokehp); //用户小精灵受到攻击
    void EnemyPokeBeAttacked(int enemypokehp); //电脑小精灵受到攻击
    void MyPokeHurtSignal(int myhurthp, QString enemyNature); //用户小精灵异常状态损失血量
    void EnemyPokeHurtSignal(int enemyhurthp, QString myNature); //电脑小精灵异常状态损失血量
    void ClearHurtSignal(); //消除异常状态动画
    void ClearHPDecSignal(); //消除血量减少动画
    void ClearSpecialAttSignal(); //消除特殊攻击动画
    void MyPokeChangeSignal(int mypokeindex); //用户小精灵轮换
    void EnemyPokeChangeSignal(int enemypokeindex); //电脑小精灵轮换

private slots:
    void receiveSwitch(QString nature, QString standard);
    void getRecvStr(QString recvStr); //获取对战信息
    void onBattleNotQualified();
    void WinOrLose(bool win, int expGot);
    void onMyPokeAttack(QString attway, QString myNature);
    void onEnemyPokeAttack(QString attway, QString enemyNature);
    void onMyPokeBeAttacked(int mypokehp);
    void onEnemyPokeBeAttacked(int enemypokehp);
    void onMyPokeHurt(int myhurthp, QString enemyNature);
    void onEnemyPokeHurt(int enemyhurthp, QString myNature);
    void ClearHurt();
    void ClearHPDec();
    void ClearSpecialAtt();
    void onMyPokeChange(int mypokeindex);
    void onEnemyPokeChange(int enemypokeindex);
    void ClearBattle(); //还原对战界面
};
```

#### **Hunt(捕捉类)** `src/hunt.h`

```c++
#define ZMin 400; //模拟远近视角
#define ZMax 600; //模拟远近视角
const int EmbryoPokemon[] = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33}; //初级形态小精灵
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
                         "Mareep", "Flaaffy", "Ampharos"}; //所有小精灵的种类
class Hunt : public QWidget
{
    Q_OBJECT

public:
    SocketClient* socketClient; //client socket
    explicit Hunt(SocketClient* sc, QWidget *parent = 0);
    explicit Hunt(QWidget *parent = 0);
    ~Hunt();

private:
    void InitUI(); //初始化UI
    void InitConnect(); //关联signal和slot
    void GetPokeKind(); //获得野生小精灵种类
    int GetZ(); //获得野生小精灵的远近距离
    QMovie* GetMovie(); //获得小精灵的gif资源
    void SetPokeGif(); //设置小精灵gif位置和大小
    void SetPokeBallPng(); //设置精灵球的位置
    Ui::Hunt *ui; 
    bool ballPressed = false; //精灵球选定标志
    bool caught = false; //捕获小精灵标志
    QMovie* movie; //小精灵gif资源
    std::string kindStr; //小精灵的种类
    std::string name; //小精灵的名字

private slots:
    void receiveSwitch(); //切换至主界面
    void backClicked(); //返回主界面
    void showWord(bool caught); //显示野生小精灵的话语
    void catchPokemon(); //捕捉小精灵
    bool eventFilter(QObject *watched, QEvent *event); //事件过滤器

signals:
    void switchToMainPage(); //切换至主界面
};

```

#### **MainPage(主页面类)** `src/mainpage.h`

```c++
#define MAXSIZE_PLAYER 10 //最多可显示的用户数目
#define TABLE_ROW 3 //小精灵table widget行数
#define TABLE_COL 6 //小精灵table widget列数
#define MAXSIZE_POKEMON 15 //最多可显示的小精灵数目
const int PokeNumLevel[3] = {5, 10, 20}; //神奇宝贝训练家徽章标准，根据小精灵数目
const int RankLevel[3] = {10, 50, 100}; //斗技大神徽章标准，根据排位分数
const double RateLevel[3] = {20, 50, 80}; //常胜将军徽章标准，根据胜率
class MainPage : public QWidget
{
    Q_OBJECT
public:
    SocketClient* socketClient; //client socket
    explicit MainPage(SocketClient* sc, QWidget *parent = 0);
    explicit MainPage(QWidget *parent = 0);
    ~MainPage();
private:
    void InitUI(); //初始化UI
    void SetEventFilter(); //设置事件过滤器
    void SetToolTips(); //设置tool tip
    void SetCursors(); //设置cursor
    void SetStyles(); //设置样式表
    void InitPlayerArea(); //初始化在线用户/排位区域
    void InitPokemonArea(); //初始换小精灵区域
    void InitConnect(); //关联signal和slot
    void LoadOnlinePlayer(json &recvJ); //加载在线用户
    void RecvAndSendOnlinePlayer(json j); //与服务器交换信息
    Ui::MainPage *ui;
    std::string recvString = ""; //接受信息缓存
    int delHLayIndex = -1; //放回/取出小精灵时删除表项的记录
    bool exClicked = false; //放回/取出按钮按下标志
    QTableWidgetItem* pokeTableItem[MAXSIZE_POKEMON]; //展示小精灵table widget
    QLabel* headLabel[MAXSIZE_PLAYER]; //在线用户头像
    QPushButton* playerPokeButton[MAXSIZE_PLAYER]; //在线用户小精灵按钮
    QPushButton* thumbButton[MAXSIZE_PLAYER]; //在线用户点赞按钮
    QPushButton* rankPokeButton[MAXSIZE_PLAYER]; //排位用户小精灵按钮
    QPushButton* rankThumbButton[MAXSIZE_PLAYER]; //排位用户点赞按钮
    QVBoxLayout* scrollVLayout; //用户背包/仓库竖直布局
    QHBoxLayout* scrollHLayout[MAXSIZE_POKEMON]; //用户背包/仓库水平布局
    QLabel* pokePicLabel[MAXSIZE_POKEMON]; //用户小精灵图标
    QLabel* pokeTextLabel[MAXSIZE_POKEMON]; //用户小精灵名字
    QPushButton* exButton[MAXSIZE_POKEMON]; //用户取出/放回按钮
    std::string playerNames[MAXSIZE_PLAYER]; //在线用户名字记录
    std::string rankPlayerNames[MAXSIZE_PLAYER]; //排位用户名字记录
    std::vector<std::string> packPokemon; //背包精灵名字
    std::vector<std::string> stoPokemon; //仓库精灵名字

private slots:
    void receiveSwitch(); //接收到切换至此信号
    void onOnlinePlayerClicked(); //加载在线用户
    void onOnlinePlayerReloadClicked(); //刷新在线用户
    void onPlayerPokeClicked(int i); //加载在线用户小精灵
    void onPlayerThumbClicked(int i); //给在线用户点赞
    void onMyInfoClicked(); //加载本人信息
    void onRankClicked(); //加载排位信息
    void onRankPokeClicked(int i); //加载排位玩家小精灵
    void onRankThumbClicked(int i); //给排位玩家点赞
    void onPackageClicked(); //加载背包
    void onStorageClicked(); //加载仓库
    void onUpDownClicked(); //展示/隐藏战前选择
    void onExButtonClicked(QObject* obj, int i); //取出/放回小精灵
    Q_INVOKABLE bool getRecvStr(QString str); //获取服务器信息
    bool eventFilter(QObject *watched, QEvent *event); //事件过滤器
    void SetOnlinePlayerIcon(int i); //设置在线用户界面图标
    void SetRankIcons(int i); //设置排位界面图标
    void SetPackageScrollArea(QString symbol, QString kind, QString name, QString tip, int index); //设置背包/仓库界面
    void ClearScrollArea(QString packorsto); //还原背包/仓库界面
    void SetMyInfo(int pokenum, int rank, double rate, QString info); //设置本人信息界面
    void OpenInChrome(const QUrl& url); //在默认浏览器打开GitHub本项目页
    void ExchangePokemon(QString symbol, bool success); //取出/放回小精灵
    void SwitchClear(); //还原本界面

signals:
    void SwitchToHunt();
    void SwitchToBattle(QString nature, QString standard);
    void SetOnlinePlayerIconSignal(int i);
    void SetRankIconSignal(int i);
    void SetPackegeScrollAreaSignal(QString symbol, QString kind, QString name, QString tip, int index);
    void SetMyInfoSignal(int pokenum, int rank, double rate, QString info);
    void ClearScrollAreaSignal(QString packorsto);
    void PlayerPokeClicked(int i);
    void PlayerThumbClicked(int i);
    void RankPokeClicked(int i);
    void RankThumbClicked(int i);
    void ExButtonClicked(QObject* obj, int i);
    void ExchangePokeSignal(QString symbol, bool success);
};

```

#### **Sign In(登录类)** `src/signin.h`

```c++
class SignIn : public QWidget
{
    Q_OBJECT

public:
    SocketClient *socketClient; //client socket
    explicit SignIn(SocketClient* sc, QWidget *parent = 0);
    explicit SignIn(QWidget *parent = 0);
    ~SignIn();

private:
    void InitUI(); //初始化UI
    void SetGosankeGif(); //设置御三家gif
    void SetEventFilter(); //设置事件过滤器
    void InitConnect(); //关联signal和slot
    Ui::SignIn *ui;
    std::string recvString = ""; //接受信息缓存

private slots:
    void onSignInClicked(); //登录按钮点击
    void onTopSignOnClicked(); //注册按钮点击，切换至注册界面
    void receiveSwitch(); //接受到切换至此信号
    Q_INVOKABLE bool setRecvStrSignIn(QString s);
    bool eventFilter(QObject *watched, QEvent *event); //事件过滤器

signals:
    void switchToSignOn(); //切换至注册界面
    void switchToMainPage(); //切换主界面
};
```

#### **Sign On(注册类)** `src/signon.h`

```c++
class SignOn : public QWidget
{
    Q_OBJECT

public:
    SocketClient* socketClient; //client socket
    explicit SignOn(SocketClient* sc, QWidget *parent = 0);
    explicit SignOn(QWidget *parent = 0);
    ~SignOn();

private:
    void InitUI(); //初始化UI
    void SetGosankeGif(); //设置御三家gif
    void SetEventFilter(); //设置时间过滤器
    void InitConnect(); //关联signal和slot
    Ui::SignOn *ui;
    bool isUNNotCLK = true; //user name 文本框点击标志
    bool isPWNotCLK = true; //password文本框点击标识
    bool isEPWNotCLK = true; //密码确认文本框点击标识
    std::string recvString = ""; //接受信息缓存

private slots:
    void onTopSignInClicked(); //切换至登录
    void signOnButtonClicked(); //注册
    void receiveSwitch(); //接受切换至此信号
    Q_INVOKABLE bool setRecvStrSignOn(QString s);
    bool eventFilter(QObject *watched, QEvent *event); //事件过滤器

signals:
    void switchToSignIn(); //切换至登录界面
    void switchToAcq(); //切换至分配御三家界面
};
```



#### **SocketClient(客户端socket类)** `src/socketclient.h`

```c++
#define DEFAULT_BUFLEN 10240 //传送信息的最大长度
#define DEFAULT_PORT "27015" //端口号
class SocketClient
{
public:
    SocketClient() {}
    ~SocketClient() { this->Cleanup(); }
	void Prepare(); //创建socket
	void InitWinSock(); //初始化socket
	void ResolveAddrPort(); //解析地址
	void ConnectToServer(); //与server建立连接
	void Try(); //尝试与服务器建立连接
	void Shutdown(); //关闭socket
    void setPlayerName(std::string playerName);
    std::string getPlayerName();
	int Receive(); //从服务器接受
	void Cleanup(); //释放资源
	void ClearRecvBuf(); //清空recvbuf
	std::string GetRecvStr();
	SOCKET getConnectSocket(); //获得connect socket
    char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
private:
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;
    std::string playerName = "";
};
```



