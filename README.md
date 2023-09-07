# 前言
对于这次解密Flag拿奖励的活动被终止，我深表歉意，希望大家下次能积极参与进来。其次，能不能解开Flag都是无关紧要的，我的初衷是能调动大家一起学习网安知识，分享网络/编程技术。

本CM已开源：[https://github.com/hujiayucc/Crackme2](https://github.com/hujiayucc/Crackme2 "https://github.com/hujiayucc/Crackme2")

------------

# 解密过程
### 调试环境
- Debian buster Arm64
- Termux OpenSSH
- Radare 5.8
- g++/clang++(用于编译运行payload)

注：可用gcc/clang平替g++/clang++

![](https://oss.hujiayucc.cn/blog/OSSvJpW20230906035944.jpg)

### Crackme2
简单查看一下ELF文件，可以发现是一个UPX压缩壳，脱壳方法有很多，可以通过DUMP内存来获取源程序的代码，也可以通过UPX官方提供的工具来解压，网上教程很多不过多介绍。

解密后用Radare2分析
![](https://oss.hujiayucc.cn/blog/OSSkWvX20230906091120.png)

跳转定位到main函数
![](https://oss.hujiayucc.cn/blog/OSSFLrW20230906091317.jpg)
没有发现有价值的线索，简单阅读一下汇编：
```assembly
str w0, [sp, 0x1c]  ; 第一个参数
str x1, [sp, 0x10]   ; 第二个参数的第二个元素数据 例如args[1]
cmp w0, 1             ; 判断第一个参数是否为1
; ...忽略为1代码段....
adrp x0, map._root_Crackme2_Crackme2.rw_ ; 将map._root_Crackme2_Crackme2.rw_地址存到x0寄存器
ldr x0, [x0, 0xfb8]   ; 将x0 + 0xfb8的地址加载到x0寄存器
str x1, [x0]               ; 将x1的数据写入x0，x0=0x5f24b05000
```

也就是将输入的参数赋值给了0x5f24b05000地址！

这里再讲一下ELF文件的代码执行顺序，main并非是最先执行的，也不是最后执行的，从入口进入如果有其他函数存在，就不会最先执行main，相同道理程序运行结束也是，所以之前有人粗心没注意看entry入口，说我CM写的有问题。

这里没有发现entry.init，自动忽略，直接来看到entry.fini：
![](https://oss.hujiayucc.cn/blog/OSSR3kl20230906101900.jpg)
汇编语法上面做了简要说明，这里不一一介绍了（懒得码字/doge）
这里是通过sym.imp.dlsym函数加载了一个外部动态库，动态库名字地址为0x5f24b05fa0，我们打印看一下
![](https://oss.hujiayucc.cn/blog/OSSk6pJ20230906102438.png)
check为sym.imp.dlsym加载的函数或变量/常量，下面对check进行了传参，那说明是一个函数。
传入的数据0x5f24b05fb8正是我们输入的参数，显而易见了，是验证flag。

这里讲个小知识，在Linux中可以使用ldd命令查看可执行文件link的库文件～

### libxy.so
![](https://oss.hujiayucc.cn/blog/OSSXY2A20230906103537.jpg)
从汇编代码中看到
```assembly
stp x29, x30, [sp, -0x20]!  ; 调用了一个地址-0x20的函数
; 这里传入两个参数，第一个参数为传入的flag
stp x19, x20, [sp, 0x10]
mov x19, x0
; 第二个为flag的长度
bl sym.imp.strlen
mov w1, w0
mov x0, x19
```

直接转到-0x20地址
![](https://oss.hujiayucc.cn/blog/OSSJOtl20230906105741.jpg)
看到verify函数了，这里有一个循环判断，判断flag字符是否为第一个
有多层异或代码，反汇编看一下伪代码
![](https://oss.hujiayucc.cn/blog/OSSO4Or20230906233732.jpg)

简化一下代码
```c
bool verify(long *param_1, int param_2)
{
  for (int i = 0;i < param_2)
  {
    long *l = *(0xbd0 + i);
    switch (i)
    {
      case 0:
        if (l != (param_2 ^ (0xc6e6 | 0xbb0000) ^ param_1[i])) return false;
        break;
      default:
        if (l != (i * (0x5677 | 0x1310000) ^ param_1[i]) ^ param_2) return false;
				break;
		}
	}
  return param_2 == 0x17;
}
```

这里调用了0xbd0地址的数据，循环了23次，每次往后推8个字节，由此我们可以这样做
![](https://oss.hujiayucc.cn/blog/OSS4wfV20230907004042.png)

然后我们可以写个小小的exp来输出为16进制：
```cpp
#include <iostream>

using namespace std;

const long buffer[23] = {
    12306071, 20010508, 40021144, 60031765,
    80042416, 100053036, 120063727, 140074275,
    160084895, 180095570, 200106113, 220116835,
    240127410, 260138109, 280148645, 300159127,
    320169809, 340180357, 360191096, 380201633,
    400212334, 420222903, 440233552
};

int main()
{
	for (int i = 0;i < 23;i++)
    {
        cout << "0x" << hex << buffer[i] << ", ";
    }

    cout << endl;
}
```
也可以不转换，我这儿转换只是为了看着舒服(被编程惯坏了)
![](https://oss.hujiayucc.cn/blog/OSSA44i20230907035354.png)

### payload
根据上面解析出来的验证规则，写一个payload，实现自动输出flag
```cpp
#include <iostream>

using namespace std;

const long buffer[23] = {
    0xbbc697, 0x131560c, 0x262ac98,
    0x3940315, 0x4c559b0, 0x5f6b02c,
    0x72806ef, 0x8595d23, 0x98ab39f,
    0xabc0a52, 0xbed6081, 0xd1eb763,
    0xe500db2, 0xf81647d, 0x10b2baa5,
    0x11e41097, 0x13156751, 0x1446bd85,
    0x15781478, 0x16a96aa1, 0x17dac16e,
    0x190c17b7, 0x1a3d6e50,
};

int get(long l, int i)
{
    if (i == 0) return (23 ^ (0xc6e6 | 0xbb0000) ^ l);
    else return (i * (0x5677 | 0x1310000) ^ l ^ 23);
}

int main()
{
	for (int i = 0;i < 23;i++)
    {
        cout << ((char) get(buffer[i], i));
    }

    cout << endl;
}
```

得到flag以后直接进行验证
![](https://oss.hujiayucc.cn/blog/OSSJrJ320230907022209.png)
验证成功！！！

挂个payload2
```cpp
#include <dlfcn.h>
#include <iostream>

using namespace std;

int get(long l, int i, int ii)
{
    if (i == 0) return l ^ ((2001L*615*1950/195) ^ ii);
    return (l ^ (20010615*i)) ^ ii;
}

int main()
{
    void *handle = dlopen("libxy.so", RTLD_LAZY);
    if (handle == NULL) exit(-1);
    long *buffer = (long*) dlsym(handle, "buffer");
    char flag[23] = {};
	for (int i = 0;buffer[i] > 0;i++)
    {
        flag[i] = get(buffer[i], i, 23);
    }
	cout << flag << endl;;
    dlclose(handle);
}
```

### 总结
本次讲解希望大家能多多学习相关知识，之后还会推出不同难度的Crackme，包括但不限于这次已经讲解的内容。重在参与，主要以分享知识为目的！！！
