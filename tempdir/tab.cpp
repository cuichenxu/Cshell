#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <readline/history.h>
#include <readline/readline.h>

using namespace std;

vector<string> vocabulory{"help","cp","mv","mkdir","ls","ps","pwd","exit","echo","touch","remove_dir"};

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

int main(int argc, char **argv) {
    if (argc > 1 && string(argv[1])=="-d") {
        rl_bind_key('\t',rl_insert); //tab
    }
    rl_attempted_completion_function = autoCommand;

    char* buf;
    string cmd;

    while((buf=readline("> "))!=nullptr) {
        cmd = string(buf);
        if (cmd.size() > 0) {
            add_history(buf);
        }
        free(buf);
        stringstream scmd(cmd);
        scmd>>cmd;

        vector<string>::iterator iter = 
        find(vocabulory.begin(),vocabulory.end(),cmd);

        if(iter!=vocabulory.end()) {
            cout<< cmd << ":" << distance(vocabulory.begin(),iter)<<endl;
        } else {
            cout<< "Invalid number name"<<endl;
        }
    }
    cout<<endl;
    return 0;
}