#include <bits/stdc++.h>
using namespace std;
vector<vector<int>> all_partitions;

void back(int k, int r, int s, vector<int> partition) {
    if (k == s) {
        all_partitions.push_back(partition);
        return;
    }
    for (int i = 1; i <= r; i++) {
        partition[k] = i;
        back(k + 1, r, s, partition);
    }
}
int main() {
    int r, s;
    cin >> r >> s;
    vector<int> partition(s, 0);
    back(0, r, s, partition);

    for (int i = 0; i < all_partitions.size(); i++) {
        for (int j = 0; j < all_partitions[i].size(); j++) {
            cout << all_partitions[i][j] << " ";
        }
        cout << endl;
    }
}
