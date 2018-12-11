/* Copyright 2018 - BSD-3-Clause
 *
 * Copyright Holder (alphabetical):
 *
 * Cramer, Simon
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
 * @author Cramer
 * @date 17 Apr 2018
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include "Input.h"
#include "../Util/Util.h"
#include "../Exceptions/ParameterExistanceError.h"
#include "../Exceptions/ParameterConversionError.h"

#include <iostream>
#include <fstream>
#include <tinyxml2/tinyxml2.h>
#include <string>
#include <set>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace tinyxml2;
using namespace std;



Input::Input(std::string name_, tinyxml2::XMLElement* el){
    element = el;
    children.clear();
    name = name_;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
}

Input::Input(std::string name): Input(name, nullptr){
}

Input::Input(): Input("null", nullptr){

}

Input::~Input(){
    children.clear();
}
Input::fromFile Input::readFromFile(std::string filename){
    fromFile returnObj;

    std::vector<Input> allInputs;

    // check that the file exists
    if(!std::ifstream(filename).good())
    {
        cerr << "Could not open file: "
             << filename << endl;
        exit(1);
    }

    // folder of the main input files
    // filenames for included input files will be given relative to folder.
    string folder = Util::removeFilename(filename);

    //Load XML file
    XMLDocument* doc = new XMLDocument;
    returnObj.doc = doc;
    doc->LoadFile(filename.c_str());

    // Get root element
    XMLElement* root = doc->RootElement(); // <settings>

    if(root == NULL)
    {
        cerr << "Could not parse input file - You may want to run xmllint "
             << "on your file to make sure it is valid XML." << endl;
        exit(1);
    }

    for(XMLElement* includeElem = root->FirstChildElement("include"); includeElem != nullptr;
        includeElem = includeElem->NextSiblingElement("include"))
    {
        std::vector<Input> includedInput = includeXML(folder, includeElem);
        allInputs.insert(allInputs.end(), includedInput.begin(), includedInput.end());
    }

    // Iterate over all cases
    for (XMLElement* caseElem = root->FirstChildElement("case"); caseElem != nullptr;
         caseElem = caseElem->NextSiblingElement("case")) // <case>
    {

        Input input("case");
        // Find all include directives
        for(XMLElement* includeElem = caseElem->FirstChildElement("include"); includeElem != nullptr;
            includeElem = includeElem->NextSiblingElement("include"))
        {
            std::vector<Input> includedInput = includeXML(folder, includeElem);
            if(includedInput.size() != 1)
                throw "included files in <case> segment may only have one a single case.";
            input.insert(includedInput.front());
        }

        for (XMLElement* paramElem = caseElem->FirstChildElement(); paramElem != nullptr;
             paramElem = paramElem->NextSiblingElement()) // <case>
        {
            if(paramElem->Name() == string("include"))
                continue;

            parse(input, paramElem);
        }

        size_t repetitions = input("repetitions") ? input["repetitions"].getSizeT() : 1;
        for(size_t k = 0; k < repetitions; k++){
            allInputs.push_back(input);
        }


    }

    returnObj.inputs = allInputs;
    if(root->FirstChildElement("writer")){
        returnObj.writerClass = root->FirstChildElement("writer")->GetText();
        std::transform(returnObj.writerClass.begin(), returnObj.writerClass.end(), returnObj.writerClass.begin(), ::tolower);
    }
    if(root->FirstChildElement("filename")){
        returnObj.filename =root->FirstChildElement("filename")->GetText();
    }
    if(root->FirstChildElement("numthreads")){
        root->FirstChildElement("numthreads")->QueryIntText(&returnObj.numThreads);
    }
    if(root->FirstChildElement("simulationsperfile")){
        int temp;
        root->FirstChildElement("simulationsperfile")->QueryIntText(&temp);
        returnObj.simulationsPerFile = std::size_t(temp);
    }

    return returnObj;
}

void Input::parse(Input& root, XMLElement* elem){
    if(!elem->FirstChildElement()){
        root.addChild(string(elem->Name()), elem);
    }
    else{
        //Check is GroupElement exists. create if not
        //Umweg über Pointer notwendig, da Referenzen nicht auf Null initialisiert werden können.
        Input* newRoot = nullptr;
        if(!root.hasChild(string(elem->Name()))){
            root.addChild(string(elem->Name()));
            newRoot = &(root.getChild(string(elem->Name())));
        }else{
            newRoot = &(root.getChild(string(elem->Name())));
        }
        //Iterate over children
        for (XMLElement* iterElem = elem->FirstChildElement(); iterElem != nullptr;
             iterElem = iterElem->NextSiblingElement()) // all other values
        {
            parse(*newRoot, iterElem);
        }
    }
}


std::vector<Input> Input::includeXML(string folder, XMLElement* includeElement)
{
    string relPath = string(includeElement->GetText());
    string absPath;
    if(folder == "")
        absPath = relPath;
    else
        absPath = folder + '/' + relPath;

    cout << "Found <include>-section. Including file '" << absPath << "'" << endl;
    Input::fromFile fileParams = readFromFile(absPath);

    return fileParams.inputs;
}

void Input::insert(Input insertValue){
    std::vector<Input>& newChildren = insertValue.getChildren();

    for (auto &child : newChildren) {
        if(!hasChild(child.getName())){
            children.push_back(child);
            /**std::vector<Input*> childrenToAppend = child->getChildren();
            children.insert(children.end(),childrenToAppend.begin(), childrenToAppend.end());**/
        }
        else{
            getChild(child.getName()).insert(child);
        }
    }
}

bool Input::hasChildren(){
    if(children.size() == 0){
        return false;
    }
    return true;
}

bool Input::hasValue(){
    if(element != nullptr && element->GetText() != NULL){
        return true;
    }
    return false;
}

std::vector<Input>& Input::getChildren(){
    return children;
}

bool Input::hasChild(std::string name) {

    if(children.size() == 0)
        return false;

    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    for (auto &child : children){
        if(child.getName() == name){
            return true;
        }
    }
    return false;
}

Input& Input::getChild(std::string name){
    Input input;
    if(! hasChild(name))
        throw ParameterExistanceError(this->name, name);


    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    for (auto &child : children){
        if(child.getName() == name){
            return child;
        }
    }
    return *this; //Never reached but surpresses warnings...
}

void Input::checkConversionError(tinyxml2::XMLError error, std::string convType){
    if (error == tinyxml2::XML_CAN_NOT_CONVERT_TEXT || error == tinyxml2::XML_NO_TEXT_NODE) throw ParameterConversionError(getName(),convType);
}


std::string Input::getString(){
    if (!hasValue()){
        throw "No Value present!";
    }
    if(element == nullptr) throw ParameterExistanceError(getName());

    std::string ret = std::string(element->GetText());

    std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
    return ret;
}
int Input::getInt(){
    if (!hasValue()){
        throw "No Value present!";
    }
    if(element == nullptr) throw ParameterExistanceError(getName());
    int intValue = 0;
    XMLError error = element->QueryIntText(&intValue);
    checkConversionError(error, "integer");
    return intValue;
}
double Input::getDouble(){
    if (!hasValue()){
        throw "No Value present!";
    }
    if(element == nullptr) throw ParameterExistanceError(getName());
    double doubleValue = 0;
    XMLError error = element->QueryDoubleText(&doubleValue);
    checkConversionError(error, "double");
    return doubleValue;
}
size_t Input::getSizeT(){
    if (!hasValue()){
        throw "No Value present!";
    }
    int temp = getInt();
    if(temp < 0) throw ParameterConversionError(getName(), "size_t");
    return static_cast<size_t>( temp );
}

std::vector<int> Input::getIntVector(){
    vector<int> ret;

    for(auto& child : children){
        if(child.hasChildren()){
            throw "GetVector only possibel if children have no children!";
        }
        ret.push_back(child.getInt());
    }
    return ret;
}

std::set<int> Input::getIntSet(){
    set<int> ret;

    for(auto& child : children){
        if(child.hasChildren()){
            throw "GetVector only possibel if children have no children!";
        }
        ret.emplace(child.getInt());
    }
    return ret;
}

bool Input::getBool(){
    if (!hasValue()){
        throw "No Value present!";
    }
    if(element == nullptr) throw ParameterExistanceError(getName());
    bool ret;
    XMLError error = element->QueryBoolText(&ret);
    checkConversionError(error, "bool");
    return ret;

}

std::string Input::getName(){
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    return name;
}

void Input::addChild(std::string name){
    return addChild(name, nullptr);
}

void Input::addChild(std::string name, tinyxml2::XMLElement* el){
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    if (hasChild(name)){
        std::cerr << "Children have to be named uniquely, or have an ID set!" << std::endl;
        throw "Children have to be named uniquely, or have an ID set!";
    }

        Input newChild(name, el) ;
        children.push_back(newChild);


}


Input& Input::operator[](std::string name){
    return getChild(name);
}


bool Input::operator()(std::string name){
    return hasChild(name);
}
