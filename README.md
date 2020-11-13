# 基于C完成的Linux shell（LinuxC shell）

[![shell,c](https://img.shields.io/badge/%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84-C%2B%2B-brightgreen)](https://github.com/cuichenxu/Cshell)

本仓库包括以下内容：
1. 基于c实现的Shell源码和运行脚本，~其中包括了初始demo和第一个版本~（已删除）
2. 在实验中遇到的问题和解决策略(暂未上传)

## 内容列表
- [说明](#说明)
- [使用](#使用)
- [展望](#展望)
- [问题](#问题)
- [如何贡献](#如何贡献)

## 说明
此仓库目前由下列成员共同开发而成：
* [@LiFulian](https://github.com/LiFulian)
* [@cuichenxu](https://github.com/cuichenxu)
* [@ambitiousCC](https://github.com/ambitiousCC)

目前已经完成基本shell命令16个(by 2020.11.13)

其中代码学习了Internet中较好的shell实现方法，经过理解后整合成独立程序。目前测试中，暂无较大bug，但由于系统的多样性和不可预知性，在您的机器上可能出现问题。如果您发现问题，请第一时间提交issue。

## 使用
[![Linux GCC](https://img.shields.io/badge/Linux-GCC-yellow)](https://github.com/cuichenxu/Cshell)

### 运行

直接运行`run.sh`即可

```powershell
$ chmod a+x run.sh
$ ./run.sh
```

## 展望

此程序只是一个雏形，我们团队正在努力不断更新新的内容。

## 问题

1. 数组溢出问题：检查缓存数组初始化大小
2. 类型转换问题如`(char*)malloc(val);`
3. 动态链接库readline，需要执行:
```powershell
# Debian
$sudo apt-get install libreadline-dev

#centos
$ yum install libtermcap-devel ncurses-devel libevent-devel readline-devel
```

## 如何贡献

只需要您`star`一下我们的开源仓库，`fork`提交代码即可
