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

#include<bits/stdc++.h>
#define ll long long
#define pb push_back
#define all(x) (x).begin(), (x).end()

using namespace std;

struct node {
    node * child[26];
    // no of words having prefix formed till this node
    ll prefixes;
    // no of words ending at this node
    ll count;
};

node * newNode() {
    node * temp = new node();
    for(int i=0; i<26; ++i)
        temp->child[i] = NULL;
    temp->prefixes = temp->count = 0;
    return temp;
}

// insert string s into the trie
void insert(node * root, string s) {
    node * curr = root;
    ll n = s.length();
    curr->prefixes ++;
    for(ll i=0; i<n; ++i) {
        if(curr->child[s[i]-'A'] == NULL) {
            node * temp = newNode();
            curr->child[s[i]-'A'] = temp;
        }
        curr = curr->child[s[i]-'A'];
        // curr will be a prefix of s
        curr->prefixes ++;
    }
    // string ends at curr
    curr->count++;
}

bool search(node * root, string s) {
    node * curr = root;
    ll n = s.length();
    for (ll i = 0; i < n; ++i) {
        if (curr->child[s[i] - 'A'] == NULL) {
            return false; // Character not found in the Trie
        }
        curr = curr->child[s[i] - 'A'];
    }
    // If we reached the end of the string and the count at the last node is greater than 0, it means the string is in the Trie
    return (curr->count > 0);
}

bool startsWith(node * root, strings s){
	node * curr = root;
    ll n = s.length();
    for(ll i = 0; i < n; i++){
        if(curr->child[s[i] - 'A'] == NULL){
         	return false;  	
        }
        curr = curr->child[s[i] - 'A'];
    }
    return true;
}

ll dfs(node * u, ll k, ll depth) {
    // if cannot group into sets of k words
    if(u->prefixes < k)
        return 0;
    ll ans = 0;
    // here = no of strings ending at this node
    ll here = u->count;
    for(int i=0; i<26; ++i) {
        // v is child node
        node * v = u->child[i];
        if(v == NULL)
            continue;
        // add child ans
        ans += dfs(v, k, depth + 1);
        // add child residues to 'here'
        here += (v->prefixes % k);
    }
    // finally, ans = child_ans + sum of scores of groups formed by strings counted by 'here'
    ans += depth * (here / k);
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin>>t;
    for(int tc=1; tc<=t; ++tc) {
        ll n, k;
        cin>>n>>k;
        node * trie = newNode();
        for(ll i=0; i<n; ++i) {
            string s;
            cin>>s;
            insert(trie, s);
        }
        ll ans = dfs(trie, k, 0);
        cout<<"Case #"<<tc<<": "<<ans<<endl;
    }    
}
