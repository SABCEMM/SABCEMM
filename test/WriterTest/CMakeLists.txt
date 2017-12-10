if(WITH_HDF5)
    add_library(WriterTest
            WriterTxtTest.cpp
            WriterHDF5Test.cpp)
else()
    add_library(WriterTest
            WriterTxtTest.cpp)
endif()
