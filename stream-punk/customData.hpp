/*
自定义类型注册表
左边类型,可以写域操作符,以及特化模板类型.
右边类型名称,只能是id,并且不可重复.
*/
# define Xt_CustomType(X__) \
X__(Test , Test) \
X__(MQTT , MQTT) \


