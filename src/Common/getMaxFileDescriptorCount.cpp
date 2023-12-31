#include <IO/ReadHelpers.h>
#include <IO/WriteBufferFromString.h>
#include <IO/ReadBufferFromFile.h>
#include <Common/ShellCommand.h>
#include <Common/getMaxFileDescriptorCount.h>
#include <filesystem>

int getMaxFileDescriptorCount()
{
    namespace fs = std::filesystem;
    int result = -1;
#if defined(OS_LINUX) || defined(OS_DARWIN)
    using namespace DB;

    if (fs::exists("/proc/sys/fs/file-max"))
    {
        ReadBufferFromFile reader("/proc/sys/fs/file-max");
        readIntText(result, reader);
    }
    else
    {
        auto command = ShellCommand::execute("ulimit -n");
        try
        {
            readIntText(result, command->out);
            command->wait();
        }
        catch (...) // NOLINT(bugprone-empty-catch)
        {
        }
    }

#endif

    return result;
}
