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
    const char *env = std::getenv("MTLOG_LOGFILE");
    std::string logfile = env ? env : "mt_logger.log";

    const char *clear = std::getenv("MTLOG_CLEAR_ON_START");
    if (clear != nullptr)
    {
      std::ofstream clear_file(logfile, std::ios::out | std::ios::trunc);
    }

    const char *lvl = std::getenv("MTLOG_LEVEL");
    if (lvl)
    {
      std::string s = lvl;
      if (s == "debug")
        min_level_.store(LogLevel::Debug);
      else if (s == "info")
        min_level_.store(LogLevel::Info);
      else if (s == "warn")
        min_level_.store(LogLevel::Warn);
      else if (s == "error")
        min_level_.store(LogLevel::Error);
    }

    outfile_.open(logfile, std::ios::app);
    thread_ = std::thread([this]
                          { run(); });
  }

  LoggerThread::~LoggerThread()
  {
    stop();
    if (thread_.joinable())
      thread_.join();
    outfile_.close();
  }

  void LoggerThread::set_logfile(const std::string &filename)
  {
    std::lock_guard<std::mutex> lock(mtx_);
    outfile_.close();
    outfile_.open(filename, std::ios::app);
  }

  void LoggerThread::set_min_level(LogLevel lvl)
  {
    min_level_.store(lvl);
  }

  void LoggerThread::stop()
  {
    stop_flag_.store(true);
    accepting_logs.store(false, std::memory_order_relaxed);
    cv_.notify_all();
  }

  void LoggerThread::run()
  {
    std::vector<LogJob> batch;
    batch.reserve(256);

    while (true)
    {

      {
        std::unique_lock<std::mutex> lock(mtx_);

        cv_.wait(lock,
                 [&]
                 { return stop_flag_.load() || !q_.empty(); });

      // NB: caused a data race using wiat_for at 50ms - it is not needed to poll the q not empty
      //  cv_.wait_for(lock, std::chrono::milliseconds(50),
      //                [&]
      //                { return stop_flag_.load() || !q_.empty(); });                 

        if (stop_flag_.load() && q_.empty())
          break;

        while (!q_.empty() && batch.size() < 256)
        {
          batch.push_back(std::move(q_.front()));
          q_.pop_front();
        }
      }

      for (auto &job : batch)
        write(job);

      outfile_.flush();
      batch.clear();
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
  }

} // namespace mt_logging
