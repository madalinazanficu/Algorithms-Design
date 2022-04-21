#include<bits/stdc++.h>

int n = 0;
int k = 0;
int maximTarget = 0;
std::vector<int> targetList;
std::vector<int> points;

std::vector<int> divisors(int n) {
	std::vector<int> divs;
	divs.push_back(1);
	int sqr = sqrt(n);
	for (int i = 2; i <= sqr; i++) {
		if (n % i == 0) {
			divs.push_back(i);
			divs.push_back(n / i);
		}
	}
	divs.push_back(n);
	return divs;
}

/* Precompute the number of steps necessary for every target
   Maintain a queue of elements such as: (1, 0) (2, 1) (3, 2) ... (first, second):
   first = target
   second = number of steps to obtain the target */
std::vector<int> computeOperations() {
	std::vector<int> steps(maximTarget + 1, maximTarget + 2);
	steps[1] = 0;
	steps[2] = 1;

	std::queue<std::pair<int, int>> q;
	std::pair<int, int> new_pair;
	new_pair.first = 2;
	new_pair.second = 1;
	q.push(new_pair);

	while(!q.empty()) {
		std::pair<int, int> front = q.front();
		q.pop();
		int target = front.first;
		int freq = front.second;

		/* Generate the next targets based on divisors */
		std::vector<int> divs = divisors(target);
		for (int div : divs) {
			int nextTarget = target + div;
			int nextFreq = freq + 1;

			/* The number of steps computed now is smaller => better computation */
			if (nextTarget <= maximTarget && nextFreq < steps[nextTarget]) {
				std::pair<int, int> new_pair;
				new_pair.first = nextTarget;
				new_pair.second = nextFreq;

				/* Continue to generate nextTargets based on
				   the smallest computational ones */
				q.push(new_pair);
				steps[nextTarget] = nextFreq;
			}
		}
	}
	return steps;
}


/* The index of the line represents the index of the target
   The index of the column represents the number of steps used
   dp[i][j] = total points considering or not target i, using at most j steps */
int DP() {
	std::vector<int> steps = computeOperations();
	int sum = 0;
	int all_points = 0;
	for (int i = 0; i < n; i++) {
		sum += steps[targetList[i]];
		all_points += points[i];
	}
	/* In case all the steps could be done => return all_points */
	if (sum <= k) {
		return all_points;
	}

	std::vector<std::vector<int>> dp(n + 1, std::vector<int>(k + 1, 0));
	for (int i = 1; i <= n; i++) {
		int idx = i - 1;
		int necessarySteps = steps[targetList[idx]];
		for (int j = 0; j <= k; j++) {
			int remainingSteps = j - necessarySteps;
			/* At the current stage, it's possible to make necessarySteps */
			if (necessarySteps <= j) {
				dp[i][j] = std::max(dp[idx][j], points[idx] + dp[idx][remainingSteps]);
			} else {
				/* Keep the previous result */
				dp[i][j] = dp[idx][j];
			}
		}
	}
	return dp[n][k];
}

int main() {
	std::ifstream in;
	in.open("./prinel.in");

	std::ofstream out;
	out.open("./prinel.out");

	in >> n >> k;
	int x;
	for (int i = 0; i < n; i++) {
		in >> x;
		targetList.push_back(x);
		if (x > maximTarget) {
			maximTarget = x;
		}
	}
	for (int i = 0; i < n; i++) {
		in >> x;
		points.push_back(x);
	}
	out << DP();

	in.close();
	out.close();
	return 0;
}
