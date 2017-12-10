add_library(Algorithms
        brent/brent.cpp
)

# silence all warnings from brent
target_compile_options(Algorithms PUBLIC -w)
