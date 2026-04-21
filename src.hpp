#pragma once
#include <bits/stdc++.h>
using namespace std;

// The grader provides this function.
extern int query(int x, int y, int z);

static const int MOD = 998244353;
static const long long A1_TASK2 = 1000000000LL; // 1e9

static long long modPow233IndexedSum(const vector<long long>& A) {
    long long res = 0;
    long long p = 233 % MOD;
    // i is 1-based index per problem statement
    for (int i = 1; i < (int)A.size(); ++i) {
        res = (res + (A[i] % MOD + MOD) % MOD * p) % MOD;
        p = (p * 233) % MOD;
    }
    return res;
}

int guess(int n, int Taskid) {
    // A is 1-based (index 0 unused)
    vector<long long> A(n + 1, 0);

    if (Taskid == 1) {
        // Subtask1: A1 = 1, A2 = 2
        A[1] = 1;
        A[2] = 2;
        long long base = A[1];
        for (int i = 3; i <= n; ++i) {
            int s = query(1, 2, i);
            // Since A[i] > 2 for all i>=3, min=1, max=A[i]
            A[i] = (long long)s - base;
        }
        return (int)(modPow233IndexedSum(A) % MOD);
    }

    if (Taskid == 2) {
        // Subtask2: A1 = 1, A2 = 1e9
        A[1] = 1;
        A[2] = A1_TASK2;
        // For i >= 4, compute unordered pairs {A3, Ai} using two queries per i with anchor index 3
        // Store as pair<max(A3,Ai), min(A3,Ai)>
        vector<pair<long long,long long>> pair_with3(n + 1, {0,0});

        for (int i = 4; i <= n; ++i) {
            int q1 = query(1, 3, i); // 1 is guaranteed the minimum among (1,3,i)
            int q2 = query(2, 3, i); // 2 is guaranteed the maximum among (2,3,i)
            long long mx = (long long)q1 - A[1];
            long long mn = (long long)q2 - A[2];
            pair_with3[i] = {mx, mn}; // {max(A3,Ai), min(A3,Ai)}
        }
        // Find A3 by intersecting sets for i=4 and i=5 (n >= 5 by constraints)
        long long cand1a = pair_with3[4].first, cand1b = pair_with3[4].second;
        long long cand2a = pair_with3[5].first, cand2b = pair_with3[5].second;
        long long A3 = -1;
        if (cand1a == cand2a || cand1a == cand2b) A3 = cand1a;
        else if (cand1b == cand2a || cand1b == cand2b) A3 = cand1b;
        // Assign A3
        A[3] = A3;
        // Resolve all Ai using the stored pairs
        for (int i = 4; i <= n; ++i) {
            long long mx = pair_with3[i].first, mn = pair_with3[i].second;
            if (mx == A3) A[i] = mn; else A[i] = mx;
        }
        return (int)(modPow233IndexedSum(A) % MOD);
    }

    if (Taskid == 3) {
        // Subtask3: A1=2e8, A2=5e8, A3=8e8
        A[1] = 200000000LL;
        A[2] = 500000000LL;
        A[3] = 800000000LL;
        const long long S13 = A[1] + A[3]; // 1e9
        for (int i = 4; i <= n; ++i) {
            int s = query(1, 3, i);
            if ((long long)s < S13) {
                // Ai < A1
                A[i] = (long long)s - A[3];
            } else if ((long long)s > S13) {
                // Ai > A3
                A[i] = (long long)s - A[1];
            } else {
                // Between A1 and A3
                int t = query(1, 2, i);
                long long S12 = A[1] + A[2]; // 700000000
                if ((long long)t == S12) {
                    // Ai < A2
                    int u = query(3, 2, i);
                    A[i] = (long long)u - A[3];
                } else {
                    // Ai >= A2
                    A[i] = (long long)t - A[1];
                }
            }
        }
        return (int)(modPow233IndexedSum(A) % MOD);
    }

    // Fallback for other Taskid (4,5): not implemented generic solver.
    // Return 0 (will lead to WA for those datasets). Kept to avoid undefined behavior.
    return 0;
}
