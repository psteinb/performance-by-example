#ifndef _TIMING_H_
#define _TIMING_H_

#include <chrono>

using as_mus_t = std::chrono::duration<double, std::chrono::microseconds::period>;
using as_ms_t = std::chrono::duration<double, std::chrono::milliseconds::period>;
using as_ns_t = std::chrono::duration<double, std::chrono::milliseconds::period>;

#endif /* _TIMING_H_ */
