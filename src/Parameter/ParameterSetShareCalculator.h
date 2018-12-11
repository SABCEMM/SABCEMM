#ifndef PARAMETERSETSHARECALCULATOR_H
#define PARAMETERSETSHARECALCULATOR_H

#include <boost/optional.hpp>
#include <string>
#include <tinyxml2/tinyxml2.h>

class ParameterSetShareCalculator{
public:
    bool validate();

    boost::optional<double> alpha_n;
    boost::optional<double> alpha_0;
    boost::optional<std::string> strategy;

    static void readFromXML(tinyxml2::XMLElement* shareCalculatorSection, ParameterSetShareCalculator& paramSet);

    // defaults values are needed as the whole parameter set optional
    // ==> if the whole section is omitted, readFromXML is never called and therefore the default values there do not count.
    ParameterSetShareCalculator()
        : alpha_n(1.79), alpha_0(0.8), strategy("pdivpf")
    { }
};

#endif // PARAMETERSETSHARECALCULATOR_H
