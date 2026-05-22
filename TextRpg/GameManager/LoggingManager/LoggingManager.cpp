#include "LoggingManager.h"
#include <filesystem>

LoggingManager::LoggingManager()
{
    
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

template <typename T>
void LoggingManager::Add(LogHeader headText, T data)
{
    if (!logFile.is_open())
    {
        return;
    }
    
    std::string headTextToString;
    if (headText == LogHeader::Error)
    {
        headTextToString = "Error";
    }
    if (headText == LogHeader::Warning)
    {
        headTextToString = "Warning";
    }
    if (headText == LogHeader::Info)
    {
        headTextToString = "Info";
    }
    
    std::string logDataString = std::to_string(data);
    std::string lineForLogging = "[" + headTextToString+ "] " + logDataString;
    
    logFile << lineForLogging << std::endl;
    logFile.flush();
}


void LoggingManager::ViewLog() const
{
    
}
