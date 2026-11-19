#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <cerrno>
#include <cstdlib>

int main(int argc, char* argv[])
{
    int max_args = 0;
    int command_start = 1;

    if (argc > 2 && std::string(argv[1]) == "-n") {
        max_args = std::stoi(argv[2]);
        command_start = 3;
    }

    std::vector<std::string> command;

    for (int i = command_start; i < argc; ++i) {
        command.push_back(argv[i]);
    }

    if (command.empty()) {
        command.push_back("echo");
    }

    std::vector<std::string> arguments;
    std::string word;

    auto execute = [&]() -> int {
        if (arguments.empty()) {
            return 0;
        }

        std::vector<std::string> full_command = command;

        for (const auto& arg : arguments) {
            full_command.push_back(arg);
        }

        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            return 1;
        }

        if (pid == 0) {
            std::vector<char*> args;

            for (auto& arg : full_command) {
                args.push_back(const_cast<char*>(arg.c_str()));
            }

            args.push_back(nullptr);

            execvp(args[0], args.data());

            perror("execvp");
            _exit(127);
        }

        int status;

        while (waitpid(pid, &status, 0) == -1) {
            if (errno != EINTR) {
                perror("waitpid");
                return 1;
            }
        }

        arguments.clear();

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }

        if (WIFSIGNALED(status)) {
            return 128 + WTERMSIG(status);
        }

        return 1;
    };

    while (std::cin >> word) {
        arguments.push_back(word);

        if (max_args > 0 &&
            static_cast<int>(arguments.size()) >= max_args) {
            if (execute() != 0) {
                return 1;
            }
        }
    }

    if (execute() != 0) {
        return 1;
    }

    return 0;
}
