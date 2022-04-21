import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Scanner;


public class Statistics {

	private ArrayList<String> words = new ArrayList<>();
	private Integer n;
	private Integer maximWords = -1;

	public void read() throws FileNotFoundException {
		File in = new File("statistics.in");
		Scanner scanner = new Scanner(in);
		this.n = scanner.nextInt();
		for (int i = 0; i < this.n; i++) {
			String word = scanner.next();
			this.words.add(word);
		}
		scanner.close();
	}

	/**
	 * Iterating in the alphabet, every character is a target
	 * Iterate in the list of words and count total occurrences of the target.
	 * Every word will have an ArrayList entry where:
	 * - targetOCC =  occurrences of the target character in the current word
	 * - othersOcc =  apparitions of the others characters
	 * Sort the frequencies of the words in descending order depending on
	 * the difference between targetOcc and othersOcc and analyze the solution.
	 */
	public void greedy() {
		for (char ch = 'a'; ch <= 'z'; ch++) {
			ArrayList<ArrayList<Integer>> freq = new ArrayList<>();
			int totalOcc = 0;
			int totalLen = 0;

			for (String word: words) {
				ArrayList<Integer> entry = new ArrayList<>();
				char target = ch;
				int targetOcc = (int)(word.chars().filter(c -> c == target).count());
				int othersOcc = word.length() - targetOcc;

				entry.add(targetOcc);
				entry.add(othersOcc);

				totalOcc += targetOcc;
				totalLen += word.length();

				freq.add(entry);
			}
			Collections.sort(freq, new Comparator<ArrayList<Integer>>() {
				@Override
				public int compare(ArrayList<Integer> o1, ArrayList<Integer> o2) {
					return (o2.get(0) - o2.get(1)) - (o1.get(0) - o1.get(1));
				}
			});

			/* The best candidate for the result
				=> the number of all words is the result */
			if (totalOcc > totalLen / 2) {
				this.maximWords = words.size();
				break;
			}
			/* Erase from the back the worst choice of word */
			for (int i = freq.size() - 1; i >= 0; i--) {
				ArrayList<Integer> entry = freq.get(i);
				totalOcc = totalOcc - entry.get(0);
				totalLen = totalLen - (entry.get(1) + entry.get(0));

				/* Current suitable candidate for the result
				   But continue searching in the alphabet */
				if (totalOcc > totalLen / 2) {
					if (this.maximWords < i) {
						this.maximWords = i;
					}
					break;
				}
			}
		}
	}
	public void write() throws FileNotFoundException {
		PrintWriter printWriter = new PrintWriter("statistics.out");
		printWriter.print(maximWords);
		printWriter.close();;
	}

	public static void main(String[] args) throws FileNotFoundException {
		Statistics statistics = new Statistics();
		statistics.read();
		statistics.greedy();
		statistics.write();
	}

}
