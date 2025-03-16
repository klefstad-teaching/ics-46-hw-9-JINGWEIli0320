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


vector<string> generate_word_ladder(const string& begin_word,
    const string& end_word,
    const set<string>& word_list) 
{

  if (begin_word == end_word) {
  error(begin_word, end_word, "Start and end words are the same!");
  return {};
}

  if (word_list.find(end_word) == word_list.end()) {
  error(begin_word, end_word, "End word is not in the dictionary!");
  return {};
}

queue<vector<string>> q;
q.push({ begin_word });


set<string> visited;
visited.insert(begin_word);


const int MAX_LADDER_LENGTH = 20;

while (!q.empty()) {
vector<string> ladder = q.front();
q.pop();


if ((int)ladder.size() > MAX_LADDER_LENGTH) {
continue;
}


string last_word = ladder.back();


if (last_word == end_word) {
return ladder;
}


for (size_t i = 0; i < last_word.size(); i++) {
string new_word = last_word;
for (char c = 'a'; c <= 'z'; c++) {
char old = new_word[i];
if (c == old) continue; 
new_word[i] = c;
if (word_list.find(new_word) != word_list.end() && !visited.count(new_word)) {
visited.insert(new_word);
vector<string> new_ladder = ladder;
new_ladder.push_back(new_word);
q.push(new_ladder);
}
new_word[i] = old; 
}


for (size_t i = 0; i <= last_word.size(); i++) {
for (char c = 'a'; c <= 'z'; c++) {
string new_word = last_word;
new_word.insert(new_word.begin() + i, c);
if (word_list.find(new_word) != word_list.end() && !visited.count(new_word)) {
visited.insert(new_word);
vector<string> new_ladder = ladder;
new_ladder.push_back(new_word);
q.push(new_ladder);
}
}
}


for (size_t i = 0; i < last_word.size(); i++) {
string new_word = last_word;
new_word.erase(new_word.begin() + i);
if (word_list.find(new_word) != word_list.end() && !visited.count(new_word)) {
visited.insert(new_word);
vector<string> new_ladder = ladder;
new_ladder.push_back(new_word);
q.push(new_ladder);
}
}
}


error(begin_word, end_word, "No word ladder exists between these words!");
return {};
}
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
            std::cout << " ";
    }
    std::cout << " " << std::endl;
}


void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    
    #define my_assert(e) {cout << #e << ((e) ? " passed": " failed") << endl;}
    
    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}

