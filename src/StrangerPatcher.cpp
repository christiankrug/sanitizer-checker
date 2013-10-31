/*
 * StrangerPatcher.cpp
 *
 *  Created on: Oct 30, 2013
 *      Author: baki
 */

#include "StrangerPatcher.h"

StrangerPatcher::StrangerPatcher(string patcher_dep_graph_file_name,string patchee_dep_graph_file_name, string input_field_name) {

	this->patcher_dep_graph_file_name = patcher_dep_graph_file_name;
	this->patcher_dep_graph_file_name = patchee_dep_graph_file_name;
	this->input_field_name = input_field_name;

	// read dep graphs
	this->patcher_dep_graph = DepGraph::parseDotFile(patcher_dep_graph_file_name);
	this->patchee_dep_graph = DepGraph::parseDotFile(patchee_dep_graph_file_name);

	// initialize input nodes
	this->patcher_uninit_field_node = patcher_dep_graph.findInputNode(input_field_name);
	if (patcher_uninit_field_node == NULL) {
		throw StrangerStringAnalysisException("Cannot find input node " + input_field_name + " in patcher dep graph.");
	}
	message(stringbuilder() << "patcher uninit node(" << patcher_uninit_field_node->getID() << ") found.");
	this->patchee_uninit_field_node = patchee_dep_graph.findInputNode(input_field_name);
	if (patchee_uninit_field_node == NULL) {
		throw StrangerStringAnalysisException("Cannot find input node " + input_field_name + " in patchee dep graph.");
	}
	message(stringbuilder() << "patchee uninit node(" << patchee_uninit_field_node->getID() << ") found.");

	// initialize input relevant graphs
	this->patcher_field_relevant_graph = patcher_dep_graph.getInputRelevantGraph(patcher_uninit_field_node);
	this->patchee_field_relevant_graph = patchee_dep_graph.getInputRelevantGraph(patchee_uninit_field_node);

	// get sorted node lists for each field
	patcher_field_relevant_graph.sort(patcher_field_relevant_graph);
	this->patcher_sorted_field_relevant_nodes = patcher_field_relevant_graph.getSortedNodes();
	patchee_field_relevant_graph.sort(patchee_field_relevant_graph);
	this->patchee_sorted_field_relevant_nodes = patchee_field_relevant_graph.getSortedNodes();
}

StrangerPatcher::~StrangerPatcher() {
	delete this->patcher_uninit_field_node;
	delete this->patchee_uninit_field_node;
}

void StrangerPatcher::message(string msg) {
	cout << endl << "~~~~~~~~~~~>>> StrangerPatcher says: " << msg << endl;
}

void StrangerPatcher::printAnalysisResults(AnalysisResult& result) {
	cout << endl;
	for (AnalysisResultConstIterator it = result.begin(); it != result.end(); it++ ) {
		cout << "Printing automata for node ID: " << it->first << endl;
		(it->second)->toDot();
		cout << endl << endl;
	}
}

/**
 * Initial backward analysis phase for extracting validation behavior
 */
StrangerAutomaton* StrangerPatcher::extractValidationPatch() {

		ForwardImageComputer::staticInit();
		ForwardImageComputer analyzer;

		AnalysisResult validationExtractionResults =
				analyzer.doBackwardAnalysis_ValidationPhase(patcher_dep_graph, patcher_field_relevant_graph, patcher_sorted_field_relevant_nodes);

		StrangerAutomaton* negVPatch = validationExtractionResults[patcher_uninit_field_node->getID()];
		validation_patch_auto = negVPatch->complement(patcher_uninit_field_node->getID());

		message("validation patch is generated for input: " + input_field_name);
//		validation_patch_auto->toDotAscii(0);
		return validation_patch_auto;

}

/**
 *
 */
StrangerAutomaton* StrangerPatcher::checkSanitizationDifference() {

	AnalysisResult patcherAnalysis;
	AnalysisResult patcheeAnalysis;

	UninitNodesList patcherUninitNodes = patcher_dep_graph.getUninitNodes();
	UninitNodesList patcheeUninitNodes = patchee_dep_graph.getUninitNodes();

	// best case for us to have one uninit node and one sink

	// initialize patcher input nodes to sigma star
	message("initializing patcher inputs with sigma star");
	for (UninitNodesListConstIterator it = patcherUninitNodes.begin(); it != patcherUninitNodes.end(); it++) {
		patcherAnalysis[(*it)->getID()] = StrangerAutomaton::makeAnyString((*it)->getID());
	}

	//TODO discuss initialization here,
	message("initializing patchee inputs with empty string");
	for (UninitNodesListConstIterator it = patcheeUninitNodes.begin(); it != patcheeUninitNodes.end(); it++) {
		patcheeAnalysis[(*it)->getID()] = StrangerAutomaton::makeEmptyString((*it)->getID());
	}

	// initialize uninit node that we are interested in with validation patch_auto
	message(stringbuilder() << "initializing input node(" << patchee_uninit_field_node->getID() << ") with validation patch auto");

	delete patcheeAnalysis[patchee_uninit_field_node->getID()];
	patcheeAnalysis[patchee_uninit_field_node->getID()] = validation_patch_auto;

	message("input node initializations completed...");

//	ForwardImageComputer::staticInit();
	ForwardImageComputer patcherAnalyzer;
	ForwardImageComputer patcheeAnalyzer;

	message("starting forward analysis for patcher");
	patcherAnalyzer.doForwardAnalysis_CheckSanitDiffPhase(patcher_dep_graph,patcher_field_relevant_graph,patcher_sorted_field_relevant_nodes,patcherAnalysis);

//	message("starting forward analysis for patchee");
//	patcheeAnalyzer.doForwardAnalysis_CheckSanitDiffPhase(patchee_dep_graph, patchee_field_relevant_graph, patchee_sorted_field_relevant_nodes, patcheeAnalysis);

	return NULL;
}


