/*
 * ForwardImageComputer.h
 *
 *  Created on: Aug 19, 2013
 *      Author: muath
 */

#ifndef FORWARDIMAGECOMPUTER_H_
#define FORWARDIMAGECOMPUTER_H_

#include "StringAnalysisTypes.h"
#include "StrangerStringAnalysisException.h"
#include "DepGraph.h"
#include "StrangerAutomaton.hpp"
#include "PerfInfo.h"
#include "StringBuilder.h"
#include <string>
#include <map>
#include <vector>




class ForwardImageComputer {
public:
	ForwardImageComputer();
	virtual ~ForwardImageComputer();
    static void staticInit();
    AnalysisResult computeFwImage(DepGraph& origDepGraph, DepGraph& acyclicWorkGraph,
                                  /*SccNodes& sccNodes,*/ AnalysisResult& inputValuesMap,
                                  bool multiTrack);
    void doForwardAnalysis(DepGraph& origDepGraph, DepGraph& acyclicWorkGraph,
                           /*SccNodes& sccNodes,*/ AnalysisResult& inputValuesMap,
                           bool multiTrack, AnalysisResult& analysisResult);
    void doNodeComputation(DepGraph& acyclicWorkGraph, DepGraph& origDepGraph,
                           AnalysisResult& analysisResult, DepGraphNode* node,
                           AnalysisResult& inputValuesMap, bool multiTrack);
    StrangerAutomaton* makeAutoForOp(DepGraphOpNode* opNode, AnalysisResult& analysisResult, DepGraph& depGraph, bool multiTrack);
    
    AnalysisResult computeBwImage(DepGraph& origDepGraph, DepGraph& acyclicWorkGraph,
                                  /*SccNodes& sccNodes,*/ AnalysisResult& inputValuesMap,
                                  StrangerAutomaton* intersectionAuto,
                                  DepGraphNode* inputNode,
                                  AnalysisResult& fwAnalysisResult);
    AnalysisResult doBackwardAnalysis_ValidationPhase(DepGraph& origDepGraph,
                                                                DepGraph& inputDepGraph,
                                                                 NodesList& sortedNodes);
    void doBackwardNodeComputation_ValidationPhase(DepGraph& origDepGraph, DepGraph& inputDepGraph,
                                   AnalysisResult& bwAnalysisResult,
                                    DepGraphNode* node);
    AnalysisResult doBackwardAnalysis(DepGraph& origDepGraph, DepGraph& inputDepGraph,
                                      /*SccNodes& sccNodes,*/
                                      NodesList& sortedNodes,
                                      StrangerAutomaton* intersectionAuto,
                                      const AnalysisResult& fwAnalysisResult);
    void doBackwardNodeComputation(DepGraph& inputDepGraph, DepGraph& origDepGraph,
                                   AnalysisResult& bwAnalysisResult,
                                   const AnalysisResult& fwAnalysisResult,
                                   DepGraphNode* node,
                                   bool multiTrack);
    StrangerAutomaton* makeBackwardAutoForOpChild_ValidationPhase( DepGraphOpNode* opNode,
    			 DepGraphNode* childNode,
    			AnalysisResult& bwAnalysisResult, DepGraph& depGraph);

    StrangerAutomaton* makeBackwardAutoForOpChild(DepGraphOpNode* opNode,
                                                                        DepGraphNode* childNode,
                                                  AnalysisResult& bwAnalysisResult, const AnalysisResult& fwAnalysisResult, DepGraph& depGraph, boolean fixPoint);

    static PerfInfo perfInfo;
    
private:
    static int numOfProcessedNodes;
    static bool initialized;
    static int debugLevel;
    static int autoDebugLevel;
    NodesList f_unmodeled;
    
    static void debug(std::string s, int dlevel);
    static void debugAuto(StrangerAutomaton* dfa, int dlevel, int printlevel);
    static void debugMemoryUsage(int dlevel);
    std::string getLiteralValue(DepGraphNode* node);
    bool isLiteral(DepGraphNode* node, NodesList successors);
};

#endif /* FORWARDIMAGECOMPUTER_H_ */
