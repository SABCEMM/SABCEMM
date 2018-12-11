#include "ParameterSetShareCalculator.h"
#include "../Switching/ShareCalculator.h"
#include <iostream>

using namespace std;
using namespace tinyxml2;

bool ParameterSetShareCalculator::validate()
{
    #define REQUIRED(variable) if(!(variable)){cerr << "%%% " << #variable << " not specified. %%%" << endl; return false;}

    if(strategy)
    {
        if(stringToShareCalculatorStrategy.find(*strategy) == stringToShareCalculatorStrategy.end())
        {
            cerr << "invalid mode specified for share calculator." << endl;
            return false;
        }
    }

    return true;
}

void ParameterSetShareCalculator::readFromXML(XMLElement* shareCalculatorSection, ParameterSetShareCalculator& paramSet){
    if(shareCalculatorSection){
        // default values
        if(!paramSet.alpha_0)
            paramSet.alpha_0 = 0.8;
        if(!paramSet.alpha_n)
            paramSet.alpha_n = 1.79;
        if(paramSet.strategy)
            paramSet.strategy = "pdivpf";

        if(XMLElement* alpha0Element = shareCalculatorSection->FirstChildElement("alpha_0"))
        {
            double alpha0 = 0;
            alpha0Element->QueryDoubleText(&alpha0);
            paramSet.alpha_0 = alpha0;
        }
        if(XMLElement* alphanElement = shareCalculatorSection->FirstChildElement("alpha_n"))
        {
            double alphan = 0;
            alphanElement->QueryDoubleText(&alphan);
            paramSet.alpha_n = alphan;
        }
        if(XMLElement* strategyElement = shareCalculatorSection->FirstChildElement("strategy"))
            paramSet.strategy = string(strategyElement->GetText());
    }
}
