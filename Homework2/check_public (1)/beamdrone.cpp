#include <bits/stdc++.h>
using namespace std;
class Solve {
 public:
    int n;
    int m;
    int x_start;
    int y_start;
    int x_finish;
    int y_finish;
    std::vector<std::vector<int>> matrix;

    // all_costs[i][j] = costul nodului de pe linia i, coloana j
    std::vector<std::vector<int>> all_costs;

    // visited[i][j] = 1 =>> nodul de pe linia i, coloana j a fost  vizitat
    std::vector<std::vector<int>> visited;

    struct Node {
        int x;
        int y;
        int cost;
        int direction;
        bool visited;

        Node() { }
        Node(int x, int y, int cost, int direction) {
            this->x = x;
            this->y = y;
            this->cost = cost;
            this->direction = direction;
            this->visited = false;
        }
    };
    int opposite_direction(int &direction) {
        if (direction == 0) {
            return 0;
        }
        if (direction == 1) {
            return 2;
        }
        if (direction == 2) {
            return 1;
        }
        if (direction == 3) {
            return 4;
        }
        return 3;
    }

    void read() {
        // Citire din fisier;
        std::ifstream in;
        in.open("./beamdrone.in");
        in >> n >> m;
        in >> x_start >> y_start >> x_finish >> y_finish;
        for (int i = 0; i < n; i++) {
            std::vector<int> new_line;
            for (int j = 0; j < m; j++) {
                char c;
                in >> c;
                if (c == '.') {
                    new_line.push_back(0);
                }
                if (c == 'W') {
                    new_line.push_back(1);
                }
            }
            matrix.push_back(new_line);
            new_line.clear();

            // Se formeaza si vectorul de costuri
            std::vector<int> new_costs(m, INT_MAX);
            all_costs.push_back(new_costs);

            // Se formeaza si vectorul de vizitati
            std::vector<int> new_visited(m, 0);
            visited.push_back(new_visited);
        }
    }

    Node check_neigh(int &x, int &y, int &direction,
                    int &prev_direction, int &prev_cost) {
        // Verificare: sa nu iasa din matrice, si sa nu fie obstacol
        if (x >= 0 && y >= 0 && x < n && y < m && matrix[x][y] == 0) {
            int cost;
            if (opposite_direction(prev_direction) == direction) {
                Node null_node{-1, -1, -1, -1};
                return null_node;
            }

            // Aceeasi directie cu nodul anterior => costul nu se schimba
            if (prev_direction == direction || prev_direction == 0)
                cost = prev_cost;
            else
                cost = prev_cost + 1;

            // Verificare cost mai mic decat cel anterior
            if (cost <= this->all_costs[x][y]) {
                this->all_costs[x][y] = cost;
                Node new_node(x, y, cost, direction);
                return new_node;
            }
        }
        Node null_node{-1, -1, -1, -1};
        return null_node;
    }

    void Dijsktra() {
        // Metoda de comparare folosita pentru priority_queue
        auto cmp = [](Node x, Node y) {return x.cost > y.cost;};
        std::priority_queue<Node, std::vector<Node>, decltype(cmp)> pq(cmp);

        // Directia din care vine nu adauga cost pe muchiile nodurilor vecine
        // Costul nodului curent este 0
        Node start(this->x_start, this->y_start, 0, 0);
        start.visited = false;
        pq.push(start);

        while (!pq.empty()) {
            Node node = pq.top();
            pq.pop();
            int prev_cost = node.cost;
            int prev_direction = node.direction;

            // Directia: 1 = Nord, 2 = Sud, 3 = Vest, 4 = Est
            if (visited[node.x][node.y] == 0) {
                visited[node.x][node.y] = 1;

                // Nord
                int x1 = node.x - 1;
                int y1 = node.y;
                int direction1 = 1;
                Node neigh1 = check_neigh(x1, y1, direction1,
                                            prev_direction, prev_cost);
                if (neigh1.x != -1) {
                    pq.push(neigh1);
                }

                // Sud
                int x2 = node.x + 1;
                int y2 = node.y;
                int direction2 = 2;
                Node neigh2 = check_neigh(x2, y2, direction2,
                                            prev_direction, prev_cost);
                if (neigh2.x != -1) {
                    pq.push(neigh2);
                }

                // Vest
                int x3 = node.x;
                int y3 = node.y - 1;
                int direction3 = 3;
                Node neigh3 = check_neigh(x3, y3, direction3,
                                            prev_direction, prev_cost);
                if (neigh3.x != -1) {
                    pq.push(neigh3);
                }

                // Est
                int x4 = node.x;
                int y4 = node.y + 1;
                int direction4 = 4;
                Node neigh4 = check_neigh(x4, y4, direction4,
                                            prev_direction, prev_cost);
                if (neigh4.x != -1) {
                    pq.push(neigh4);
                }
            } else {
                continue;
            }
        }
        for (int i = 0; i < this->n; i++) {
            for (int j = 0; j < this->m; j++) {
                if (this->all_costs[i][j] == INT_MAX) {
                    this->all_costs[i][j] = -1;
                }
            }
        }
    }
    int solution() {
        return this->all_costs[x_finish][y_finish];
    }
};


int main() {
    // Afisare in fisier
    std::ofstream out;
    out.open("./beamdrone.out");
    auto* solving = new Solve();
    if (!solving) {
        std::cerr << "Fail!\n";
        return -1;
    }
    solving->read();
    solving->Dijsktra();
    out << solving->solution();
    delete solving;
    return 0;
}
