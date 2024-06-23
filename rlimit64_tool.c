#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <string.h>
#include <errno.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

// struct to map rlimit names to constants
struct rlimit_mapping {
    const char *name;
    int resource;
};

// array of rlimit mappings
struct rlimit_mapping rlimit_map[] = {
    {"cpu", RLIMIT_CPU},
    {"fsize", RLIMIT_FSIZE},
    {"data", RLIMIT_DATA},
    {"stack", RLIMIT_STACK},
    {"core", RLIMIT_CORE},
    {"rss", RLIMIT_RSS},
    {"nproc", RLIMIT_NPROC},
    {"nofile", RLIMIT_NOFILE},
    {"memlock", RLIMIT_MEMLOCK},
    {"as", RLIMIT_AS},
    {"locks", RLIMIT_LOCKS},
    {"sigpending", RLIMIT_SIGPENDING},
    {"msgqueue", RLIMIT_MSGQUEUE},
    {"nice", RLIMIT_NICE},
    {"rtprio", RLIMIT_RTPRIO},
    {"rttime", RLIMIT_RTTIME},
    {NULL, -1}  // End marker
};

// function to get the resource constant from a string
int get_resource_from_name(const char *name) {
    for (int i = 0; rlimit_map[i].name != NULL; i++) {
        if (strcmp(rlimit_map[i].name, name) == 0) {
            return rlimit_map[i].resource;
        }
    }
    return -1;
}

int main(int argc, char *argv[]) {
    struct rlimit old, new;
    struct rlimit *newp;
    pid_t pid;
    int resource;

    if (!(argc == 3 || argc == 5)) {
        fprintf(stderr, "Usage: %s <pid> <resource> [<soft-limit> <hard-limit>]\n", argv[0]);
        fprintf(stderr, "Resources: cpu, fsize, data, stack, core, rss, nproc, nofile, memlock, as, locks, sigpending, msgqueue, nice, rtprio, rttime\n");
        exit(EXIT_FAILURE);
    }

    pid = atoi(argv[1]); // PID of target process

    resource = get_resource_from_name(argv[2]);
    if (resource == -1) {
        fprintf(stderr, "Invalid resource: %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    newp = NULL;
    if (argc == 5) {
        new.rlim_cur = (rlim_t) atoll(argv[3]);
        new.rlim_max = (rlim_t) atoll(argv[4]);
        newp = &new;
    }

    if (syscall(SYS_prlimit64, pid, resource, newp, &old) == -1)
        errExit("prlimit-1");
    printf("Old limits: soft=%lld; hard=%lld\n",
           (long long) old.rlim_cur, (long long) old.rlim_max);

    if (syscall(SYS_prlimit64, pid, resource, NULL, &old) == -1)
        errExit("prlimit-2");
    printf("New limits: soft=%lld; hard=%lld\n",
           (long long) old.rlim_cur, (long long) old.rlim_max);

    exit(EXIT_SUCCESS);
}
