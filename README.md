# FlightDecode
基于FlightAware和WireShare的ADS-B本地解码项目。

程序通过提取指定样本文件获取数据，然后完成对数据的解码保存等工作。

样本同工程仓库一并保存，在使用时需要单独提取并保存在指定路径。

样本文件路径路径：D:/FlightAwareData/Data_00001_20200810165511

以下为开发版本说明:

#v1.1
版本说明：
尝试完成pcap文件的解码。

开发日志：
考虑流程为帧同步->判决->数据提取->转码。
考虑使用堆栈存储原始数据。
尝试构造堆栈模块

实现情况：
同一行内使用cout输出流会出现pop顺序颠倒的情况
基于封装的构造宜改为三个输入：指针，起始秩，终结秩哨兵

# v0.0:
版本说明：
初步实现了hex文件的读取,并且实现了项目的丢失，项目结束。
