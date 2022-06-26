#include <bits/stdc++.h>
using namespace std;

class Solve {
 public:
	int n;
	int m;
	std::vector<std::vector<char>> grid;
	int r = 0; 	// number of robots
	int s = 0; 	// number of dirty spaces
	struct Node {
        int x;
        int y;

        Node() {}
        Node(int x, int y) {
            this->x = x;
            this->y = y;
        }
	};
	// robot[i] = Node with coordinates (x, y) of a robot in the grid
	std::vector<Node> robots;

	// to_clean[i] = Node with coordinates (x, y) of a dirty space in the grid
	std::vector<Node> to_clean;

	// dist_to_clean[i][j] = distance between to_clean[i] and to_clean[j]
	std::vector<std::vector<int>> dist_to_clean;

	// all_dists[i][j] = distance between robot[i] and to_clean[j]
	std::vector<std::vector<int>> dist_robots_spaces;

	void read() {
		std::ifstream in;
        in.open("./curatare.in");

		in >> n >> m;
		for (int i = 0; i < n; i++) {
			std::vector<char> grid_line;
			for (int j = 0; j < m; j++) {
				char x;
				in >> x;
				grid_line.push_back(x);

				// the position of the robot
				if (x == 'R') {
					Node new_robot(i, j);
					robots.push_back(new_robot);
					this->r++;
				}
				// the position of a dirty place
				if (x == 'S') {
					Node space_to_clean(i, j);
					to_clean.push_back(space_to_clean);
					this->s++;
				}
			}
			grid.push_back(grid_line);
			grid_line.clear();
		}
	}

	void init_distances() {
		// init distances between spaces to clean
		for (int i = 0; i < to_clean.size(); i++) {
			std::vector<int> new_line;
			for (int j = 0; j < to_clean.size(); j++) {
				new_line.push_back(INT_MAX / 3);
			}
			dist_to_clean.push_back(new_line);
		}

		// init between robots and spaces to clean
		for (int i = 0; i < robots.size(); i++) {
			std::vector<int> new_line;
			for (int j = 0; j < to_clean.size(); j++) {
				new_line.push_back(INT_MAX / 3);
			}
			dist_robots_spaces.push_back(new_line);
		}
	}

	/* Generate the neighbour of node
		flagOY = +1 east, -1 west
		flagOX = -1 north, 1 south
	*/
	std::pair<Node, int> gen_neigh(Node node, int flagOX, int flagOY, int dist,
									std::vector<std::vector<int>> &visited) {
		// Adding +1/ -1 in order to move on axes
		int next_x = node.x + flagOX;
		int next_y = node.y + flagOY;

		// Check the border of the Grid
		if (next_x >= 0 && next_x < n && next_y >= 0 && next_y < m &&
			grid[next_x][next_y] != 'X' && visited[next_x][next_y] != 1) {
			Node node_neigh(next_x, next_y);
			return {node_neigh, dist + 1};
		}

		// Error - out of bounds
		Node null_node(-100, -100);
		return {null_node, -100};
	}

	/* Check all searched destinations
	    src_index -> the index of BFS source node
		node -> the current node
		spaces_dest -> seached dirty spaces
		robots_dest -> searched robot spaces
	*/
	void check_destinations(int src_index, Node node, int dist,
								std::vector<Node> &spaces_dest,
								std::vector<Node> &robots_dest) {
		for (int i = 0; i < spaces_dest.size(); i++) {
			Node dst = spaces_dest[i];
			if (node.x == dst.x && node.y == dst.y) {
				// distance between space src_index and space i
				dist_to_clean[src_index][i] = dist;
				return;
			}
		}

		for (int i = 0; i < robots_dest.size(); i++) {
			Node dst = robots_dest[i];
			if (node.x == dst.x && node.y == dst.y) {
				// distance between robot i and space src_index
				dist_robots_spaces[i][src_index] = dist;
				return;
			}
		}
	}

	/* Minimum distance using BFS, starting from Node src
	Treated the grid as a graph, where Node is a grid position with (x, y) */
	void compute_distance(Node src, int src_index,
							std::vector<Node> &spaces_dest,
							std::vector<Node> &robots_dest) {
		std::queue<std::pair<Node, int>> q;
		std::vector<std::vector<int>> visited(n, std::vector<int>(m, 0));

		std::pair<Node, int> start;
		start.first = src;
		start.second = 0;
		q.push(start);

		while (!q.empty()) {
			std::pair<Node, int> top = q.front();
			q.pop();
			Node current_node = top.first;
			int dist = top.second;
			visited[current_node.x][current_node.y] = 1;

			if (grid[current_node.x][current_node.y] == 'R' ||
				grid[current_node.x][current_node.y] == 'S') {
				check_destinations(src_index, current_node, dist, spaces_dest, robots_dest);
			}

			// Check and generate the neighbours
			std::pair<Node, int> north = gen_neigh(current_node, -1, 0, dist, visited);
			std::pair<Node, int> south = gen_neigh(current_node, 1, 0, dist, visited);
			std::pair<Node, int> east = gen_neigh(current_node, 0, 1, dist, visited);
			std::pair<Node, int> west = gen_neigh(current_node, 0, -1, dist, visited);

			// Continue to add the valid neighbours in queue
			if (north.first.x != -100) {
				q.push(north);
				visited[north.first.x][north.first.y] = 1;
			}
			if (south.first.x != -100) {
				q.push(south);
				visited[south.first.x][south.first.y] = 1;
			}
			if (east.first.x != -100) {
				q.push(east);
				visited[east.first.x][east.first.y] = 1;
			}
			if (west.first.x != -100) {
				q.push(west);
				visited[west.first.x][west.first.y] = 1;
			}
		}
	}

	void compute_all_dist() {
		// Destinations -> dirty searched places
		std::vector<Node> spaces_dest;
		for (int i = 0; i < to_clean.size(); i++) {
			Node dst = to_clean[i];
			spaces_dest.push_back(dst);
		}

		// Destinations -> robots
		std::vector<Node> robots_dest;
		for (int i = 0; i < robots.size(); i++) {
			Node dst = robots[i];
			robots_dest.push_back(dst);
		}

		// Staring a BFS from every dirty places to destinations
		for (int i = 0; i < to_clean.size(); i++) {
			Node src = to_clean[i];
			compute_distance(src, i, spaces_dest, robots_dest);
		}
	}

	// Compute the time of all robots to clean the assigned spaces
	int check_partition(vector<int> partition) {
		int total_time = 0;
		std::unordered_map<int, vector<int>> link;

		// link every robot with all spaces he is in charge of
		for (int i = 0; i < partition.size(); i++) {
			int robot = partition[i];
			int space = i;

			link[robot].push_back(space);
		}
		for (auto it = link.begin(); it != link.end(); it++) {
			// the robot need to clean all these spaces
			int robot_time = 0;
			int robot = it->first;
			vector<int> spaces = it->second;
			if (spaces.size() == 0) {
				continue;
			}

			// the robot chooses the smallest distance to go first
			int min_time = INT_MAX;
			int min_index = -1;
			for (int i = 0; i < spaces.size(); i++) {
				int dist = dist_robots_spaces[robot][spaces[i]];

				if (min_time > dist) {
					min_time = dist;
					min_index = i;
				}
			}
			// next starting point of the robot is the prev index he cleaned
			int prev_index = spaces[min_index];
			robot_time = robot_time + min_time;

			// the robot cleaned the min_index space
			spaces.erase(spaces.begin() + min_index);

			// the robot need to visit all the assigned spaces
			while (spaces.size() > 0) {
				min_time = INT_MAX;
				min_index = -1;

				// choose the smallest distance from the prev_index
				for (int i = 0; i < spaces.size(); i++) {
					int dist = dist_to_clean[prev_index][spaces[i]];
					if (min_time > dist) {
						min_time = dist;
						min_index = i;
					}
				}
				// next starting point
				prev_index = spaces[min_index];
				robot_time = robot_time + min_time;
				spaces.erase(spaces.begin() + min_index);
			}

			// check the final time
			total_time = std::max(total_time, robot_time);
		}
		return total_time;
	}

	/* Genereate all the posibilities of how dirty spaces could be cleaned
	partition [i] = R <=> robot R cleans space i
	S1     S2     S3    S4
	______________________
	R1     R1     R1    R1   -> meaning R1 is cleaning all dirty spaces
	R1     R1     R1    R2
	R1     R1     R1    R3 ... */
	void back(int k, int r, int s, std::vector<int> partition,
					std::vector<std::vector<int>> &all_partitions) {
		if (k == s) {
			all_partitions.push_back(partition);
			return;
		}
		for (int i = 0; i < r; i++) {
			partition[k] = i;
			back(k + 1, r, s, partition, all_partitions);
		}
	}

	int final_time() {
		std::vector<int> partition(this->s, 0);
		std::vector<std::vector<int>> all_partitions;
		back(0, r, s, partition, all_partitions);
		int min_time = INT_MAX;

		// the final time is the minimum time from all partitions
		for (int i = 0; i < all_partitions.size(); i++) {
			int local_time = check_partition(all_partitions[i]);
			min_time = std::min(min_time, local_time);
		}
		return min_time;
	}
};

int main() {
    std::ofstream out;
    out.open("./curatare.out");

    auto* solving = new Solve();
    if (!solving) {
        std::cerr << "Fail!\n";
        return -1;
    }
    solving->read();
	solving->init_distances();
	solving->compute_all_dist();
	out << solving->final_time();

    delete solving;
    return 0;
}
