#include "orbit.h"
#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <algorithm>
#include <thread>
#include <chrono>
#include <vector>

using std::min;
using namespace std::chrono;

#define PGSIZE 4096UL
#define KB 1024UL
#define MB (1024UL * KB)
#define GB (1024UL * MB)

// Force assert regardless of NDEBUG matter
#define assert(expr)                         \
    do                                       \
    {                                        \
        if (!(expr))                         \
            fprintf(stderr, #expr "failed"); \
    } while (0)

unsigned long empty(void *store, void *argbuf)
{
    (void)store;
    (void)argbuf;
    std::this_thread::sleep_for(milliseconds(5));
    return 0;
}

orbit_module *m;
orbit_pool *pool;
orbit_allocator *alloc;

// Make pages dirty
void dirty_pages(orbit_pool *pool)
{
    for (char *page = (char *)pool->rawptr;
         page < (char *)pool->rawptr + pool->used;
         page += PGSIZE)
    {
        *page = 'h';
    }
}

void orbit_one(long long *duration)
{
    orbit_task task;
    int ret = orbit_call_async(m, 0, 1, &pool, NULL, NULL, 0, &task);
    assert(ret == 0);

    orbit_result result;
    auto t1 = high_resolution_clock::now();
    ret = orbit_recvv(&result, &task);
    auto t2 = high_resolution_clock::now();
    assert(ret == 0 && result.retval == 0);

    *duration = *duration + duration_cast<nanoseconds>(t2 - t1).count();
}

void bench_snap(size_t num, size_t size, bool csv)
{
    pool = orbit_pool_create(NULL, size);
    alloc = orbit_allocator_from_pool(pool, false);
    m = orbit_create("snap percentage", empty, NULL);
    assert(m);

    if (!csv)
        printf("Testing snapshot size %lu\n", size);

    void *ptr = orbit_alloc(alloc, size);
    assert(ptr);
    assert(size == pool->used);

    dirty_pages(pool);
    long long duration = 0;
    std::vector<std::thread> th_v;

    for (size_t i = 0; i < num; i++)
    {
        std::thread orbit(orbit_one, &duration);
        th_v.push_back(std::move(orbit));
    }

    for (auto &i : th_v)
    {
        i.join();
    }

    long long avg = duration / num;

    if (csv)
        printf("%lu,%lld\n", num, avg);
    else
        printf("Snap %lu takes %lld ns.\n", num, avg);

    orbit_destroy(m->gobid);
}

void usage(const char *prog_name, int exit_val)
{
    fprintf(stderr, "Usage: %s <size>\n", prog_name);
    exit(exit_val);
}

int main(int argc, const char *argv[])
{
    if (argc != 2)
        usage(argv[0], 1);
    size_t size = 4194304 * 256;
    size_t num;
    if (sscanf(argv[1], "%ld", &num) != 1)
        usage(argv[0], 1);
    bench_snap(num, size, true);
    return 0;
}