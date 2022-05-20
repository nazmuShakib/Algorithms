#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

template <typename T>
vector<int> suffixArray(const T& s) {
	int  n = (int) s.size();
	vector<int> order(n), classes(n);
	iota(begin(order), end(order), 0);
	sort(begin(order), end(order), [&s] (int i, int j) {
		return s[i] < s[j]; // initial order including only one element
	});
	// setting classes
	classes[order[0]] = 0;
	for (int i = 1; i < n; ++i) {
		classes[order[i]] = classes[order[i - 1]] + (s[order[i]] != s[order[i - 1]]);
	}
	int k = 0;
	while ((1 << k) < n) {
		// going (1 << k) elements behind, first (1 << k) elements are not sorted but last (1 << k) are sorted
		for (int i = 0; i < n; ++i) {
			order[i] = (order[i] - (1 << k) + n) % n;
		}
		// count sort starts
		vector<int> cnt(n);
		for (int x : classes) cnt[x]++;
		vector<int> newOrder(n);
		for (int i = 1; i < n; ++i)
			cnt[i] += cnt[i - 1];
		for (int i = n - 1; i >= 0; --i) {
			int x = order[i];
			int id = classes[x];
			newOrder[cnt[id] - 1] = x;
			cnt[id]--;
		}
		// count sort ends
		swap(order, newOrder); // the new order after sorting
		vector<int> newClasses(n);
		// setting new classes
		newClasses[order[0]] = 0;
		for (int i = 1; i < n; ++i) {
			pair<int, int> prev = {classes[order[i - 1]], classes[(order[i - 1] + (1 << k)) % n]};
			pair<int, int> now = {classes[order[i]], classes[(order[i] + (1 << k)) % n]};
			if (prev == now) {
				newClasses[order[i]] = newClasses[order[i - 1]];
			}
			else {
				newClasses[order[i]] = newClasses[order[i - 1]] + 1;
			}
		}
		swap(classes, newClasses);
		++k;
	}
	// 0 based index
	return order;
}

int32_t main()
{
	cin.tie(0)->sync_with_stdio(false);
	cout << setprecision(10) << fixed;

	int n;
	cin >> n;
	vector<int> a(n);
	for (int& x : a) cin >> x;
	string str;
	cin >> str;
	str += '$';
	auto v = suffixArray(str);
	for (int i = 0; i < (int) v.size(); ++i) {
		cout << v[i] << " \n"[i + 1 == (int) v.size()];
	}
	a.emplace_back(INT32_MIN);
	auto u = suffixArray(a);
	for (int i = 0; i < (int) u.size(); ++i) {
		cout << u[i] << " \n"[i + 1 == (int) u.size()];
	}
	return 0;
}