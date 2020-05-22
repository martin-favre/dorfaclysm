#include "Logging.h"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

const std::string Logging::mLogFilename{"log.log"};
std::unique_ptr<std::thread> Logging::mLogThread;
bool Logging::mRunning{false};
std::mutex Logging::mMutex;
std::queue<std::pair<std::string, Logging::Level>> Logging::mLogQueue;
std::ofstream Logging::mFile;
bool Logging::mHasMessages{false};
bool Logging::mInitialized{false};

void Logging::initialize() {
  mRunning = true;
  mFile.open(mLogFilename);
  mLogThread = std::make_unique<std::thread>(Logging::logUpdate);
  LOGL("Logging initialized", Logging::info);
  mInitialized = true;
}

bool Logging::isInitialized() { return mInitialized; }

void Logging::teardown() {
  LOGL("Logging torn down", Logging::info);
  mHasMessages = true;
  mRunning = false;
  mInitialized = false;
  if (mFile.is_open()) {
    mFile.close();
  }
  if (mLogThread.get()) {
    mLogThread->join();
  }
}

void Logging::logUpdate() {
  while (mRunning || mHasMessages) {
    { 
      std::scoped_lock lock(mMutex);
      if (!mLogQueue.empty()) {
        std::pair<std::string, Level> logPackage = mLogQueue.front();
        mLogQueue.pop();
        printLogMessage(logPackage);
      }
      mHasMessages = !mLogQueue.empty();
    }
    if (!mHasMessages) {
      // If we don't have anything to log, chill for a while
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
  }
}
void Logging::printLogMessage(const std::pair<std::string, Level>& message) {
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);
  std::ostringstream oss;
  oss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
  std::string printStr = oss.str() + ": " + message.first + "\n";
  if (message.second != trace) {
    std::cout << printStr;
  }
  mFile << printStr;
  mFile.flush();
}
void Logging::log(const std::string& text, Level level) {
  std::scoped_lock lock(mMutex);
  mLogQueue.emplace(text, level);
}

void Logging::log(const std::ostream& text, Level level) {
  std::stringstream ss;
  ss << text.rdbuf();
  Logging::log(ss.str(), level);
}
