#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define OO 0x3f3f3f3f 
#define MAX_CITIES 1000 

int citys_num, roads_num;
int *adjacency[MAX_CITIES + 1];
int *distance_count;
int *path;
int source = -1, destination = -1;

typedef struct QueueNode {
    int city;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *front;
    QueueNode *back;
} Queue;

void initQueue(Queue *queue) {
    queue->front = queue->back = NULL;
}

bool isQueueEmpty(Queue *queue) {
    return queue->front == NULL;
}

void enqueue(Queue *queue, int city) {
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    newNode->city = city;
    newNode->next = NULL;
    if (queue->back == NULL) {
        queue->front = queue->back = newNode;
    } else {
        queue->back->next = newNode;
        queue->back = newNode;
    }
}

int dequeue(Queue *queue) {
    if (isQueueEmpty(queue)) {
        return -1;
    }
    QueueNode *temp = queue->front;
    int city = temp->city;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->back = NULL;
    }
    free(temp);
    return city;
}

void freeQueue(Queue *queue) {
    while (!isQueueEmpty(queue)) {
        dequeue(queue);
    }
}

bool BFS(int source) {
    Queue queue;
    initQueue(&queue);
    distance_count[source] = 0;
    enqueue(&queue, source);
    while (!isQueueEmpty(&queue)) {
        int parent = dequeue(&queue);
        for (int *neighbor = adjacency[parent]; *neighbor != -1; neighbor++) {
            int child = *neighbor;
            if (distance_count[child] == OO) {
                path[child] = parent;
                if (child == destination) {
                    freeQueue(&queue);
                    return true;
                }
                enqueue(&queue, child);
                distance_count[child] = distance_count[parent] + 1;
            }
        }
    }
    freeQueue(&queue);
    return false;
}

void print_path(int city) {
    if (city == source) {
        printf("%d", city);
        return;
    }
    print_path(path[city]);
    printf(" %d", city);
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    scanf("%d %d", &citys_num, &roads_num);
    distance_count = (int *)malloc((citys_num + 1) * sizeof(int)); 
    path = (int *)malloc((citys_num + 1) * sizeof(int)); 
    for (int i = 1; i <= citys_num; i++) {
        adjacency[i] = (int *)malloc((roads_num + 1) * sizeof(int));
        for (int j = 0; j <= roads_num; j++) {
            adjacency[i][j] = -1;
        }
        distance_count[i] = OO;
    }
    for (int i = 0; i < roads_num; i++) {
        int u, v;
        scanf("%d", &u);
        scanf("%d", &v);
        int j = 0;
        while (adjacency[u][j] != -1) j++;
        adjacency[u][j] = v;
        j = 0;
        while (adjacency[v][j] != -1) j++;
        adjacency[v][j] = u;
    }
    while (source < 1 || source > citys_num) {
        scanf("%d", &source);
        if (source >= 1 && source <= citys_num) break;
    }
    while (destination < 1 || destination > citys_num) {
        scanf("%d", &destination);
        if (destination >= 1 && destination <= citys_num) break;
    }
    if (source != destination) {
        if (BFS(source)) {
            print_path(destination);
        } else {
            printf("-1");
        }
    }
    for (int i = 1; i <= citys_num; i++) {
        free(adjacency[i]);
    }
    free(distance_count);
    free(path);
    return 0;
}
