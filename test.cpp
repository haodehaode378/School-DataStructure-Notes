#include <iostream>
#include <queue>
#include <cstring>
using namespace std;

#define MAX_VERTEX_NUM 20   //最大顶点数
#define INFINITY 65535
//无穷大（用于有权图权值或无权图的“无连接”标记）

//图的类型枚举（有向图、有向网、无向图、无向网）
typedef enum { DG, DN, UDG, UDN } GraphKind;

//邻接矩阵的“边”单元
typedef struct ArcCell{
	int adj;// 无权图用0/1表示是否连接，有权图存储权值
	int * info;//弧/边的附加信息的指针(int类型可以改)
}ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

//邻接矩阵表示的图
typedef struct MGraph {
    char vexs[MAX_VERTEX_NUM];  //顶点向量
    AdjMatrix arcs;             //邻接矩阵
    int vexnum;                 //当前顶点数
    int arcnum;                 //当前弧/边数
    GraphKind kind;             //图的类型标志
}MGraph;

//邻接表的“弧结点”
typedef struct ArcNode {
    int adjvex;                 //邻接顶点的位置下标
    struct ArcNode * nextarc;   //指向下一条弧的指针
    int * info;                 //弧的附加信息
} ArcNode;

//邻接表的“顶点结点”
typedef struct VNode {
    char data;                  //顶点信息
    ArcNode * firstarc;         //指向第一条依附该顶点的弧的指针
} VNode, AdjList[MAX_VERTEX_NUM];

//邻接表表示的图
typedef struct ALGraph{
    AdjList vertices;           //邻接表数组
    int vexnum;                 //当前顶点数
    int arcnum;                 //当前弧/边数
    GraphKind kind;             //图的类型标志
} ALGraph;

//定位顶点在邻接矩阵中的下标
int LocateVex(MGraph& G, char v) {
    for (int i = 0; i < G.vexnum; i++) {
        if (G.vexs[i] == v) return i;
    }
    return -1;//顶点不存在
}

//创建邻接矩阵图(以无向图为例，可删去一行变为有向图)
void CreateMGraph(MGraph& G, GraphKind kind) {
    cout << "输入顶点数和弧/边数：";
    cin >> G.vexnum >> G.arcnum;

    cout << "输入" << G.vexnum << "个顶点：" << endl;

    for (int i = 0; i < G.vexnum; i++) {
        cin >> G.vexs[i];
    }

    //初始化邻接矩阵
    for (int i = 0; i < G.vexnum; i++) {
        for (int j = 0; j < G.vexnum; j++) {
            G.arcs[i][j].adj = INFINITY;
            G.arcs[i][j].info = 0;
        }
    }

    //输入弧/边信息
    cout << "输入每条弧/边的两个顶点（如A B）：" << endl;
    char v1, v2;
    int i, j;
    for (int k = 0; k < G.arcnum; k++) {
        cin >> v1 >> v2;
        i = LocateVex(G, v1);
        j = LocateVex(G, v2);
        G.arcs[i][j].adj = 1;//"1"也可以是权值
        G.arcs[j][i].adj = 1;//若是有向图可以删除这一行
    }
}

//邻接矩阵→插入边
void InsertEdge_MGraph(MGraph& G, char v1, char v2) {
    int i = LocateVex(G, v1);
    int j = LocateVex(G, v2);
    if (i == -1 || j == -1) {
        return;
    }
    G.arcs[i][j].adj = 1;
    G.arcs[j][i].adj = 1;//若是有向图可以删除这一行
    G.arcnum++;
}

//邻接矩阵→删除边
void DeleteEdge_MGraph(MGraph& G, char v1, char v2) {
    int i = LocateVex(G, v1);
    int j = LocateVex(G, v2);
    if (i == -1 || j == -1) return;
    G.arcs[i][j].adj = 0;
    G.arcs[j][i].adj = 0;//若是有向图可以删除这一行
    G.arcnum--;
}


//邻接矩阵→出度（有向图）
int OutDegreeMGraph(MGraph& G, int v) {
    if (v < 0 || v >= G.vexnum) {
        return -1;
    }
    int degree = 0;
    for (int j = 0; j < G.vexnum; j++) {
        if (G.arcs[v][j].adj == 1) {
            degree++;
        }
    }
    return degree;
}

//邻接矩阵→入度（有向图）
int InDegreeMGraph(MGraph& G, int v) {
    if (v < 0 || v >= G.vexnum) {
        return -1;
    }
    int degree = 0;
    for (int i = 0; i < G.vexnum; i++) {
        if (G.arcs[i][v].adj == 1) {
            degree++;
        }
    }
    return degree;
}

//邻接矩阵→邻接表
void MGraphToALGraph(MGraph& G, ALGraph& AL) {
    AL.vexnum = G.vexnum;
    AL.arcnum = G.arcnum;
    AL.kind = G.kind;

    //初始化顶点表
    for (int i = 0; i < AL.vexnum; i++) {
        AL.vertices[i].data = G.vexs[i];
        AL.vertices[i].firstarc = NULL;
    }

    //建邻接表（链表）
    for (int i = 0; i < G.vexnum; i++) {
        for (int j = 0; j < G.vexnum; j++) {
            if (G.arcs[i][j].adj == 1) {
                ArcNode* p = new ArcNode;
                p->adjvex = j;
                p->info = G.arcs[i][j].info;
                p->nextarc = AL.vertices[i].firstarc;//头插法
                AL.vertices[i].firstarc = p;
            }
        }
    }
}

//邻接矩阵→DFS遍历(深度优先搜索)
void DFS_MGraph(MGraph& G, int v, bool visited[]) {
    cout << G.vexs[v] << " ";
    visited[v] = true;
    for (int j = 0; j < G.vexnum; j++) {
        if (G.arcs[v][j].adj == 1 && !visited[j]) {
            DFS_MGraph(G, j, visited);//对尚未访问的顶点调用DFS
        }
    }
}

//邻接矩阵→BFS遍历(广度优先搜索)
void BFS_MGraph(MGraph& G, int v, bool visited[]) {
    queue<int> q;
    q.push(v);
    visited[v] = true;
    cout << G.vexs[v] << " ";
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int j = 0; j < G.vexnum; j++) {
            if (G.arcs[u][j].adj == 1 && !visited[j]) {
                visited[j] = true;
                cout << G.vexs[j] << " ";
                q.push(j);
            }
        }
    }
}

//在邻接表中根据顶点数据找下标
int LocateVex_AL(ALGraph& AL, char v) {
    for (int i = 0; i < AL.vexnum; i++) {
        if (AL.vertices[i].data == v) {//遍历顶点数组匹配数据
            return i;//找到返回下标
        }
    }
    return -1;  //未找到返回-1
}

//创建邻接表（不调用插入边函数，内部直接实现边插入）
void CreateALGraph(ALGraph& AL, GraphKind kind) {
    AL.kind = kind;
    cout << "输入邻接表的顶点数和边数：";
    cin >> AL.vexnum >> AL.arcnum;

    cout << "输入" << AL.vexnum << "个顶点（如A B C）：";
    for (int i = 0; i < AL.vexnum; i++) {
        cin >> AL.vertices[i].data; //初始化顶点数据
        AL.vertices[i].firstarc = NULL; //初始没有弧
    }

    //输入边信息并直接构建邻接表
    cout << "输入" << AL.arcnum << "条边（如A B）：" << endl;
    char v1, v2;
    int i, j;
    for (int k = 0; k < AL.arcnum; k++) {
        cin >> v1 >> v2;
        // 1. 定位两个顶点的下标
        i = LocateVex_AL(AL, v1);
        j = LocateVex_AL(AL, v2);
        if (i == -1 || j == -1) {
            cout << "顶点不存在，跳过该边！" << endl;
            continue;
        }

        //2.插入 v1→v2 的弧（头插法）
        ArcNode* p = new ArcNode;
        p->adjvex = j;
        p->info = 0; //附加信息初始化为0（可根据需求修改）
        p->nextarc = AL.vertices[i].firstarc;
        AL.vertices[i].firstarc = p;

        //3.无向图还需插入 v2→v1 的反向弧
        if (AL.kind == UDG || AL.kind == UDN) {
            ArcNode* q = new ArcNode;
            q->adjvex = i;
            q->info = 0;
            q->nextarc = AL.vertices[j].firstarc;
            AL.vertices[j].firstarc = q;
        }
    }
}

//邻接表→邻接矩阵
void ALGraphToMGraph(ALGraph& AL, MGraph& MG) {
    MG.vexnum = AL.vexnum;
    MG.arcnum = AL.arcnum;
    MG.kind = AL.kind;

    //复制顶点信息
    for (int i = 0; i < MG.vexnum; i++) {
        MG.vexs[i] = AL.vertices[i].data;
    }

    //初始化邻接矩阵
    for (int i = 0; i < MG.vexnum; i++) {
        for (int j = 0; j < MG.vexnum; j++) {
            MG.arcs[i][j].adj = INFINITY;
            MG.arcs[i][j].info = 0;
        }
    }

    //遍历邻接表填充矩阵
    for (int i = 0; i < AL.vexnum; i++) {
        ArcNode* p = AL.vertices[i].firstarc;
        while (p) {
            MG.arcs[i][p->adjvex].adj = 1;
            MG.arcs[p->adjvex][i].adj = 1;//若是有向图可以删除这一行
            p = p->nextarc;
        }
    }
}

// 邻接表→插入边
void InsertEdge_ALGraph(ALGraph& AL, char v1, char v2) {
    int i = LocateVex_AL(AL, v1);
    int j = LocateVex_AL(AL, v2);
    if (i == -1 || j == -1) return;

    ArcNode* p = new ArcNode;
    p->adjvex = j;
    p->info = 0;
    p->nextarc = AL.vertices[i].firstarc;
    AL.vertices[i].firstarc = p;

    if (AL.kind == UDG || AL.kind == UDN) {
        ArcNode* q = new ArcNode;
        q->adjvex = i;
        q->info = 0;
        q->nextarc = AL.vertices[j].firstarc;
        AL.vertices[j].firstarc = q;
    }
    AL.arcnum++;
}

//邻接表→删除边
void DeleteEdge_ALGraph(ALGraph& AL, char v1, char v2) {
    int i = LocateVex_AL(AL, v1);
    int j = LocateVex_AL(AL, v2);
    if (i == -1 || j == -1) return;

    //删除i→j的弧
    ArcNode* p = AL.vertices[i].firstarc;
    ArcNode* prev = NULL;
    while (p && p->adjvex != j) {
        prev = p;
        p = p->nextarc;
    }
    if (p) {
        if (prev) prev->nextarc = p->nextarc;
        else AL.vertices[i].firstarc = p->nextarc;
        delete p;
    }

    //无向图还要删除反向弧
    if (AL.kind == UDG || AL.kind == UDN) {
        p = AL.vertices[j].firstarc;
        prev = NULL;
        while (p && p->adjvex != i) {
            prev = p;
            p = p->nextarc;
        }
        if (p) {
            if (prev) prev->nextarc = p->nextarc;
            else AL.vertices[j].firstarc = p->nextarc;
            delete p;
        }
    }
    AL.arcnum--;
}

//邻接表→出度
int OutDegree_ALGraph(ALGraph& AL, int v) {
    if (v < 0 || v >= AL.vexnum) {
        return -1;
    }
    int degree = 0;
    ArcNode* p = AL.vertices[v].firstarc;
    while (p) {
        degree++;
        p = p->nextarc;
    }
    return degree;
}

//邻接表→入度(要遍历所有点，非常复杂，可以转化为逆邻接表)
int InDegree_ALGraph(ALGraph& AL, int v) {
    if (v < 0 || v >= AL.vexnum) return -1;
    int degree = 0;
    for (int i = 0; i < AL.vexnum; i++) {
        ArcNode* p = AL.vertices[i].firstarc;
        while (p) {
            if (p->adjvex == v) degree++;
            p = p->nextarc;
        }
    }
    return degree;
}

//邻接表→逆邻接表（有向图，存储“入边”）
void ALGraphToInverse(ALGraph& AL, ALGraph& inverseAL) {
    inverseAL.vexnum = AL.vexnum;
    inverseAL.arcnum = AL.arcnum;
    inverseAL.kind = AL.kind;

    //初始化顶点表
    for (int i = 0; i < inverseAL.vexnum; i++) {
        inverseAL.vertices[i].data = AL.vertices[i].data;
        inverseAL.vertices[i].firstarc = NULL;
    }

    //构建逆邻接表
    for (int i = 0; i < AL.vexnum; i++) {
        ArcNode* p = AL.vertices[i].firstarc;
        while (p) {
            ArcNode* q = new ArcNode;
            q->adjvex = i;
            q->info = p->info;
            q->nextarc = inverseAL.vertices[p->adjvex].firstarc;//头插法
            inverseAL.vertices[p->adjvex].firstarc = q;
            p = p->nextarc;
        }
    }
}

//根据逆邻接表计算入度（逆邻接表中v的邻接表长度即为入度）
//仅适用于有向图，无向图的逆邻接表与原表一致
int InDegree_InvALGraph(ALGraph& invAL, int v) {
    if (v < 0 || v >= invAL.vexnum) {//检查顶点索引合法性
        return -1;
    }
    int inDegree = 0;
    ArcNode* p = invAL.vertices[v].firstarc;//指向逆邻接表中v的第一个邻接节点
    while (p != NULL) {//遍历邻接表，统计节点数量
        inDegree++;
        p = p->nextarc;
    }
    return inDegree;
}

//邻接表→DFS遍历(深度优先搜索)
void DFS_ALGraph(ALGraph& AL, int v, bool visited[]) {
    cout << AL.vertices[v].data << " ";
    visited[v] = true;
    ArcNode* p = AL.vertices[v].firstarc;
    while (p) {
        if (!visited[p->adjvex]) {
            DFS_ALGraph(AL, p->adjvex, visited);
        }
        p = p->nextarc;
    }
}

//邻接表→BFS遍历(广度优先搜索)
void BFS_ALGraph(ALGraph& AL, int v, bool visited[]) {
    queue<int> q;
    q.push(v);
    visited[v] = true;
    cout << AL.vertices[v].data << " ";
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        ArcNode* p = AL.vertices[u].firstarc;
        while (p) {
            if (!visited[p->adjvex]) {
                visited[p->adjvex] = true;
                cout << AL.vertices[p->adjvex].data << " ";
                q.push(p->adjvex);
            }
            p = p->nextarc;
        }
    }
}

//邻接表销毁函数：释放所有动态分配的ArcNode节点，避免内存泄漏
void DestroyALGraph(ALGraph& AL) {
    //遍历每个顶点的邻接表
    for (int i = 0; i < AL.vexnum; i++) {
        ArcNode* current = AL.vertices[i].firstarc;//指向当前顶点的第一条弧
        while (current != NULL) {
            ArcNode* temp = current;//保存当前节点指针
            current = current->nextarc;//移动到下一条弧
            delete temp;//释放当前节点（用new分配的必须用delete释放）
            temp = NULL;//避免野指针
        }
        AL.vertices[i].firstarc = NULL; //清空顶点的弧指针
    }
}

int main() {
    //略
    return 0;
}