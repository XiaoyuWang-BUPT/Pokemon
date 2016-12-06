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

### v0.1.7(2016/11/19)

* 对`Player`测试创建新玩家对象并输出信息
* 数据库小精灵实例化 传`struct PokemonInfo` 
* 捕捉精灵 对战精灵实例化 传`kind level name`
* 玩家实例化 传`struct PlayerInfo`新玩家新信息 老玩家从数据库获取
* TODO

  * 将`Player` 和 `Pokemon` 与数据库链接测试
  * 将`Socket Server`和`Socket Client`分别添加至*project server* 和 *project client*
  * 添加`session.h` `session.cpp`测试会话


### v0.1.8(2016/11/20)

* `Player`与数据库测试连接
* `PoorORMLite`修复bug(`insert`语句不正确)
* 新增`Helper.h`用于消除未知的头文件包含问题
* TODO 
  * 将`Pokemon` 与数据库链接测试
  * 将`Socket Server`和`Socket Client`分别添加至*project server* 和 *project client*
  * 添加`session.h` `session.cpp`测试会话

### v0.1.9(2016/11/21)

* `Pokemon`与数据库测试连接
* `PoorORMLite`修复bug(`insert`语句*sqlCmd*中*where*前无空格 导致数字无法得到识别 但字符串可以识别)
  * 将`Socket Server`和`Socket Client`分别添加至*project server* 和 *project client*
  * 添加`session.h` `session.cpp`测试会话

### v0.2.0(2016/11/23)

* `PokemonServer`初步完成`Sign In登录` `Sign On注册`界面及跳转
* TODO
  * 将`Socket Server`和`Socket Client`分别添加至*project server* 和 *project client*
  * 添加`session.h` `session.cpp`测试会话

### v0.2.1(2016/12/1)

* `添加Socket Server` 和 `Socket Client`到项目
* 可实现登录 操作即 `sign in` ，客户端发送用户名及密码，服务器在数据库查询后返回查询结果
* 添加`json.hpp`实现信息传输的序列化和反序列化
* TODO
  * 在`Server/Client`创建*Helper Class*用于取代*extern*，通过传*Helper Object*实现面向对象地在多个`.cpp`中访问值
  * 规范通信协议
  * 完善*unit test*


### v0.2.2(2016/12/2)

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

### v0.2.3(2016/12/2)

* 完善*Unit test*测试`pokemon`的属性
* 调整玩家对战机制，玩家发起对战请求，服务器在其他请求对战的用户中选择建立对战
* **被呼叫**功能:只在主页查看聊天信息。进入主界面时创建`CalledThread`线程用于`recv`，接受到后显示。玩家发送聊天信息，则同普通发送行为

### v0.2.4(2016/12/3)

* `client`端删除冗余的*Helper Class*
* `CalledThread`加入`socketClient`用于`recv`由于目前尚未在`server`端实现`call`功能故`CalledThread`处于阻塞在`recv`阶段

### v0.2.5(2016/12/3)

* 实现注册功能
* 实现焦点高亮
* 支持`Key-Tab` 和 `Key_Enter / Key_Return` 操作

### v0.2.6(2016/12/4)

* 美化UI

### v0.2.7(2016/12/4)

* `server`端增加`onlinePlayer`数组存储在线用户的`name` 和 `rank`
* `client:MainPage统一CalledThreadFunc`和`RecvThreadFunc`到`RecvThreadFunc`并将线程初始化置于`RecvSwitch`中，即切换到主界面即开始阻塞式接受，根据接受内容调用负责显示的功能函数

### v0.2.8(2016/12/5)

* `client`端加入`Hunt`界面用于捕捉小精灵

### v0.2.9(2016/12/6)

* 完成捕捉动画并传送捕捉到的精灵信息
* TODO
  * 完善服务器接受到捕捉信息后更新数据操作