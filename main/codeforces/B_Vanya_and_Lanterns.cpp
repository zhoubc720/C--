//#pragma GCC optimize(3)
#include <bits/stdc++.h>
//#define int LL
#define endl '\n'
#define size_t int
#define all(v) v.begin(), v.end()
using namespace std;
typedef long long LL;
typedef vector<int> vint;
typedef vector<vint> vvint;
typedef vector<string> vstr;
typedef pair<int, int> pii;
typedef vector<pii> vpii;

void solve()
{
    int N, T;
    cin >> N >> T;
    vector<double> numss(N);
    for (int i = 0; i < N; i++)
    {
        cin >> numss[i];
    }

    sort(numss.begin(), numss.end());

    double Max = numss[0];
    for (int i = 1; i < N - 1; i++)
    {
        Max = max(Max, (numss[i] - numss[i - 1]) / 2.00000000);
    }
    Max = max(T - numss[N - 1], Max);

    cout << Max << endl;
}
signed main()
{
    //cin.tie(0)->ios::sync_with_stdio(0);
    cout << setiosflags(ios::fixed) << setprecision(10);
    int T = 1;
    while (T--)
    {
        solve();
    }
    return 0;
}