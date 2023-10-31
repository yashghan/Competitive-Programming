// Prefix tree: storing strings based on their common prefix
// Problem link: https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ffc7/00000000001d3ff3

#include <bits/stdc++.h>

using namespace std;

#define ar array
#define ll long long

const int MAX_N = 1e5 + 1;
const int MOD = 1e9 + 7;
const int INF = 1e9;
const ll LINF = 1e18;

const int MAX_S = 2e6; // total number of characters in all strings
const int MAX_C = 26; // total number of distinct characters 

int N, K, num, ans;
int trie[MAX_S][MAX_C], cnt[MAX_S];

// initially all values in trie are 0 (not allocated)
// cnt[u] = number of strings that end at node u in the trie => this is to mark the end of a string in the trie
// "num" stores the total number of allocated nodes in the trie

void dfs(int u = 0, int d = 0) {
    for (int v = 0; v < 26; v++) {
        if (trie[u][v]) {
            dfs(trie[u][v], d + 1);
            // now cnt[u] stores the total number of strings that has a common prefix from the root to node u
            cnt[u] += cnt[trie[u][v]];
        }
    }
    // greedily dividing into groups of K
    while (cnt[u] >= K) { 
        cnt[u] -= K;
        ans += d;
    }
}

// insert a string into the trie
void insert(string s) {
    // start at the root node
    int u = 0; 
    for (char c : s) {
        // if this node has not been allocated, then allocate the node and increase num by 1
        if (!trie[u][c - 'A']) trie[u][c - 'A'] = ++num; 
        // move to the next node
        u = trie[u][c - 'A']; 
    }
    // mark the end of string
    cnt[u]++; 
}

// check if a string exist in the trie
bool search(string s) { 
    int u = 0;
    for (char c : s) {
        if (!trie[u][c - 'A']) return false;
        u = trie[u][c - 'A'];
    }
    return cnt[u] > 0;
}

void solve() {
    // need to reset after each test case
    ans = 0; num = 0;
    memset(trie, 0, sizeof trie);
    memset(cnt, 0, sizeof cnt);

    cin >> N >> K;
    for (int i = 0; i < N; i++) {
        string s; cin >> s;
        insert(s);
    }
    dfs();
    cout << ans << "\n";
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    int tc; cin >> tc;
    for (int t = 1; t <= tc; t++) {
        cout << "Case #" << t  << ": ";
        solve();
    }
}
// ------------------------------------------------------------------------------------------------------------------
// Template - 2 for trie:
#include <bits/stdc++.h>
#define ll long long
#define pb push_back
#define all(x) (x).begin(), (x).end()

using namespace std;

class Trie {
private:
    struct Node {
        Node* child[26];
        ll prefixes;
        ll count;
    };

    Node* root;

public:
    Trie() {
        root = newNode();
    }

    Node* newNode() {
        Node* temp = new Node();
        for (int i = 0; i < 26; ++i)
            temp->child[i] = nullptr;
        temp->prefixes = temp->count = 0;
        return temp;
    }

    void insert(string s) {
        Node* curr = root;
        ll n = s.length();
        curr->prefixes++;
        for (ll i = 0; i < n; ++i) {
            if (curr->child[s[i] - 'A'] == nullptr) {
                Node* temp = newNode();
                curr->child[s[i] - 'A'] = temp;
            }
            curr = curr->child[s[i] - 'A'];
            curr->prefixes++;
        }
        curr->count++;
    }

    bool search(string s) {
        Node* curr = root;
        ll n = s.length();
        for (ll i = 0; i < n; ++i) {
            if (curr->child[s[i] - 'A'] == nullptr) {
                return false; // Character not found in the Trie
            }
            curr = curr->child[s[i] - 'A'];
        }
        return (curr->count > 0);
    }

    bool startsWith(string s) {
        Node* curr = root;
        ll n = s.length();
        for (ll i = 0; i < n; i++) {
            if (curr->child[s[i] - 'A'] == nullptr) {
                return false;
            }
            curr = curr->child[s[i] - 'A'];
        }
        return true;
    }

    ll dfs(ll k) {
	Node * u = root;
        return dfs(u, k, 0);
    }

    ll dfs(Node* u, ll k, ll depth) {
        if (u->prefixes < k)
            return 0;
        ll ans = 0;
        ll here = u->count;
        for (int i = 0; i < 26; ++i) {
            Node* v = u->child[i];
            if (v == nullptr)
                continue;
            ans += dfs(v, k, depth + 1);
            here += (v->prefixes % k);
        }
        ans += depth * (here / k);
        return ans;
    }

     bool dfs2(int r, int c, vector<vector<char>> &board, vector<string> &res, string &path){
        Node * curr = root;
        return dfs(r, c, board, res, curr, path);
    }

    bool dfs2(int r, int c, vector<vector<char>> &board, vector<string> &res, Node * curr, string &path){
        char letter = board[r][c];
        if (letter == '#') {
            return false;
        }
        
        curr = curr->child[letter - 'a'];
        if (curr == nullptr) {
            return false;
        }

        path.push_back(letter);
        board[r][c] = '#'; // Mark the cell as visited

        if (curr->count > 0) {
            res.push_back(path); // If a word is found in the Trie, add it to the result set
            curr->count--; // Mark it as visited in the Trie
        }

        // Explore adjacent cells
        int dr[] = {1, -1, 0, 0};
        int dc[] = {0, 0, 1, -1};
        
        for (int i = 0; i < 4; i++) {
            int newRow = r + dr[i];
            int newCol = c + dc[i];
            
            if (newRow >= 0 && newRow < board.size() && newCol >= 0 && newCol < board[0].size()) {
                dfs(newRow, newCol, board, res, curr, path);
            }
        }

        // Restore the cell to its original state
        board[r][c] = letter;
        path.pop_back();
        
        return true;
    }
    
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin >> t;
    for (int tc = 1; tc <= t; ++tc) {
        ll n, k;
        cin >> n >> k;
        Trie trie;
        for (ll i = 0; i < n; ++i) {
            string s;
            cin >> s;
            trie.insert(s);
        }
        ll ans = trie.dfs(k);
        cout << "Case #" << tc << ": " << ans << endl;
    }
}

