#include <mtlog/mt_log.hpp>
#include <iostream>

namespace mt_logging
{

  LoggerThread &logger()
  {
    static LoggerThread instance;
    return instance;
  }

  LoggerThread::LoggerThread()
  {
    std::unique_lock<std::mutex> lock(mtx_);

    const char *env = std::getenv("MTLOG_LOGFILE");
    std::string logfile = env ? env : "mt_logger.log";

    // 1. Truncate the file once at startup
    const char *clear = std::getenv("MTLOG_CLEAR_ON_START");
    if (clear != nullptr)
    {
      std::ofstream clear_file(logfile, std::ios::out | std::ios::trunc);
      // clear_file closes automatically here
    }

    // 2. Reopen in append mode for the lifetime of the logger
    outfile_.open(logfile, std::ios::app);

    // 3. Start worker thread
    thread_ = std::thread([this]
                          { run(); });

    // 4. Wait for worker thread to be ready
    cv_.wait(lock, [&]
             { return ready_; });
  }

  LoggerThread::~LoggerThread()
  {
    stop();
    thread_.join();
    outfile_.close();
  }

  void LoggerThread::set_logfile(const std::string &filename)
  {
    std::lock_guard<std::mutex> lock(mtx_);
    outfile_.close();
    outfile_.open(filename, std::ios::app);
  }

  void LoggerThread::stop()
  {
    {
      std::lock_guard<std::mutex> lock(mtx_);
      stop_flag_ = true;
    }
    cv_.notify_all();
  }

  void LoggerThread::run()
  {
    std::unique_lock<std::mutex> lock(mtx_);
    ready_ = true;
    cv_.notify_one();

    while (true)
    {
      cv_.wait(lock, [&]
               { return stop_flag_ || !q_.empty(); });
      if (stop_flag_ && q_.empty())
        return;

      LogJob job = std::move(q_.front());
      q_.pop();

      lock.unlock();
      write(job);
      lock.lock();
    }
  }

  void LoggerThread::write(const LogJob &job)
  {
    if (!outfile_.good())
      return;

    std::string ts = timestamp();

    if (job.include_thread_id)
    {
      outfile_ << fmt::format("{} [thread {}]  {}\n",
                              ts,
                              job.caller_thread_id,
                              job.line);
    }
    else
    {
      outfile_ << fmt::format("{}  {}\n", ts, job.line);
    }

    outfile_.flush();
  }

} // namespace mt_logging
