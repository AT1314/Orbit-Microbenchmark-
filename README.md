# Orbit-Microbenchmark 
To measure the performance of the orbit API(s), microbenchmarks are written, which can also help analyse and find the potential performance bottleneck. The snap.cpp, run.sh and plot.py files in each directory should be copied into the GUEST VM's orbit/experiment/micro-call repo to replace the original one, then run the script to generate the result.

## orbit_pool_create
```struct orbit_pool *orbit_pool_create(struct orbit_module *ob, size_t init_pool_size);```


![Figure 1. orbit_pool_create performance](/orbit_area_create/figure1.png)



## orbit_call
```long orbit_call(struct orbit_module *module, size_t npool, struct orbit_pool** pools, orbit_entry func, void *arg, size_t argsize);```


![Figure 2. orbit_call performance](/orbit_call/figure1.png)



## orbit_create
```orbit *orbit_create(const char *name, orbit_entry entry, void* (*init)(void));```


![Figure 3. orbit_create performance](/orbit_create/figure1.png)



## orbit_async_call
```orbit_future *orbit_call_async(orbit *ob, int flags, size_t narea, orbit_area** areas, orbit_entry func_once, ...)```


![Figure 4. orbit_async_call performance related to the number of orbit calls](/orbit_async_call/figure1.png)
