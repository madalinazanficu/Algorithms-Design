#include <bits/stdc++.h>
using namespace std;


class Solve {
 public:
    int n;                          // number of localities = nodes
    int m;                          // number of edges
    int k;                          // number of fortifications
    int b;
    std::vector<int> barbarity;     // b positions controlled by barbars

    // edges[i][j] = time taken between node i and node j
    std::unordered_map<int, std::vector<std::pair<int, int>>> edges;

    // used in Dijsktra
    std::vector<int> dist;

    // all distances to all barbars
    std::vector<int> all_paths;

    void init() {
        // edges
        for (int i = 0; i <= n; i++) {
            std::vector<std::pair<int, int>> neighs;
            edges[i] = neighs;
        }
        // distance used in Dijsktra
        for (int i = 0; i <= n; i++) {
            dist.push_back(INT_MAX / 4);
        }
    }

    void read() {
        std::ifstream in;
        in.open("./fortificatii.in");
        in >> n >> m >> k;
        in >> b;
        for (int i = 0; i < b; i++) {
            int x;
            in >> x;
            barbarity.push_back(x);
        }
        init();
        for (int i = 0; i < m; i++) {
            int x, y, t;
            in >> x >> y >> t;

            // Edge x ---> y
            std::pair<int, int> new_neigh(y, t);
            edges[x].push_back(new_neigh);

            // Edge y ---> x
            std::pair<int, int> new_neigh1(x, t);
            edges[y].push_back(new_neigh1);
        }
    }

    bool not_barbar(int node) {
        for (int i = 0; i < barbarity.size(); i++) {
            if (barbarity[i] == node) {
                return false;
            }
        }
        return true;
    }

    // Searching the distance to all non-barbaian cities
    void Dijsktra() {
        std::vector<int> visited(n + 1, 0);
        auto cmp = [](pair<int, int> p1, pair<int, int> p2) {
            return p1.second > p2.second;
        };
        std::priority_queue<std::pair<int, int>,
                            std::vector<std::pair<int, int>>,
                            decltype(cmp)> pq(cmp);

        // The first node is the capital, with cost = 0
        std::pair<int, int> start(1, 0);
        dist[1] = 0;
        pq.push(start);

        while (!pq.empty()) {
            std::pair<int, int> top = pq.top();
            pq.pop();
            int node = top.first;
            int cost = top.second;

            if (visited[node] == 0) {
                visited[node] = 1;
                // Adding the non barbarian neighbours
                std::vector<std::pair<int, int>> neighs = edges[node];
                for (int i = 0; i < neighs.size(); i++) {
                    // Check not to be a barbar
                    int nei = neighs[i].first;
                    int nei_cost = neighs[i].second;
                    bool check = not_barbar(nei);

                    // Neigh[i] is not barbar
                    if (check == true) {
                        if (dist[nei] > dist[node] + nei_cost) {
                            dist[nei] = dist[node] + nei_cost;
                            pq.push(neighs[i]);
                        }
                    }
                }
            } else {
                continue;
            }
        }

        for (int i = 0; i < dist.size(); i++) {
            if (dist[i] == INT_MAX / 4) {
                dist[i] = -1;
            }
        }
    }

    // For all barbars, find all the distance on all the paths
    void find_all_paths() {
        for (int i = 0; i < barbarity.size(); i++) {
            int bar = barbarity[i];

            // Check the neighs of the barbar
            std::vector<std::pair<int, int>> neighbours = edges[bar];
            for (std::pair<int, int> nei : neighbours) {
                int neigh = nei.first;
                int weight_edge = nei.second;
                int weight_way = dist[neigh];

                /* Total path is the cost of the edge between nodes +
                minimum distance to the neight ->
                weight_way (Computed with Dijstrka) */
                if (not_barbar(neigh) && weight_way != -1) {
                    int total_weight = weight_edge + weight_way;
                    all_paths.push_back(total_weight);
                }
            }
        }
        sort(all_paths.begin(), all_paths.end());
    }

    int search() {
        sort(all_paths.begin(), all_paths.end());
        // the smallest past to town
        int l = all_paths[0];
        // the smallest past to town + max number of fortifications
        int r = all_paths[0] + this->k;
        // searching for a average path
        int mid = (l + r) / 2;

        int position = 0;
        int respons = 0;
        // Finding the closest path to the average one
        for (int i = 0; i < all_paths.size() && all_paths[i] <= mid; i++) {
            position = i;
        }
        mid = all_paths[position];

        int total_costs = 0;
        for (int i = 0; i < all_paths.size(); i++) {
            total_costs = total_costs + all_paths[i];
        }

        bool done = true;
        /* Considering mid = the average path
           In case all paths could be supplied with fortifications 
           in order to be average paths. This would be a solution */
        while (mid <= all_paths[all_paths.size() - 1] && done == true) {
            int fort = k;
            done = true;
            for (int i = 0; i <= position && done == true; i++) {
                if (all_paths[i] <= mid) {
                    int diff = mid - all_paths[i];
                    // Not enough fortifications
                    if (diff > fort) {
                        done = false;
                        break;
                    }
                    fort = fort - diff;
                }
            }
            /* The solution is correct, but can we do better?
               Try is a bigger path */
            if (done == true) {
                respons = mid;
                mid++;
            }
        }

        if (mid > all_paths[all_paths.size() - 1]) {
            int avg = total_costs / all_paths.size();
            return avg + k / all_paths.size() + k % all_paths.size();
        }
        return respons;
    }
};

int main() {
    std::ofstream out;
    out.open("./fortificatii.out");
    auto* solving = new Solve();
    if (!solving) {
        std::cerr << "Fail!\n";
        return -1;
    }
    solving->read();
    solving->Dijsktra();
    solving->find_all_paths();
    out << solving->search();

    delete solving;
    return 0;
}
