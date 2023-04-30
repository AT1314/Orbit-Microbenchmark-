# Orbit-Microbenchmark 
To measure the performance of the orbit API(s), microbenchmarks are written, which can also help analyse and find the potential performance bottleneck.

## orbit_pool_create
'''struct orbit_pool *orbit_pool_create(struct orbit_module *ob, size_t init_pool_size)'''



## orbit_call
'''long orbit_call(struct orbit_module *module,
                size_t npool, struct orbit_pool** pools,
                orbit_entry func, void *arg, size_t argsize);'''
