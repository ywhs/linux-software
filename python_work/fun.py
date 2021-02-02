# 函数的参数

'''
    对于不可变对象作为函数参数，相当于C系语言的值传递；
    对于可变对象作为函数参数，相当于C系语言的引用传递。
    可变对象：list dict set
    不可变对象：tuple string int float bool
    
    如果想在内部函数中改变数值的话，最好的方式是return
'''

# 位置参数
'''
    定义一个函数，函数中的x和n为参数，称为位置参数
    计算x的n次方
'''
def power(x, n):
    s = 1
    while n > 0:
        n = n - 1
        s = s * x
    return s

# 默认参数 重要：在设置默认参数时，默认参数必须指向不变对象！比如常量和str或者元组
'''
    默认参数，在不传入某个参数时，可以使用事先定义好的数值
    默认参数尽量放到后面,这里默认给值为2,通常计算某个数值的2次方比较多
'''
def power_default(x, n = 2):
    s = 1
    while n > 0:
        n = n - 1
        s = s * x
    return s

'''
    这里模拟北京小学生入学统计，默认入学年龄为9，城市统一为北京
    这样的话只需要输入学生的名字和性别即可
'''
def enroll(name, gender, age = 9, city = 'Beijing'):
    print('name: ', name, 'gender: ', gender, 'age: ', age, 'city: ', city)
    

# 可变参数，定义函数时只写一个参数位置，传入时可以多个参数
'''
    计算a^2 + b^2 + c^2……
    使用*可变参数时，传入进来后会组成tuple，即不可改变
'''
def calc(*numbers):
    sum = 0
    for n in numbers:
        sum = sum + n * n
    return sum


# 关键字参数
'''
    关键字参数和可变参数除了写法上多了个*
    从运行来看可以传入字典类型的数据，并且可以把整个字典变量传入后拆分
    重新组装了一个字典？ **kw是传参的一份拷贝，在内部改变不会影响函数外的数据
    可变参数传入后变为tuple 不可改变，关键字传入的是copy，改变也没用
'''
def person(name, age, **kw):
    print('name:', name, 'age:', age, 'other:', kw)
    
    
# 命名关键字参数

'''
    对于关键字参数来说用户没有任何限制，当我们想对进行限制时，可以使用命名关键字参数进行限制
    如果参数中没有可变参数的话，要以*隔开，后面写限制的参数
    命名关键字参数必须传入参数名，也就是说传入参数时以下面的方式
    person_define('ywh', 24, city = 'beijing', job = 'enginner')  √
    person_define('ywh', 24, 'beijing', 'enginner')     ×
    不加参数名的话，会被当做位置参数来使用，报错信息如下
    person_define() takes 2 positional arguments but 4 were given
'''    
def person_define(name, age, *, city, job):
    print(name, age, city, job)
    
'''
    以下函数允许计算两个数的乘积，请稍加改造，
    变成可接收一个或多个数并计算乘积：
'''
def product(*params):
    r_value = 1
    if not isinstance(params, (int, float, list, tuple)):
        raise TypeError('类型错误')
    if len(params) == 0:
        raise TypeError('请输入一个或多个数')
    for item in params:
        r_value *= item
    return r_value








