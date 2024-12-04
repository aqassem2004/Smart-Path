#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <string.h> 
#include <math.h>
#include <conio.h> 

const int radius = 15, start_x = 300, start_y = 50;
#define OO 0x3f3f3f3f
#define MAX_CITIES 1000

int citys_num, roads_num;
int *adjacency[MAX_CITIES + 1];
int *distance_count;
int *path;
int source = -1, destination = -1;

typedef struct point {
    int x;
    int y;
}point;
                        /*Start implementing the queue*/
/////////////////////////////////////////////////////////////////////////////////
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

bool Empty(Queue *queue) {
    return queue->front == NULL;
}

void push(Queue *queue, int city) {
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

int pop(Queue *queue) {
    if (Empty(queue)) {
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
    while (!Empty(queue)) {
        pop(queue);
    }
}

/////////////////////////////////////////////////////////////////////////////////

                        /*Graphics Functions*/
/////////////////////////////////////////////////////////////////////////////////
void print_colored_road( point city1,  point city2, int color) {
    setcolor(color);
    line(city1.x, city1.y + radius, city2.x, city2.y - radius);
}

void print_colored_city( point city, int val, int color) {
    setcolor(color);
    circle(city.x, city.y, radius);
    char s[10];
    sprintf(s, "%d", val);
    outtextxy(city.x - 5, city.y - 8, s);
}
/////////////////////////////////////////////////////////////////////////////////


                        /*Algorithm implementation*/
/////////////////////////////////////////////////////////////////////////////////
bool BFS(int source) {
    Queue queue;
    initQueue(&queue);
    bool found = false;
    distance_count[source] = 0;
    push(&queue, source);

    while (!Empty(&queue)) {
        int parent = pop(&queue);

        for (int *neighbor = adjacency[parent]; *neighbor != -1; neighbor++) {
            int child = *neighbor;

            if (distance_count[child] == OO) {
                path[child] = parent;
                if (child == destination) {
                    found = true;
                }
                push(&queue, child);
                distance_count[child] = distance_count[parent] + 1;
            }
        }
    }

    freeQueue(&queue);
    return found;
}

void generate_best_path(int source, int destination, point cities[], int *path) {
    int current = destination;
    char path_string[500] = {0};
    char temp[10];
    strcat(path_string, "This is best path  ");
    while (current != source) {
        print_colored_road(cities[path[current]], cities[current], LIGHTBLUE);
        print_colored_city(cities[current], current, LIGHTBLUE);

        sprintf(temp, "%d", current);
        strcat(path_string, temp);
        strcat(path_string, " <--- ");
        
        delay(750); 
        current = path[current];
    }
    
    sprintf(temp, "%d", source);
    strcat(path_string, temp);

    print_colored_city(cities[source], source, LIGHTGREEN);
    print_colored_city(cities[destination], destination, LIGHTMAGENTA);
    setcolor(WHITE);
    outtextxy(200, 600, path_string);
}                        
/////////////////////////////////////////////////////////////////////////////////


                        /*Organize city locations on the screen*/
/////////////////////////////////////////////////////////////////////////////////
#define PI 3.14159
void generate_geometric_cities( point cities[], int city_count, int center_x, int center_y) {
    int _radius = 200;
    float angle_step = 2 * PI / city_count; 
    for (int i = 1; i <= city_count; i++) {
        cities[i].x = center_x + (int)(_radius * cos(i * angle_step));
        cities[i].y = center_y + (int)(_radius * sin(i * angle_step));
    }
}
/////////////////////////////////////////////////////////////////////////////////


int main() {

         /*Defining libraries and running the display screen*/
/////////////////////////////////////////////////////////////////////////////////
    char data[] = "C:\\MinGW\\lib\\libbgi.a"; 
    initwindow(1000, 700, data); 
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
/////////////////////////////////////////////////////////////////////////////////

    freopen("Test3.txt", "r", stdin);//Open test file

    scanf("%d %d", &citys_num, &roads_num);

    point cities[citys_num + 1];
    generate_geometric_cities(cities, citys_num, 500, 350);
    
    for (int i = 1; i <= citys_num; i++) {
        print_colored_city(cities[i], i, WHITE);
    }
                        /*Memory processing*/
//////////////////////////////////////////////////////////////////////////////////

    distance_count = (int *)malloc((citys_num + 1) * sizeof(int));
    path = (int *)malloc((citys_num + 1) * sizeof(int));

    for (int i = 1; i <= citys_num; i++) {
        adjacency[i] = (int *)malloc((roads_num + 1) * sizeof(int));
        for (int j = 0; j <= roads_num; j++) {
            adjacency[i][j] = -1;
        }
        distance_count[i] = OO;
    }
//////////////////////////////////////////////////////////////////////////////////

                        /*Read input*/
//////////////////////////////////////////////////////////////////////////////////

    for (int i = 0; i < roads_num; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        int j = 0;
        while (adjacency[u][j] != -1)
            j++;
        adjacency[u][j] = v;
        j = 0;
        while (adjacency[v][j] != -1)
            j++;
        adjacency[v][j] = u;
        print_colored_road(cities[u], cities[v],WHITE);
    }
    scanf("%d %d", &source, &destination);
//////////////////////////////////////////////////////////////////////////////////


                        /*Call Algorithm*/
//////////////////////////////////////////////////////////////////////////////////
   char message1[50] = {0};
    sprintf(message1, "From %d   To %d", source, destination);
    outtextxy(350, 50, message1);
    char message2[50] = "You can't reach this city.";

    if (BFS(source)) {
        generate_best_path(source, destination, cities, path);

    } else {
        outtextxy(200, 600, message2);
    }
//////////////////////////////////////////////////////////////////////////////////



                        /*Clear memory*/
/////////////////////////////////////////////////////////////////////////////////   
    for (int i = 1; i <= citys_num; i++) {
        free(adjacency[i]);
    }
    free(distance_count);
    free(path);
/////////////////////////////////////////////////////////////////////////////////   


                        /*Close window*/
/////////////////////////////////////////////////////////////////////////////////
    getch();
    closegraph();
/////////////////////////////////////////////////////////////////////////////////

    return 0;
}
