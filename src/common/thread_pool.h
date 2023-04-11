#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace spx {

class ThreadPool {
public:
  ThreadPool();
  ~ThreadPool();

  void submit(std::function<void()> &&task);

  void stop();

private:
  std::vector<std::thread> threads_;
  std::queue<std::function<void()>> tasks_;
  std::mutex tasks_mutex_;
  std::condition_variable tasks_cv_;
  std::atomic<bool> done_{false};
};

} // namespace spx
