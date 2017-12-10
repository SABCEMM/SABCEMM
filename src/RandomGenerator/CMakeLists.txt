#todo: replace with target_sources() (needs newer cmake.)

if(WITH_INTEL_MKL)
        SET(RNG_MKL
		RandomGeneratorIntelMKL.cpp
	)
endif(WITH_INTEL_MKL)

if(WITH_NAG)
        SET(RNG_NAG
                RandomGeneratorNAG.cpp
        )
endif(WITH_NAG)


add_library(RandomGenerator
        RandomGenerator.cpp
        RandomGeneratorRANDU.cpp
        RandomGeneratorStdLib.cpp
        RandomGeneratorStdLibOld.cpp
	RandomGeneratorFromFile.cpp
        RandomNumberPool.cpp
        Algorithms/sabcemm_rand.cc
		${RNG_MKL}
		${RNG_NAG}
)

add_coverage(RandomGenerator)
