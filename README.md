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

      ​