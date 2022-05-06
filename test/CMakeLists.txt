add_executable(test-library tests.c test_unbounded_int.c)

target_compile_features(test-library PUBLIC c_std_11)

target_link_libraries(test-library PRIVATE library)
