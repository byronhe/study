# **模板与范型编程** #
***




## **41: 了解隐式接口和编译期多态** ##
    (Understand implicit interfaces and compile-time polymorphism.)
    1) 类和模板都支持接口和多态
    2) 对类而言接口是显式的, 以函数签名为中心; 多态则时通过虚函数发生于运行
       期
    3) 对模板参数而言, 接口是隐式的, 奠基于有效表达式; 多态则是通过模板具现
       化和函数重载解析发生于编译期



## **42: 了解typename的双重意义** ##
    (Understand the two meanings of typename.)
    1) 声明template参数时, 前缀关键字class和typename可互换
    2) 请使用关键字typename标识嵌套从属类型名称; 但不得在基类列表或成员初始
       化列表内以它作为基类的修饰符



## **43: 学习处理模板化基类内的名称** ##
    (Know how to access names in tempatized base classes.)
    1) 在derived class templates内通过"this->"指涉基类模板内的成员名称, 或
       籍由一个明白写出的"子类资格修饰符"完成


## **44: 将与参数无关的代码抽离templates** ##
    (Factor parameter-independent code out of templates.)
    1) templates生成多个classes和多个函数, 所以任何template代码都不该与某个
       造成膨胀的template参数产生相依关系
    2) 因非类型模板参数而造成的代码膨胀, 往往可以消除, 做法是以函数参数或类
       成员变量替换template参数
    3) 因类型参数而造成的代码膨胀, 往往可降低, 做法是让带有完全相同二进制表
       述的具现类型共享实现码



## **45: 运用成员函数模板接受所有兼容类型** ##
    (Use member function templates to accept "all compatible types.")
    1) 请使用成员函数模板生成"可接受所有兼容类型"的函数
    2) 如果你声明member templates用于"泛化copy构造"和"泛化assignment操作", 
       你还是需要声明正常的拷贝构造函数和拷贝赋值操作符



## **46: 需要类型转换时请为模板定义非成员函数** ##
    (Define non-member functions inside templates when type conversions are 
     desired.)
    1) 当我们编写一个类模板, 而它所提供的"与此template相关的"函数支持"所有
       参数之隐式类型转换"时, 请将那些函数定义为"类模板内部的友元函数"



## **47: 请使用traits classes表现类型信息** ##
    (Use traits classes for information about types.)
    1) Traits classes使得"类型相关信息"在编译期可用; 它们以templates和"模板 
       特化"完成实现
    2) 整合重载技术后, traits classes有可能在编译期对类型执行if...else测试



## **48: 认识template元编程** ##
    (Be aware of template metaprogramming.)
    1) 模板元编程可将工作由运行期移到编译期, 因而得以实现早期错误侦测和更高
       的执行效率
    2) TMP可被用来生成"基于政策选择组合"的客户定制代码, 也可用来避免生成对
       某些特殊类型并不适合的代码