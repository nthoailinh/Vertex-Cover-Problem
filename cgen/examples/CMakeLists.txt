add_executable(str_id str_id.c)
add_executable(s2i_pprint s2i_pprint.c)
add_executable(s2i_pprint_custom s2i_pprint_custom.c)
add_executable(dll_songs dll_songs.c)
add_executable(gtype_demo_io gtype_demo_io.c)

foreach (p str_id s2i_pprint s2i_pprint_custom dll_songs)
 target_link_libraries(${p} bkcgen)
endforeach()

add_subdirectory(benchmarks)
add_subdirectory(OOP)
add_subdirectory(bs)
add_subdirectory(arr)
add_subdirectory(rbm)
add_subdirectory(hfunc)
add_subdirectory(p1w)
add_subdirectory(hmap)
add_subdirectory(gsl)
add_subdirectory(gvec)
add_subdirectory(hset)
add_subdirectory(rbs)
add_subdirectory(gtype)
add_subdirectory(utils)