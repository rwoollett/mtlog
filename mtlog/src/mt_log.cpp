#include <mtlog/mt_log.hpp>

namespace mt_logging
{
  LoggerThread &logger()
  {
    static LoggerThread instance;
    return instance;
  }

  // LoggerThread::LoggerThread()
  //     : thread_([this]
  //               { run(); }) {}
  LoggerThread::LoggerThread()
  {
    std::unique_lock<std::mutex> lock(mtx_);
    thread_ = std::thread([this]
                          { run(); });
  }

  LoggerThread::~LoggerThread()
  {
    stop();
    thread_.join();
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
    LogJob job;
    while (true)
    {
      {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [&]
                 { return stop_flag_ || !q_.empty(); });
        if (stop_flag_ && q_.empty())
          return;
        job = std::move(q_.front());
        q_.pop();
      }
      write(job);
    }
  }

  void LoggerThread::write(const LogJob &job)
  {
    std::ofstream outfile(job.file_name, job.mode);
    if (!outfile.good())
      return;

    std::string ts = timestamp();

    if (job.include_thread_id)
    {
      outfile << fmt::format("{} [thread {}]  {}\n",
                             ts,
                             job.caller_thread_id,
                             job.line);
    }
    else
    {
      outfile << fmt::format("{}  {}\n", ts, job.line);
    }
  }

} // namespace mt_logging
