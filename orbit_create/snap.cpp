// Test for orbit_create
#include "orbit.h"
#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <algorithm>
#include <thread>
#include <chrono>

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

// Create an orbit instance according to the paper description
long long orbit_one(void)
{
	auto t1 = high_resolution_clock::now();
	m = orbit_create("snap percentage", empty, NULL);
	auto t2 = high_resolution_clock::now();

	return duration_cast<nanoseconds>(t2 - t1).count();
}

void bench_snap(size_t size, bool csv)
{
	pool = orbit_pool_create(NULL, size);
	alloc = orbit_allocator_from_pool(pool, false);

	if (!csv)
		printf("Testing snapshot size %lu\n", size);

	void *ptr = orbit_alloc(alloc, size);
	assert(ptr);
	assert(size == pool->used);

	/*  Fill the allocated space with non-zero data to ensure the kernel
		actually allocated a physical page for it */
	dirty_pages(pool);

	/*  Calculate the running time */
	long long duration = orbit_one();

	if (csv)
		printf("%lu,%lld\n", size, duration);
	else
		printf("Snap %lu takes %lld ns.\n", size, duration);

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
	size_t size;
	if (sscanf(argv[1], "%ld", &size) != 1)
		usage(argv[0], 1);
	bench_snap(size, true);
	return 0;
}
