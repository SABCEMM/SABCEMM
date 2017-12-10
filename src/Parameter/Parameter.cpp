
/*
 * Parameter.cpp
 *
 *  Created on: 01.05.2016
 *      Author: simon
 */

#include "Parameter.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>

using namespace std;
using namespace tinyxml2;
using namespace boost;


/** Standardconstructor
 */
Parameter::Parameter() = default;

/** Constructor which initialises all values
 */
Parameter::Parameter(
                     boost::optional<int>numSteps,
                     boost::optional<double>deltaT,
                     boost::optional<string>writerClass,
                     boost::optional<string>outputname,
                     ParameterSetExcessDemandCalculator parameterSetExcessDemandCalculator,
                     boost::optional<double> startPrice,
                     vector<ParameterSetAgent> parameterSetAgent,
                     ParameterSetPriceCalculator parameterSetPriceCalculator,
                     ParameterSetRNG parameterSetRNG,
                     vector<ParameterSetDataItemCollector> parameterSetDataItemCollector,
                     ParameterSetDividend parameterSetDividend) :

    outputname(std::move(outputname)),
    parameterSetAgent(std::move(parameterSetAgent)),
    parameterSetPriceCalculator(std::move(parameterSetPriceCalculator)),
    parameterSetRNG(std::move(parameterSetRNG)),
    parameterSetDataItemCollector(std::move(parameterSetDataItemCollector)),
    parameterSetDividend(std::move(parameterSetDividend)),
    parameterSetExcessDemandCalculator(std::move(parameterSetExcessDemandCalculator)),
    writerClass(std::move(writerClass)),
    numSteps(std::move(numSteps)),
    deltaT(std::move(deltaT)),
    startPrice(std::move(startPrice))
{ }

/** Performs a consistency check on the parameter object.
 * It only checks if all necessary parameters are set, it is NOT checked weather the parameters make sens.
 * This is done at object creation/initialisation mostly in the Simulation.cpp.
 * \return True if all necessary parameters are set, False if some parameters are missing.
 */
bool Parameter::validate()
{
#define REQUIRED(variable) if(!(variable)){cerr << "%%% " << #variable << " not specified. %%%" << endl; return false;}
    //Validate all direct Parameters

    REQUIRED(numSteps);
    REQUIRED(deltaT);
    REQUIRED(writerClass);
    REQUIRED(outputname);
    REQUIRED(startPrice);

    // Validate Sets
    if(parameterSetAgent.empty()){
        cerr << "%%% Agents not specified. %%%" << endl;
        return false;
    }
    for (auto &i : parameterSetAgent) {
        if(!i.validate()) return false;
    }
    for (auto &i : parameterSetDataItemCollector) {
        if(!i.validate()) return false;
    }

    // DIFFICULT!! Dependencies between parameter sets
    for (auto &i : parameterSetAgent) {
        if(*(i.type) == "AgentHarras"){
            REQUIRED(parameterSetPriceCalculator.marketDepth);
        }
        if(*(i.type) == "AgentRII" || *(i.type) == "AgentEMB"){
            REQUIRED(parameterSetDividend.Z1);
        }
        if(*(i.type) == "AgentRII" || *(i.type) == "AgentEMB" ||
                *(i.type) == "AgentCrossWealth"){
            if(!parameterSetDividend.validate()) return false;
        }
    }

    if(!parameterSetRNG.validate()) return false;
    if(!parameterSetPriceCalculator.validate()) return false;
    if(!parameterSetExcessDemandCalculator.validate()) return false;

    return true;
}

/** Read a parameter file and generate valid parameter objetcs.
 * Parameter objects are not checked for consistency. This can be done via validate().
 * \note All known parameters are read from the file. Additional/Unnecessary parameters should be ignored/go unused
 * in the simulation.
 * \param filename Path to the parameter file in XML format. (Either absolute or relative)
 * \return Vector containing all parameter objects
 */
vector<Parameter*> Parameter::readFromFile(string filename)
{
    // check that the file exists
    if(!std::ifstream(filename).good())
    {
        cerr << "Could not open file: "
             << filename << endl;
        exit(1);
    }
    vector<Parameter*> returnData;

    //Load XML file
    XMLDocument doc;
    doc.LoadFile(filename.c_str());

    // Get root element
    XMLElement* root = doc.RootElement(); // <settings>

    if(root == NULL)
    {
        cerr << "Could not parse input file - You may want to run xmllint "
             << "on your file to make sure it is valid XML." << endl;
        exit(1);
    }


    // Iterate over all cases
    for (XMLElement* elem = root->FirstChildElement(); elem != nullptr;
         elem = elem->NextSiblingElement()) // <case>
    {
        // Read RNGSection from file (passed to subclass)
        XMLElement* RNGSection = elem->FirstChildElement("RNGSettings");
        ParameterSetRNG parameterSetRNG = ParameterSetRNG::readFromXML(RNGSection);

        // Read priceCalculatorSection from file (passed to subclass)
        XMLElement* priceCalculatorSection = elem->FirstChildElement("priceCalculatorSettings");
        ParameterSetPriceCalculator parameterSetPriceCalculator =
                ParameterSetPriceCalculator::readFromXML(priceCalculatorSection);

        // Read dataItemCollectorClassesElement from file (passed to subclass)
        XMLElement* dataItemCollectorClassesElement =  elem->FirstChildElement("dataItemCollectorClasses");
        vector<ParameterSetDataItemCollector> parameterSetDataItemCollector =
                ParameterSetDataItemCollector::readFromXML(dataItemCollectorClassesElement);

        // Read dataItemCollectorClassesElement from file (passed to subclass)
        XMLElement* dividendElement =  elem->FirstChildElement("dividendSettings");
        ParameterSetDividend parameterSetDividend = ParameterSetDividend::readFromXML(dividendElement);

        // Read agents from file (passed to subclass)
        XMLElement* agent = elem->FirstChildElement("agents");
        vector<ParameterSetAgent> parameterSetAgent = ParameterSetAgent::readFromXML(agent);

        // Read excess demand settings
        XMLElement* excessDemandElement = elem->FirstChildElement("excessDemandCalculatorSettings");
        ParameterSetExcessDemandCalculator parameterSetExcessDemandCalculator = ParameterSetExcessDemandCalculator::readFromXML(excessDemandElement);

        // Read all single value which are stored directly in the parameter object
        optional<int> numSteps;
        if(XMLElement* numStepsElement = elem->FirstChildElement("numsteps"))
        {
            numSteps = 0; numStepsElement->QueryIntText(&*numSteps);
        }

        // has a default value - if repetitions was not specified in the input file,
        // no test cases are generated. That is counter-intuitive.
        optional<int> repetitions = 1;
        if(XMLElement* repetitionsElement = elem->FirstChildElement("repetitions"))
        {
            repetitions = 1; repetitionsElement->QueryIntText(&*repetitions);
        }

        optional<string> outname;
        if(XMLElement* outputNameElement = elem->FirstChildElement("outputname"))
        {
            outname = string(outputNameElement->GetText());
        }
        else
        {
            // outname is special (like repetitions), as it is used to generate distinct testcases.
            // A default variable is mandatory.
            cerr << "%%% outputname not specified %%%." << endl;
            cerr << "Assuming 'NONAME' as outputname" << endl;
            outname = "NONAME";
        }


        optional<double> deltaT;
        if(XMLElement* deltaTElement = elem->FirstChildElement("deltaT"))
        {
            deltaT = 0;
            deltaTElement->QueryDoubleText(&*deltaT);
        }

        optional<string> writerClass;
        if(XMLElement* writerElement = elem->FirstChildElement("writerClass"))
        {
            writerClass = string(writerElement->GetText());
        }


        optional<double> startPrice;
        if(XMLElement* startPriceElement =  elem->FirstChildElement("startPrice"))
        {
            startPrice = 0;
            startPriceElement->QueryDoubleText(&*startPrice);
        }


        //Generate as many parameter objects as needed to meet repetition argument.
        vector<Parameter*> caseParameters;

        for(int i = 1; i <= repetitions; ++i)
        {
            optional<string> actualOutputName = outname;
            *actualOutputName += "_";

            stringstream ss;
            ss << i;
            *actualOutputName += ss.str();

            // to the outputname the term _i is appended, where i is index of the iteration.
            // that resembles the prior behaviour
            Parameter* p = new Parameter(
                                         numSteps,
                                         deltaT,
                                         writerClass,
                                         actualOutputName,
                                         parameterSetExcessDemandCalculator,
                                         startPrice,
                                         parameterSetAgent,
                                         parameterSetPriceCalculator,
                                         parameterSetRNG,
                                         parameterSetDataItemCollector,
                                         parameterSetDividend);
            caseParameters.push_back(p);
        }


        // It looks like there is no nicer way to append a vector to another one
        returnData.reserve(returnData.size() + caseParameters.size());
        returnData.insert(returnData.end(), caseParameters.begin(), caseParameters.end());
    }

	return returnData;
}

/** Destructor
 */
Parameter::~Parameter() = default;

