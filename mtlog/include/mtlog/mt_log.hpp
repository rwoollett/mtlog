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

namespace mt_logging
{

  struct LogJob
  {
    std::string file_name;
    std::string line;
    std::ios_base::openmode mode = std::ios::app;
    bool include_thread_id = false;
    std::string caller_thread_id; 
  };

  class LoggerThread
  {
  public:
    LoggerThread();
    ~LoggerThread();

    inline void log(LogJob job)
    {
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
    std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<LogJob> q_;
    bool stop_flag_ = false;
    std::thread thread_;

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

  LoggerThread& logger();

} // namespace mt_logging
