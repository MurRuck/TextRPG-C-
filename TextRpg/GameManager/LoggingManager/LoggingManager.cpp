#include "LoggingManager.h"
#include <filesystem>

LoggingManager::LoggingManager()
{
    Initialize();
}

LoggingManager::~LoggingManager()
{
    if (logFile.is_open())
    {
        logFile.close();
    }
}

bool LoggingManager::Initialize(const std::string& logDirectory)
{
    namespace fs = std::filesystem;
    
    if (!fs::exists(logDirectory))
    {
        fs::create_directory(logDirectory);
    }
    
    std::string timestamp = GetTimestamp();
    currentLogFilePath = logDirectory + "/log" + timestamp + ".txt";
    
    logFile.open(currentLogFilePath);
    
    return logFile.is_open();
}



void LoggingManager::ViewLog() const
{
    
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
