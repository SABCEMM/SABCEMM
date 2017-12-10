#include "ParameterSetExcessDemandCalculator.h"

#include <iostream>
using namespace std;
using namespace tinyxml2;

bool ParameterSetExcessDemandCalculator::validate()
{
    #define REQUIRED(variable) if(!(variable)){cerr << "%%% " << #variable << " not specified. %%%" << endl; return false;}

    REQUIRED(excessDemandCalculatorClass);

    return true;
}

ParameterSetExcessDemandCalculator ParameterSetExcessDemandCalculator::readFromXML(XMLElement* EDSection){
    ParameterSetExcessDemandCalculator paramSet;

    if(EDSection){
        if(XMLElement* excessDemandCalculatorClassElement = EDSection->FirstChildElement("excessDemandCalculatorClass"))
            paramSet.excessDemandCalculatorClass = string(excessDemandCalculatorClassElement->GetText());
    }

    return paramSet;
}
