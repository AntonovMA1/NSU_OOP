#include <ctime>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>

class logstream {
private:
    std::ofstream fileStream;
    std::string prefix;
    logstream() {};
    logstream(logstream& logger) = delete;
    void operator=(logstream& logger) = delete;
public:
    static logstream& getInstance() {
        static logstream instance;
        return instance;
    }

    void set_log_prefix(const std::string& _prefix) {
        prefix = _prefix;
    }

    void set_log_file(const std::string& name) {
        if (fileStream.is_open()) {
            fileStream.flush();
            fileStream.close();
        }
        fileStream.open(name);
    }

    ~logstream() {
        fileStream.flush();
    }

    template <typename T>
    logstream& operator<<(const T& message) {
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
        fileStream << "[" << timestamp << "] " << prefix << ": " << message << "\n";
        return *this;
    }

    logstream& operator<<(std::basic_ostream<char,std::char_traits<char>>& (*func)(std::basic_ostream<char,std::char_traits<char>>&)) {
        if(func == std::endl<char, std::char_traits<char>>){
            fileStream.flush();
        }
        return *this;
    }
};


int main()
{
    logstream& logger = logstream::getInstance();

    logger.set_log_file("logfile.txt");
    logger.set_log_prefix("DEBUG");
    logger << "Debug message" << "Debug message2" << std::endl;
    sleep(1);

    logger.set_log_file("logfile2.txt");
    logger.set_log_prefix("ERROR");
    logger << "Error message" << "Error message2" << std::endl;
    return 0;
}