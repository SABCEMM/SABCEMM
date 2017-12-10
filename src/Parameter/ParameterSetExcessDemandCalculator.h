#ifndef PARAMETERSETEXCESSDEMANDCALCULATOR_H
#define PARAMETERSETEXCESSDEMANDCALCULATOR_H

#include <boost/optional.hpp>
#include <string>
#include <tinyxml2/tinyxml2.h>

class ParameterSetExcessDemandCalculator
{
public:
    bool validate();

    boost::optional<std::string> excessDemandCalculatorClass;

    static ParameterSetExcessDemandCalculator readFromXML(tinyxml2::XMLElement* dividend);

    ParameterSetExcessDemandCalculator() = default;
};

#endif // PARAMETERSETEXCESSDEMANDCALCULATOR_H
