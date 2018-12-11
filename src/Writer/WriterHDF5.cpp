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
#include <chrono>
#include <cmath>
#include <algorithm>
#include "../Exceptions/ParameterExistanceError.h"

#if WITH_OPENMP
#endif

#include "WriterHDF5.h"
#include "../Version/Version.h"

using namespace std;



/** Standardconstructor
 */
WriterHDF5::WriterHDF5(): WriterHDF5("defaultOutputLocation") {
}

/** Creates a HDF5-File with the name of newOutputLocation where all the various dataobjects will be written.
 */
WriterHDF5::WriterHDF5(const string &outputLocation): Writer() {
    /* Create a new file using default properties. */
    herr_t status = -1;
    status = H5open();
    std::cout << "H5open: " << status << std::endl;

    auto now = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();

    this->outputLocation = std::string("./output/")+outputLocation+"_"+std::to_string(time)+std::string(".h5");

    create_plist = H5::FileCreatPropList::DEFAULT;


    access_plist = H5::FileAccPropList();
    access_plist.setFcloseDegree(H5F_close_degree_t::H5F_CLOSE_STRONG);
    // Getting Cache Parameters
    int nelemts;
    size_t nslots;
    size_t nbytes;  /* Size of chunk cache in bytes */
    double w0;      /* Chunk preemption policy */
    access_plist.getCache(nelemts, nslots, nbytes, w0);
    //Enlarge Cache to 3MB
    nbytes = 3*1024*1024;
    w0 = 1.0;
    access_plist.setCache(nelemts, nslots, nbytes, w0);


#if WITH_OPENMP
#pragma omp critical (HDF5_ACCESS)
#endif
    {
        H5::H5File outputFileID = H5::H5File(this->outputLocation.c_str(), H5F_ACC_TRUNC, create_plist, access_plist);
        outputFileID.close();
    }
    simulationNumber = 0;
}

/** Destructor
 */
WriterHDF5::~WriterHDF5() {
    herr_t status = -1;
    status = H5garbage_collect();
    std::cout << "Garabage collection: " << status << std::endl;
    //status = H5free_memory(NULL);
    //std::cout << "Free memory: " << status << std::endl;
    //status = H5close();
    //std::cout << "Closing: " << status << std::endl;
}

void WriterHDF5::setStringAttribute(H5::Group &_group, std::string _attributeName, std::string _attributeValue){
    // Create new dataspace for attribute
    H5::DataSpace attr_dataspace = H5::DataSpace(H5S_SCALAR);
    // Create new string datatype for attribute
    if(_attributeValue.length() == 0){
        _attributeValue = "error while writing inputs with hdf5";
    }
    H5::StrType strdatatype(H5::PredType::C_S1, _attributeValue.length()); // reserve needed space
    H5::Attribute myatt_in = _group.createAttribute(_attributeName, strdatatype, attr_dataspace);
    myatt_in.write(strdatatype, _attributeValue);
    myatt_in.close();
    strdatatype.close();
    attr_dataspace.close();
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

void WriterHDF5::saveInput(Input& input){
#if WITH_OPENMP
#pragma omp critical (HDF5_ACCESS)
#endif
    {
        //https://support.hdfgroup.org/ftp/HDF5/examples/misc-examples/stratt.cpp
        //https://support.hdfgroup.org/ftp/HDF5/current/src/unpacked/c++/examples/h5tutr_crtatt.cpp
        H5::H5File outputFileID = H5::H5File(this->outputLocation.c_str(), H5F_ACC_RDWR, create_plist, access_plist);
        H5::Group simulationGroup = outputFileID.openGroup(simulationIdentifier);
        H5::Group inputGroup = simulationGroup.openGroup("input");
        for (auto& inputElem : input.getChildren()) {
            parse(inputElem, inputGroup);
        }
        inputGroup.close();
        simulationGroup.close();
        outputFileID.close();
    }
}

void WriterHDF5::parse(Input& input, H5::Group& group){
    if(input.hasChildren()){
        H5::Group newGroup = group.createGroup(input.getName());

        for (auto& inputElem : input.getChildren()){
            parse(inputElem, newGroup);
        }
        newGroup.close();
    }
    else{
        if (input.hasValue()){
            setStringAttribute(group, input.getName(), input.getString());
        }else{
            setStringAttribute(group, input.getName(), input.getName());
        }


    }

}


void WriterHDF5::saveBuildInfo(){
#if WITH_OPENMP
#pragma omp critical (HDF5_ACCESS)
#endif
    {
        H5::H5File outputFileID= H5::H5File(this->outputLocation.c_str(), H5F_ACC_RDWR, create_plist, access_plist);
        H5::Group simulationGroup = outputFileID.openGroup(simulationIdentifier);
        H5::Group buildInfoGroup = simulationGroup.openGroup("buildInfo");

        setStringAttribute(buildInfoGroup, "GIT_VERSION", buildinfo::GIT_VERSION);
        setStringAttribute(buildInfoGroup, "GIT_SHA1", buildinfo::GIT_SHA1);
        setStringAttribute(buildInfoGroup, "OS", buildinfo::OS);
        setStringAttribute(buildInfoGroup, "OS_NAME", buildinfo::OS_NAME);
        setStringAttribute(buildInfoGroup, "FQDN", buildinfo::FQDN);
        setStringAttribute(buildInfoGroup, "BUILD_TYPE", buildinfo::BUILD_TYPE);
        setStringAttribute(buildInfoGroup, "CXX", buildinfo::CXX);

        buildInfoGroup.close();
        simulationGroup.close();
        outputFileID.close();
    }
}

void WriterHDF5::rngInformation(std::size_t &uniformGenerated, std::size_t &uniformUnused, std::size_t &normalGenerated,
                                std::size_t &normalUnused, int &seed) {
#if WITH_OPENMP
#pragma omp critical (HDF5_ACCESS)
#endif
    {
        H5::H5File outputFileID= H5::H5File(this->outputLocation.c_str(), H5F_ACC_RDWR, create_plist, access_plist);
        H5::Group simulationGroup = outputFileID.openGroup(simulationIdentifier);
        H5::Group usageGroup = simulationGroup.openGroup("usage");

        H5::Group rngGroup = usageGroup.createGroup("rngInformation");

        setIntAttribute(rngGroup, "uniformGenerated", (int) uniformGenerated);
        setIntAttribute(rngGroup, "uniformUnused", (int) uniformUnused);
        setIntAttribute(rngGroup, "normalGenerated", (int) normalGenerated);
        setIntAttribute(rngGroup, "normalUnused", (int) normalUnused);
        setIntAttribute(rngGroup, "seed", seed);

        rngGroup.close();
        usageGroup.close();
        simulationGroup.close();
        outputFileID.close();
    }
}

void WriterHDF5::saveTime(double time){
#if WITH_OPENMP
#pragma omp critical (HDF5_ACCESS)
#endif
    {
        H5::H5File outputFileID= H5::H5File(this->outputLocation.c_str(), H5F_ACC_RDWR, create_plist, access_plist);
        H5::Group simulationGroup = outputFileID.openGroup(simulationIdentifier);
        H5::Group usageGroup = simulationGroup.openGroup("usage");

        setDoubleAttribute(usageGroup, "Simulation Time", time);

        usageGroup.close();
        simulationGroup.close();
        outputFileID.close();
    }
}


void WriterHDF5::addSimulation(std::string simulationIdentifier) {
#if WITH_OPENMP
#pragma omp critical (HDF5_ACCESS)
#endif
    {
        this->simulationIdentifier = simulationIdentifier + "_" +  std::to_string(simulationNumber);
        simulationNumber++;
        H5::H5File outputFileID= H5::H5File(this->outputLocation.c_str(), H5F_ACC_RDWR, create_plist, access_plist);
        H5::Group simulationGroup = outputFileID.createGroup(this->simulationIdentifier);

        H5::Group outputGroup = simulationGroup.createGroup("output");
        outputGroup.close();
        H5::Group usageGroup = simulationGroup.createGroup("usage");
        usageGroup.close();
        H5::Group buildInfoGroup = simulationGroup.createGroup("buildInfo");
        buildInfoGroup.close();
        H5::Group inputGroup = simulationGroup.createGroup("input");
        inputGroup.close();
        simulationGroup.close();
        outputFileID.close();
    }
}



void WriterHDF5::addQoI(string method, QuantityOfInterest::Quantity quantity, int groupID,
                        std::vector<std::vector<double>> *newVector, string &name_) {
#if WITH_OPENMP
#pragma omp critical (HDF5_ACCESS)
#endif
    {
        H5::H5File outputFileID= H5::H5File(this->outputLocation.c_str(), H5F_ACC_RDWR, create_plist, access_plist);
        H5::Group simulationGroup = outputFileID.openGroup(simulationIdentifier);
        H5::Group outputGroup = simulationGroup.openGroup("output");

        H5::Group quantityGroup;

        herr_t status = 0;
        /* Surpress printing of HDF5 error. It's handled ... */
        H5Eset_auto(H5E_DEFAULT, NULL, NULL);
        status = H5Lget_info (outputGroup.getId(), name_.c_str(), 0, H5P_DEFAULT);
        if(status != -1){
            quantityGroup = outputGroup.openGroup(name_);
        }
        else{
            quantityGroup = outputGroup.createGroup(name_);
            setIntAttribute(quantityGroup, "Group To Track", groupID);
            setStringAttribute(quantityGroup, "Quantity", QuantityOfInterest::quantityToString.at(quantity));
        }


        addDataSet(quantityGroup, method, newVector);

        quantityGroup.close();
        outputGroup.close();
        simulationGroup.close();
        outputFileID.close();
    }
}

void WriterHDF5::addDataSet(H5::Group group, std::string name, std::vector<std::vector<double>>* data){
    // wir benutzen max 2 Dimensionale matrizen --> RANK = 2
    const int RANK = 2;
    std::size_t ROWS = data->size();
    std::size_t COLUMNS = Util::getMaxDimensions(data);


    herr_t      status;
    /* Surpress printing of HDF5 error. It's handled ... */
    H5Eset_auto(H5E_DEFAULT, NULL, NULL);
    status = H5Lget_info (group.getId(), name.c_str(), 0, H5P_DEFAULT);

    // If the Dataset has never been writen before, we create a dataset with respective names and properties
    if(status != -1){
        /* Surpress printing of HDF5 error. It's handled ... */
        //H5::DataSet dataSet = group.openDataSet(name.c_str()); wird inzeile 331 geöffnet?
    }
    else
    {
        /* group does not exists, create it */
        // Create empty DataSpace
        hsize_t dims[RANK] = {0, 0};
        hsize_t maxdims[RANK] = {H5S_UNLIMITED, H5S_UNLIMITED};
        H5::DataSpace zeroSpace(RANK, dims, maxdims); /** called with rank and dimension */

        // Create chunking --> So we can write in blocks
        // @TODO : Die Chunk Größe beeinflusst die Performance ... Welcher Wert wäre hier optimal?
        hid_t plist = H5Pcreate(H5P_DATASET_CREATE);
        H5Pset_layout(plist, H5D_CHUNKED);
        hsize_t chunk_dims[RANK] = {ROWS, std::min(COLUMNS, std::size_t(2000))};//COLUMNS}; set chunks to static length of 2000
        H5Pset_chunk(plist, RANK, chunk_dims);

        htri_t avail = H5Zfilter_avail(H5Z_FILTER_DEFLATE);
        if(avail && COLUMNS > 1500){
            H5Pset_deflate (plist, 9);
        }

        double fill_val = std::nan("");
        H5Pset_fill_value(plist, H5T_NATIVE_DOUBLE, &fill_val);

        H5::DataSet dataSet = group.createDataSet(name.c_str(), H5::PredType::NATIVE_DOUBLE, zeroSpace,
                                                  plist);
        // Clean up
        dataSet.close();
        zeroSpace.close();
    }

    // Generate Memory Space
    hsize_t dims[RANK] = {ROWS, COLUMNS};
    hsize_t maxdims[RANK] = {H5S_UNLIMITED, H5S_UNLIMITED};
    H5::DataSpace memorySpace(RANK, dims, maxdims); /** called with rank and dimension */

    // Get the Dataset and its size
    H5::DataSet dataSet = group.openDataSet(name.c_str());
    H5::DataSpace dataSpace = dataSet.getSpace();
    hsize_t actual_dim[RANK];
    H5Sget_simple_extent_dims(dataSpace.getId(), actual_dim, NULL);

    // Extend the dataset
    hsize_t new_size[RANK];
    new_size[0] = actual_dim[0] + ROWS;
    if (actual_dim[1] > COLUMNS) {
        new_size[1] = actual_dim[1];
    } else {
        new_size[1] = COLUMNS;
    }
    dataSet.extend(new_size);

    // select hyperslab.
    H5::DataSpace fileSpace = dataSet.getSpace();
    hsize_t offset[RANK] = {actual_dim[0], 0};
    hsize_t dims1[RANK] = {ROWS, COLUMNS};
    fileSpace.selectHyperslab(H5S_SELECT_SET, dims1, offset);

    //Copying the vector ... TODO: Is there a better way? Eg. Boost Multi-Arrays?
    double *dataToWrite = new double[ROWS * COLUMNS];
    for (std::size_t i = 0; i < ROWS; i++) {
        for (std::size_t j = 0; j < COLUMNS; j++) {
            if (j < data->at(i).size()) {
                dataToWrite[i * COLUMNS + j] = data->at(i).at(j);
            } else {
                dataToWrite[i * COLUMNS + j] = std::nan("");
            }
        }
    }


    //Write actual data
    dataSet.write(dataToWrite, H5::PredType::NATIVE_DOUBLE, memorySpace, fileSpace);


    // Close all connections to file

    fileSpace.close();
    dataSpace.close();
    dataSet.close();
    memorySpace.close();

    delete[] dataToWrite;
    dataToWrite = nullptr;
}