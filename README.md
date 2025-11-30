# 图数据结构实现（邻接矩阵与邻接表）
本项目用 C++ 实现了图的两种经典存储结构（邻接矩阵、邻接表）及全套核心操作，支持**文件输入驱动自动化测试**，涵盖图的创建、边的增删、度的计算、遍历（DFS/BFS）、存储结构转换等完整功能，适合数据结构课程学习参考，配套详细解析笔记方便理解。

## 功能说明
实现了图的完整生命周期操作，支持有向图（DG）、有向网（DN）、无向图（UDG）、无向网（UDN）四种类型，核心功能包括：
- **图的遍历**：
  - 深度优先遍历（DFS）
  - 广度优先遍历（BFS）
- **图的算法**：
  - 拓扑排序（Kahn 算法）
- **图的转换**：
  - 邻接矩阵转邻接表
  - 创建逆邻接表
- **其他操作**：
  - 计算顶点度数（出度、入度）

## 项目结构
```plaintext
.
├── test.cpp                          // 核心代码（结构体定义、函数实现、完整测试main函数）
├── input.txt                         // 测试输入文件（图类型、顶点/边信息、操作指令）
├── README.md                         // 项目说明文档（功能、使用方法、注意事项）
└── 图数据结构实现（邻接矩阵与邻接表）.md  // 详细解析笔记（代码模块、逻辑逐行说明）
```

### 核心结构体与函数分类

| 模块     | 关键结构体 / 函数                                            |
| -------- | ------------------------------------------------------------ |
| 基础定义 | `GraphKind`（图类型枚举）、`MAX_VERTEX_NUM`（顶点数上限）、`INFINITY`（无连接标记） |
| 邻接矩阵 | `MGraph`（结构）、`CreateMGraph`（创建）、`DFS_MGraph`/`BFS_MGraph`（遍历）、`PrintMGraph`（打印） |
| 邻接表   | `ALGraph`（结构）、`CreateALGraph`（创建）、`DFS_ALGraph`/`BFS_ALGraph`（遍历）、`PrintALGraph`（打印） |
| 核心算法 | `TopologicalSort_Kahn`（拓扑排序）                             |
| 度数计算 | `OutDegree_*`（出度）、`InDegree_InvALGraph`（逆邻接表高效入度） |
| 结构转换 | `MGraphToALGraph`（矩阵转表）、`ALGraphToInverse`（生成逆邻接表） |
| 文件输入 | `main` 函数通过 `cin.rdbuf(fin.rdbuf())` 重定向输入，读取 `input.txt` |
| 内存管理 | `DestroyALGraph`（释放邻接表动态节点）                       |

## 环境要求（见 requirements.txt）

- C++ 编译器（支持 C++11 及以上标准）：g++ 5.4.0+、clang++ 3.8.0+、MSVC 2015+
- 标准 C++ 库（STL）：包含 `<iostream>`、`<vector>`、`<queue>`、`<cstring>`、`<fstream>`（文件操作依赖）

## 使用方法

### 准备输入文件

`input.txt` 需与编译后的可执行文件放在同一目录，格式严格遵循以下规则：

```txt
UDG
4 3
A B C D
A B
A C
B D
C D
A C

DG
3 3
X Y Z
X Y
Y Z
Z X
X Z
Y Z X
```

## 注意事项

1. 顶点限制：最大顶点数由 `MAX_VERTEX_NUM` 定义（默认 20），可直接修改宏定义扩展。
2. 图类型适配：
   - 无向图（UDG/UDN）：插入 / 创建边时自动添加双向关系
   - 有向图（DG/DN）：仅保留单向关系，逆邻接表功能仅对有向图有效
3. 输入规范：
   - `input.txt` 不可包含注释，每行数据按顺序排列，否则会导致读取失败
   - 顶点需为单个字符（如 A、X），不可为多字符（如 AB、Node1）
4. 内存管理：邻接表使用动态分配的 `ArcNode` 节点，必须调用 `DestroyALGraph` 释放，代码中已在程序结束时自动调用。

如有问题或优化建议，欢迎提交 Issues 或 Pull Requests！

