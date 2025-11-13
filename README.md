# 图数据结构实现（邻接矩阵与邻接表）

本项目用 C++ 实现了图的两种经典存储结构（邻接矩阵、邻接表）及核心操作，包括图的创建、边的增删、度的计算、遍历（DFS/BFS）、存储结构转换等，适合数据结构课程学习参考。

## 功能说明

实现了图的完整生命周期操作，支持有向图（DG）、有向网（DN）、无向图（UDG）、无向网（UDN）四种类型，核心功能包括：

1. **存储结构**：
   - 邻接矩阵（`MGraph`）：适合稠密图，通过二维数组存储边关系
   - 邻接表（`ALGraph`）：适合稀疏图，通过「顶点数组 + 弧链表」存储
2. **基础操作**：
   - 创建图（`CreateMGraph`/`CreateALGraph`）
   - 插入 / 删除边（`InsertEdge_*`/`DeleteEdge_*`）
   - 计算顶点度（出度 `OutDegree_*`、入度 `InDegree_*`）
3. **高级功能**：
   - 遍历算法：深度优先搜索（DFS）、广度优先搜索（BFS）
   - 结构转换：邻接矩阵 ↔ 邻接表（`MGraphToALGraph`/`ALGraphToMGraph`）
   - 逆邻接表生成（`ALGraphToInverse`）及基于逆邻接表的高效入度计算（`InDegree_InvALGraph`）
4. **内存管理**：
   - 邻接表专用销毁函数（`DestroyALGraph`），释放动态分配的弧节点，避免内存泄漏

## 代码结构

```plaintext
.
├── test.cpp       // 核心代码（包含所有结构体定义和函数实现）
└── README.md       // 项目说明文档
```

核心结构体与函数分类：

| 模块     | 关键结构体 / 函数                                            |
| -------- | ------------------------------------------------------------ |
| 邻接矩阵 | `MGraph`（图结构）、`CreateMGraph`（创建）、`DFS_MGraph`（DFS 遍历） |
| 邻接表   | `ALGraph`（图结构）、`CreateALGraph`（创建）、`BFS_ALGraph`（BFS 遍历） |
| 度计算   | `OutDegreeMGraph`（邻接矩阵出度）、`InDegree_InvALGraph`（逆邻接表入度，高效） |
| 结构转换 | `MGraphToALGraph`（矩阵转表）、`ALGraphToInverse`（生成逆邻接表） |
| 内存管理 | `DestroyALGraph`（释放邻接表动态节点）                       |

## 注意事项

1. 顶点数量限制：当前通过宏 `MAX_VERTEX_NUM` 限制最大顶点数为 20，可根据需求修改。

2. 图类型区分：

   - 无向图（UDG/UDN）会自动添加双向边，有向图（DG/DN）仅添加单向边。
   - 逆邻接表仅对有向图有效（无向图的逆邻接表与原表一致）。

3. 内存管理：使用邻接表后必须调用 `DestroyALGraph` 释放动态分配的 `ArcNode` 节点，避免内存泄漏。

4. 输入容错：顶点不存在时会提示跳过无效边，但未处理顶点重复、自环等特殊情况，可根据需求扩展。

   

如有问题或优化建议，欢迎提交 Issues 或 Pull Requests！