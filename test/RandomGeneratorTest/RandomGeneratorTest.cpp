/* Copyright 2017 - BSD-3-Clause
 *
 * Copyright Holder (alphabetical):
 *
 * Beikirch, Maximilian
 * Cramer, Simon
 * Frank, Martin
 * Otte, Philipp
 * Pabich, Emma
 * Trimborn, Torsten
 *
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *    disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *    following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include "gtest/gtest.h"
#include <numeric>
#include <cmath>

#include "../src/RandomGenerator/RandomGenerator.h"
#include "../src/RandomGenerator/RandomGeneratorStdLib.h"
#include "../src/RandomGenerator/RandomGeneratorStdLibOld.h"
#include "../src/RandomGenerator/RandomGeneratorRANDU.h"
#include <chrono>

#if WITH_INTEL_MKL
#include "../src/RandomGenerator/RandomGeneratorIntelMKL.h"
#endif
#if WITH_NAG
#include "../src/RandomGenerator/RandomGeneratorNAG.h"
#endif

#include "../src/Writer/WriterTxt.h"
#include <cstddef> //for std::size_t

/** Parametric Class to hold different implementations of RandomGenerator
 * \param T Different implementations of our RandomGenerator
 */
template <typename T>
	class RandomGeneratorTest : public ::testing::Test {
public:
	T randomGenerator;
};

//Register all implementations of RandomGenerator here
typedef ::testing::Types<
		RandomGeneratorStdLib,
		RandomGeneratorStdLibOld
#if WITH_NAG
		, RandomGeneratorNAG
#endif
#if WITH_INTEL_MKL
		, RandomGeneratorIntelMKL
#endif
> ImplementedRandomGenerators;
//typedef ::testing::Types<RandomGeneratorRANDU> ImplementedRandomGenerators;

//Register type test Case
TYPED_TEST_CASE(RandomGeneratorTest, ImplementedRandomGenerators);

//////////////////////////////////////////////////////////////////////////////
////////////////////////////////    TESTS    //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/**
 *
 */
TYPED_TEST(RandomGeneratorTest, getUniformRandomDouble) {

	//Generate one integer
	double testDouble = 0;
	this->randomGenerator.getUniformRandomDouble(-100,100,&testDouble);

	//Check if integer is in desired range
	EXPECT_LE(-100,testDouble);
	EXPECT_GE(100,testDouble);
}

/**
 *
 */
TYPED_TEST(RandomGeneratorTest, getUniformRandomDoubleVector) {
	double lowerBound = -10;
	double upperBound = 100;
	std::size_t count = 10000; //< amount of integers to generate

	double expectedAverage = lowerBound+(upperBound-lowerBound)/2;
	double epsilon = expectedAverage * 0.02; // 1% Error margin

	std::vector<double> testDouble; //< Vector to hold generated integers

	this->randomGenerator.getUniformRandomDouble(lowerBound,upperBound,&testDouble,count);

	//Check if each interger is between lower and upper bound. Further sum all integers and compute average
	for(std::size_t i = 0; i<testDouble.size(); i++){
		EXPECT_LE(lowerBound,testDouble[i]);
		EXPECT_GE(upperBound,testDouble[i]);
	}

	double mean = std::accumulate(testDouble.begin(),testDouble.end(),0.0)/testDouble.size();

	std::vector<double> diff(testDouble.size());
	std::transform(testDouble.begin(), testDouble.end(), diff.begin(), [mean](double x) { return x - mean; });
	double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
	double stdev = std::sqrt(sq_sum / testDouble.size());

	//Check if computed avergare is in the one percent error margin
	EXPECT_LE(expectedAverage-epsilon,mean);
	EXPECT_GE(expectedAverage+epsilon,mean);
	EXPECT_NE(stdev, 0);
}

/**
 *
 */
TYPED_TEST(RandomGeneratorTest, getUniformRandomInt) {

	//Generate one integer
	int testInt = 0;
	this->randomGenerator.getUniformRandomInt(-100,100,&testInt);

	//Check if integer is in desired range
	EXPECT_LE(-100,testInt);
	EXPECT_GE(100,testInt);
}


/**
 *
 */
TYPED_TEST(RandomGeneratorTest, getUniformRandomIntVector) {
	int lowerBound = -10;
	int upperBound = 100;
	std::size_t count = 10000; //< amount of integers to generate

	double expectedAverage = lowerBound + (upperBound-lowerBound)/2;
    double epsilon = expectedAverage * 0.04; // 4% Error margin

	std::vector<int> testInt; //< Vector to hold generated integers

	this->randomGenerator.getUniformRandomInt(lowerBound,upperBound,&testInt,count);

	//Check if each interger is between lower and upper bound. Further sum all integers and compute average
	for(std::size_t i = 0; i<testInt.size(); i++){
		EXPECT_LE(lowerBound,testInt[i]);
		EXPECT_GE(upperBound,testInt[i]);
	}
	double mean = std::accumulate(testInt.begin(),testInt.end(),0.0)/testInt.size();

	std::vector<double> diff(testInt.size());
	std::transform(testInt.begin(), testInt.end(), diff.begin(), [mean](double x) { return x - mean; });
	double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
	double stdev = std::sqrt(sq_sum / testInt.size());

	//Check if computed avergare is in the one percent error margin
	EXPECT_LE(expectedAverage-epsilon,mean);
	EXPECT_GE(expectedAverage+epsilon,mean);
	EXPECT_NE(stdev, 0);
}



/**
 *
 */
TYPED_TEST(RandomGeneratorTest, getNormalRandomVector) {
	double mu = 0.0;
	double sigma = 2.0;
    std::size_t count = 1e5; //< amount of integers to generate
    long double epsilon = 0.1; //< Allow Error of 1%
	long double expMoments[10]; //< Analytic Moment of normrnd(0,2)
	long double moments[11];

	expMoments[0] = 0;
	expMoments[1] = 4;
	expMoments[2] = 0;
	expMoments[3] = 48;
	expMoments[4] = 0;
	expMoments[5] = 960;
	expMoments[6] = 0;
	expMoments[7] = 26880;
	expMoments[8] = 0;
	expMoments[9] = 967680;

	std::vector<double> testDouble; //< Vector to hold generated integers

	this->randomGenerator.getNormalRandomDouble(mu, sigma, &testDouble ,count);

    long double mean = std::accumulate(testDouble.begin(),testDouble.end(),0.0)/testDouble.size();


	std::vector<long double> diff(testDouble.size());
	std::transform(testDouble.begin(), testDouble.end(), diff.begin(), [mean](long double x) { return x - mean; });
	long double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
	long double stdev = std::sqrt(sq_sum / testDouble.size());


	for(int j=0; j<10;j++){
		moments[j] = 0;
	}
	for(std::size_t i=0; i < testDouble.size(); i++){
		for(std::size_t j=0; j<10;j++)
		{
			moments[j] += std::pow(testDouble.at(i),(double)(j+1));
		}
	}
	for(std::size_t j=1; j<10;j+=2){
		moments[j] = moments[j]/testDouble.size();
        EXPECT_LE(abs(moments[j]-expMoments[j])/expMoments[j],0.5);
	}


	//Check if computed avergare is in the one percent error margin
	EXPECT_LE(mean,mu+epsilon);
	EXPECT_GE(mean,mu-epsilon);
	EXPECT_LE(stdev,sigma+epsilon);
	EXPECT_GE(stdev,sigma-epsilon);
	//EXPECT_LE(sixth,sixthMoment+epsilon);
	//EXPECT_GE(sixth,sixthMoment-epsilon);
}

#if ENABLE_RNGSPEEDTEST
TYPED_TEST(RandomGeneratorTest, speedTest){
    typedef std::chrono::high_resolution_clock Clock;
    std::string name = "";


    if(dynamic_cast<RandomGeneratorStdLib*>(&(this->randomGenerator)) != nullptr){
        name = "stdlib";
    }
    else if(dynamic_cast<RandomGeneratorStdLibOld*>(&(this->randomGenerator)) != nullptr){
        name = "stdlibold";
    }
    else if(dynamic_cast<RandomGeneratorRANDU*>(&(this->randomGenerator)) != nullptr){
        name = "randu";
    }
#if WITH_INTEL_MKL
    else if(dynamic_cast<RandomGeneratorIntelMKL*>(&(this->randomGenerator)) != nullptr){
        name = "intelmkl";
    }
#endif
#if WITH_NAG
    else if(dynamic_cast<RandomGeneratorNAG*>(&(this->randomGenerator)) != nullptr){
        name = "naglib";
    }
#endif
    else{
        return;
    }

    auto startTime = Clock::now();
    auto endTime = Clock::now();
    double pos;

    std::vector<double> pos2;
    std::vector<double> amount;
    std::vector<double> timings;

    int max = 21e6;
    int factor = 2;
    int min = 1e6;
    int reps = 10;

    //Generater Amounts
    amount.clear();
    int i = min;
    while (i<max){
        amount.push_back(i);
        i = i*factor;
    }

    WriterTxt writer("speedtest_"+name);
    writer.vectorToFile(&amount, "AmountOfNumbers");

    timings.clear();

    for (int j = 0; j<amount.size(); j++){
        std::vector<double> timings_;
        for (int k=0;k<reps;k++){
            startTime = Clock::now();

            for(int i=0; i<amount.at(j); i++){
                this->randomGenerator.getNormalRandomDouble(0,1,&pos);
            }

            endTime = Clock::now();

            double simulationTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() * 1e-9;
            timings_.push_back(simulationTime);
        }

        timings.push_back(Util::mean(timings_));
    }
    writer.vectorToFile(&timings, "SingleProduction_"+name);

    // Special IntelMKL Batch test:

    if (name == "intelmkl"){
        timings.clear();
        std::vector<double> pos2;

        for (int j = 0; j<amount.size(); j++){
            std::vector<double> timings_;
            for (int k=0;k<reps;k++){
                startTime = Clock::now();

                this->randomGenerator.getNormalRandomDouble(0,1,&pos2,amount.at(j));

                endTime = Clock::now();
                pos2.clear();

                double simulationTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() * 1e-9;
                timings_.push_back(simulationTime);
            }

            timings.push_back(Util::mean(timings_));
        }
        writer.vectorToFile(&timings, "BatchProduction_"+name);



    }
}
#endif

/**
TYPED_TEST(RandomGeneratorTest, writeTenSamplesToFile){
	WriterTxt writerTxt;
	writerTxt.setFolderPath("./output/");
	writerTxt.setFileExtension(".txt");
	std::vector<double> testDouble;//< Vector to hold generated integers
	int count = 1e6;
	std::string input;
	long double mu = 0.0;
	long double sigma = 2.0;

	std::cout << "Do you want to save 10 samples of 10 Million Random Numbers to disk? yes/no" << std::endl;
	std::cin >> input;

	if(input != "yes"){
		return;
	}
	for(int i=0; i<10; i++){
		this->randomGenerator.getNormalRandomDouble(mu, sigma, &testDouble ,count);
		writerTxt.vectorToFile(&testDouble,std::string(typeid(TypeParam).name())+std::to_string(i),false);
	}

}*/

TEST(RandomGeneratorSeedTest, seed){
	RandomGeneratorStdLib randomGenerator;
	std::vector<int> testSeed;
	testSeed.clear();

	//Generate 10 Seeds
	for(int i=0; i<10;i++){
		testSeed.push_back(randomGenerator.createSeed());
	}

	//Check if all Seeds are different
	for(std::size_t i=0; i<10; i++){
		ASSERT_NE(testSeed[i],0) << "Seeding returned inital value 0!";
		ASSERT_NE(testSeed[i],5465) << "Opening uRandom failed. Standard seed returned!";
		for(std::size_t j=0;j<10; j++){
			//Skip same indexes
			if(i==j) continue;
			EXPECT_NE(testSeed[i],testSeed[j]) << "Seed " << i << " ," << j << "are the same: " << testSeed[i] << ", " << testSeed[j];
		}
	}
}

TEST(RandomGeneratorRANDUTest, getUniformRandomDouble) {

    //Generate one integer
    double testDouble = 0;
    RandomGeneratorRANDU randomGenerator;
    randomGenerator.getUniformRandomDouble(-100,100,&testDouble);

    //Check if integer is in desired range
    EXPECT_LE(-100,testDouble);
    EXPECT_GE(100,testDouble);
}

TEST(RandomGeneratorRANDUTest, getUniformRandomInt) {

    //Generate one integer
    int testInt = 0;
    RandomGeneratorRANDU randomGenerator;
    randomGenerator.getUniformRandomInt(-100,100,&testInt);

    //Check if integer is in desired range
    EXPECT_LE(-100,testInt);
    EXPECT_GE(100,testInt);
}