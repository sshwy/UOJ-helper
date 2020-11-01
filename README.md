# UOJ Helper

这是一个辅助你配置 UOJ 题目的 C++ 库。

目前只支持 Linux 环境。

如果你是把 Codeforces Polygon 上的题转移到 UOJ 上，那么你找对工具了。

## 环境与要求

需要要保证有 `/usr/bin/time` 命令。然后就是 C++11。

目前只支持使用内建 checker 的题目（也就是说不支持 Special Judge）。

## API

使用 `UOJ_problem` 类来创建一个题目配置。

- `UOJ_problem(string,int,int)`：用题目名称（可执行文件名，不含后缀）、时间限制（单位秒）、内存限制（单位MB）初始化一个题目配置。默认值为 `prob`，1，256。
- `void addTextTest(...)`：以文本形式添加一个测试点。传参方式与 `printf` 相同。
- `void addTextSample(...)`：以文本形式添加一个样例测试点。传参方式与 `printf` 相同。
- `void addScriptTest(...)`：以命令的形式添加一个测试点。传参方式与 `printf` 相同。要求：传入的是数据生成器以及相关参数的执行命令，这条命令要将生成的数据输出到stdout。
- `void addScriptSample(...)`：以命令的形式添加一个样例测试点。要求同上。
- `void setTimeLimit(int)`：设置时间限制（单位秒）。
- `void setMemoryLimit(int)`：设置内存限制（单位MB）。
- `void setOutputLimit(int)`：设置输出限制（单位KB，默认值64）。
- `void setSubtask(int score)`：将目前还不属于任何一个子任务的测试点全部打包成一个子任务，该子任务的分值为 `score`。如果你一次都没有调用过这个函数，那么题目默认不使用子任务评测。
- `void addSubtaskDependence(int i,int j)`：配置子任务依赖，编号为 i 的子任务依赖编号为 j 的子任务。
- `void regusterStd(string)`：添加 std 的可执行文件的路径（相对路径也可以），用于生成输出。如果你想要把 std 的源代码加到题目配置中，请手动复制。
- `void regusterValidator(string)`：添加 validator 的可执行文件的路径（相对路径也可以），用于检查读入是否合法。如果你想要把 validator 的源代码加到题目配置中，请手动复制。
- `void setBuildinChecker(string)`：指定内建 checker。
- `void build(string path)`：将当前的配置导出到 `path` 文件夹。如果没有，我们会创建它。

## 示例

这是一个示例：

```cpp
//by Sshwy
#include<bits/stdc++.h>
#include "UOJhelper.h"
using namespace std;
#define pb push_back
#define FOR(i,a,b) for(int i=(a);i<=(b);++i)
#define ROF(i,a,b) for(int i=(a);i>=(b);--i)

UOJ_problem uoj("anc",2,512); 
// 文件名形如 anc*.in, anc*.out。时间限制 2s，空间限制 512MB

void g(int n,int alp){
    uoj.addScriptTest("./gen -n %d -alp %d",n,alp); // generator
}

int main(){
    // subtask 1
    g(50,1);
    g(50,2);
    uoj.setSubtask(10);

    //subtask 2
    g(100,3);
    g(100,5);
    uoj.setSubtask(20);

    //subtask 3
    g(900,3);
    g(900,5);
    uoj.setSubtask(30);

    //subtask 4
    g(10000,3);
    g(10000,5);
    uoj.setSubtask(40);

    uoj.addTextSample("%d\naabcab",6); // 添加一个文本形式的样例

    uoj.registerStd("./std"); // std 可执行文件
    uoj.setBuildinChecker("ncmp"); // 指定内建 checker

    uoj.build("./data"); // 导出到当前目录的 data 文件夹
    return 0;
}
```
