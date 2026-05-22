#pragma once
#include <fstream>
#include <string>
#include <sstream>

enum class LogHeader { Error, Warning, Info };

class LoggingManager
{
public:
    LoggingManager();
    ~LoggingManager();
    
    bool Initialize(const std::string& logDirectory = "Logs");
    
    template<typename T>
    void Add(LogHeader headText, std::string dataLabel, const T& data)
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
    
        std::ostringstream logDataStream;
        logDataStream << data;
        std::string lineForLogging = "[" + headTextToString+ "] " + dataLabel +  logDataStream.str();
    
        logFile << lineForLogging << std::endl;
        logFile.flush();
    };

    void ViewLog() const;
    
private:
    std::string currentLogFilePath;
    std::ofstream logFile;
    
    std::string GetTimestamp() const;
};
