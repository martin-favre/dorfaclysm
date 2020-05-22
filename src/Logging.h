#pragma once
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#define LOG(string) Logging::log(std::stringstream() << string)
#define LOGL(string, level) Logging::log(std::stringstream() << string, level)

class Logging {
 public:
  enum Level { trace, info, warning, error };
  static void initialize();
  static void teardown();
  static void log(const std::string& text, Level level = Level::trace);
  static void log(const std::ostream& text, Level level = Level::trace);
  static bool isInitialized();

 private:
  static void flush();
  static void logUpdate();
  static void printLogMessage(const std::pair<std::string, Level>& message);
  static std::unique_ptr<std::thread> mLogThread;
  static std::mutex mMutex;
  static bool mRunning;
  static const std::string mLogFilename;
  static std::ofstream mFile;
  static std::queue<std::pair<std::string, Level>> mLogQueue;
  static bool mHasMessages;
  static bool mInitialized;
};