#include <bits/stdc++.h>
#define int long long

using namespace std;

const int MOD = 1000000007;
const int P = 100000007;
const int MAX_HEIGHT = 4;
const int COLORS = 37; // max colors in game
const int INF = 1e18;

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

bool ok_for_transit(const vector<int>& bottle) { // check if we can move from bottle
    for (int i = 1; i < bottle.size(); ++i) {
        if (bottle[i] != bottle[0]) return true;
    }
    return bottle.size() < MAX_HEIGHT;
}

int make_transition(vector<int>& from, vector<int>& to, bool modify) { // returns how much liquid was moved
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

void transit_cnt(vector<int>& from, vector<int>& to, int cnt) { // moved cnt liquid from from to to
    for (int i = 0; i < cnt; ++i) {
        int x = from.back();
        from.pop_back();
        to.push_back(x);
    }
}

bool check_win(const vector<vector<int>>& state) { // check if state is a win
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

namespace Bruteforce {
    int cost(const vector<vector<int>>& state) { // cost function for bruteforce
        int res = 0;
        for (int i = 0; i < state.size(); ++i) {
            for (int j = 1; j < state[i].size(); ++j) {
                if (state[i][j] != state[i][j - 1]) {
                    ++res;
                }
            }
        }
        return res;
    }

    unordered_set<int> used;
    vector<pair<int, int>> path;

    bool go(vector<vector<int>> &state) { // bruteforce main routine
        int hashed_state = hashed(state);
        if (used.count(hashed(state)) != 0) return false;
        used.insert(hashed_state);
        if (used.size() % 1000 == 0) {
            cout << "Processed " << used.size() << " states, still searching" << endl;
        }
        bool win = check_win(state);
        if (win) {
            return true;
        }
        vector<pair<int, pair<int, int>>> to;
        for (int i = 0; i < state.size(); ++i) {
            for (int j = 0; j < state.size(); ++j) {
                if (i == j || !ok_for_transit(state[i])) continue;
                if (make_transition(state[i], state[j], false) > 0) {
                    int moved = make_transition(state[i], state[j], true);
                    int cur_cost = cost(state);
                    to.push_back({cur_cost, {i, j}});
                    transit_cnt(state[j], state[i], moved);
                }
            }
        }
        sort(to.begin(), to.end());
        reverse(to.begin(), to.end());
        for (int k = 0; k < to.size(); ++k) {
            int i = to[k].second.first;
            int j = to[k].second.second;
            int moved = make_transition(state[i], state[j], true);
            path.emplace_back(i, j);
            win = go(state);
            if (win) {
                return true;
            }
            transit_cnt(state[j], state[i], moved);
            path.pop_back();
        }
        return false;
    }

    vector<pair<int, int>> bruteforce(vector<vector<int>> state) {
        bool res = go(state);
        if (!res) {
            return {{-1, -1}};
        }
        return path;
    }
}

namespace A_star {
    int cost(const vector<vector<int>>& state) { // cost function for A*
        int res = 0;
        for (int i = 0; i < state.size(); ++i) {
            for (int j = 1; j < state[i].size(); ++j) {
                if (state[i][j] != state[i][j - 1]) {
                    ++res;
                }
            }
        }
        return res;
    }

    vector<pair<int, int>> a_star(vector<vector<int>> state) { // A* main routine
        set<pair<int, int>> q; // (dist from initial state + lower bound of dist to final state, hash of current state)
        unordered_map<int, int> dist;
        unordered_map<int, int> f;
        unordered_map<int, pair<int, pair<int, int>>> parent; // map from state hash to (prev state hash, (from bottle, to bottle))
        unordered_map<int, vector<vector<int>>> hash_to_state;
        int cur_hash = hashed(state);
        dist[cur_hash] = 0;
        f[cur_hash] = dist[cur_hash] + cost(state);
        parent[cur_hash] = {-1, {-1, -1}};
        hash_to_state[cur_hash] = state;
        q.insert({f[cur_hash], cur_hash});
        bool found = false;
        int iters = 0;
        while (q.size() > 0) {
            ++iters;
            if (iters % 1000 == 0) {
                cout << "Processed " << iters << " states" << endl;
            }
            cur_hash = q.begin()->second;
            state = hash_to_state[cur_hash];
            q.erase(q.begin());
            if (check_win(state)) {
                found = true;
                break;
            }
            for (int i = 0; i < state.size(); ++i) {
                for (int j = 0; j < state.size(); ++j) {
                    if (i == j || !ok_for_transit(state[i]) || make_transition(state[i], state[j], false) == 0)
                        continue;
                    vector<vector<int>> next_state = state;
                    make_transition(next_state[i], next_state[j], true);
                    int next_hash = hashed(next_state);
                    if (!hash_to_state.count(next_hash)) {
                        hash_to_state[next_hash] = next_state;
                        f[next_hash] = INF;
                        dist[next_hash] = INF;
                        parent[next_hash] = {-1, {-1, -1}};
                    }
                    dist[next_hash] = min(dist[next_hash], dist[cur_hash] + 1);
                    if (f[next_hash] > dist[next_hash] + cost(next_state)) {
                        q.erase({f[next_hash], next_hash});
                        f[next_hash] = dist[next_hash] + cost(next_state);
                        parent[next_hash] = {cur_hash, {i, j}};
                        q.insert({f[next_hash], next_hash});
                    }
                }
            }
        }
        if (!found) {
            return {{-1, -1}};
        }
        vector<pair<int, int>> ans;
        while (parent[cur_hash].first != -1) {
            ans.emplace_back(parent[cur_hash].second.first, parent[cur_hash].second.second);
            cur_hash = parent[cur_hash].first;
        }
        reverse(ans.begin(), ans.end());
        return ans;
    }
}


int32_t main() {
    long long n;
    cin >> n;
    vector<vector<int>> starting_state;
    map<string, int> str_to_int;
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
    vector<pair<int, int>> path;
    while (true) {
        cout << "What method do you want to use? (bruteforce/A*)" << endl;
        string method;
        cin >> method;
        if (method == "bruteforce") {
            path = Bruteforce::bruteforce(starting_state);
            break;
        } else if (method == "A*") {
            path = A_star::a_star(starting_state);
            break;
        } else {
            cout << "Unknown method" << endl;
        }
    }
    if (path.size() == 0) {
        cout << "Not found\n";
        return 0;
    }
    cout << "Found!\n";
    for (const auto& kek : path) {
        cout << kek.first + 1 << ' ' << kek.second + 1 << '\n';
    }

}