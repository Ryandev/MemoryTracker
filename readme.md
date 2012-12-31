#MemoryTracker
**malloc tracker. Finds all unfree'd allocs & provides stacktraces**


To use this in your project all calls to _malloc_,_free_ need to be changed to _memAlloc_,_memFree_ respectively. There is a define _MEMORY_USAGE_CHECKER_ in memoryTrackerSettings.h. set to 1 for tracker enabled. 0 for release builds: malloc/free.

Run ./example_run.sh for example.


##Usage

```
$cd $memorytracker_dir/example;
chmod +x example_run.sh;
./example_run.sh
```

##Validate

```
$cd $memorytracker_dir/test;
chmod +x test_run.sh;
./test_run.sh
```

## License

MemoryTracker is available under the MIT license. See the LICENSE file for more info.