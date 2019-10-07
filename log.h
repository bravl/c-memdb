/** @file log.h
 *  @brief Functions prototypes for the logging system
 *
 *  This file contains the prototypes and macros used by the
 *  logging system
 *
 *  @author Bram Vlerick (bram.vlerick@ucast.be)
 *  @bug
 *  * None at the moment
 *
 *	This code works, no need for changes
 */

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

#define __FILENAME__                                                           \
    (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
/*
    NOTE: Adapted from github.com/firehol/netdata logging system
*/

#ifndef FLEX_LOG_H
#define FLEX_LOG_H 1

#ifdef __GNUC__
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

#define D_OPTIONS 0x00000001
#define D_DLIST 0x00000002
#define D_BITREE 0x00000004
#define D_AVLTREE 0x00000008
#define D_HASHTABLE 0x00000010
#define D_FLEX 0x00000020
#define D_MEMORY 0x00000040
#define D_SOCKETS 0x00000080
#define D_TFTP 0x00000100
#define D_CONFIG 0x00000200
#define D_WORKERQUEUE 0x00000400
#define D_SCHEDULER 0x00000800
#define D_TESTS 0X80000000


#define DEBUG D_SCHEDULER | D_AVLTREE | D_BITREE | D_CONFIG | D_FLEX

extern unsigned long long debug_flags;
extern const char *program_name;

extern int silent;

extern int access_fd;
extern FILE *stdaccess;

extern int access_log_syslog;
extern int error_log_syslog;
extern int output_log_syslog;

extern time_t error_log_throttle_period;
extern unsigned long error_log_errors_per_period;
extern int error_log_limit(int reset);

#define error_log_limit_reset()                                                \
    do {                                                                       \
        error_log_limit(1);                                                    \
    } while (0)

#define debug(type, args...)                                                   \
    do {                                                                       \
        if (unlikely(!silent && (debug_flags & type)))                         \
            debug_int(__FILENAME__, __FUNCTION__, __LINE__, ##args);           \
    } while (0)
#define info(args...) info_int(__FILE__, __FUNCTION__, __LINE__, ##args)
#define infoerr(args...)                                                       \
    error_int("INFO", __FILENAME__, __FUNCTION__, __LINE__, ##args)
#define error(args...)                                                         \
    error_int("ERROR", __FILENAME__, __FUNCTION__, __LINE__, ##args)
#define fatal(args...) fatal_int(__FILENAME__, __FUNCTION__, __LINE__, ##args)

extern void log_date(FILE *out);
extern void debug_int(const char *file, const char *function,
                      const unsigned long line, const char *fmt, ...);
extern void info_int(const char *file, const char *function,
                     const unsigned long line, const char *fmt, ...);
extern void error_int(const char *prefix, const char *file,
                      const char *function, const unsigned long line,
                      const char *fmt, ...);
extern void fatal_int(const char *file, const char *function,
                      const unsigned long line, const char *fmt, ...)
    __attribute__((noreturn));
extern void log_access(const char *fmt, ...);

#endif
