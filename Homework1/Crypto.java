import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Scanner;

public class Crypto {
	private Integer n;
	private Integer l;
	private String key;
	private String s;
	private Long result;
	private Integer uniqueChars = 0;
	private Integer modulo = (int)Math.pow(10, 9) + 7;
	private ArrayList<Long> branches = new ArrayList<>();


	public  void read() throws FileNotFoundException {
		File input = new File("crypto.in");
		Scanner scanner = new Scanner(input);
		this.n = scanner.nextInt();
		this.l = scanner.nextInt();
		this.key = scanner.next();
		this.s = scanner.next();
		scanner.close();
	}
	public void write() throws FileNotFoundException {
		PrintWriter printWriter = new PrintWriter("crypto.out");
		printWriter.print(result);
		printWriter.close();
	}

	/* For every '?' character, the current string splits in q others strings,
	   q = number of different characters from s.*/
	public void computeBrances() {
		long branch = 1;
		this.branches.add(branch);
		this.uniqueChars = (int) s.chars().distinct().count();
		for (int i = 0; i < key.length(); i++) {
			if (key.charAt(i) == '?') {
				branch = (branch * uniqueChars) % modulo;
			}
			this.branches.add(branch);
		}
	}
	/*  Obs1. uniqueChars * dp[i - 1][j] => the previous result is multiplied
			because at every "?", the number of brances multiply with uniqueChars

		Obs2. dp[i - 1][j - 1] * branches.get(i - 1) => this is happening for
			the first column, because dp[i][0] maintain the result for 1 branch
			and branches.get(i - 1) is the current number of brances */
	public void solve() {
		long [][]dp = new long[n + 1][l + 1];

		/* Consider the first line such as the key has 0 chars => 0 matches. */
		for (int j = 0; j <= this.l; j++) {
			dp[0][j] = 0;
		}

		/* Consider the first column such as the sub-string has 0 chars => 1 match*/
		for (int i = 0; i <= this.n; i++) {
			dp[i][0] = 1;
		}
		for (int i = 1; i <= this.n; i++) {
			for (int j = 1; j <= this.l; j++) {
				if (key.charAt(i - 1) == '?') {
					if (j == 1) {
						dp[i][j] = ((uniqueChars * dp[i - 1][j]) % modulo
									+ this.branches.get(i - 1) * dp[i - 1][j - 1]) % modulo;
					} else {
						dp[i][j] = ((uniqueChars * dp[i - 1][j]) % modulo
									+ dp[i - 1][j - 1]) % modulo;
					}
				} else {
					if (key.charAt(i - 1) == s.charAt(j - 1)) {
						if (j == 1) {
							dp[i][j] = (dp[i - 1][j] + (this.branches.get(i - 1) 
										* dp[i - 1][j - 1]) % modulo) % modulo;
						} else {
							dp[i][j] = (dp[i - 1][j] + dp[i - 1][j - 1]) % modulo;
						}
					} else {
						/* Keep the previous result */
						dp[i][j] = dp[i - 1][j];
					}
				}
			}
		}
		this.result = dp[n][l];
	}

	public static void main(String[] args) throws FileNotFoundException {
		Crypto crypto = new Crypto();
		crypto.read();
		crypto.computeBrances();
		crypto.solve();
		crypto.write();
	}
}
