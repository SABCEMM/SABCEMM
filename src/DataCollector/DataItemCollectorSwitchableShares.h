#ifndef DATAITEMCOLLECTORSWITCHABLESHARES_H
#define DATAITEMCOLLECTORSWITCHABLESHARES_H

#include <vector>

#include "../Agent/Agent.h"
#include "DataItemCollector.h"
#include "../Util/Util.h"

class DataItemCollectorSwitchableShares: public DataItemCollector {
private:
        std::vector<double> chartistShareHistory;
        Switchable* switchable;
protected:
        virtual void collectData();
public:
        DataItemCollectorSwitchableShares(const std::vector<Switchable*> & switchables);

        virtual ~DataItemCollectorSwitchableShares(){};


    virtual std::vector<std::vector<double>> * getData();

        virtual void checkInitilisation(){};
};

#endif // DATAITEMCOLLECTORSWITCHABLESHARES_H
