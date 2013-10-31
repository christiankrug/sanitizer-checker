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

    /****************************************************************************************************/
    /********* VALIDATION PATCH EXTRACTION PHASE METHODS ************************************************/
    /****************************************************************************************************/

    AnalysisResult doBackwardAnalysis_ValidationPhase(DepGraph& origDepGraph,
                                                                DepGraph& inputDepGraph,
                                                                 NodesList& sortedNodes);
    void doBackwardNodeComputation_ValidationPhase(DepGraph& origDepGraph, DepGraph& inputDepGraph,
                                   AnalysisResult& bwAnalysisResult,
                                    DepGraphNode* node);

    StrangerAutomaton* makeBackwardAutoForOpChild_ValidationPhase( DepGraphOpNode* opNode,
    			 DepGraphNode* childNode,
    			AnalysisResult& bwAnalysisResult, DepGraph& depGraph);

    /****************************************************************************************************/
    /*********** SANITIZATION PATCH EXTRACTION METHODS **************************************************/
    /****************************************************************************************************/

    void doForwardAnalysis_CheckSanitDiffPhase(DepGraph& origDepGraph,  DepGraph& inputDepGraph, NodesList& sortedNodes, AnalysisResult& analysisResult);

    void doNodeComputation_CheckSanitDiffPhase(DepGraph& origDepGraph,  DepGraph& inputDepGraph, DepGraphNode* node, AnalysisResult& analysisResult);
    /****************************************************************************************************/
    /*********** REGULAR FORWARD IMAGE COMPUTATION METHODS **********************************************/
    /****************************************************************************************************/

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
    
//********************************************************************************************************************

    AnalysisResult computeBwImage(DepGraph& origDepGraph, DepGraph& acyclicWorkGraph,
                                  /*SccNodes& sccNodes,*/ AnalysisResult& inputValuesMap,
                                  StrangerAutomaton* intersectionAuto,
                                  DepGraphNode* inputNode,
                                  AnalysisResult& fwAnalysisResult);

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
