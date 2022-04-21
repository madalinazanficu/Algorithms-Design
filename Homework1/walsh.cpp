#include <fstream>
#include <iostream>
#include <vector>

void query(int n, int x, int y, int &result) {
    if (n / 2 == 0) {
        return;
    }
    // Area 1 =>  (x <= n / 2 &&  y <= n / 2) => do not modify x and y
    // Area 2 => modify only y
    if (x <= n / 2 && y > n / 2) {
        y = y - n / 2;
    }
    // Area 3 => only modfify x
    if (x > n / 2 && y <= n / 2) {
        x = x - n / 2;
    }
    // Area 4 => modify x and y
    if (x > n / 2 && y > n / 2) {
        x = x - n / 2;
        y = y - n / 2;

        // Inverse the result
        if (result == 1) {
            result = 0;
        } else {
            result = 1;
        }
    }
    query(n / 2, x, y, result);
}

int main() {
    std::ifstream in;
    in.open("./walsh.in");

    std::ofstream out;
    out.open("./walsh.out");

    int n, k;
    in >> n >> k;
    for (int i = 0; i < k; i++) {
        int x, y;

        in >> x >> y;
        int result = 0;
        query(n, x, y, result);
        out << result << std::endl;
    }
    in.close();
    out.close();
    return 0;
}
