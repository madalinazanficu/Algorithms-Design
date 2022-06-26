#include <bits/stdc++.h>
using namespace std;
class Solve {
 public:
    int n;  // the number of tracks
    int m;  // the number of cars
    int a;  // the number of trainings;
    std::vector<std::vector<int>> order;
    std::unordered_map<int, std::vector<int>> neighs;

    void read() {
        std::ifstream in;
        in.open("./curse.in");
        in >> n >> m >> a;
        // Line's value reprsents the car
        // The line represents a training
        // The column reprsents the track
        for (int i = 0; i < a; i++) {
            std::vector<int> line;
            for (int j = 0; j < n; j++) {
                int x;
                in >> x;
                line.push_back(x);
            }
            this->order.push_back(line);
        }
        // all cars will have neighbours or empty neighbours
        std::vector<int> neighbours;
        neighs[0] = neighbours;
        for (int i = 1; i <= m; i++) {
            std::vector<int> neighbours;
            neighs[i] = neighbours;
        }
    }
    void create_graph() {
        // Traversing on column
        for (int j = 0; j < this->a - 1; j++) {
            // the first track is x + 1
            int x = 0;

            while (1) {
                // checked all tracks, now stop
                if (x == n) {
                    break;
                }
                // car1 has better or equal time than car2
                int car1 = order[j][x];
                int car2 = order[j + 1][x];

                // equality, check the next track
                if (car1 == car2) {
                    x++;
                } else {
                    // edge: (car1) ---> (car2)
                    neighs[car1].push_back(car2);
                    break;
                }
            }
        }
    }
    void dfs(int node, std::vector<int> &visited, std::vector<int> &topo) {
        visited[node] = 1;

        // Traversing the neighbours
        std::vector<int> neighbours = neighs[node];
        for (int i = 0; i < neighbours.size(); i++) {
            int nei = neighbours[i];
            if (visited[nei] == 0) {
                dfs(nei, visited, topo);
            }
        }
        // After visiting the neighbours, add the current node
        topo.push_back(node);
    }
    std::vector<int> topo_sort() {
        std::vector<int> topo;
        std::vector<int> visited(m + 1, 0);
        visited[0] = 1;

        for (int i = 1; i <= m; i++) {
            if (visited[i] == 0) {
                dfs(i, visited, topo);
            }
        }
        std::reverse(topo.begin(), topo.end());
        return topo;
    }
    void result() {
        std::ofstream out;
        out.open("./curse.out");
        std::vector<int> topo = topo_sort();
        for (int i = 0; i < topo.size(); i++) {
            out << topo[i] << " ";
        }
    }
};

int main() {
    auto* solving = new Solve();
    if (!solving) {
        std::cerr << "Fail!\n";
        return -1;
    }
    solving->read();
    solving->create_graph();
    solving->result();
    delete solving;
    return 0;
}
