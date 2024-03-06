cmake_minimum_required(VERSION 3.23)
project(LSMKV)

set(CMAKE_CXX_STANDARD 14)

add_executable(LSMKV main.cpp cache.h kvstore.h SSTable.h connecter.h MemTable.h SkipList.h QuadList.h QuadListNode.h kvstore_api.h MurmurHash3.h utils.h correctness.cpp test.h persistence.cpp)
add_executable(persistence cache.h kvstore.h SSTable.h connecter.h MemTable.h SkipList.h QuadList.h QuadListNode.h kvstore_api.h MurmurHash3.h utils.h test.h persistence.cpp)
add_executable(correctness cache.h kvstore.h SSTable.h connecter.h MemTable.h SkipList.h QuadList.h QuadListNode.h kvstore_api.h MurmurHash3.h utils.h test.h correctness.cpp)
add_executable(main cache.h kvstore.h SSTable.h connecter.h MemTable.h SkipList.h QuadList.h QuadListNode.h kvstore_api.h MurmurHash3.h utils.h test.h main.cpp)
add_executable(testForReport cache.h kvstore.h SSTable.h connecter.h MemTable.h SkipList.h QuadList.h QuadListNode.h kvstore_api.h MurmurHash3.h utils.h test.h testForReport.cpp)

