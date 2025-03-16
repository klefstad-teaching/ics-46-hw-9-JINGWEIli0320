#include "ladder.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <vector>
#include <string>
#include <algorithm>


void error(string word1, string word2, string msg) {
    std::cerr << "Error: " << word1 << " -> " << word2 << ": " << msg << std::endl;
}


bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    int n = str1.size(), m = str2.size();
    if (abs(n - m) > d)
        return false;
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
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
                dp[i][j] = 1 + std::min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
        }
        int rowMin = *std::min_element(dp[i].begin(), dp[i].end());
        if (rowMin > d)
            return false;
    }
    return dp[n][m] <= d;
}


bool is_adjacent(const std::string& word1, const std::string& word2) {
    return edit_distance_within(word1, word2, 1);
}


vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {

    if (begin_word == end_word)
        return {};


    set<string> words = word_list;
    words.insert(begin_word);
    words.insert(end_word);

    queue<vector<string>> q;
    set<string> visited;
    q.push({ begin_word });
    visited.insert(begin_word);

    while (!q.empty()) {
        int levelSize = q.size();
        set<string> levelVisited;
        for (int i = 0; i < levelSize; i++) {
            vector<string> path = q.front();
            q.pop();
            string last = path.back();
            if (last == end_word) {
                return path;
            }

            for (const auto& word : words) {
                if (visited.find(word) == visited.end() && is_adjacent(last, word)) {
                    vector<string> new_path = path;
                    new_path.push_back(word);
                    levelVisited.insert(word);
                    q.push(new_path);
                }
            }
        }

        for (const auto& w : levelVisited) {
            visited.insert(w);
        }
    }
    return {}; 
}


void load_words(set<string>& word_list, const string& file_name) {
    std::ifstream in(file_name);
    if (!in) {
        std::cerr << "Error: Unable to open file " << file_name << std::endl;
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
        std::cout << "No word ladder found." << std::endl;
        return;
    }
    for (size_t i = 0; i < ladder.size(); i++) {
        std::cout << ladder[i];
        if (i != ladder.size() - 1)
            std::cout << " -> ";
    }
    std::cout << std::endl;
}


void verify_word_ladder() {
    std::cout << "Enter the begin word: ";
    string begin;
    std::cin >> begin;
    std::cout << "Enter the end word: ";
    string end;
    std::cin >> end;
    std::cout << "Enter the dictionary file name: ";
    string filename;
    std::cin >> filename;
    
    set<string> word_list;
    load_words(word_list, filename);
    
    vector<string> ladder = generate_word_ladder(begin, end, word_list);
    if (ladder.empty()) {
        error(begin, end, "No word ladder found");
    } else {
        print_word_ladder(ladder);
    }
}
