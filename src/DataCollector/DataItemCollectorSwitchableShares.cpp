#include "DataItemCollectorSwitchableShares.h"

/** Saves current chartist share in the history
 */
void DataItemCollectorSwitchableShares::collectData() {
    dataMatrix.at(0).push_back(switchable->getChartistShare());
}

DataItemCollectorSwitchableShares::DataItemCollectorSwitchableShares(const std::vector<Switchable *> &switchables)
{
    /// @todo only works for a single switchable yet, as the way of averaging is not clear.
    switchable = switchables.at(0);
    std::vector<double> temp;
    dataMatrix.push_back(temp);
}


std::vector<std::vector<double>> * DataItemCollectorSwitchableShares::getData(){
    return &dataMatrix;
}
