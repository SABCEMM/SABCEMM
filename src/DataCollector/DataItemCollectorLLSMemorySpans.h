#ifndef DATAITEMCOLLECTORLLSMEMORYSPANS_H
#define DATAITEMCOLLECTORLLSMEMORYSPANS_H


#include <vector>

#include "../Agent/AgentLLS.h"
#include "DataItemCollector.h"


/** DataCollector for the average gamma of EMB agents. Gamma is tracked before noise is added. */
class DataItemCollectorLLSMemorySpans: public DataItemCollector {
private:
        std::vector<double> memorySpans;
        std::vector<AgentLLS*>* agents; /**< Pointer to the agents vector */

        /// tracks whether memory span information has already been collected.
        /// memory spans do not change after the initial scrambling.
        bool collected;
protected:
        virtual void collectData() ;
public:
        DataItemCollectorLLSMemorySpans();
        explicit DataItemCollectorLLSMemorySpans(std::vector<Agent*>* newAgents);

        virtual void checkInitilisation();
        virtual std::vector<std::vector<double>> * getData();

        void setAgents(std::vector<Agent*>* newAgents);
};

#endif // DATAITEMCOLLECTORLLSMEMORYSPANS_H
