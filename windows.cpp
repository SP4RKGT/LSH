#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <windows.h>
#include <direct.h> // for _chdir()

std::string input;

// CD command to handle ~ expansion
void cd(const std::string& path) {
    char* home_dir = getenv("USERPROFILE");  // where is your home child?
    if (home_dir == NULL) {
        // You're homeless? 
        std::cerr << "USERPROFILE environment variable not set.\n";
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
    if (_chdir(expanded_path.c_str()) != 0) {
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
        STARTUPINFO si = {sizeof(si)};
        PROCESS_INFORMATION pi;

        std::vector<std::string> args_str = parse_input(input);
        std::string command = args_str[0];
        std::string arguments = "";

        for (size_t i = 1; i < args_str.size(); ++i) {
            arguments += args_str[i] + " ";
        }

        // Prepare the command line for CreateProcess
        std::string cmd_line = command + " " + arguments;

        if (CreateProcess(NULL, (LPSTR)cmd_line.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            // child labour
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        } else {
            // cant even make a process
            std::cerr << "CreateProcess failed\n";
        }
    }
    return 0;
}
