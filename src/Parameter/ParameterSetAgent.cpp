//
// Created by Simon Cramer on 10.05.17.
//

#include <iostream>

#include "tinyxml2/tinyxml2.h"

#include "ParameterSetAgent.h"
#include "../Agent/AgentFW.h"

using namespace std;
using namespace tinyxml2;

/** Performs a consistency check on the ParameterSetAgent object.
 * It only checks if all necessary parameters are set, it is NOT checked weather the parameters make sens.
 * This is done at object creation/initialisation mostly in the Simulation.cpp.
 * \return True if all necessary parameters are set, False if some parameters are missing.
 */
bool ParameterSetAgent::validate(){
    //
#define REQUIRED(variable) if(!(variable)){cerr << "%%% " << #variable << " not specified. %%%" << endl; return false;}

    // Test General Arguments
    REQUIRED(type);
    REQUIRED(count);

    // Test Harras
    /// @todo we should introduce a static validate function for each agent
    if( *type == "AgentHarras"){
        REQUIRED(C1);
        REQUIRED(C2);
        REQUIRED(C3);
        REQUIRED(threshold);
        REQUIRED(alpha);
        REQUIRED(neighbourhoodGeneratorClass);
    }

    // Test Cross
    if( *type == "AgentCross" || *type == "AgentCrossWealth"){
        REQUIRED(A1);
        REQUIRED(A2);
        REQUIRED(b1);
        REQUIRED(b2);
    }
    if(*type == "AgentCrossWealth"){
        REQUIRED(gamma);
    }

    // General Test if LLS
    if( *type == "AgentEMB" || *type == "AgentRII"){
        REQUIRED(stdNoiseSigma);
        REQUIRED(riskTolerance);
        REQUIRED(historyMean);
        REQUIRED(historySigma);
    }

    // Test EMB
    if( *type == "AgentEMB"){
        REQUIRED(memorySpan);
    }

    // Test RII
    if( *type == "AgentRII"){
        REQUIRED(k);
    }

    /// @todo alpha checks
    if( *type == "AgentFW") {
        REQUIRED(eta);
        REQUIRED(switchingStrategy);
        if(*switchingStrategy == "DCA")
            REQUIRED(beta);
        if(*switchingStrategy == "TPA")
            REQUIRED(nu);

        // chartist
        REQUIRED(sigma_c);
        REQUIRED(chi);

        // fundamentalist
        REQUIRED(sigma_f);
        REQUIRED(phi);
        REQUIRED(fundamentalPrice);

        REQUIRED(indexStrategy);
        if (!(*indexStrategy).any()) {
            cerr << "You need to specify at least one index strategy." << endl;
            return false;
        }
    }


    return true;
}

/** Read the content of the XML Block and generate valid ParameterSetAgent objetcs.
 * ParameterSet objects are not checked for consistency. This can be done via validate().
 * \note All known parameters are read from the file. Additional/Unnecessary parameters should be ignored/go unused
 * in the simulation.
 * \param agent XMLElement as parsed in XML from file
 * \return Vector of ParameterSetAgent objects. One for each defined agent group.
 */
vector<ParameterSetAgent> ParameterSetAgent::readFromXML(XMLElement* agent){
    vector<ParameterSetAgent> allParameterSetAgents; // return element

    if(agent)
    {
        /// @todo suggestions: find the agent name first, then find only the
        /// necessary parameters for that agent
        for(XMLElement* agentSettings = agent->FirstChildElement(); agentSettings != nullptr;
            agentSettings=agentSettings->NextSiblingElement())
        {
            auto * parameterSetAgent = new ParameterSetAgent;

            parameterSetAgent->type = agentSettings->Name();

            if(XMLElement* countElement = agentSettings->FirstChildElement("count"))
            {
                int count = 0;
                countElement->QueryIntText(&count);
                parameterSetAgent->count = count;
            }
            if(XMLElement* cashElement = agentSettings->FirstChildElement("cash"))
            {
                double cash = 0;
                cashElement->QueryDoubleText(&cash);
                parameterSetAgent->cash = cash;
            }
            if(XMLElement* stockElement = agentSettings->FirstChildElement("stock"))
            {
                double stock = 0;
                stockElement->QueryDoubleText(&stock);
                parameterSetAgent->stock = stock;
            }
            if(XMLElement* groupElement = agentSettings->FirstChildElement("Group"))
            {
                vector<int> groups;
                for(XMLElement* group = groupElement->FirstChildElement(); group != nullptr;
                    group=group->NextSiblingElement())
                {
                    int groupInt = -1;
                    group->QueryIntText(&groupInt);
                    groups.push_back(groupInt);
                }
                parameterSetAgent->groups = groups;
            }


            if(XMLElement* C1Element = agentSettings->FirstChildElement("C1"))
            {
                double C1 = 0;
                C1Element->QueryDoubleText(&C1);
                parameterSetAgent->C1 = C1;
            }
            if(XMLElement* C2Element = agentSettings->FirstChildElement("C2"))
            {
                double C2 = 0;
                C2Element->QueryDoubleText(&C2);
                parameterSetAgent->C2 = C2;
            }
            if(XMLElement* C3Element = agentSettings->FirstChildElement("C3"))
            {
                double C3 = 0;
                C3Element->QueryDoubleText(&C3);
                parameterSetAgent->C3 = C3;
            }
            if(XMLElement* thresholdElement = agentSettings->FirstChildElement("threshold"))
            {
                double threshold = 0;
                thresholdElement->QueryDoubleText(&threshold);
                parameterSetAgent->threshold = threshold;
            }
            if(XMLElement* gElement = agentSettings->FirstChildElement("g"))
            {
                double g = 0;
                gElement->QueryDoubleText(&g);
                parameterSetAgent->g = g;
            }
            if(XMLElement* alphaElement = agentSettings->FirstChildElement("alpha"))
            {
                double alpha = 0;
                alphaElement->QueryDoubleText(&alpha);
                parameterSetAgent->alpha = alpha;
            }
            if(XMLElement* neighbourGeneratorClassElement =
                    agentSettings->FirstChildElement("neighbourhoodGeneratorClass")){
                parameterSetAgent->neighbourhoodGeneratorClass = string(neighbourGeneratorClassElement->GetText());
            }


            if(XMLElement* A1Element = agentSettings->FirstChildElement("A1"))
            {
                double A1 = 0;
                A1Element->QueryDoubleText(&A1);
                parameterSetAgent->A1 = A1;
            }
            if(XMLElement* A2Element = agentSettings->FirstChildElement("A2"))
            {
                double A2 = 0;
                A2Element->QueryDoubleText(&A2);
                parameterSetAgent->A2 = A2;
            }
            if(XMLElement* b1Element = agentSettings->FirstChildElement("b1"))
            {
                double b1 = 0;
                b1Element->QueryDoubleText(&b1);
                parameterSetAgent->b1 = b1;
            }
            if(XMLElement* b2Element = agentSettings->FirstChildElement("b2"))
            {
                double b2 = 0;
                b2Element->QueryDoubleText(&b2);
                parameterSetAgent->b2 = b2;
            }
            if(XMLElement* gammaElement = agentSettings->FirstChildElement("gamma"))
            {
                double gamma = 0;
                gammaElement->QueryDoubleText(&gamma);
                parameterSetAgent->gamma = gamma;
            }

            if(XMLElement* stdNoiseSigmaElement = agentSettings->FirstChildElement("stdNoiseSigma"))
            {
                double stdNoiseSigma = 0;
                stdNoiseSigmaElement->QueryDoubleText(&stdNoiseSigma);
                parameterSetAgent->stdNoiseSigma = stdNoiseSigma;
            }

            if(XMLElement* historyMeanElement = agentSettings->FirstChildElement("historyMean"))
            {
                double historyMean = 0;
                historyMeanElement->QueryDoubleText(&historyMean);
                parameterSetAgent->historyMean = historyMean;
            }

            if(XMLElement* historySigmaElement = agentSettings->FirstChildElement("historySigma"))
            {
                double historySigma = 0;
                historySigmaElement->QueryDoubleText(&historySigma);
                parameterSetAgent->historySigma = historySigma;
            }

            if(XMLElement* initialGammaElement = agentSettings->FirstChildElement("initialGamma"))
            {
                double initialGamma = 0;
                initialGammaElement->QueryDoubleText(&initialGamma);
                parameterSetAgent->initialGamma = initialGamma;
            }

            if(XMLElement* riskToleranceElement = agentSettings->FirstChildElement("riskTolerance"))
            {
                double riskTolerance = 0;
                riskToleranceElement->QueryDoubleText(&riskTolerance);
                parameterSetAgent->riskTolerance = riskTolerance;
            }


            if(XMLElement* memorySpanElement = agentSettings->FirstChildElement("memorySpan"))
            {
                int memorySpan = 0;
                memorySpanElement->QueryIntText(&memorySpan);
                parameterSetAgent->memorySpan = memorySpan;
            }

            /// @todo this is really tedious... maybe submit a pull request to tinyxml2?
            if(XMLElement* kElement = agentSettings->FirstChildElement("k"))
            {
                double k = 0;
                kElement->QueryDoubleText(&k);
                parameterSetAgent->k = k;
            }

            // FW section

            if(XMLElement* etaElement = agentSettings->FirstChildElement("eta"))
            {
                double eta = 0;
                etaElement->QueryDoubleText(&eta);
                parameterSetAgent->eta = eta;
            }

            if(XMLElement* switchingStrategyElement = agentSettings->FirstChildElement("switchingStrategy"))
                parameterSetAgent->switchingStrategy = string(switchingStrategyElement->GetText());

            if(XMLElement* betaElement = agentSettings->FirstChildElement("beta"))
            {
                double beta = 0;
                betaElement->QueryDoubleText(&beta);
                parameterSetAgent->beta = beta;
            }

            if(XMLElement* indexStrategyElement = agentSettings->FirstChildElement("indexStrategy")) {
                string indexStrategy = string(indexStrategyElement->GetText());
                bitset<4> indexStrategyBitset;
                indexStrategyBitset.set(false);

                if (indexStrategy.find('W') != string::npos)
                    indexStrategyBitset[IndexStrategy::W] = true;
                if (indexStrategy.find('H') != string::npos)
                    indexStrategyBitset[IndexStrategy::H] = true;
                if (indexStrategy.find('P') != string::npos)
                    indexStrategyBitset[IndexStrategy::P] = true;
                if (indexStrategy.find('M') != string::npos)
                    indexStrategyBitset[IndexStrategy::M] = true;

                parameterSetAgent->indexStrategy = indexStrategyBitset;
            }

            if(XMLElement* alpha_wElement = agentSettings->FirstChildElement("alpha_w"))
            {
                double alpha_w = 0;
                alpha_wElement->QueryDoubleText(&alpha_w);
                parameterSetAgent->alpha_w = alpha_w;
            }

            if(XMLElement* alpha_nElement = agentSettings->FirstChildElement("alpha_n"))
            {
                double alpha_n = 0;
                alpha_nElement->QueryDoubleText(&alpha_n);
                parameterSetAgent->alpha_n = alpha_n;
            }

            if(XMLElement* alpha_pElement = agentSettings->FirstChildElement("alpha_p"))
            {
                double alpha_p = 0;
                alpha_pElement->QueryDoubleText(&alpha_p);
                parameterSetAgent->alpha_p = alpha_p;
            }

            if(XMLElement* alpha_0Element = agentSettings->FirstChildElement("alpha_0"))
            {
                double alpha_0 = 0;
                alpha_0Element->QueryDoubleText(&alpha_0);
                parameterSetAgent->alpha_0 = alpha_0;
            }

            if(XMLElement* nuElement = agentSettings->FirstChildElement("nu"))
            {
                double nu = 0;
                nuElement->QueryDoubleText(&nu);
                parameterSetAgent->nu = nu;
            }

            // AgentChartist
            if(XMLElement* agentChartistSection = agentSettings->FirstChildElement("AgentChartist")) {
                if (XMLElement *chiElement = agentChartistSection->FirstChildElement("chi")) {
                    double chi = 0;
                    chiElement->QueryDoubleText(&chi);
                    parameterSetAgent->chi = chi;
                }


                if (XMLElement *sigma_cElement = agentChartistSection->FirstChildElement("sigma")) {
                    double sigma_c = 0;
                    sigma_cElement->QueryDoubleText(&sigma_c);
                    parameterSetAgent->sigma_c = sigma_c;
                }
            }

            // AgentFundamentalist
            if(XMLElement* agentFundamentalistSection = agentSettings->FirstChildElement("AgentFundamentalist")) {
                if (XMLElement *phiElement = agentFundamentalistSection->FirstChildElement("phi")) {
                    double phi = 0;
                    phiElement->QueryDoubleText(&phi);
                    parameterSetAgent->phi = phi;
                }

                if (XMLElement *sigma_fElement = agentFundamentalistSection->FirstChildElement("sigma")) {
                    double sigma_f = 0;
                    sigma_fElement->QueryDoubleText(&sigma_f);
                    parameterSetAgent->sigma_f = sigma_f;
                }

                if (XMLElement *fundamentalPriceElement =
                        agentFundamentalistSection->FirstChildElement("fundamentalPrice")) {
                    double fundamentalPrice = 0;
                    fundamentalPriceElement->QueryDoubleText(&fundamentalPrice);
                    parameterSetAgent->fundamentalPrice = fundamentalPrice;
                }
            }

            allParameterSetAgents.push_back(*parameterSetAgent);
            }
    }

    return allParameterSetAgents;
}
