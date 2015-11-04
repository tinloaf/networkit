/*
 * ConnectedComponents.cpp
 *
 *  Created on: Dec 16, 2013
 *      Author: cls
 */

#include <set>

#include "ConnectedComponents.h"
#include "../structures/Partition.h"
#include "../auxiliary/Log.h"

namespace NetworKit {

ConnectedComponents::ConnectedComponents(const Graph& G) : G(G), hasRun(false) {
	if (G.isDirected()) {
		throw std::runtime_error("Error, connected components of directed graphs cannot be computed, use StronglyConnectedComponents for them.");
	}
}

void ConnectedComponents::run() {
	DEBUG("initializing labels");
	component = Partition(G.upperNodeIdBound(), none);
	numComponents = 0;

	// perform breadth-first searches
	G.forNodes([&](node u) {
		if (component[u] == none) {
			component.setUpperBound(numComponents+1);
			index c = numComponents;
			G.BFSfrom(u, [&](node v) {
				component[v] = c;
			});
			assert (component[u] != none);
			++numComponents;
		}
	});

	hasRun = true;
}


Partition ConnectedComponents::getPartition() {
	if (!hasRun) throw std::runtime_error("run method has not been called");
	return this->component;
}


std::vector<std::vector<node> > ConnectedComponents::getComponents() {
	if (!hasRun) throw std::runtime_error("run method has not been called");

	// transform partition into vector of unordered_set
	std::vector<std::vector<node> > result(numComponents);

	G.forNodes([&](node u) {
		result[component[u]].push_back(u);
	});

	return result;
}



std::map<index, count> ConnectedComponents::getComponentSizes() {
	if (!hasRun) throw std::runtime_error("run method has not been called");
	return this->component.subsetSizeMap();
}

}
