#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EDGES 10001

typedef struct {//정렬을 위해 u < v
    int u;
    int v;
    int cost;
} edge;

int V, E;
edge edges[MAX_EDGES];//간선 데이터들을 저장
int* parent;
int* rank;

int** pos;//간선들의 위치 정보 저장 (만약, 없다면 '-1' 반환)

int size = 0;//저장되어 있는 간선의 수 저장

void swap_edge(int index1, int index2) {//두 간선 swap
    edge temp = edges[index2];
    edges[index2] = edges[index1];
    edges[index1] = temp;
}

void swap_num(int* a, int* b) {//두 숫자 swap
    int temp = *a;
    *a = *b;
    *b = temp;
}

int compare(edge e1, edge e2) {//두 간선의 우선순위 비교 ('1'-> e1 우선, '0' -> e2 우선)
    if(e1.cost < e2.cost) return 1;
    else if(e1.cost > e2.cost) return 0;
    else {//cost, u, v 순서로 우선순위 비교
        if(e1.u < e2.u) return 1;
        else if(e1.u > e2.u) return 0;
        else {
            if(e1.v < e2.v) return 1;
            else return 0;
        }
    }
}

int binary_search(edge e) {//이진 탐색. edges 배열에서 e보다 첫 번째로 우선순위가 같거나 낮은 요소의 인덱스를 반환
    if(size == 0) return 0;
    if(compare(edges[size-1], e)) return size;

    int l = 0;
    int r = size - 1;
    int m;

    while(l < r) {
        m = (l + r)/2;

        if(compare(edges[m], e)) l = m + 1;
        else r = m;
    }

    return r;
}

void delete_edge(int u, int v) {//(u, v) edge를 삭제
    if(u > v) swap_num(&u, &v);//make u < v

    int posIndex = pos[v][u];//삭제할 요소의 인덱스

    for(int i = posIndex; i < size - 1; i++) {
        edges[i] = edges[i+1];
        pos[edges[i+1].v][edges[i+1].u] = i;//update pos table
    }

    size--;
    pos[v][u] = -1;//delete
}   

void insert_edge(int u, int v, int w) {//간선 삽입
    edge e = {u, v, w};

    if(size == 0) {//if edges are empty,
        edges[size++] = e;
        return; 
    }

    int posIndex = binary_search(e);

    size++;
    for(int i = size - 1; i > posIndex; i--) {
        edges[i] = edges[i-1];
        pos[edges[i-1].v][edges[i-1].u] = i;//update pos table
    }

    edges[posIndex] = e;

    pos[e.v][e.u] = posIndex;//update pos table
}

void change_weight(int u, int v, int w) {//(u, v) edge 가중치를 w로 변경
    if(u > v) swap_num(&u, &v);//make u < v
    
    delete_edge(u, v);
    insert_edge(u, v, w);
}

//Below functions are about allocating & destroying 
void initalize() {
    //initialize parent
    parent = (int*)malloc((V+1)*sizeof(int));//노드 번호와 인덱스를 일치시키기 위해 V + 1
    for(int i = 1; i <= V; i++) parent[i] = i;

    //initialize rank(여기서는 집합의 크기)
    rank= (int*)malloc((V+1)*sizeof(int));
    for(int i = 1; i<= V; i++) rank[i] = 1;

    //initialize pos
    pos = (int**)malloc((V+1) * sizeof(int*));
    for(int i = 0; i < V+1; i++) pos[i] = (int*)malloc((i+1)*sizeof(int));//[u][v] -> u > v

    for(int i = 0; i <= V; i++) for(int j = 0; j <= i; j++) pos[i][j] = -1;
}

void destroy() {
    free(parent);
    free(rank);
    for(int i = 0; i < V+1; i++) free(pos[i]);
    free(pos);
}


//Belows are Disjoint-Union-Set data structure implementation
int find_root(int x) {//u 노드가 속한 트리의 루트 노드 찾기
    while(x != parent[x]) {
        x = parent[x] = parent[parent[x]];//path compression (path-halving)
    }

    return x;
}

void union_set(int x, int y) {//두 그래프 합치기
    x = find_root(x);
    y = find_root(y);

    if(x == y) return;//already in same graph

    //더 큰 집합으로 합치기
    if(rank[x] > rank[y]) {
        parent[y] = x;
        rank[x] += rank[y]; 
    }
    else {
        parent[x] = y;
        rank[y] += rank[x];
    }
}
//finish implementing DUS data structure


int kruskal_algorithm() {//단, 이미 간선들이 heapify되었음을 가정
    int total_weight = 0;//total weight

    for(int i = 0; i < size; i++) {
        int cost = edges[i].cost;
        int u = edges[i].u;
        int v = edges[i].v;

        if(find_root(u) != find_root(v)) {
            union_set(u, v);
            total_weight += cost;
        }
    }

    return total_weight;
}

int main() {
    FILE* fr = fopen("mst.in", "r");
    FILE* fw = fopen("mst.out", "w");

    fscanf(fr, "%d", &V);

    initalize();//init

    char instruction[50];

    int u, v, weight;
    while(fscanf(fr, "%s", instruction)!=EOF) {
        if (strcmp(instruction, "insertEdge") == 0) {
            fscanf(fr, "%d %d %d", &u, &v, &weight);
            if(u > v) swap_num(&u, &v);//make u < v
            insert_edge(u, v, weight);
        } else if (strcmp(instruction, "deleteEdge") == 0) {
            fscanf(fr, "%d %d", &u, &v);
            if(u > v) swap_num(&u, &v);//make u < v
            delete_edge(u, v);
        } else if (strcmp(instruction, "changeWeight") == 0) {
            fscanf(fr, "%d %d %d", &u, &v, &weight);
            if(u > v) swap_num(&u, &v);//make u < v
            change_weight(u, v, weight);
        } else if (strcmp(instruction, "findMST") == 0) {
            fprintf(fw, "%d\n", kruskal_algorithm());
        }
    }

    destroy();//free

    return 0;
}

