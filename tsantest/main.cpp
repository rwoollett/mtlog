#include <mtlog/mt_log.hpp>
#include <thread>

int main(int argc, char **argv)
{

    mt_logging::logger().set_min_level(mt_logging::LogLevel::Info);
    int no_ts = 4;
    std::thread t_list[] = {
        std::thread(),
        std::thread(),
        std::thread(),
        std::thread(),
    };
    for (int k = 0; k < no_ts; k++)
    {
        t_list[k] = std::thread([k]
                                {for (int i = 0; i < 100; i++) {
                try {
                  mt_logging::logger().log({
                      .line = fmt::format("Thread {} token {} expired.", k, i),
                      .level = mt_logging::LogLevel::Info,
                      .include_thread_id = true
                  });

                } catch (std::exception &e) {
                    std::cerr << "std exception: " << e.what() << "\n";
                }
                //
            } });
    }

    for (int k = 0; k < no_ts; k++)
    {
        t_list[k].join();
        std::cout << "Thread " << k << " done." << std::endl;
    }

    return 0;
}