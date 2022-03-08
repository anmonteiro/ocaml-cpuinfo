#include "num_cores.h"
#include "../config.h"

#if defined(HAVE_STDINT_H)
#include <stdint.h>
#endif
#if defined(linux_HOST_OS)
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#if HAVE_STRING_H
#include <string.h>
#endif

#if defined(darwin_HOST_OS) || defined(freebsd_HOST_OS) ||                     \
    defined(netbsd_HOST_OS)
#include <sys/sysctl.h>
#include <sys/types.h>
#endif

#if defined(HAVE_SCHED_H)
#include <sched.h>
#endif

#if defined(HAVE_SYS_PARAM_H)
#include <sys/param.h>
#endif
#if defined(HAVE_SYS_CPUSET_H)
#include <sys/cpuset.h>
#endif

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

#if defined(HAVE_SIGNAL_H)
#include <signal.h>
#endif

#define RELAXED_LOAD(ptr) __atomic_load_n(ptr, __ATOMIC_RELAXED)
#define RELAXED_STORE(ptr, val) __atomic_store_n(ptr, val, __ATOMIC_RELAXED)

static uint32_t nproc_cache = 0;

// Get the number of logical CPU cores available to us. Note that this is
// different from the number of physical cores (see #14781).
uint32_t getNumberOfProcessors(void) {
  uint32_t nproc = RELAXED_LOAD(&nproc_cache);
  if (nproc == 0) {
#if defined(HAVE_SCHED_GETAFFINITY)
    cpu_set_t mask;
    CPU_ZERO(&mask);
    if (sched_getaffinity(0, sizeof(mask), &mask) == 0) {
      for (int i = 0; i < CPU_SETSIZE; i++) {
        if (CPU_ISSET(i, &mask))
          nproc++;
      }
      return nproc;
    }
#endif

#if defined(darwin_HOST_OS)
    size_t size = sizeof(uint32_t);
    if (sysctlbyname("machdep.cpu.thread_count", &nproc, &size, NULL, 0) != 0) {
      if (sysctlbyname("hw.logicalcpu", &nproc, &size, NULL, 0) != 0) {
        if (sysctlbyname("hw.ncpu", &nproc, &size, NULL, 0) != 0)
          nproc = 1;
      }
    }
#elif defined(freebsd_HOST_OS)
    cpuset_t mask;
    CPU_ZERO(&mask);
    if (cpuset_getaffinity(CPU_LEVEL_CPUSET, CPU_WHICH_PID, -1, sizeof(mask),
                           &mask) == 0) {
      return CPU_COUNT(&mask);
    } else {
      size_t size = sizeof(uint32_t);
      if (sysctlbyname("hw.ncpu", &nproc, &size, NULL, 0) != 0)
        nproc = 1;
    }
#elif defined(netbsd_HOST_OS)
    int mib[2] = {CTL_HW, HW_NCPUONLINE};
    size_t size = sizeof(nproc);
    if (sysctl(mib, 2, &nproc, &size, NULL, 0) != 0) {
      mib[1] = HW_NCPU;
      if (sysctl(mib, 2, &nproc, &size, NULL, 0) != 0) {
        nproc = 1;
      }
    }
#elif defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_ONLN)
    // N.B. This is the number of physical processors.
    nproc = sysconf(_SC_NPROCESSORS_ONLN);
#elif defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
    // N.B. This is the number of physical processors.
    nproc = sysconf(_SC_NPROCESSORS_CONF);
#else
    nproc = 1;
#endif
    RELAXED_STORE(&nproc_cache, nproc);
  }

  return nproc;
}
