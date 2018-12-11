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

#include "../../src/RandomGenerator/RandomGeneratorStdLib.h"
#include "../../src/Agent/AgentLLS.h"
#include "../../src/Group/Group.h"
#include "../../src/Simulation.h"


TEST(GroupTest, dynamicGroups) {
    AgentLLS a1( new RandomGeneratorStdLib, new Price, 0, 0, NULL, 0, 0, 0, 0, 0, "logarithmic", {1}, "original", {2}, 0, 0, 0.99, 0.01, AgentLLS::MemorySpanMode::fixed, 0, 0, 0);
    a1.setStrategy(AgentLLS::LLSStrategy::RII);
    AgentLLS a2 = a1;
    a2.setStrategy(AgentLLS::LLSStrategy::RII);
    AgentLLS a3 = a1;
    a3.setStrategy(AgentLLS::LLSStrategy::EMB);
    AgentLLS a4 = a1;
    a4.setStrategy(AgentLLS::LLSStrategy::EMB);

    EXPECT_TRUE(a1.hasGroup(1));
    EXPECT_TRUE(a2.hasGroup(1));
    EXPECT_TRUE(a3.hasGroup(2));
    EXPECT_TRUE(a4.hasGroup(2));

    a3.toggleStrategy();
    a4.toggleStrategy();

    EXPECT_TRUE(a1.hasGroup(1));
    EXPECT_TRUE(a2.hasGroup(1));
    EXPECT_TRUE(a3.hasGroup(1));
    EXPECT_TRUE(a4.hasGroup(1));

    a1.toggleStrategy();
    a2.toggleStrategy();
    a3.toggleStrategy();
    a4.toggleStrategy();

    EXPECT_TRUE(a1.hasGroup(2));
    EXPECT_TRUE(a2.hasGroup(2));
    EXPECT_TRUE(a3.hasGroup(2));
    EXPECT_TRUE(a4.hasGroup(2));
}

TEST(GroupTest, hasGroup) {

    // Create group and add group 0, 1, 2, 3
    Group group;
    for (int i = 0; i < 4; i++){
        group.addGroup(i);
    }

    // Assert all group vector has the right size
    ASSERT_EQ(group.groups_.size(), 4);

    // Test if group 0, 1, 2, 3 are members
    for(int i = 0; i < 4; i++){
        ASSERT_TRUE(group.hasGroup(i));
    }

    // Make sure group 5, 6, 7, 8, 9 are not members
    for(int i = 5; i < 10; i++){
        ASSERT_FALSE(group.hasGroup(i));
    }

}

TEST(GroupTest, addGroup) {
    Group group;

    for (int i = 0; i < 4; i++){
        group.addGroup(i);
    }

    // Assert all group vector has the right size
    ASSERT_EQ(group.groups_.size(), 4);


    for (int i = 0; i < 4; i++){
        std::vector<int>::iterator it = std::find (group.groups_.begin(), group.groups_.end(), i);

        ASSERT_TRUE(it != group.groups_.end());
    }

}

TEST(GroupTest, setGroups) {
    Group group;

    std::vector<int> groupVector;
    for (int i = 0; i < 4; i++){
        groupVector.push_back(i);
    }

    group.setGroups(groupVector);

    ASSERT_EQ(group.groups_, groupVector);
}

TEST(GroupTest, removeGroup) {
    Group group;

    for (int i = 0; i < 10; i++){
        group.addGroup(i);
    }

    group.removeGroup(0);
    group.removeGroup(9);
    group.removeGroup(4);

    for (int i = 0; i < 10; i++){
        if(i == 0 || i == 9 || i == 4){
            ASSERT_FALSE(group.hasGroup(i));
        }
        else {
            ASSERT_TRUE(group.hasGroup(i));
        }
    }

}

TEST(GroupTest, resetGroups) {
    Group group;

    std::vector<int> groupVector;

    for (int i = 0; i < 4; i++){
        groupVector.push_back(i);
    }

    group.setGroups(groupVector);

    group.resetGroups();

    ASSERT_EQ(group.groups_.size(), 0);
}

TEST(GroupTest, getGroups) {
    Group group;

    std::vector<int> groupVector;

    for (int i = 0; i < 4; i++){
        groupVector.push_back(i);
    }

    group.setGroups(groupVector);

    std::vector<int> const * testGroups;

    testGroups = group.getGroups();

    ASSERT_EQ(*testGroups, groupVector);
}
