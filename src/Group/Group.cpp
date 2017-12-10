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
 * @file Group.cpp
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include <algorithm>
#include "Group.h"


/** Constructor for Group
 */
 Group::Group(){
    groups_.clear();
}

/** Check if the object is memeber of a specific group.
 * \param groupID Group ID which the object might have.
 * \return True if Object has Group, False if not.
 */
bool Group::hasGroup(int& groupID) {


    if(groupID == -1){
        return true;
    }

     // Search for groupID in groups vector and return
    auto it = std::find (groups_.begin(), groups_.end(), groupID);

    return it != groups_.end();


}

/** Add a group to the object.
 * \param groupID ID of the Group which to add.
 */
void Group::addGroup(int groupID) {


    groups_.push_back(groupID);



}

/** Set group array, instead of single groups. ATTENTION: Clears all preexisting groups.
 * \param groups
 */
void Group::setGroups(std::vector<int> groups) {


    groups_ = std::move(groups);



}

/** Remove the Object from a single group.
 * \param groupID
 */
void Group::removeGroup(int groupID) {


    // Search groupID and delete if possible.
    auto it = std::find (groups_.begin(), groups_.end(), groupID);

    if(it != groups_.end()){

        groups_.erase(it);


    }



}

/** Remove Object from all groups it belongs to.
 */
void Group::resetGroups() {


    groups_.clear();


}

/** Get Groups of Object. Read-Only Access!
 * \return Pointer to const group vector.
 */
std::vector<int> const * Group::getGroups(){


    return &groups_;

}
