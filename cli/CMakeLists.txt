add_executable(app main.c)
target_compile_features(app PUBLIC c_std_11)

target_link_libraries(app PRIVATE library)
