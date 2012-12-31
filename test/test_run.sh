if [ ! -d ../dependancies/rbtreelib/inc ]; then
    cd ..
    echo "Loading submodules"
    git submodule init
    git submodule update
    cd test
fi


gcc -std=c99 test_memManager.c test_main.c ../dependancies/rbtreelib/src/rbtree.c ../dependancies/rbtreelib/src/rbtree_checks.c ../dependancies/rbtreelib/src/rbtree_common.c ../src/memoryTracker_list.c ../src/memoryTracker_node.c ../src/memoryTracker_stats.c ../src/memoryTracker.c -I . -I ../dependancies/rbtreelib/inc -I ../dependancies/rbtreelib/src -I ../inc -I ../src -I ../src/output_plugins -o memory_test
./memory_test
