add_rules("mode.debug", "mode.release")
add_requires("sfml")

target("Base-Quadtree")
    set_kind("binary")
    add_includedirs("include")
    add_files("main.cpp")
    add_files("src/*.cpp")
    add_packages("sfml")