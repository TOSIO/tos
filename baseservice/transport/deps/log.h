#pragma once
#include <string>
#include <vector>
#include "compat.h"
#include "tinyformat.h"
#include "toscore/log/Log.h"


static const bool DEFAULT_LOGIPS        = false;

extern bool fLogIPs;
extern std::atomic<uint32_t> logCategories;

struct CLogCategoryActive
{
    std::string category;
    bool active;
};

namespace BCLog {
    enum LogFlags : uint32_t {
        NONE        = 0,
        NET         = (1 <<  0),
        TOR         = (1 <<  1),
        MEMPOOL     = (1 <<  2),
        HTTP        = (1 <<  3),
        BENCH       = (1 <<  4),
        ZMQ         = (1 <<  5),
        DB          = (1 <<  6),
        RPC         = (1 <<  7),
        ESTIMATEFEE = (1 <<  8),
        ADDRMAN     = (1 <<  9),
        SELECTCOINS = (1 << 10),
        REINDEX     = (1 << 11),
        CMPCTBLOCK  = (1 << 12),
        RAND        = (1 << 13),
        PRUNE       = (1 << 14),
        PROXY       = (1 << 15),
        MEMPOOLREJ  = (1 << 16),
        LIBEVENT    = (1 << 17),
        COINDB      = (1 << 18),
        QT          = (1 << 19),
        LEVELDB     = (1 << 20),
        ALL         = ~(uint32_t)0,
    };
}
/** Return true if log accepts specified category */
static inline bool LogAcceptCategory(uint32_t category)
{
    return (logCategories.load(std::memory_order_relaxed) & category) != 0;
}

/** Returns a string with the log categories. */
std::string ListLogCategories();

/** Returns a vector of the active log categories. */
std::vector<CLogCategoryActive> ListActiveLogCategories();

/** Return true if str parses as a log category and set the flags in f */
bool GetLogCategory(uint32_t *f, const std::string *str);


/** Get format string from VA_ARGS for error reporting */
template<typename... Args> std::string FormatStringFromLogArgs(const char *fmt, const Args&... args) { return fmt; }

static inline void MarkUsed() {}
template<typename T, typename... Args> static inline void MarkUsed(const T& t, const Args&... args)
{
    (void)t;
    MarkUsed(args...);
}

// Be conservative when using LogPrintf/error or other things which
// unconditionally log to debug.log! It should not be the case that an inbound
// peer can fill up a users disk with debug.log entries.

#ifdef USE_COVERAGE
#define LogPrintf(...) do { MarkUsed(__VA_ARGS__); } while(0)
#define LogPrint(category, ...) do { MarkUsed(__VA_ARGS__); } while(0)
#else
#define LogPrintf(...) do { \
    std::string _log_msg_; /* Unlikely name to avoid shadowing variables */ \
    try { \
        _log_msg_ = tfm::format(__VA_ARGS__); \
    } catch (tinyformat::format_error &fmterr) { \
        /* Original format string will have newline so don't add one here */ \
        _log_msg_ = "Error \"" + std::string(fmterr.what()) + "\" while formatting log message: " + FormatStringFromLogArgs(__VA_ARGS__); \
    } \
    cnote <<_log_msg_; \
} while(0)

#define LogPrint(category, ...) do { \
    if (LogAcceptCategory((category))) { \
        LogPrintf(__VA_ARGS__); \
    } \
} while(0)
#endif

template<typename... Args>
bool error(const char* fmt, const Args&... args)
{
    //LogPrintStr("ERROR: " + tfm::format(fmt, args...) + "\n");
    cnote <<"ERROR: "<<tfm::format(fmt, args...)<<"\n";
    return false;
}


void PrintExceptionContinue(const std::exception *pex, const char* pszThread);