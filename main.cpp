#include <vector>
#include <string>
#include <set>
#include <map>
#include <iostream>
#include <unordered_set>
#define int long long

using namespace std;

const int MOD = 1000000007;
const int P = 100000007;
const int MAX_HEIGHT = 4;
const int COLORS = 37;
map<string, int> str_to_int;

int hashed(const vector<vector<int>>& state) {
    int res = 0;
    for (int i = 0; i < state.size(); ++i) {
        int num = 0;
        for (int j = 0; j < state[i].size(); ++j) {
            num *= COLORS;
            num += state[i][j] + 1;
        }
        res *= P;
        res += num + 1;
        res %= MOD;
    }
    return res;
}

unordered_set<int> used;
vector<pair<int, int>> path;

bool ok_for_transit(const vector<int>& bottle) {
    for (int i = 1; i < bottle.size(); ++i) {
        if (bottle[i] != bottle[0]) return true;
    }
    return bottle.size() < MAX_HEIGHT;
}

int make_transition(vector<int>& from, vector<int>& to, bool modify) {
    int cnt = 0;
    vector<int> from_copy = from;
    vector<int> to_copy = to;
    while (from_copy.size() > 0 && to_copy.size() < MAX_HEIGHT && (to_copy.size() == 0 || from_copy.back() == to_copy.back())) {
        int x = from_copy.back();
        from_copy.pop_back();
        to_copy.push_back(x);
        ++cnt;
    }
    if (modify) {
        from = from_copy;
        to = to_copy;
    }
    return cnt;
}

void transit_cnt(vector<int>& from, vector<int>& to, int cnt) {
    for (int i = 0; i < cnt; ++i) {
        int x = from.back();
        from.pop_back();
        to.push_back(x);
    }
}

bool check_win(const vector<vector<int>>& state) {
    for (int i = 0; i < state.size(); ++i) {
        for (int x : state[i]) {
            if (x != state[i][0]) return false;
        }
    }
    set<int> tops;
    for (int i = 0; i < state.size(); ++i) {
        if (state[i].empty()) continue;
        if (tops.count(state[i][0])) return false;
        tops.insert(state[i][0]);
    }
    return true;
}

bool go(vector<vector<int>>& state) {
    int hashed_state = hashed(state);
    if (used.count(hashed(state)) != 0) return false;
    used.insert(hashed_state);
    if (used.size() % 1000 == 0) {
        cout << "Processed " << used.size() << " states, still searching..." << endl;
    }
    if (hashed_state == 111267834) {
        int kek = 0;
    }
    bool win = check_win(state);
    if (win) {
        return true;
    }
    for (int i = 0; i < state.size(); ++i) {
        for (int j = 0; j < state.size(); ++j) {
            if (i == j || !ok_for_transit(state[i])) continue;
            if (make_transition(state[i], state[j], false) > 0) {
                int moved = make_transition(state[i], state[j], true);
                path.emplace_back(i, j);
                bool win = go(state);
                if (win) {
                    return true;
                }
                transit_cnt(state[j], state[i], moved);
                path.pop_back();
            }
        }
    }
    return false;
}


int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    long long n;
    cin >> n;
    vector<vector<int>> starting_state;
    for (int i = 0; i < n; ++i) {
        starting_state.emplace_back();
        string x;
        while (true) {
            cin >> x;
            if (x == "end") break;
            if (str_to_int.count(x) == 0) {
                str_to_int[x] = str_to_int.size() + 1;
            }
            starting_state.back().push_back(str_to_int[x]);
        }
    }
    go(starting_state);
    if (path.size() == 0) {
        cout << "Not found\n";
        return 0;
    }
    cout << "Found!\n";
    for (const auto& kek : path) {
        cout << (long long) kek.first + 1 << ' ' << (long long) kek.second + 1 << '\n';
    }

}
