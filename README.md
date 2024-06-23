# rlimit64_tool
Change rlimits on a running process in x86_64 linux

Usage:
```
Usage: ./rlimit64_tool <pid> <resource> [<soft-limit> <hard-limit>]
Resources: cpu, fsize, data, stack, core, rss, nproc, nofile, memlock, as, locks, sigpending, msgqueue, nice, rtprio, rttime
```

uses syscall(SYS_prlimit64.. to retrieve old limits, and set new limits specified by user in one call.
```
syscall(SYS_prlimit64, pid, resource, new, &old)
```

I included all resources defined in linux src, but have not tested modifying each on a running proc:
```
┌──(root✕hepta)-[/home/hedz/rlimit64_tool]
└─# grep "define RLIMIT_" /usr/include/asm-generic/resource.h
#define RLIMIT_CPU              0       /* CPU time in sec */
#define RLIMIT_FSIZE            1       /* Maximum filesize */
#define RLIMIT_DATA             2       /* max data size */
#define RLIMIT_STACK            3       /* max stack size */
#define RLIMIT_CORE             4       /* max core file size */
# define RLIMIT_RSS             5       /* max resident set size */
# define RLIMIT_NPROC           6       /* max number of processes */
# define RLIMIT_NOFILE          7       /* max number of open files */
# define RLIMIT_MEMLOCK         8       /* max locked-in-memory address space */
# define RLIMIT_AS              9       /* address space limit */
#define RLIMIT_LOCKS            10      /* maximum file locks held */
#define RLIMIT_SIGPENDING       11      /* max number of pending signals */
#define RLIMIT_MSGQUEUE         12      /* maximum bytes in POSIX mqueues */
#define RLIMIT_NICE             13      /* max nice prio allowed to raise to
#define RLIMIT_RTPRIO           14      /* maximum realtime priority */
#define RLIMIT_RTTIME           15      /* timeout for RT tasks in us */
```

-->

Example:

Terminal 1 (using an open file tester i included):
```
┌──(root✕hepta)-[/home/hedz/rlimit64_tool]
└─# ./open_files_test 50  240
Opening file: file1.txt
Opening file: file2.txt
Opening file: file3.txt
Opening file: file4.txt
Opening file: file5.txt
Error opening file: Too many open files
```

Terminal 2 (running rlimit64 tool for nofile):
```
┌──(root✕hepta)-[/home/hedz/rlimit64_tool]
└─# ps faux|grep test
root       14207  0.0  0.0   2776   968 pts/2    S+   19:42   0:00          |               \_ ./open_files_test 50 240
┌──(root✕hepta)-[/home/hedz/rlimit64_tool]
└─# ./rlimit64_tool 14207 nofile 1 1
Old limits: soft=1024; hard=1048576
New limits: soft=1; hard=1
```
