#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define OO 0x3f3f3f3f // "Infinity" value, representing an unreachable city
#define MAX_CITIES 1000 // Define an upper limit for the number of cities

int citys_num, roads_num;
int *adjacency[MAX_CITIES + 1]; // Array of pointers for adjacency list (1-indexed)
int *distance_count; // Array to store the shortest distance from source to each city
int *path; // Array to store the path (parent) for each city
int source = -1, destination = -1; // Source and destination cities

// Define a structure for the queue node
typedef struct QueueNode {
    int city; // The city this node represents
    struct QueueNode *next; // Pointer to the next node in the queue
} QueueNode;

// Define the queue structure
typedef struct Queue {
    QueueNode *front; // Front of the queue (dequeue)
    QueueNode *back; // Back of the queue (enqueue)
} Queue;

// Function to initialize the queue
void initQueue(Queue *queue) {
    queue->front = queue->back = NULL; // Initialize queue with no elements
}

// Function to check if the queue is empty
bool isQueueEmpty(Queue *queue) {
    return queue->front == NULL; // Queue is empty if the front is NULL
}

// Function to enqueue a city into the queue
void enqueue(Queue *queue, int city) {
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode)); // Create a new node
    newNode->city = city; // Set the city in the node
    newNode->next = NULL; // The new node does not point to anything

    // If the queue is empty, the new node becomes both front and back
    if (queue->back == NULL) {
        queue->front = queue->back = newNode;
    } else {
        // Otherwise, add the new node at the back of the queue
        queue->back->next = newNode;
        queue->back = newNode;
    }
}

// Function to dequeue a city from the queue
int dequeue(Queue *queue) {
    if (isQueueEmpty(queue)) {
        return -1;  // Queue is empty, return an invalid city
    }

    QueueNode *temp = queue->front; // Get the front node
    int city = temp->city; // The city in the front node
    queue->front = queue->front->next; // Move the front pointer to the next node

    if (queue->front == NULL) {
        queue->back = NULL; // If the queue is empty, set the back pointer to NULL
    }

    free(temp); // Free the memory of the dequeued node
    return city; // Return the city that was dequeued
}

// Function to free all the memory used by the queue
void freeQueue(Queue *queue) {
    while (!isQueueEmpty(queue)) {
        dequeue(queue); // Dequeue all elements in the queue
    }
}

// BFS function using linked list-based queue to find the shortest path
bool BFS(int source) {
    Queue queue;
    initQueue(&queue); // Initialize the queue

    distance_count[source] = 0; // The distance to the source city is 0
    enqueue(&queue, source); // Enqueue the source city

    // BFS loop: process cities level by level
    while (!isQueueEmpty(&queue)) {
        int parent = dequeue(&queue); // Dequeue the next city to process

        // Explore all neighbors of the current city (parent)
        for (int *neighbor = adjacency[parent]; *neighbor != -1; neighbor++) {
            int child = *neighbor; // Get the neighboring city

            // If the city has not been visited yet (distance is still "infinity")
            if (distance_count[child] == OO) {
                path[child] = parent; // Record the parent city for the current city
                if (child == destination) { // If we reach the destination, stop
                    freeQueue(&queue); // Free the queue memory
                    return true; // Found the destination
                }
                enqueue(&queue, child); // Enqueue the neighboring city for further exploration
                distance_count[child] = distance_count[parent] + 1; // Set the distance to this city
            }
        }
    }

    freeQueue(&queue); // Free the queue memory after BFS
    return false; // No path found to the destination
}

// Function to print the path from the source to the destination recursively
void print_path(int city) {
    if (city == source) {
        printf("%d", city); // If the city is the source, just print it
        return;
    }
    print_path(path[city]); // Recursively print the path from the parent city
    printf(" ---> %d", city); // Print the current city
}

int main() {
    // Prompt user for the number of cities and roads
    printf("Enter number of cities and roads: ");
    scanf("%d %d", &citys_num, &roads_num);

    // Allocate memory for distance_count and path arrays (1-indexed)
    distance_count = (int *)malloc((citys_num + 1) * sizeof(int)); 
    path = (int *)malloc((citys_num + 1) * sizeof(int)); 

    // Allocate and initialize adjacency lists for each city
    for (int i = 1; i <= citys_num; i++) {  // 1-indexed
        adjacency[i] = (int *)malloc((roads_num + 1) * sizeof(int)); // Room for all neighbors
        for (int j = 0; j <= roads_num; j++) {
            adjacency[i][j] = -1; // Initialize with -1 to mark the end of neighbors
        }
        distance_count[i] = OO; // Initialize all distances to "infinity"
    }

    // Prompt user for road connections between cities
    printf("Enter roads (u v):\n");
    for (int i = 0; i < roads_num; i++) {
        int u, v;
        printf("From: ");
        scanf("%d", &u);
        printf("To: ");
        scanf("%d", &v);
        
        // Add road from city u to city v
        int j = 0;
        while (adjacency[u][j] != -1) j++; // Find the first empty space in the adjacency list
        adjacency[u][j] = v; // Add city v as a neighbor of city u
        j=0;
        while (adjacency[v][j] != -1) j++; 
        adjacency[v][j] = u; // and city u as a neighbor of city v 

        if(i != roads_num - 1)
            printf("Next road\n"); // Inform the user to input the next road if not the last road
    }

    // Validate the source city input
    while (source < 1 || source > citys_num) {
        printf("Where are you: ");
        scanf("%d", &source);
        if (source >= 1 && source <= citys_num) break;
        printf("This city does not exist\n");
    }

    // Validate the destination city input
    while (destination < 1 || destination > citys_num) {
        printf("Where do you want to go: ");
        scanf("%d", &destination);
        if (destination >= 1 && destination <= citys_num) break;
        printf("This city does not exist\n");
    }

    // If source and destination are the same, no need to find a path
    if (source == destination) {
        printf("You are already here.\n");
    } else {
        // Perform BFS to find the shortest path
        if (BFS(source)) {
            printf("Path: ");
            print_path(destination); // Print the path from source to destination
            printf("\n");
        } else {
            printf("You can't reach this city.\n");
        }
    }

    // Free memory allocated for adjacency list, distance_count, and path arrays
    for (int i = 1; i <= citys_num; i++) {
        free(adjacency[i]);
    }
    free(distance_count);
    free(path);

    return 0;
}
