if(WITH_HDF5)
    add_library(Writer
            Writer.cpp
            WriterHDF5.cpp
            WriterNone.cpp
            WriterTxt.cpp
        )
else()
    add_library(Writer
            Writer.cpp
            WriterNone.cpp
            WriterTxt.cpp
        )
endif()

target_link_libraries(Writer Version)

add_coverage(Writer)
