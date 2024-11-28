#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const int OO = 0x3f3f3f3f; // "Infinity" value, representing an unreachable city
int citys_num, roads_num; // Number of cities and roads
vector<vector<int>> adjacency; // Adjacency list to store roads between cities
vector<int> distance_count; // Array to store the shortest distance from source to each city
vector<int> path; // Array to store the path (parent) for each city
int source = -1, destination = -1; // Source and destination cities

// BFS function to find the shortest path from the source to the destination
bool BFS(int source) {
    queue<int> queue_list; // Queue to perform BFS
    distance_count[source] = 0; // Distance to the source is 0
    queue_list.push(source); // Start BFS from the source city

    while (!queue_list.empty()) {
        int parent = queue_list.front(); // Get the city at the front of the queue
        queue_list.pop(); // Remove the city from the queue
        
        // Explore all neighboring cities of the current city (parent)
        for (int children : adjacency[parent]) {
            // If the neighboring city has not been visited (still has "infinity" distance)
            if (distance_count[children] == OO) {
                path[children] = parent; // Set the parent city for the current city
                if (children == destination) // If we have reached the destination
                    return true;
                queue_list.push(children); // Add the neighboring city to the queue for exploration
                distance_count[children] = distance_count[parent] + 1; // Set the distance to the neighboring city
            }
        }
    }
    return false; // Return false if no path is found to the destination
}

// Recursive function to print the path from the source to the destination
void print_path(int city) {
    if (city == source) {
        cout << city << " "; // If the current city is the source, print it
        return;
    }
    print_path(path[city]); // Recursively print the path from the parent city
    cout << " ---> " << city; // Print the current city
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr); // Faster I/O for large inputs
    
    // Prompt user to input the number of cities and roads
    cout << "Enter number of cities and roads: ";
    cin >> citys_num >> roads_num;

    // Resize the adjacency list, path, and distance_count arrays
    adjacency.resize(citys_num + 1); // 1-indexed adjacency list
    path.resize(citys_num + 1); // 1-indexed path array
    distance_count.assign(citys_num + 1, OO); // Set all cities' distances to "infinity"

    // Prompt user to input road connections between cities
    cout << "Enter roads (u v):\n";
    for (int i = 0; i < roads_num; ++i) {
        int u, v;
        cout << "From: "; cin >> u;
        cout << "To: "; cin >> v;
        adjacency[u].push_back(v); // Add road from u to v
        if (i != roads_num - 1) {
            cout << "Next road\n"; // Prompt for the next road if it's not the last road
        }
    }

    // Validate and input the source city
    while (source < 1 || source > citys_num) {
        cout << "Where are you: ";
        cin >> source;
        if (source >= 1 && source <= citys_num) break;
        cout << "This city does not exist\n";
    }

    // Validate and input the destination city
    while (destination < 1 || destination > citys_num) {
        cout << "Where do you want to go: ";
        cin >> destination;
        if (destination >= 1 && destination <= citys_num) break;
        cout << "This city does not exist\n";
    }

    // If source and destination are the same, no need to find a path
    if (source == destination) {
        cout << "You are already here\n";
    } else {
        // Perform BFS to find the shortest path
        if (BFS(source)) {
            cout << "Path: ";
            print_path(destination); // Print the path from source to destination
        } else {
            cout << "You can't reach this city.\n"; // No path found to destination
        }
    }
    return 0;
}
