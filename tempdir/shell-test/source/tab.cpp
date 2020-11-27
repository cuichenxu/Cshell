#include <algorithm>
#include <sstream>
#include <vector>
#include <iostream>
#include <readline/history.h>
#include <readline/readline.h>
#include "tab.h"

using namespace std;
vector<string> vocabulory{"help","cp","mv","mkdir","ls","ps","pwd","exit","echo","touch","remove_dir"};
/** tab auto commands **/
char *autoCommand(const char *text, int state) {
    static vector<string> matches;
    static size_t match_index = 0;
    if (0==state) {
        matches.clear();
        match_index = 0;

        string textstr(text);
        for(auto word : vocabulory) {
            if(word.size() >= textstr.size() && word.compare(0,textstr.size(),textstr)==0) {
                matches.push_back(word);
            }
        }
    }

    if(match_index >= matches.size()) {
        return nullptr;
    } else {
        return strdup(matches[match_index++].c_str());
    }
}

char **autoCommand(const char* text, int start, int end) {
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text,autoCommand);
}