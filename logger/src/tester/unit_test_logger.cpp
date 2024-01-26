
#include <logger.h>

#include <chrono>

void measure(std::function<void()> func) {
  auto start = std::chrono::system_clock::now();
  for (int i = 0; i < 1000; ++i) {
    func();
  }
  auto lapse = std::chrono::system_clock::now() - start;
  std::cout << "DURATION: " << lapse.count() << "\n";
}

int main() {
  {
    measure([]() {
      DebugLog("Debug");
      InfoLog("Info");
      ErrorLog("Error");
      });
  }

  {
    // Initialize all to stdout
    InitializeLogger(std::cout);
    measure([]() {
      DebugLog("Debug");
      InfoLog("Info");
      ErrorLog("Error");
      });
    measure([]() {
      DebugLog("Debug" << 2);
      InfoLog("Info" << 2.52);
      ErrorLog("Error" << 3);
      });
    DestroyLogger();
  }

  {
    // Initialize all to log file
    InitializeLogger("file.log");
    measure([]() {
      DebugLog("Debug");
      InfoLog("Info");
      ErrorLog("Error");
      });
    DestroyLogger();
  }

  {
    // Initialize from given level to stdout
    InitializeLoggerLevel(utils::INFO, std::cout);
    DebugLog("Debug");
    InfoLog("Info");
    ErrorLog("Error");
    DestroyLogger();
  }

  {
    // Initialize from given level to stdout
    InitializeLoggerLevel(utils::ERROR, "file.log");
    DebugLog("Debug");
    InfoLog("Info");
    ErrorLog("Error");
    DestroyLogger();
  }

  return 0;
}