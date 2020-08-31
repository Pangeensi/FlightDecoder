# FlightDecode
基于FlightAware和WireShare的ADS-B本地解码项目。

程序通过提取指定样本文件获取数据，然后完成对数据的解码保存等工作。

样本同工程仓库一并保存，在使用时需要单独提取并保存在指定路径。

样本文件路径路径：D:/FlightAwareData/sample

以下为开发版本说明:

# v1.12:
```
版本说明：
尝试先对push和pop的正确性进行校验。

开发日志：
尝试构造Message列表。
对列表进行修正和测试。
目前考虑使用Message指针列表
发现在Vector进行expand时出现堆错误。
考虑炸掉的原因可能是delete[]一个空指针。
尝试对可能的地方进行初始化
发现Message()没有具体实例化的话会直接析构。

实现情况：
考虑在下一版中将msgList特化成MessageList。
```

# v1.11:
```
版本说明：
尝试调整分类情况，并实现存储。

开发日志：
发现每次同步后都会校验。
尝试先读头，定位完成后开始收集。
尝试读一帧Message
尝试HTTP引导TCP，TCP进行检查。
首先尝试对HTTP导引进行裁剪。
HTTP裁剪必要性不大，考虑在TCP校验出错后把数据吐回去。
吐只能吐校验位，吐不出帧同步。考虑延长缓存的生命周期。
吐回去发生错误。
发现校验字填充有误。
开始时就要填充一波。

实现情况：
解决了校验字有误的问题，但是吐回去发生错误的问题没有验证。
```

# v1.10:
```
版本说明：
尝试在每一次帧同步都加入数据读取。

开发日志：
尝试在帧捕获TCP校验中判断当前标志，并据此录入数据。
尝试根据分类输出数据。
发现存在hex码转字符串的问题。
尝试保存或虑去暂时无用的帧头。

实现情况：
数据分类标准暂有问题。
数据保存思路暂不确定。
```

# v1.9:
```
版本说明：
尝试读取每一帧的文本内容。

开发日志：
尝试每一帧dummy 76 bit，然后读取剩下的。
在读取文本时，应每读取一次检查一次帧同步
尝试在Message层写帧同步和数据封装的过程。

实现情况：
```

# v1.8:
```
版本说明：
完成帧同步和TCP校验的清洁版本。

开发日志：

实现情况：
完成。
```

# v1.7:
```
版本说明：
尝试对sample进行数据清洗，然后对清洗过的数据进行帧同步和TCP校验。

开发日志：
原始数据考虑改成队列。
尝试新数据栈加一个reverse。
尝试修复Stack的empty。
完成了TCP的校验，从另一个角度讲同步帧的捕获正确。

实现情况：
下一版中清理格式。
```

# v1.6:
```
版本说明：
尝试以文本的方式读取sample数据。

开发日志：
尝试将TCP校验提到Message层。
尝试重新写帧同步的逻辑。
尝试重新编写帧同步装填函数。
可以完成帧同步，但是数据混乱。
发现有帧会被\r\n截断，所以需要先做数据清洗。

实现情况：
下一版先加载底层，然后恢复empty()，然后清洗数据，最后重写。
```

# v1.5:
```
版本说明：
尝试用一组伪数据亦步亦趋进行实现。

开发日志：
尝试制作小份的数据样本，包括数据头乱码，HTTP和TCP。
首先信息存储单元需要和数据进行帧同步，尝试将帧同步数据提升到信息层。
发现不能在构造的时候构造子类。
发现构造函数不能多次使用，尝试在栈中加入批量填充，或首先先不封装的批量填充
1.需要解决pop时的内存溢出问题
2.需要考虑改变读取字符的格式。
尝试先继续往下写，读完一帧。
发现每次判别同步帧之后都会启动一个Vector的析构函数，导致异常。
启动的析构函数是形参类的析构函数，所以以后要注意类不能做形参，要改成类指针。

实现情况：
完成一次帧捕获。

另外：在此版本中，测试文件换为了生成的sanple文件，同样也放在项目中一共提取，测试时应放置到指定路径。
```

# v1.4:
```
版本说明：
尝试实现数据的文本化提取

开发日志：
尝试将整数据文件读到数组当中。
尝试将数据封装到栈当中。
尝试构造Message类，来统筹HTTP和TCP
读写混乱，尝试先存取数据。

实现情况：
之后尝试用一组伪数据亦步亦趋进行实现。
```

# v1.3:
```
版本说明：
尝试构造TCP类并进行一般性测试

开发日志：
顺便创建HTTP类

实现情况：
```

# v1.2:
```
版本说明：
尝试修改栈中基于封装的构造，实现外部输入首尾值。
尝试构造TCP和HTTP类。

开发日志：
完成基于封装的修改的构造。
发现TCP和HTTP可以基于向量类进行构造，可以先构造向量类，并继承Stack类。
基本完成向量构造和继承，尝试进行稳定性的检查。
尝试构造TCP类。

实现情况：
TCP类需要做大量测试，保存至下一版本
```

# v1.1:
```
版本说明：
尝试完成pcap文件的解码。

开发日志：
考虑流程为帧同步->判决->数据提取->转码。
考虑使用堆栈存储原始数据。
尝试构造堆栈模块

实现情况：
同一行内使用cout输出流会出现pop顺序颠倒的情况
基于封装的构造宜改为三个输入：指针，起始秩，终结秩哨兵
```

# v1.0:
```
版本目标：
尝试完成完整的数据读取与校验，使之成为可读的char数组存储。

开发日志：

实现情况：
完成读取。
```

# v0.0:
```
版本说明：
初步实现了hex文件的读取,并且实现了项目的丢失，项目结束。
```
