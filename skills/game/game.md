# **Game开发技巧**



## **1. B*自动寻路算法**
> ### **1.1 定义**
    1) 探索节点: 在寻路过程中向前探索的节点为探索节点, 其实节点就是探索节点;
    2) 自由探索节点:
       探索节点朝着目标前进, 如果前方不是阻挡, 探索节点可以继续向前进入下一
       个地图格子, 这种探索节点自由探索节点;
    3) 绕爬的探索节点:
       探索节点朝着目标前进, 如前方阻挡, 探索节点将试图绕过阻挡, 绕行的探索
       节点就是绕爬的探索节点;
> ### **1.2 算法**
    1) 探索节点为自由节点, 从原点出发, 向目标前进;
    2) 自由节点前进中判断前面是否为障碍:
       a) 不是障碍, 向目标前进一步, 仍为自由节点;
       b) 是障碍, 以前方障碍为界, 分左右两个分支, 分别视图绕过障碍, 这两个分
          支节点就成了两个绕爬的探索节点
    3) 绕爬的探索节点绕过障碍后又成为自由节点, 回到2);
    4) 探索节点前进后, 判断当前地图格子是否为目标格子, 如果是则寻路成功, 根
       据寻路过程构造出完整路径;
    5) 寻路过程中, 如果探索节点没有了, 则寻路结束, 表明没有目标格子, 不能够
       到达;



## **2. 简易自动寻路算法**
    1) 在场景中标记出若干waypoint, 这些点一般在大片无障碍区域的中央以及分叉
       路口(可以靠人工设置); 
       Waypoint设置重点: 场景中所有可达区域都能至少看到一个waypoint;
    2) 将所有waypoint用直线连接起来, 得到一张图;
    3) 每次需要得到场景任意两点间的路径, 从起始点和目标点各取一个临近可见的
       waypoint, 然后求这两个waypoint之间在图上的最短路径;
    4) 得到路径后角色在场景行走时, 间隙判断是否可以直接看到路径上的下一个
       waypoint, 只要可以看到就沿直线朝路径上最远可见的waypoint移动;



## **3. 简易排行**
    适用与积分区间不大(0 ~ 5000)
    1) 做5000个桶, 每一个桶里记录这个分数有多少个人;
    2) 玩家分数变化, 把原来的桶玩家数减一, 新的桶加一;
    3) 排行查询仅需要把分数靠前的桶累加, 则可以知道查询者的名次;
    4) 真正需要精确知道的人名是榜单前n(100)个, 对前n个做排序也很快;



## **4. 海量玩家积分排行**
> ### **4.1 简单SQL查询**
    select 1 + count(uid) from rank where uid = uid and score > uscore;
    主要应对数据规模比较小, 性能要求不是太高的情况;
> ### **4.2 均匀分区设计**
    积分按照1000分区段分表, 更新数据的时候相应修改对应区段的表;
    玩家的积分首先可确定其所属的区段, 把高于s积分的区段的count累加, 在查出
    玩家在该用户区段的排名, 二者相加即可;
> ### **4.3 积分排名数组**
    用一个大小为1000000的数组表示积分和排名对应关系, rank[s]表示s积分所对应
    的排名;
    当积分从s变成s+n后, 只需要将rank[s]~rank[s+n-1]n个元素的值增加1即可;



## **5. MySQL读写性能**
    * 测试的是每秒的吞吐量
    * 写性能
      不开binlog, guid主键, 无索引: myisam(1639), innodb(971)
      开binlog, guid主键, 无索引:   myisam(675), innodb(374)
      开binlog, guid主键, 有索引:   myisam(599), innodb(329)
      开binlog, 自增值主键, 有索引: myisam(595), innodb(347)
    * 读性能
      guid主键:       myisam(2372), innodb(2553)
      自增值主键:     myisam(2195), innodb(2273)
    
    * 结论:
      1) 开启binlog, 写性能下降60%
      2) myisam与innodb相比, 写速度更快(快40%), 读速度差不多;


## **6. redis**
> ### **6.1 优点**
    1) 高性能 - 支持超过100K+/s的读写频率;
    2) 数据类型丰富 - strings, lists, hashes, sets和sorted sets;
    3) 原子 - 所有操作都是原子操作, 还支持几个操作合并后的原子性执行;
    4) 丰富的特性 - 支持publish/subscribe, 通知, key过期等等;
> ### **6.2 持久化**
    1) 快照: 定期将数据以异步方式从内存以RDB格式写入磁盘;
        * 默认的持久化方式;
          save 900 1    # 900秒内超过一个key被修改, 则发起快照保存
          save 300 10   # 300秒内超过10个key被修改, 则发起快照保存
        * 过程:
          a. redis调用fork(有子进程和父进程);
          b. 父进程继续处理client请求, 子进程负责将内存内容写到临时文件; 由
             于OS的写时复制机制父子进程会共享相同的物理页面, 当父进程处理写
             请求时OS会为父进程要修改的页面创建副本, 而不是写共享的页面; 所
             以子进程的数据是fork时整个数据库的一个快照;
          c. 子进程将快照写入临时文件后, 用临时文件替换原来的快照, 子退出;
    2) 追加: AOF格式, 将数据集修改操作记录下来, 可在后台对只可追加的记录做
             修改来避免无限增长的日志;
        * Append-Only File
          比快照有更好的数据持久化, redis会将每一个收到的写命令都追加到文件;
          重启是会通过重新执行文件中保存的写命令在内存中重建整个数据库内容;
          appendonly yes      # 启用AOF持久化方式
          appendfsync always  # 每次收到写命令都立即写入磁盘, 最慢
          appendfsync everysec  # 每秒强制写一次磁盘, 推荐
          appendfsync no      # 完全依赖操作系统, 性能最好, 持久化没保证
        * bgrewriteaof命令: 将内存数据以命令方式保持到临时文件, 替换原文件
          a. redis调用fork
          b. 子根据内存中的数据快照, 将重建数据库状态命令写入临时文件
          c. 父进程继续处理client请求, 处理把命令写入原来aof文件; 同时将收到
             的写命令缓存起来, 这样如果子进程重写失败也不会出问题;
          d. 当子将快照写入临时文件后, 通知父; 父将缓存的命令写入临时文件;
          e. 父用临时文件替换原来的aof文件, 重命名; 后面收到的写命令也往新的
             aof文件追加;