
#include <mtlog/mt_log.hpp>
#include <thread>

#include "gtest/gtest.h"

class MTLogTest : public ::testing::Test
{
public:
  void SetUp() override
  {
    std::cerr << "Setup\n";
    
  }

  void TearDown() override
  {
  }

public:
};

TEST_F(MTLogTest, LogFileFromTwoThreads)
{
  mt_logging::logger().set_min_level(mt_logging::LogLevel::Info);
  bool hasRun = false;
  int no_ts = 2;
  std::thread t_list[] = {
      std::thread(),
      std::thread(),
  };
  for (int k = 0; k < no_ts; k++)
  {
    t_list[k] = std::thread([k, this]
                            {for (int i = 0; i < 100; i++) {
                try {
                  //
                  // Log using Log method
                  std::string msg = fmt::format("Token {} expired at {}", i, "dd");
                  //mt_logging::logger().log({ msg, true });
                  mt_logging::logger().log({
                      .line = msg,
                      .level = mt_logging::LogLevel::Info,
                      .include_thread_id = true
                  });

                } catch (std::exception &e) {
                    std::cerr << "std exception: " << e.what() << "\n";
                }
                //
            } });
  }
  int mc = 0, mm = 0;
  while (mc < 2000)
  {
    try
    {

      mc++;
    }
    catch (std::exception &e)
    {
      std::cerr << "std exception: " << e.what();
    }
  }

  for (int k = 0; k < no_ts; k++)
  {
    t_list[k].join();
  }
  std::cerr << "\nTime outs " << mm << std::endl;
  ASSERT_EQ(2000, mc);
}
