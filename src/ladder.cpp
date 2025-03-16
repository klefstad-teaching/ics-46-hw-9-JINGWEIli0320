#include "ladder.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <vector>
#include <string>
#include <algorithm>


void error(string word1, string word2, string msg) {
    cerr << "Error: " << word1 << " -> " << word2 << ": " << msg << endl;
}


bool edit_distance_within(const string& str1, const string& str2, int d) {
    int n = str1.size(), m = str2.size();
    if (abs(n - m) > d)
        return false;
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 0; i <= n; i++) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= m; j++) {
        dp[0][j] = j;
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (str1[i - 1] == str2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
        }
        int rowMin = *min_element(dp[i].begin(), dp[i].end());
        if (rowMin > d)
            return false;
    }
    return dp[n][m] <= d;
}


bool is_adjacent(const string& word1, const string& word2) {
    if (word1 == word2)
        return false;
    if (word1.size() != word2.size())
        return false;

    int diff = 0;
    for (size_t i = 0; i < word1.size(); i++) {
        if (word1[i] != word2[i])
            diff++;
        if (diff > 1)
            return false;
    }
    return diff == 1;
}


vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (begin_word == end_word)
        return { begin_word };

    queue<vector<string>> q;
    set<string> visited;
    q.push({ begin_word });
    visited.insert(begin_word);

    while (!q.empty()) {
        vector<string> path = q.front();
        q.pop();
        string last = path.back();

        for (const auto& word : word_list) {
            if (visited.find(word) == visited.end() && is_adjacent(last, word)) {
                vector<string> new_path = path;
                new_path.push_back(word);
                if (word == end_word)
                    return new_path;
                q.push(new_path);
                visited.insert(word);
            }
        }
    }
    return {}; 
}


void load_words(set<string>& word_list, const string& file_name) {
    ifstream in(file_name);
    if (!in) {
        cerr << "Error: Unable to open file " << file_name << endl;
        return;
    }
    string word;
    while (in >> word) {
        word_list.insert(word);
    }
    in.close();
}


void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i];
        if (i != ladder.size() - 1)
            cout << " -> ";
    }
    cout << endl;
}


void verify_word_ladder() {
    cout << "Enter the begin word: ";
    string begin;
    cin >> begin;
    cout << "Enter the end word: ";
    string end;
    cin >> end;
    cout << "Enter the dictionary file name: ";
    string filename;
    cin >> filename;
    
    set<string> word_list;
    load_words(word_list, filename);
    
    vector<string> ladder = generate_word_ladder(begin, end, word_list);
    if (ladder.empty()) {
        error(begin, end, "No word ladder found");
    } else {
        print_word_ladder(ladder);
    }
}
