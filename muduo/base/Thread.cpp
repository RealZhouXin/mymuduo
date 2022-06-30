#include "Thread.h"
#include "CurrentThread.h"

#include <type_traits>

#include <errno.h>
#include <linux/unistd.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

using namespace muduo;

namespace muduo {
namespace detail {
pid_t gettid() {
    return static_cast<pid_t>(::syscall(SYS_gettid));
}
}
} // namespace muduo::detail

void CurrentThread::cacheTid()
{
  if (t_cachedTid == 0)
  {
    t_cachedTid = detail::gettid();
    t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
  }
}