add_library(Agent
        Agent.cpp
        AgentFWChartist.cpp
        AgentCrossWealth.cpp
        AgentCross.cpp
        AgentEMB.cpp
        AgentFWFundamentalist.cpp
        AgentFW.cpp
        AgentHarras.cpp
        AgentRII.cpp
        )

add_coverage(Agent)

add_dependencies(Agent
        Group)
