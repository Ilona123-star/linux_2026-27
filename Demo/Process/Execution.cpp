#include <Utils/Process/Execution.h>

#include <cassert>
#include <cstdlib>
#include <sys/wait.h>

int main()
{
    int status = Process::ExecuteCommand("exit 0");
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 0);

    status = Process::ExecuteCommand("exit 42");
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 42);

    status = Process::ExecuteCommand("echo Hello > /dev/null");
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 0);

    status = Process::ExecuteCommand("echo Hello | grep Hello > /dev/null");
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 0);

    status = Process::ExecuteCommand("nonexistent_hw8_command");
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 127);

    const int expected = std::system("exit 17");
    const int actual =  Process::ExecuteCommand("exit 17");

    assert(expected == actual);
}
