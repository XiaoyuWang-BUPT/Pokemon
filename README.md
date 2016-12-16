# Pokemon 

### 2016 BUPT C++ Programming Experiment -- Pokemon

----

## Change  Log

### v0.1.0 (2016/10/9)

* 完成Pokemon基类
* TODO 
  * 派生类覆盖虚函数 

  * 单元测试


### v0.1.1 (2016/10/10)

* 改善构造函数
* 初步完成工厂模式
* 优化了随机产生属性的方法
* TODO
  * 特殊攻击函数加血bug修复
  * 用户自定义小精灵名字 当前无名字属性
  * 单元测试
  * 派生类的Evolution函数


### v0.1.2 (2016/10/11)

* 特殊攻击函数封装 加血bug修复
* 输出小精灵状态
* 修复细节bug
* 将Evolution函数改为Pokemon类的成员函数
* TODO
  * 放弃自定义小精灵名字
  * 单元测试

###  v0.1.3 (2016/10/12)

* 修复升级和进化bug
* 完成单元测试
* 注意: Create口袋妖怪时应将Kind和Level匹配才可以成功创建


### v0.1.4 (2016/10/13)

* 确定C/S框架 原源程序在Server端运行 将QWidget程序改为QConsole程序
* 存在自定义小精灵名字的方法
* TODO
  * server端数据库
  * server client 间 socket通信

### v0.1.5 (2016/10/14)

* 创建Player类
* Client端窗口切换
* 使用SQLite3维护数据库
* TODO
  * 学习ORMLite动态创建数据库
  * server client间 socket通信

### v0.1.6 (2016/10/21)

* 完成PoorORMLite 添加`PoorORMLite.h`到*server* 

* TODO

  *   创建与`Class Player`对应的`Struct Player`存储`Player` 信息 `Primary Key`为`name`
  *   创建与`Class Pokemon`对应的`Struct Pokemon`存储`Pokemon`信息 `Primary Key`为`name` 并存储`owner`作为`condition: Where`

### v0.1.7 (2016/11/19)

* 对`Player`测试创建新玩家对象并输出信息
* 数据库小精灵实例化 传`struct PokemonInfo` 
* 捕捉精灵 对战精灵实例化 传`kind level name`
* 玩家实例化 传`struct PlayerInfo`新玩家新信息 老玩家从数据库获取
* TODO

  * 将`Player` 和 `Pokemon` 与数据库链接测试
  * 将`Socket Server`和`Socket Client`分别添加至*project server* 和 *project client*
  * 添加`session.h` `session.cpp`测试会话


### v0.1.8 (2016/11/20)

* `Player`与数据库测试连接
* `PoorORMLite`修复bug(`insert`语句不正确)
* 新增`Helper.h`用于消除未知的头文件包含问题
* TODO 
  * 将`Pokemon` 与数据库链接测试
  * 将`Socket Server`和`Socket Client`分别添加至*project server* 和 *project client*
  * 添加`session.h` `session.cpp`测试会话

### v0.1.9 (2016/11/21)

* `Pokemon`与数据库测试连接
* `PoorORMLite`修复bug(`insert`语句*sqlCmd*中*where*前无空格 导致数字无法得到识别 但字符串可以识别)
  * 将`Socket Server`和`Socket Client`分别添加至*project server* 和 *project client*
  * 添加`session.h` `session.cpp`测试会话

### v0.2.0 (2016/11/23)

* `PokemonServer`初步完成`Sign In登录` `Sign On注册`界面及跳转
* TODO
  * 将`Socket Server`和`Socket Client`分别添加至*project server* 和 *project client*
  * 添加`session.h` `session.cpp`测试会话

### v0.2.1 (2016/12/1)

* `添加Socket Server` 和 `Socket Client`到项目
* 可实现登录 操作即 `sign in` ，客户端发送用户名及密码，服务器在数据库查询后返回查询结果
* 添加`json.hpp`实现信息传输的序列化和反序列化
* TODO
  * 在`Server/Client`创建*Helper Class*用于取代*extern*，通过传*Helper Object*实现面向对象地在多个`.cpp`中访问值
  * 规范通信协议
  * 完善*unit test*


### v0.2.2 (2016/12/2)

* 创建*Helper* 消除*extern*

* 消除了*while(true)*，降低了CPU占用率

* `PokemonClient / main.cpp`关键改动

  ```c++
  // - socketClient->Cleanup()
  // - delete socketClient;
  // Instead,we call Cleanup() in ~socketClient() && we delete socketClient in ~SignIn()
  ```

* TODO
  * 对方通信协议
  * 完善*Unit test*
  * `Client`端优化过程损失了**被呼叫**功能 (创建线程`recv`)

### v0.2.3 (2016/12/2)

* 完善*Unit test*测试`pokemon`的属性
* 调整玩家对战机制，玩家发起对战请求，服务器在其他请求对战的用户中选择建立对战
* **被呼叫**功能:只在主页查看聊天信息。进入主界面时创建`CalledThread`线程用于`recv`，接受到后显示。玩家发送聊天信息，则同普通发送行为

### v0.2.4 (2016/12/3)

* `client`端删除冗余的*Helper Class*
* `CalledThread`加入`socketClient`用于`recv`由于目前尚未在`server`端实现`call`功能故`CalledThread`处于阻塞在`recv`阶段

### v0.2.5 (2016/12/3)

* 实现注册功能
* 实现焦点高亮
* 支持`Key-Tab` 和 `Key_Enter / Key_Return` 操作

### v0.2.6 (2016/12/4)

* 美化UI

### v0.2.7 (2016/12/4)

* `server`端增加`onlinePlayer`数组存储在线用户的`name` 和 `rank`
* `client:MainPage统一CalledThreadFunc`和`RecvThreadFunc`到`RecvThreadFunc`并将线程初始化置于`RecvSwitch`中，即切换到主界面即开始阻塞式接受，根据接受内容调用负责显示的功能函数

### v0.2.8 (2016/12/5)

* `client`端加入`Hunt`界面用于捕捉小精灵

### v0.2.9 (2016/12/6)

* 完成捕捉动画并传送捕捉到的精灵信息
* TODO
  * 完善服务器接受到捕捉信息后更新数据操作

### v0.3.0 (2016/12/7)

* 完成捕捉信息的传送和数据库更新操作

### v0.3.1 (2016/12/8)

* 完成在线用户列表 增加点赞功能
* TODO
  * 查看在线用户的宠物小精灵

### v0.3.2 (2016/12/8)

* 完成查看当前玩家信息

### v0.3.3 (2016/12/9)

* 完成查看排位信息
* TODO
  * 查看宠物小精灵

### v0.3.4 (2016/12/9)

* 修复捕捉结束返回主界面查看信息延迟的bug
* 可查看玩家拥有的不限于背包中的小精灵
* 可查看玩家胜率
* 添加显示版本信息显示 链接至`GitHub`
  * 查看玩家自己的背包和仓库


### v0.3.5 (2016/12/10)

* 玩家可查看自己背包中的宠物小精灵
* 修正 '关闭`rank`和`online player`窗口时，`pokemon table`子窗口不关闭' 的bug

### v0.3.6 (2016/12/10)

* 玩家可查看仓库中的小精灵
* 增加三种成就(神奇宝贝训练家，斗技达人，常胜将军)
* TODO
  * 将背包中的小精灵放回仓库， 将仓库中的小精灵放入背包

### v0.3.7 (2016/12/11)

* 玩家可将小精灵放回仓库，可将小精灵放入背包
* 修复游戏时间显示超长的bug
* TODO
  * 对战

### v0.3.8 (2016/12/11)

* 添加`选择对战`功能
* TODO
  * 对战

### v0.3.9 (2016/12/12)

* `server`添加战斗准备部分，创建战斗的实例对象
* TODO
  * 对战

### v0.4.0 (2016/12/13)

* 服务器实现普通攻击逻辑
* TODO
  * 服务器特殊攻击
  * 客户端显示

### v0.4.1 (2016/12/14)

* 初步完成服务器端对战功能
* 修复了将最末位小精灵"放回仓库"和"放入背包"崩溃的bug
* TODO
  * 客户端显示对战

### v0.4.2 (2016/12/15)

* 初步完成显示对战
* TODO
  * bug攻击时血条有情况不减
  * bug对局结束升级的情况下，立刻再次战斗，程序崩溃
  * bug对局进行时返回，再次对战时继续上一次对战

### v0.4.3 (2016/12/15)

* 对局不结束，不可返回主界面
* 增加受到减益效果时的画面
* TODO
  * 血条有时不减少，卡顿
  * 对局结束升级，立刻再次战斗，程序崩溃
  * 有时对局进行中画面静止

### v0.4.4 (2016/12/15)

* 血条改为`widget`实现，血条减少动画流畅
* 修复`切换小精灵时血条为空`的bug
* 修复`服务器端同时完成行动条冲突`的bug
* TODO
  * 修复`对局结束升级，立即再次战斗，程序崩溃`的bug
  * 战斗结束后显示小精灵获得的经验、升级与否、进化与否
  * 修复`用户胜率只能为0,1`的bug(整型运算的舍去处理造成)

### v0.4.5 (2016/12/16)

* 修复`对局结束升级，立即再次战斗，程序崩溃`的bug
* 修复`用户胜率0,1`的bug
* TODO
  * 回合过多的情况下，服务器发送对战信息失败
  * 战斗结束后显示小精灵获得的经验、升级与否、进化与否


### v0.4.6 (2016/12/16)

* 修复`回合过多，服务器发送对战信息失败`，原因:小精灵满级后的升级条件不合理
* TODO
  * 战斗结束后显示小精灵获得的经验、升级与否、进化与否