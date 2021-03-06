### Algorithms Design Homework
### Zanficu Madalina 323CA

#### **walsh.cpp** - Divide et Impera
- Having the initial coordinates (x, y) from the matrix,
  the main idea is to recursively find in which area (x, y)
  are positionated.
-  [A1, A2;
    A3, A4]
- In case it's area A1, A2, A3 there will not be applied any negation.
- For area A4, negation will be applied.
- For example: x = 1, y = 4 (indexed from 1)
[0, 0, 0, 0;
 0, 1, 0, 1;
 0, 0, 1, 1;
 0, 1, 1, 0]
- base case = 0                                     
- 1st area found is: 2 => no negation will be applied on the base case    => 0
- 2nd area found is: 4 => negation will be applied on the previous result => 1
- Result = 1;
- **Time Complexity  O(logN)**

#### **Statistics.java** - Greedy
The main idea is considering every letter of the alphabet,
and choosing the best solution based on the maximum number of words used.
1. By turn consider every letter a target and:
  - Count the total occurrences of the target in all the words => totalOcc.
  - Sum the lengths of all words => totalLen.

2. For every word:
  - Count the total occurrences of the target char => targetOcc.
  - Count the occurrences of chars different with the target => othersOcc.

- Stock pairs (targetOcc, othersOcc) in an ArrayList and sort it
  in order to find the words with the biggest difference between
  targetOcc - othersOcc.

- **The best solution** would be to keep **all the words** given with
  the condition that: **totalOcc / 2 > totalLen**.
- In case the condition is not accomplished, consider eliminating
  the words with the biggest negative sign (smallest ones,
  considering the frequencies vector), this way the proportion of
  target char is smaller.
- **Time Complexity** O(32 * (N + NlogN + N)) => **O(32 * NlogN)**


#### **prinel.cpp** - Dynamic Programming
The flow of the implementation in divided in:
1.  Compute Operations
    - Computing the minimum number of steps necessary
      for completing all the targets.
    - All the steps (from 1 to maximTarget) are computed at the same time
      (only one call on this function).
    - Used a queue which contains pairs (target, stepsTarget) in order to
      compute next target based on the current one and its divisors.
    - **Time Complexity: O(maximTarget * divs.size())**
    - Computing at most maximTarget => **maximTarget steps**
    - For every target iterate in divs to generate the next target
    - => **divs.size() steps**

2. Computing DP matrix:
    - dp[i][j] maximum points using or not using i-th target, 
      but consuming at most j steps
    - i = index of the target that needs to be completed, i -> [0, n)
    - j = the number of steps used, j -> [0, k]
    - Every target has necessarySteps in order to be completed and
      remainingSteps for other actions
    - At every j, the necessarySteps of the current target is checked
    - Case 0: There are enough steps => consider and not consider this target,
      and take the best option => maximum
    - Case 1: Not enough steps => getting the result from the previous target.
    - **Time Complexity: O(N * K) completing the table**

#### **Crypto.java** - Dynamic Programming
- Source: https://www.geeksforgeeks.org/find-number-times-string-occurs-given-string/
- I got the formula analyzing the source site and taking multiple examples on paper.
- The main idea of computing dp table:
- dp[i][j - 1] = dp[i - 1][j] + dp[i - 1][j - 1]
- dp[i - 1][j] = prev result that include the current character in subseq
- dp[i - 1][j - 1] = prev result that include the entire subseq without the current character

- Everytime '?' the branches are multipling with uniqueChars times.
- Fill the dp using bottom-up manner.
- Case '?' => multiply with uniqueChars because the current string is splitting
  in uniqueChars branches => uniqueChars * dp[i - 1][j]
- Case j == 1  => multiply with the current number of branches because
  column 0 is the base case for 1 branch => dp[i - 1][j - 1] * branches.get(i - 1)
- **Time Complexity** O(N * L) + 2 * O(N) + O(L) => **O(N * L)**
