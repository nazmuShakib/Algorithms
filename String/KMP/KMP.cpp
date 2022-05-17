#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

template<typename T>
vector<int> prefix_function(const T& pattern) {
	int m = int(pattern.size());
	// lps = longest prefix suffix
	std::vector<int> lps(m);
	for (int i = 1; i < m; i++) {
		int j = lps[i - 1];
		// search for longest prefix-suffix
		while (j > 0 && pattern[j] != pattern[i])
			j = lps[j - 1];
		if (pattern[i] == pattern[j]) ++j;
		lps[i] = j;
	}
	return lps;
}

template <typename T>
vector<int> KMP(const T& main, const T& pattern) {
	int n = int(main.size());
	int m = int(pattern.size());

	// compute the prefix array
	auto lps = prefix_function(pattern);
	int j = 0; // pattern index
	int i = 0; // main index

	vector<int> positions; // to store matched positions
	while (i < n) {
		if (main[i] == pattern[j])
			++j, ++i;
		// If the pattern index is equal to its length, then we have a match. So we include the start index of the matched pattern in our positions array and look for next longest proper prefix.
		if (j == m) {
			positions.emplace_back(i - m);
			j = lps[j - 1];
		}
		if (i < n && pattern[j] != main[i]) {
			if (j == 0) ++i; // we look for a suffix of length 1 in the main array
			else j = lps[j - 1]; // looking for a prefix that is also a suffix
		}
	}
	// 0-Indexed array
	return positions;
}
int32_t main()
{
	cin.tie(0)->sync_with_stdio(false);
	cout << setprecision(10) << fixed;

	int n, m;
	cin >> n >> m;
	vector<int> a(n);
	vector<int> b(m);
	for (int i = 0; i < n; ++i) cin >> a[i];
	for (int i = 0; i < m; ++i) cin >> b[i];
	auto v = KMP(a, b);
	for (int i = 0; i < (int) v.size(); ++i)
		cout << v[i] << " \n"[i + 1 == (int) v.size()];
	string main, pat;
	cin >> main >> pat;
	auto u = KMP(main, pat);
	for (int i = 0; i < (int) u.size(); ++i)
		cout << u[i] << " \n"[i + 1 == (int) u.size()];

	return 0;
}