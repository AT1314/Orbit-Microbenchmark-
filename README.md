# Orbit-Microbenchmark 
To measure the performance of the orbit API(s), microbenchmarks are written, which can also help analyse and find the potential performance bottleneck.

## orbit_pool_create
```struct orbit_pool *orbit_pool_create(struct orbit_module *ob, size_t init_pool_size);```


![Figure 1. orbit_pool_create performance](/orbit_area_create/figure1.png)



## orbit_call
```long orbit_call(struct orbit_module *module, size_t npool, struct orbit_pool** pools, orbit_entry func, void *arg, size_t argsize);```


![Figure 2. orbit_call performance](/orbit_call/figure1.png)

