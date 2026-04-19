#pragma once
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <thread>
#include <chrono>
#include <ctime>
#include <sstream>

#include <fmt/format.h>

#include <mtlog/format_pairs.hpp>
#include <fmt/ranges.h>
#include <fmt/chrono.h>

namespace mt_logging
{
  enum class LogLevel
  {
    Debug = 0,
    Info,
    Warn,
    Error
  };

  struct LogJob
  {
    std::string line;
    LogLevel level = LogLevel::Info;
    bool include_thread_id = false;
    std::string caller_thread_id;
  };

  class LoggerThread
  {
  public:
    LoggerThread();
    ~LoggerThread();

    void set_logfile(const std::string &filename);
    void set_min_level(LogLevel lvl);

    inline void log(LogJob job)
    {
      if (!accepting_logs.load(std::memory_order_relaxed))
        return;
        
      if (job.level < min_level_)
        return; // filtered out

      if (job.include_thread_id)
      {
        std::ostringstream oss;
        oss << std::this_thread::get_id();
        job.caller_thread_id = oss.str();
      }

      {
        std::lock_guard<std::mutex> lock(mtx_);
        q_.push(std::move(job));
      }
      cv_.notify_one();
    }

  private:
    LogLevel min_level_ = LogLevel::Debug;
    std::atomic<bool> accepting_logs{true};

    std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<LogJob> q_;
    bool stop_flag_ = false;
    bool ready_ = false;

    std::thread thread_;
    std::ofstream outfile_;

    void run();
    void stop();
    void write(const LogJob &job);

    inline std::string timestamp()
    {
      using namespace std::chrono;
      auto now = system_clock::now();
      auto t = system_clock::to_time_t(now);

      std::tm tm_buf;
      localtime_r(&t, &tm_buf);

      char buffer[32];
      std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm_buf);
      return buffer;
    }
  };

  LoggerThread &logger();

} // namespace mt_logging
