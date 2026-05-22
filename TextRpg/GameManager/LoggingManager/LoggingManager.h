#pragma once
#include <fstream>
#include <string>

enum class LogHeader { Error, Warning, Info };

class LoggingManager
{
public:
    LoggingManager();
    ~LoggingManager();
    
    bool Initialize(const std::string& logDirectory = "Logs");
    
    template<typename T>
    void Add(LogHeader headText, T data);
    void ViewLog() const;
    
private:
    std::string currentLogFilePath;
    std::ofstream logFile;
    
    std::string GetTimestamp() const;
};
