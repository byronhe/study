# **继承与面向对象设计** #
***



## **32: 确定你的public继承塑膜出is-a关系** ##
    (Make sure public inheritance models "is-a".)
    1) "public继承"意味is-a; 适用于基类身上的每一件事情一定也适用于子类身上,
       因为每一个子类对象也都是一个基类对象



## **33: 避免遮掩继承而来的名称** ##
    (Avoid hiding inherited names.)
    1) 子类内的名称会遮掩基类内的名称, 在public继承下从来没人希望如此
    2) 为了让被遮掩的名称在见天日, 可以使用using声明式或转交函数


## **34: 区分接口继承和实现继承** ##
    (Differentiate between inheritance of interface and inheritance of 
     implementation.)
    1) 以public形式的继承
        * 成员函数的接口总是会被继承
        * 声明一个纯虚函数的目的是为了让子类只继承函数接口
        * 声明简朴的非纯虚函数的目的是让子类继承该函数的接口和缺省实现
    2) 声明非虚函数的目的是为了令子类继承函数的接口以及一份强制性实现
    3) 接口继承和实现继承不同, 在public继承之下, 子类总是继承基类的接口
    4) 纯虚函数只是具体指定接口继承
    5) 简朴(非纯)虚函数具体指定接口继承以及缺省实现继承
    6) 非虚函数具体指定接口继承以及强制性实现继承



## **35: 考虑虚函数以外的其他选择** ##
    (Consider alternatives to virtual functions.)
    1) 虚函数的替代方案包括NVI手法以及Strategy设计模式的多种形式, NVI手法自
       身是一个特殊形式的Template Method设计模式
    2) 将机能从成员函数移到class外部函数, 带来的一个缺点是, 非成员函数无法访
       问class的non-public成员
    3) tr1::function对象的行为就像一般函数指针, 这样的对象可接纳"与给定之目
       标签名式兼容"的所有可调用物



## **36: 绝不重新定义继承而来的non-virtual函数** ##
    (Never redefine an inherited non-virtual function.)
    1) 绝对不要重新定义继承而来的non-virtual函数



## **37: 绝不重新定义继承而来的缺省参数值** ##
    (Never redefine a function's inherited default parameter value.)
    1) 虚函数是动态绑定, 缺省参数值确实静态绑定, 即绑定的基类接口的缺省参数
    2) 绝对不要重新定义一个继承而来的缺省参数值, 因为缺省参数值都是静态绑定,
       而虚函数却是动态绑定的



## **38: 通过复合塑模出has-a或"根据某物实现出"** ##
    (Model "has-a" or "is-implementation-in-terms-of" through composition.)
    1) 复合的意义和public继承完全不同
    2) 在应用域, 复合意味has-a; 在实现域, 复合意味is-implementation-in-term-
       of



## **39: 明智而审慎地使用private继承** ##
    (Use private inheritance judiciously.)
    1) private继承意味is-implementation-in-terms-of, 它通常比复合几级别低, 
       但是derived class需要访问protected base class的成员, 或需要重新定义
       继承而来的虚函数, 这么设计是合理的
    2) 和复合不同, private继承可以造成empty base最优化, 这对致力于"对象尺寸
       最小化"的程序库开发者而言, 可能很重要



## **40: 明智而审慎地使用多重继承** ##
    (Use multiple inheritance judiciously.)
    1) 对于虚基类
        * 非必要不使用虚基类, 平常使用非虚基类
        * 如果你必须使用虚基类, 尽可能避免在其中放置数据
    2) 多重继承比单一继承复杂, 可能导致新的歧义性, 以及对虚继承的需要
    3) 虚继承会增加大小, 速度, 初始化复杂度等等成本; 如果虚基类不带任何数据
       将是最具实用价值的情况
    4) 多继承的确有正当用途, 其中一个情节涉及"public继承某个接口类"和"私有
       继承某个协助实现的类"的两相组合