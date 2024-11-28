#include <iostream>
#include <vector>
#include <queue>

using namespace std;
const int OO = 0x3f3f3f3f;//infinity value
int citys_num, roads_num;
vector<vector<int>>adjacency;
vector<int>distance_count;
vector<int>path;
int source = -1, destination = -1;
bool BFS(int source)
{
    queue<int>queue_list;
    distance_count[source] = 0;
    queue_list.push(source);
    while (!queue_list.empty())
    {
        int parent = queue_list.front();
        queue_list.pop();
        for (int children : adjacency[parent])
        {
            if (distance_count[children] == OO)
            {
                path[children] = parent;
                if (children == destination)
                    return true;
                queue_list.push(children);
                distance_count[children] = distance_count[parent] + 1;

            }
        }
    }
    return false;
}
void print_path(int city)
{
    if (city == source)
    {
        cout << city << " ";
        return;
    }
    print_path(path[city]);
    cout << " ---> " << city;
}
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    cout << "Enter number of cities and roads: ";
    cin >> citys_num >> roads_num;
    adjacency.resize(citys_num + 1);
    path.resize(citys_num + 1);
    distance_count.assign(citys_num + 1, OO);
    cout << "Enter roads (u v):\n";
    for (int i = 0; i < roads_num; ++i) {
        int u, v;
        cout << "From: "; cin >> u;
        cout << "To: "; cin >> v;
        adjacency[u].push_back(v);
        if(i!=roads_num-1)
        cout << "Next road\n";
    }
    while (source > citys_num || source < 1) {
        cout << "where are you: ";
        cin >> source;
        if (source >= 0 && source <= citys_num)
            break;
        cout << "This city is not exist\n";
    }
    while (destination > citys_num || destination < 1) {
        cout << "where do you want to go: ";
        cin >> destination;
        if (destination >= 0 && destination <= citys_num)
            break;
        cout << "This city is not exist\n";
    }
    if (source == destination)
        cout << "you are already here\n";
    else
    {
        if (BFS(source))
        {
            cout << "Path: ";
            print_path(destination);
        }
        else
        {
            cout << "You can't reach this city.";
        }
    }
    return 0;
}
