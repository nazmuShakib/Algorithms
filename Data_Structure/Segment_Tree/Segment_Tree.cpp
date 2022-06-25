#include <iostream>
#include <vector>
#include <iomanip>
#include <cassert>
#include <algorithm>

using namespace std;

template<typename T>
class segTree {
public:
	struct point {
		int value = 0;
		template <typename M>
		void apply(int which, int l, int r, const M& val) {
			if (which & 1) {
				value += val;
				lazy += val;
			}
			else {
				value = val;
				assign = val;
				put = true;
				lazy = 0;
			}
		}
		int lazy = 0;
		int assign = 0;
		bool put = false;
	};
private:
	int n;
	vector<point> tree;
	inline point merge(const point& p1, const point& p2) {
		point res;
		res.value = max(p1.value, p2.value);
		return res;
	}
	inline void push(int x, int l, int r) {
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		if (tree[x].put) {
			tree[x + 1].apply(0, l, y, tree[x].assign);
			tree[z].apply(0, y + 1, r, tree[x].assign);
			tree[x].put = false;
		}
		if (tree[x].lazy != 0) {
			tree[x + 1].apply(1, l, y, tree[x].lazy);
			tree[z].apply(1, y + 1, r, tree[x].lazy);
			tree[x].lazy = 0;
		}
	}
	inline void pull(int x, int z) {
		tree[x] = merge(tree[x + 1], tree[z]);
	}
	void build(int x, int l, int r, const vector<T>& v) {
		if (l == r) {
			tree[x].apply(0, l, r, v[l]);
			return;
		}
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		build(x + 1, l, y, v);
		build(z, y + 1, r, v);
		pull(x, z);
	}
	template<int which, typename M>
	void modify(int x, int l, int r, int ll, int rr, const M& v)
	{
		if (ll <= l && r <= rr) {
			tree[x].apply(which, l, r, v);
			return;
		}
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		push(x, l, r);
		if (ll <= y) {
			modify<which>(x + 1, l, y, ll, rr, v);
		}
		if (rr > y) {
			modify<which>(z, y + 1, r, ll, rr, v);
		}
		pull(x, z);
	}
	point query(int x, int l, int r, int ll, int rr) {
		if (ll <= l && r <= rr) {
			return tree[x];
		}
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		push(x, l, r);
		point res {};
		if (rr <= y) {
			res = query(x + 1, l, y, ll, rr);
		}
		else {
			if (ll > y) {
				res = query(z, y + 1, r, ll, rr);
			}
			else {
				res = merge(query(x + 1, l, y, ll, rr), query(z, y + 1, r, ll, rr));
			}
		}
		pull(x, z);
		return res;
	}
	int find_first_knowingly(int x, int l, int r, const function<bool(const point&)>& f) {
		if (l == r) {
			return l;
		}
		push(x, l, r);
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		int res;
		if (f(tree[x + 1])) {
			res = find_first_knowingly(x + 1, l, y, f);
		}
		else {
			res = find_first_knowingly(z, y + 1, r, f);
		}
		pull(x, z);
		return res;
	}

	int find_first(int x, int l, int r, int ll, int rr, const function<bool(const point&)>& f) {
		if (ll <= l && r <= rr) {
			if (!f(tree[x])) {
				return -1;
			}
			return find_first_knowingly(x, l, r, f);
		}
		push(x, l, r);
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		int res = -1;
		if (ll <= y) {
			res = find_first(x + 1, l, y, ll, rr, f);
		}
		if (rr > y && res == -1) {
			res = find_first(z, y + 1, r, ll, rr, f);
		}
		pull(x, z);
		return res;
	}

	int find_last_knowingly(int x, int l, int r, const function<bool(const point&)>& f) {
		if (l == r) {
			return l;
		}
		push(x, l, r);
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		int res;
		if (f(tree[z])) {
			res = find_last_knowingly(z, y + 1, r, f);
		}
		else {
			res = find_last_knowingly(x + 1, l, y, f);
		}
		pull(x, z);
		return res;
	}

	int find_last(int x, int l, int r, int ll, int rr, const function<bool(const point&)>& f) {
		if (ll <= l && r <= rr) {
			if (!f(tree[x])) {
				return -1;
			}
			return find_last_knowingly(x, l, r, f);
		}
		push(x, l, r);
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		int res = -1;
		if (rr > y) {
			res = find_last(z, y + 1, r, ll, rr, f);
		}
		if (ll <= y && res == -1) {
			res = find_last(x + 1, l, y, ll, rr, f);
		}
		pull(x, z);
		return res;
	}
public:
	segTree(int _n) : n(_n) {
		tree.resize(2 * n - 1);
	}
	void build(const vector<T>& v) {
		build(0, 0, n - 1, v);
	}
	point query(int ll, int rr) {
		assert(0 <= ll && ll <= rr && rr <= n - 1);
		return query(0, 0, n - 1, ll, rr);
	}

	point query(int p) {
		assert(0 <= p && p <= n - 1);
		return query(0, 0, n - 1, p, p);
	}

	template <int which, typename M>
	void modify(int ll, int rr, const M v) {
		assert(0 <= ll && ll <= rr && rr <= n - 1);
		modify<which>(0, 0, n - 1, ll, rr, v);
	}
	int find_first(int ll, int rr, const function<bool(const point&)>& f) {
		assert(0 <= ll && ll <= rr && rr <= n - 1);
		return find_first(0, 0, n - 1, ll, rr, f);
	}

	int find_last(int ll, int rr, const function<bool(const point&)>& f) {
		assert(0 <= ll && ll <= rr && rr <= n - 1);
		return find_last(0, 0, n - 1, ll, rr, f);
	}
};

int32_t main()
{
	cin.tie(0)->sync_with_stdio(false);
	cout << setprecision(10) << fixed;

	int n;
	cin >> n;
	vector<int> a(n);
	for (int i = 0; i < n; ++i)
		cin >> a[i];
	segTree<int> tree(n);
	tree.build(a);
	cout << tree.query(0, 3).value << '\n';
	tree.modify<2>(0, 2, 2);
	cout << tree.query(0, 3).value << '\n';
	tree.modify<2>(1, 3, 1);
	cout << tree.query(0, 3).value << '\n';
	return 0;
}