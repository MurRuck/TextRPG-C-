#include "LoggingManager.h"

#include <filesystem>
#include <iostream>

std::string LoggingManager::currentLogFilePath;
std::ofstream LoggingManager::logFile;
bool LoggingManager::initialized = false;

#ifdef _WIN32
HANDLE LoggingManager::logPipeWrite = nullptr;
PROCESS_INFORMATION LoggingManager::logProcessInfo{};
#endif

LoggingManager::LoggingManager()
{
    Initialize();
}

LoggingManager::~LoggingManager()
{
}

bool LoggingManager::Initialize(const std::string& logDirectory)
{
    if (initialized && logFile.is_open())
    {
        return true;
    }

    namespace fs = std::filesystem;

    if (!fs::exists(logDirectory))
    {
        fs::create_directory(logDirectory);
    }

    std::string timestamp = GetTimestamp();
    currentLogFilePath = logDirectory + "/log" + timestamp + ".txt";

    logFile.open(currentLogFilePath);
    InitializeLogConsole();
    initialized = logFile.is_open();

    return initialized;
}

bool LoggingManager::InitializeLogConsole()
{
#ifdef _WIN32
    if (logPipeWrite != nullptr)
    {
        return true;
    }

    SECURITY_ATTRIBUTES securityAttributes{};
    securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    securityAttributes.bInheritHandle = TRUE;
    securityAttributes.lpSecurityDescriptor = nullptr;

    HANDLE pipeRead = nullptr;
    HANDLE pipeWrite = nullptr;

    if (!CreatePipe(&pipeRead, &pipeWrite, &securityAttributes, 0))
    {
        return false;
    }

    SetHandleInformation(pipeWrite, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA startupInfo{};
    startupInfo.cb = sizeof(STARTUPINFOA);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdInput = pipeRead;
    startupInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    startupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    std::string command = "LogConsole.exe";
    BOOL success = CreateProcessA(
        nullptr,
        command.data(),
        nullptr,
        nullptr,
        TRUE,
        CREATE_NEW_CONSOLE,
        nullptr,
        nullptr,
        &startupInfo,
        &logProcessInfo
    );

    CloseHandle(pipeRead);

    if (!success)
    {
        CloseHandle(pipeWrite);
        return false;
    }

    logPipeWrite = pipeWrite;
    return true;
#else
    return false;
#endif
}

void LoggingManager::WriteToLogConsole(const std::string& line)
{
#ifdef _WIN32
    if (!logPipeWrite)
    {
        return;
    }

    std::string output = line + "\n";
    DWORD written = 0;
    WriteFile(
        logPipeWrite,
        output.c_str(),
        static_cast<DWORD>(output.size()),
        &written,
        nullptr
    );
#endif
}


std::string LoggingManager::GetTimestamp() const
{
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};
#ifdef _WIN32
    localtime_s(&localTime, &timeT);
#else
    localtime_r(&timeT, &localTime);
#endif

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y%m%d_%H%M%S");
    return oss.str();
}
