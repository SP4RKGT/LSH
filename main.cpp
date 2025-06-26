#include <iostream>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <sstream>
#include <stdlib.h>
#include <cstring>

std::string input;

// CD command to handle ~ expansion
void cd(const std::string& path) {
    char* home_dir = getenv("HOME");  // where is your home child?
    if (home_dir == NULL) {
        // You're homeless? 
        std::cerr << "HOME environment variable not set.\n";
        return;
    }

    std::string expanded_path = path;

    if (path[0] == '~') {
        // domain expansion TILDA HOME
        if (path.size() == 1) {
            // go home tilda boy
            expanded_path = home_dir;
        } else {
            // pack it up tilda
            expanded_path = std::string(home_dir) + path.substr(1);
        }
    }

    // i dont know where i am
    if (chdir(expanded_path.c_str()) != 0) {
        perror("cd failed");
    }
}

// Prompt function (keep me busy)
int prompt() {
    std::cout << "LSH>>> "; // Feel free to add any fancy text here
    std::getline(std::cin, input);
    
    if (input == "exit") {
        return 0; // Exit the shell when you type "exit"
    }
    return 1; // Keep going, we're not done yet
}

// Parse the input command into arguments (let's break it down)
std::vector<std::string> parse_input(const std::string& str) {
    std::vector<std::string> args;
    std::stringstream ss(str);
    std::string word;

    while (ss >> word) {
        args.push_back(word);
    }
    return args;
}

int main() {
    while (true) {
        if (prompt() == 0) {
            break; // std::cout << "Goodbye World\n";
        }

        // Let's handle cd first, don't want to mess that up
        if (input.substr(0, 2) == "cd") {
            std::vector<std::string> args = parse_input(input);
            if (args.size() > 1) {
                // If there's an argument for the cd command, let's go there
                cd(args[1]);
            } else {
                // for thou art in confusion, thou shalt be guided by the hand of thy code
                cd("~");
            }
            continue;
        }

        // SUMMON THE CHILD 
        pid_t pid = fork();

        if (pid == -1) {
            // fork() failed, oh nooo
            perror("Fork Failed");
            return 1;
        } else if (pid == 0) {
            // I'm just gonna kill my child... (jk, we need it)
            std::vector<std::string> args_str = parse_input(input);
            std::vector<char*> args_c_str;

            for (const std::string& arg : args_str) {
                args_c_str.push_back(const_cast<char*>(arg.c_str()));
            }
            // some nullptr idk bro
            args_c_str.push_back(nullptr);

            // Let's execute the command in the child process
            execvp(args_c_str[0], args_c_str.data());
            // Oops, we failed 
            perror("Execvp Failed");
            _exit(1); // Exit the child if execvp fails
        } else {
            // child labour
            int status;
            waitpid(pid, &status, 0);
        }
    }
    return 0;
}
