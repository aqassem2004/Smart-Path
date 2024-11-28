#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define OO 0x3f3f3f3f // "Infinity" value
#define MAX_CITIES 1000 // Define an upper limit for the number of cities

int citys_num, roads_num;
int *adjacency[MAX_CITIES + 1]; // Array of pointers for adjacency list (1-indexed)
int *distance_count;
int *path;
int source = -1, destination = -1;

// Define a structure for the queue node
typedef struct QueueNode {
    int city;
    struct QueueNode *next;
} QueueNode;

// Define the queue structure
typedef struct Queue {
    QueueNode *front;
    QueueNode *back;
} Queue;

// Function to initialize the queue
void initQueue(Queue *queue) {
    queue->front = queue->back = NULL;
}

// Function to check if the queue is empty
bool isQueueEmpty(Queue *queue) {
    return queue->front == NULL;
}

// Function to enqueue a city
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

// Function to dequeue a city
int dequeue(Queue *queue) {
    if (isQueueEmpty(queue)) {
        return -1;  // Queue is empty
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

// Function to free the queue's memory
void freeQueue(Queue *queue) {
    while (!isQueueEmpty(queue)) {
        dequeue(queue);
    }
}

// BFS function using linked list-based queue
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
    printf(" ---> %d", city);
}

int main() {
    printf("Enter number of cities and roads: ");
    scanf("%d %d", &citys_num, &roads_num);

    distance_count = (int *)malloc((citys_num + 1) * sizeof(int));  // 1-indexed
    path = (int *)malloc((citys_num + 1) * sizeof(int));  // 1-indexed

    // Allocate and initialize adjacency lists
    for (int i = 1; i <= citys_num; i++) {  // 1-indexed
        adjacency[i] = (int *)malloc((roads_num + 1) * sizeof(int));  // Room for all neighbors
        for (int j = 0; j <= roads_num; j++) {
            adjacency[i][j] = -1;  // Initialize with -1 to mark the end of neighbors
        }
        distance_count[i] = OO;
    }

    printf("Enter roads (u v):\n");
    for (int i = 0; i < roads_num; i++) {
        int u, v;
        printf("From: ");scanf("%d", &u);
        printf("To: ");scanf("%d", &v);
        int j = 0;
        while (adjacency[u][j] != -1) j++;
        adjacency[u][j] = v;  // Add road from u to v (1-indexed)
        if(i!=roads_num-1)
            printf("Next road\n");
    }

    while (source < 1 || source > citys_num) {
        printf("Where are you: ");
        scanf("%d", &source);
        if (source >= 1 && source <= citys_num) break;
        printf("This city does not exist\n");
    }

    while (destination < 1 || destination > citys_num) {
        printf("Where do you want to go: ");
        scanf("%d", &destination);
        if (destination >= 1 && destination <= citys_num) break;
        printf("This city does not exist\n");
    }

    if (source == destination) {
        printf("You are already here.\n");
    } else {
        if (BFS(source)) {
            printf("Path: ");
            print_path(destination);
            printf("\n");
        } else {
            printf("You can't reach this city.\n");
        }
    }

    // Free memory
    for (int i = 1; i <= citys_num; i++) {
        free(adjacency[i]);
    }
    free(distance_count);
    free(path);

    return 0;
}
