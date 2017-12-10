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
#include <vector>
#include <string>
#include <H5Cpp.h>
#include <ctime>
#include <cmath>
#include <sys/types.h>
#include <unistd.h>
#include <boost/core/ignore_unused.hpp>

#include "WriterHDF5.h"
#include "../Version/Version.h"


#include <chrono>
#include <cstddef> //for std::size_t



/** Standardconstructor
 */
WriterHDF5::WriterHDF5(): WriterHDF5("defaultOutputLocation") {
}

/** Creates a HDF5-File with the name of newOutputLocation where all the various dataobjects will be written.
 */
WriterHDF5::WriterHDF5(const string &outputLocation): Writer() {
    /* Create a new file using default properties. */
    auto now = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
    pid_t processID = getpid();

    this->outputLocation = std::string("./output/")+outputLocation+"_"+std::to_string(processID)+
            "_"+std::to_string(time)+std::string(".h5");

    outputFileID = H5::H5File(this->outputLocation.c_str(), H5F_ACC_TRUNC);

    outputGroup = outputFileID.createGroup("/output");
    outputGroup.close();

    outputFileID.close();
}

/** Destructor
 */
WriterHDF5::~WriterHDF5() {
    /* Terminate access to the file. */
    outputFileID.close();
}

void WriterHDF5::vectorToFile(std::vector<double>* newVector, std::string name, int groupID,
                              Util::DataItemCollectorMethod method){
    std::string dataSetName = name + "_Group";
    if(groupID==-1){
        dataSetName += "All";
    }
    else{
        dataSetName += std::to_string(groupID);
    }
    dataSetName += "_";
    dataSetName += Util::dataItemCollectorMethodToString(method);
    vectorToFile(newVector, dataSetName);
}

void WriterHDF5::vectorToFile(std::vector<double>* newVector, std::string name, int groupID){
    std::string dataSetName = name + "_Group";
    if(groupID==-1){
        dataSetName += "All";
    }
    else{
        dataSetName += std::to_string(groupID);
    }
    vectorToFile(newVector, dataSetName);
}

void WriterHDF5::vectorToFile(std::vector<double>* newVector, std::string name) {
    // wir benutzen 1 Dimensionale Arrays --> RANK = 1
    const int RANK = 1;
    // Create unique Name
    // OpenFile and OutputGroup
    outputFileID.openFile(this->outputLocation.c_str(), H5F_ACC_RDWR);
	outputGroup = outputFileID.openGroup("/output");

    // If the DataItemCollector has never been writen before, we create a dataset with respective names and
    try {
        /* Surpress printing of HDF5 error. It's handled ... */
        H5Eset_auto (H5E_DEFAULT, NULL, NULL);
        H5::DataSet dataSet = outputGroup.openDataSet(name.c_str());
    }
    catch(H5::Exception& e){
        /* group does not exists, create it */
    // Create empty DataSpace
        hsize_t dims[RANK] = {0};
        hsize_t maxdims[RANK] = { H5S_UNLIMITED };
        H5::DataSpace zeroSpace ( RANK, dims, maxdims ); /** called with rank and dimension */

        // Create chunking --> So we can write in blocks
        // @TODO : Die Chunk Größe beeinflusst die Performance ... Welcher Wert wäre hier optimal? Vermutung: Wir
        // schreiben eigentlich immer vektoren gleicher Größe. Also nehmen wir die Größe des Vektors :)
        hid_t plist = H5Pcreate(H5P_DATASET_CREATE);
        H5Pset_layout(plist, H5D_CHUNKED);
        hsize_t chunk_dims[RANK] = {newVector->size()};
        H5Pset_chunk(plist, RANK, chunk_dims);

        H5::DataSet dataSet = outputGroup.createDataSet(name.c_str(), H5::PredType::NATIVE_DOUBLE, zeroSpace, plist);

        //H5::DataSet dataSet(outputGroup.createDataSet(dataSetName.c_str(), H5::PredType::NATIVE_DOUBLE, zeroSpace));


        // Set Name Attribute
        H5::DataSpace name_attr_dataspace = H5::DataSpace(H5S_SCALAR);
        H5::StrType strdatatype(H5::PredType::C_S1, 256); // of length 256 characters
        H5::Attribute name_myatt_in = dataSet.createAttribute("name", strdatatype, name_attr_dataspace);
        name_myatt_in.write(strdatatype, name);

        // Clean up
        dataSet.close();
        zeroSpace.close();
    }

    // Generate Memory Space
    hsize_t dims[RANK] = {newVector->size() };
    hsize_t maxdims[RANK] = { H5S_UNLIMITED };
    H5::DataSpace memorySpace ( RANK, dims, maxdims ); /** called with rank and dimension */

    // Get the Dataset and its size
    H5::DataSet dataSet = outputGroup.openDataSet(name.c_str());
    H5::DataSpace dataSpace = dataSet.getSpace();
    const hsize_t actual_dim = static_cast<hsize_t>(dataSpace.getSimpleExtentNpoints());

    // Extend the dataset
    hsize_t new_size[RANK];
    new_size[0] = actual_dim + newVector->size();
    dataSet.extend(new_size);

    // select hyperslab.
    H5::DataSpace fileSpace = dataSet.getSpace();
    hsize_t offset[RANK] = { actual_dim };
    hsize_t dims1[RANK] = { newVector->size() };
    fileSpace.selectHyperslab(H5S_SELECT_SET, dims1, offset);

    //Write actual data
    dataSet.write(newVector->data(), H5::PredType::NATIVE_DOUBLE, memorySpace, fileSpace);

    // Close all connections to file
    fileSpace.close();
    dataSpace.close();
    dataSet.close();
    memorySpace.close();

	outputGroup.close();
    outputFileID.close();
}

void WriterHDF5::matrixToFile(std::vector<std::vector<double>>* newVector, std::string name, int groupID){
    std::string dataSetName = name + "_Group";
    if(groupID==-1){
        dataSetName += "All";
    }
    else{
        dataSetName += std::to_string(groupID);
    }
    matrixToFile(newVector, dataSetName);
}
void WriterHDF5::matrixToFile(std::vector<std::vector<double>>* newVector, std::string name, int groupID,
                              Util::DataItemCollectorMethod method){
    std::string dataSetName = name + "_Group";
    if(groupID==-1){
        dataSetName += "All";
    }
    else{
        dataSetName += std::to_string(groupID);
    }
    dataSetName += "_";
    dataSetName += Util::dataItemCollectorMethodToString(method);
    matrixToFile(newVector, dataSetName);
}

void WriterHDF5::matrixToFile(std::vector<std::vector<double>>* newVector, std::string name){
    const int RANK = 2;
    std::size_t ROWS = newVector->size();
    std::size_t COLUMNS = Util::getMaxDimensions(newVector);
    // Create unique Name


    // OpenFile and OutputGroup
    outputFileID.openFile(this->outputLocation.c_str(), H5F_ACC_RDWR);
    outputGroup = outputFileID.openGroup("/output");

    // If the DataItemCollector has never been writen before, we create a dataset with respective names and
    try {
        /* Surpress printing of HDF5 error. It's handled ... */
        H5Eset_auto (H5E_DEFAULT, NULL, NULL);
        H5::DataSet dataSet = outputGroup.openDataSet(name.c_str());
    }
    catch(H5::Exception& e){
        /* group does not exists, create it */
        // Create empty DataSpace
        hsize_t dims[RANK] = {0,0};
        hsize_t maxdims[RANK] = { H5S_UNLIMITED, H5S_UNLIMITED };
        H5::DataSpace zeroSpace ( RANK, dims, maxdims ); /** called with rank and dimension */

        // Create chunking --> So we can write in blocks
        // @TODO : Die Chunk Größe beeinflusst die Performance ... Welcher Wert wäre hier optimal?
        hid_t plist = H5Pcreate(H5P_DATASET_CREATE);
        H5Pset_layout(plist, H5D_CHUNKED);
        hsize_t chunk_dims[RANK] = {ROWS, COLUMNS};
        H5Pset_chunk(plist, RANK, chunk_dims);

        double fill_val = std::nan("");
        H5Pset_fill_value( plist, H5T_NATIVE_DOUBLE , &fill_val );

        H5::DataSet dataSet = outputGroup.createDataSet(name.c_str(), H5::PredType::NATIVE_DOUBLE, zeroSpace, plist);

        //H5::DataSet dataSet(outputGroup.createDataSet(dataSetName.c_str(), H5::PredType::NATIVE_DOUBLE, zeroSpace));

        // Set Name Attribute
        H5::DataSpace name_attr_dataspace = H5::DataSpace(H5S_SCALAR);
        H5::StrType strdatatype(H5::PredType::C_S1, 256); // of length 256 characters
        H5::Attribute name_myatt_in = dataSet.createAttribute("name", strdatatype, name_attr_dataspace);
        name_myatt_in.write(strdatatype, name);

        // Clean up
        dataSet.close();
        zeroSpace.close();
    }

    // Generate Memory Space
    hsize_t dims[RANK] = {ROWS, COLUMNS };
    hsize_t maxdims[RANK] = { H5S_UNLIMITED, H5S_UNLIMITED };
    H5::DataSpace memorySpace ( RANK, dims, maxdims ); /** called with rank and dimension */

    // Get the Dataset and its size
    H5::DataSet dataSet = outputGroup.openDataSet(name.c_str());
    H5::DataSpace dataSpace = dataSet.getSpace();
    hsize_t actual_dim[RANK];
    H5Sget_simple_extent_dims(dataSpace.getId(), actual_dim, NULL);

    // Extend the dataset
    hsize_t new_size[RANK];
    new_size[0] = actual_dim[0]+ROWS;
    if (actual_dim[1] > COLUMNS){
        new_size[1] = actual_dim[1];
    }else{
        new_size[1] = COLUMNS;
    }
    dataSet.extend(new_size);

    // select hyperslab.
    H5::DataSpace fileSpace = dataSet.getSpace();
    hsize_t offset[RANK] = { actual_dim[0], 0};
    hsize_t dims1[RANK] = { ROWS, COLUMNS};
    fileSpace.selectHyperslab(H5S_SELECT_SET, dims1, offset);

    //Copying the vector ... TODO: Is there a better way? Eg. Boost Multi-Arrays?
    double* data = new double[ROWS*COLUMNS];
    for(std::size_t i = 0; i < ROWS;i++){
        for(std::size_t j= 0; j< COLUMNS; j++){
            if(j<newVector->at(i).size()){
                data[i*COLUMNS + j] = newVector->at(i).at(j);
            }
            else{
                data[i*COLUMNS + j] = std::nan("");
            }
        }
    }


    //Write actual data
    dataSet.write(data, H5::PredType::NATIVE_DOUBLE, memorySpace, fileSpace);

    // Close all connections to file
    fileSpace.close();
    dataSpace.close();
    dataSet.close();
    memorySpace.close();

    outputGroup.close();
    outputFileID.close();
}

void WriterHDF5::arrayToFile(double* newArray, int arraySize, std::string name, int groupID) {
    /// @todo Implement if necessary
    boost::ignore_unused(newArray, arraySize, name, groupID);
	throw("Not yet implemented!");
}

void WriterHDF5::setStringAttribute(H5::Group &_group, std::string _attributeName, std::string _attributeValue){
    // Create new dataspace for attribute
	H5::DataSpace attr_dataspace = H5::DataSpace(H5S_SCALAR);
    // Create new string datatype for attribute
	H5::StrType strdatatype(H5::PredType::C_S1, 256); // of length 256 characters
    H5::Attribute myatt_in = _group.createAttribute(_attributeName, strdatatype, attr_dataspace);
    myatt_in.write(strdatatype, _attributeValue);
}

void WriterHDF5::setDoubleAttribute(H5::Group &_group, std::string _attributeName, double _attributeValue){
    // Create new dataspace for attribute
    H5::DataSpace attr_dataspace = H5::DataSpace(H5S_SCALAR);
    H5::Attribute myatt_in = _group.createAttribute(_attributeName, H5::PredType::NATIVE_DOUBLE, attr_dataspace);
    myatt_in.write(H5::PredType::NATIVE_DOUBLE, &_attributeValue);
}

void WriterHDF5::setIntAttribute(H5::Group &_group, std::string _attributeName, int _attributeValue){
    // Create new dataspace for attribute
    H5::DataSpace attr_dataspace = H5::DataSpace(H5S_SCALAR);
    H5::Attribute myatt_in = _group.createAttribute(_attributeName, H5::PredType::NATIVE_INT, attr_dataspace);
    myatt_in.write(H5::PredType::NATIVE_INT, &_attributeValue);
}

void WriterHDF5::saveInput(Parameter* newParameter){
	//https://support.hdfgroup.org/ftp/HDF5/examples/misc-examples/stratt.cpp
	//https://support.hdfgroup.org/ftp/HDF5/current/src/unpacked/c++/examples/h5tutr_crtatt.cpp
    outputFileID.openFile(this->outputLocation.c_str(), H5F_ACC_RDWR);
	H5::Group inputGroup = outputFileID.createGroup("/input");

    // General Parameters
    if(newParameter->outputname){
        setStringAttribute(inputGroup,"outputname", *(newParameter->outputname));
    }
    if(newParameter->writerClass){
        setStringAttribute(inputGroup,"writerClass", *(newParameter->writerClass));
    }
    if(newParameter->numSteps){
        setIntAttribute(inputGroup, "numSteps", static_cast<int>(*(newParameter->numSteps)));
    }
    if(newParameter->deltaT){
        setDoubleAttribute(inputGroup, "deltaT", *(newParameter->deltaT));
    }
    if(newParameter->startPrice){
        setDoubleAttribute(inputGroup, "startPrice", *(newParameter->startPrice));
    }


    H5::Group EDGroup = inputGroup.createGroup("SettingsExcessDemandCalculator");
    ParameterSetExcessDemandCalculator& p = newParameter->parameterSetExcessDemandCalculator;
    if(p.excessDemandCalculatorClass){
        setStringAttribute(EDGroup,"excessDemandCalculatorClass", *(p.excessDemandCalculatorClass));
    }
    EDGroup.close();


    H5::Group dividendGroup = inputGroup.createGroup("SettingsDividend");
    if(newParameter->parameterSetDividend.Z1){
        setDoubleAttribute(dividendGroup, "Z1", *(newParameter->parameterSetDividend.Z1));
    }
    if(newParameter->parameterSetDividend.Z2){
        setDoubleAttribute(dividendGroup, "Z2", *(newParameter->parameterSetDividend.Z2));
    }
    if(newParameter->parameterSetDividend.interestRate){
        setDoubleAttribute(dividendGroup, "interestRate", *(newParameter->parameterSetDividend.interestRate));
    }
    if(newParameter->parameterSetDividend.initialDividend){
        setDoubleAttribute(dividendGroup, "initialDividend", *(newParameter->parameterSetDividend.initialDividend));
    }
    dividendGroup.close();

    // ParameterSetRNG
    H5::Group RNGGroup = inputGroup.createGroup("SettingsRNG");
    if(newParameter->parameterSetRNG.type){
        setStringAttribute(RNGGroup,"randomGeneratorClass", *(newParameter->parameterSetRNG.type));
    }
    if(newParameter->parameterSetRNG.seed){
        setIntAttribute(RNGGroup, "seed", *(newParameter->parameterSetRNG.seed));
    }
    if(newParameter->parameterSetRNG.poolSizeNormal){
        setIntAttribute(RNGGroup, "poolSizeNormal", static_cast<int>(*(newParameter->parameterSetRNG.poolSizeNormal)));
    }
    if(newParameter->parameterSetRNG.poolSizeUniform){
        setIntAttribute(RNGGroup, "poolSizeUniform", static_cast<int>(*(newParameter->parameterSetRNG.poolSizeUniform)));
    }
    if(newParameter->parameterSetRNG.enablePool){
        setIntAttribute(RNGGroup, "enablePool", *(newParameter->parameterSetRNG.enablePool));
    }
    if(newParameter->parameterSetRNG.fileName){
        setStringAttribute(RNGGroup,"fileName", *(newParameter->parameterSetRNG.fileName));
    }
    RNGGroup.close();

    // ParameterSetPriceCalculator
    H5::Group priceCalculatorGroup = inputGroup.createGroup("SettingsPriceCalculator");
    if(newParameter->parameterSetPriceCalculator.type){
        setStringAttribute(priceCalculatorGroup,"priceCalculatorClass",
                           *(newParameter->parameterSetPriceCalculator.type));
    }
    if(newParameter->parameterSetPriceCalculator.epsilon){
        setDoubleAttribute(priceCalculatorGroup, "epsilon", *(newParameter->parameterSetPriceCalculator.epsilon));
    }
    if(newParameter->parameterSetPriceCalculator.maxIterations){
        setIntAttribute(priceCalculatorGroup, "maxIterations",
                        *(newParameter->parameterSetPriceCalculator.maxIterations));
    }
    if(newParameter->parameterSetPriceCalculator.lowerBound){
        setDoubleAttribute(priceCalculatorGroup, "lowerBound",
                           *(newParameter->parameterSetPriceCalculator.lowerBound));
    }
    if(newParameter->parameterSetPriceCalculator.upperBound){
        setDoubleAttribute(priceCalculatorGroup, "upperBound", *(newParameter->parameterSetPriceCalculator.upperBound));
    }
    if(newParameter->parameterSetPriceCalculator.theta){
        setDoubleAttribute(priceCalculatorGroup, "theta", *(newParameter->parameterSetPriceCalculator.theta));
    }
    if(newParameter->parameterSetPriceCalculator.fFunction){
        setStringAttribute(priceCalculatorGroup,"fFunction", *(newParameter->parameterSetPriceCalculator.fFunction));
    }
    if(newParameter->parameterSetPriceCalculator.fConstant){
        setDoubleAttribute(priceCalculatorGroup, "fConstant", *(newParameter->parameterSetPriceCalculator.fConstant));
    }
    if(newParameter->parameterSetPriceCalculator.gFunction){
        setStringAttribute(priceCalculatorGroup,"gFunction", *(newParameter->parameterSetPriceCalculator.gFunction));
    }
    if(newParameter->parameterSetPriceCalculator.gConstant){
        setDoubleAttribute(priceCalculatorGroup, "gConstant", *(newParameter->parameterSetPriceCalculator.gConstant));
    }
    if(newParameter->parameterSetPriceCalculator.marketDepth){
        setDoubleAttribute(priceCalculatorGroup, "marketDepth",
                           *(newParameter->parameterSetPriceCalculator.marketDepth));
    }
    if(newParameter->parameterSetPriceCalculator.noiseFactor){
        setDoubleAttribute(priceCalculatorGroup, "noiseFactor",
                           *(newParameter->parameterSetPriceCalculator.noiseFactor));
    }
    if(newParameter->parameterSetPriceCalculator.mu){
        setDoubleAttribute(priceCalculatorGroup, "mu",
                           *(newParameter->parameterSetPriceCalculator.mu));
    }
    priceCalculatorGroup.close();

    H5::Group agentsGroup = inputGroup.createGroup("SettingsAgents");
    // ParameterSetAgent
    for (std::size_t i = 0; i < newParameter->parameterSetAgent.size(); ++i) {
        H5::Group agent_i = agentsGroup.createGroup(std::string("Agent_")+std::to_string(i));

        if(newParameter->parameterSetAgent.at(i).type){
            setStringAttribute(agent_i,"agentClass", *(newParameter->parameterSetAgent.at(i).type));
        }
        if(newParameter->parameterSetAgent.at(i).count){
            setIntAttribute(agent_i, "count", static_cast<int>(*(newParameter->parameterSetAgent.at(i).count)));
        }
        if(newParameter->parameterSetAgent.at(i).cash){
            setDoubleAttribute(agent_i, "cash", *(newParameter->parameterSetAgent.at(i).cash));
        }
        if(newParameter->parameterSetAgent.at(i).stock){
            setDoubleAttribute(agent_i, "stock", *(newParameter->parameterSetAgent.at(i).stock));
        }
        if(newParameter->parameterSetAgent.at(i).C1){
            setDoubleAttribute(agent_i, "C1", *(newParameter->parameterSetAgent.at(i).C1));
        }
        if(newParameter->parameterSetAgent.at(i).C2){
            setDoubleAttribute(agent_i, "C2", *(newParameter->parameterSetAgent.at(i).C2));
        }
        if(newParameter->parameterSetAgent.at(i).C3){
            setDoubleAttribute(agent_i, "C3", *(newParameter->parameterSetAgent.at(i).C3));
        }
        if(newParameter->parameterSetAgent.at(i).threshold){
            setDoubleAttribute(agent_i, "threshold", *(newParameter->parameterSetAgent.at(i).threshold));
        }
        if(newParameter->parameterSetAgent.at(i).g){
            setDoubleAttribute(agent_i, "g", *(newParameter->parameterSetAgent.at(i).g));
        }
        if(newParameter->parameterSetAgent.at(i).alpha){
            setDoubleAttribute(agent_i, "alpha", *(newParameter->parameterSetAgent.at(i).alpha));
        }
        if(newParameter->parameterSetAgent.at(i).neighbourhoodGeneratorClass){
            setStringAttribute(agent_i,"neighbourhoodGeneratorClass",
                               *(newParameter->parameterSetAgent.at(i).neighbourhoodGeneratorClass));
        }
        if(newParameter->parameterSetAgent.at(i).A1){
            setDoubleAttribute(agent_i, "A1", *(newParameter->parameterSetAgent.at(i).A1));
        }
        if(newParameter->parameterSetAgent.at(i).A2){
            setDoubleAttribute(agent_i, "A2", *(newParameter->parameterSetAgent.at(i).A2));
        }
        if(newParameter->parameterSetAgent.at(i).b1){
            setDoubleAttribute(agent_i, "b1", *(newParameter->parameterSetAgent.at(i).b1));
        }
        if(newParameter->parameterSetAgent.at(i).b2){
            setDoubleAttribute(agent_i, "b2", *(newParameter->parameterSetAgent.at(i).b2));
        }
        if(newParameter->parameterSetAgent.at(i).gamma){
            setDoubleAttribute(agent_i, "gamma", *(newParameter->parameterSetAgent.at(i).gamma));
        }
        if(newParameter->parameterSetAgent.at(i).stdNoiseSigma){
            setDoubleAttribute(agent_i, "stdNoiseSigma", *(newParameter->parameterSetAgent.at(i).stdNoiseSigma));
        }
        if(newParameter->parameterSetAgent.at(i).riskTolerance){
            setDoubleAttribute(agent_i, "riskTolerance", *(newParameter->parameterSetAgent.at(i).riskTolerance));
        }
        if(newParameter->parameterSetAgent.at(i).historyMean){
            setDoubleAttribute(agent_i, "historyMean", *(newParameter->parameterSetAgent.at(i).historyMean));
        }
        if(newParameter->parameterSetAgent.at(i).historySigma){
            setDoubleAttribute(agent_i, "historySigma", *(newParameter->parameterSetAgent.at(i).historySigma));
        }
        if(newParameter->parameterSetAgent.at(i).initialGamma){
            setDoubleAttribute(agent_i, "initialGamma", *(newParameter->parameterSetAgent.at(i).initialGamma));
        }
        if(newParameter->parameterSetAgent.at(i).memorySpan){
            setIntAttribute(agent_i, "memorySpan", *(newParameter->parameterSetAgent.at(i).memorySpan));
        }
        if(newParameter->parameterSetAgent.at(i).k){
            setDoubleAttribute(agent_i, "k", *(newParameter->parameterSetAgent.at(i).k));
        }
        // FW
        ParameterSetAgent set = newParameter->parameterSetAgent.at(i);
        if(set.eta)
            setDoubleAttribute(agent_i, "eta", *set.eta);
        if(set.switchingStrategy)
            setStringAttribute(agent_i, "switchingStrategy", *set.switchingStrategy);
        if(set.beta)
            setDoubleAttribute(agent_i, "beta", *set.beta);
        if(set.indexStrategy)
            setStringAttribute(agent_i, "indexStrategy", (*set.indexStrategy).to_string());
        if(set.alpha_w)
            setDoubleAttribute(agent_i, "alpha_w", *set.alpha_w);
        if(set.alpha_n)
            setDoubleAttribute(agent_i, "alpha_n", *set.alpha_n);
        if(set.alpha_p)
            setDoubleAttribute(agent_i, "alpha_p", *set.alpha_p);
        if(set.alpha_0)
            setDoubleAttribute(agent_i, "alpha_0", *set.alpha_0);
        if(set.nu)
            setDoubleAttribute(agent_i, "nu", *set.nu);
        if(set.sigma_c)
            setDoubleAttribute(agent_i, "sigma_c", *set.sigma_c);
        if(set.chi)
            setDoubleAttribute(agent_i, "chi", *set.chi);
        if(set.sigma_f)
            setDoubleAttribute(agent_i, "sigma_f", *set.sigma_f);
        if(set.phi)
            setDoubleAttribute(agent_i, "phi", *set.phi);
        if(set.fundamentalPrice)
            setDoubleAttribute(agent_i, "fundamentalPrice", *set.fundamentalPrice);

        if(set.groups){
            H5::Group groups_i = agent_i.createGroup(std::string("Groups"));
            for (std::size_t j = 0; j < set.groups->size(); ++j) {
                setIntAttribute(groups_i, std::string("g_")+std::to_string(j), set.groups->at(j));
            }
            groups_i.close();
        }

        agent_i.close();
    }
    agentsGroup.close();

    // ParameterSetDataItemCollector
    H5::Group dataCollectorGroup = inputGroup.createGroup("SettingsDataCollector");
    for (std::size_t i = 0; i < newParameter->parameterSetDataItemCollector.size(); ++i) {

        H5::Group dataItemCollector_i = dataCollectorGroup.createGroup(std::string("DataItemCollector_")+
                                                                               std::to_string(i));

        if(newParameter->parameterSetDataItemCollector.at(i).type){
            setStringAttribute(dataItemCollector_i,"dataItemCollectorClass",
                               *(newParameter->parameterSetDataItemCollector.at(i).type));
        }
        if(newParameter->parameterSetDataItemCollector.at(i).writeInterval){
            setIntAttribute(dataItemCollector_i, "writeInterval",
                            *(newParameter->parameterSetDataItemCollector.at(i).writeInterval));
        }
        if(newParameter->parameterSetDataItemCollector.at(i).collectInterval){
            setIntAttribute(dataItemCollector_i, "collectInterval",
                            *(newParameter->parameterSetDataItemCollector.at(i).collectInterval));
        }
        if(newParameter->parameterSetDataItemCollector.at(i).groupToTrack){
            setIntAttribute(dataItemCollector_i, "groupToTrack",
                            *(newParameter->parameterSetDataItemCollector.at(i).groupToTrack));
        }
        if(newParameter->parameterSetDataItemCollector.at(i).method){
            setStringAttribute(dataItemCollector_i, "method",
                               Util::dataItemCollectorMethodToString(*(newParameter->
                                       parameterSetDataItemCollector.at(i).method)));
        }

        dataItemCollector_i.close();
    }
    dataCollectorGroup.close();

	inputGroup.close();
    outputFileID.close();
}


void WriterHDF5::saveBuildInfo(){
    outputFileID.openFile(this->outputLocation.c_str(), H5F_ACC_RDWR);
	H5::Group buildInfoGroup = outputFileID.createGroup("/buildInfo");

	setStringAttribute(buildInfoGroup, "GIT_VERSION", buildinfo::GIT_VERSION);
	setStringAttribute(buildInfoGroup, "GIT_SHA1", buildinfo::GIT_SHA1);
	setStringAttribute(buildInfoGroup, "OS", buildinfo::OS);
	setStringAttribute(buildInfoGroup, "OS_NAME", buildinfo::OS_NAME);
	setStringAttribute(buildInfoGroup, "FQDN", buildinfo::FQDN);
	setStringAttribute(buildInfoGroup, "BUILD_TYPE", buildinfo::BUILD_TYPE);
    setStringAttribute(buildInfoGroup, "CXX", buildinfo::CXX);

	buildInfoGroup.close();
    outputFileID.close();
}

std::string WriterHDF5::getOutputLocation(){
    return outputLocation;
}

void WriterHDF5::rngInformation(std::size_t uniformPoolInitialSize, std::size_t uniformPoolSize, int uniformPoolFills,
                                std::size_t normalPoolInitialSize, std::size_t normalPoolSize, int normalPoolFills, int seed){
    outputFileID.openFile(this->outputLocation.c_str(), H5F_ACC_RDWR);

    H5::Group usageGroup;

    try {
        /* Surpress printing of HDF5 error. It's handled ... */
        H5Eset_auto (H5E_DEFAULT, NULL, NULL);
        usageGroup = outputFileID.openGroup("/usage");
    }
    catch(H5::Exception& e){
        usageGroup = outputFileID.createGroup("/usage");
    }

    H5::Group rngGroup = usageGroup.createGroup("rngInformation");

    setIntAttribute(rngGroup, "uniformPoolInitialSize", (int)uniformPoolInitialSize);
    setIntAttribute(rngGroup, "uniformPoolUnused", (int)uniformPoolSize);
    setIntAttribute(rngGroup, "uniformPoolFills", uniformPoolFills);
    setIntAttribute(rngGroup, "normalPoolInitialSize", (int)normalPoolInitialSize);
    setIntAttribute(rngGroup, "normalPoolUnused", (int)normalPoolSize);
    setIntAttribute(rngGroup, "normalPoolFills", normalPoolFills);
    setIntAttribute(rngGroup, "seed", seed);

    rngGroup.close();
    usageGroup.close();
    outputFileID.close();
}

void WriterHDF5::saveTime(double time){
    outputFileID.openFile(this->outputLocation.c_str(), H5F_ACC_RDWR);

    H5::Group usageGroup;

    try {
        /* Surpress printing of HDF5 error. It's handled ... */
        H5Eset_auto (H5E_DEFAULT, NULL, NULL);
        usageGroup = outputFileID.openGroup("/usage");
    }
    catch(H5::Exception& e){
        usageGroup = outputFileID.createGroup("/usage");
    }

    setDoubleAttribute(usageGroup, "Simulation Time", time);

    usageGroup.close();
    outputFileID.close();
}
