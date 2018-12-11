add_library(Agent
        Agent.cpp        
        AgentCrossWealth.cpp
        AgentCross.cpp

        AgentFW.cpp
        AgentFWChartist.cpp
        AgentFWFundamentalist.cpp

        AgentHarras.cpp

        AgentLLS.cpp
        )

add_coverage(Agent)

add_dependencies(Agent
        Group)
