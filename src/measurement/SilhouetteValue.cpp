#include "SilhouetteValue.h"

using namespace std;
using namespace lemon;

namespace ysk{

//TODO: Experimental Stuff, makes 0 sense right now as edges are not a distance function

	SilhouetteValue::~SilhouetteValue(){};

	double SilhouetteValue::getValue(){

		//Treat negative values as offsets OR zeros?
		const bool negativeToZero = false;
		const bool negativeOffset = true;

		double minimumEdgeWeight = 0;

		if (negativeOffset){
			for (FullGraph::EdgeIt it(_instance->getOrig()); it != INVALID; ++it){
				if (_instance->getWeight(it) < minimumEdgeWeight){
					minimumEdgeWeight = _instance->getWeight(it);
				}
			}
			if (verbosity > 4) cout << "Minimum edge weight: " << minimumEdgeWeight <<endl;
		}

		//Calculate average dissimilarities to own cluster (a(i))
		map<int,double> _averageDissimilaritiesOwn = map<int,double>();
		for(auto const &cluster : _solution){
			for (auto const &node : cluster){
				double dissim = 1.0/cluster.size();
				//Sum Factor
				double sum = 0.0;
				for (auto const &otherNode : cluster){
					if (node == otherNode) continue; //Not of interest
					FullGraph::Edge edge = _instance->getOrig().findEdge(
							_instance->getOrig().nodeFromId(node),
							_instance->getOrig().nodeFromId(otherNode),
					INVALID);
					double edgeWeight = negativeOffset ? _instance->getWeight(edge) - minimumEdgeWeight : _instance->getWeight(edge) ;
					if (negativeToZero){
						sum += edgeWeight > 0 ? edgeWeight : 0.0;
					}
					else{
						sum += edgeWeight;
					}
				}
				dissim *= sum;
				if (verbosity > 4){
					cout << "a("<< node << ")= " << dissim << endl;
				}
				_averageDissimilaritiesOwn[node]=dissim;
			}
		}

		//Calculate average dissimilarities to other clusters (b(i))
		map<int,double> _averageDissimilaritiesForeign= map<int,double>();
		for(auto const &cluster : _solution){
			for (auto const &node : cluster){
				if (_instance->getSize() == cluster.size()){
					//We can't calculate this because there are no other clusters
					//TODO: Output useful error msg
				}
				double dissim = 1.0/(_instance->getSize()-cluster.size());
				//Sum Factor
				double sum = 0.0;
				for (auto const &otherCluster : _solution){
					//Skip internal edges
					if (otherCluster == cluster) continue;

					for (auto const &otherNode : otherCluster){
						FullGraph::Edge edge = _instance->getOrig().findEdge(
								_instance->getOrig().nodeFromId(node),
								_instance->getOrig().nodeFromId(otherNode),
						INVALID);
						double edgeWeight = negativeOffset ? _instance->getWeight(edge)- minimumEdgeWeight: _instance->getWeight(edge) ;
						if (negativeToZero){
							sum += edgeWeight > 0 ? edgeWeight : 0.0;
						}
						else{
							sum += edgeWeight;
						}
					}
				}
				dissim *= sum;
				if (verbosity > 4){
					cout << "b("<< node << ")= " << dissim << endl;
				}
				_averageDissimilaritiesForeign[node]=dissim;

			}
		}

		//Calculate silhouettes
		map<int,double> _silhouettes= map<int,double>();
		for (auto const &cluster: _solution){
			for (auto const &node : cluster){
				double max = std::max(_averageDissimilaritiesForeign[node],_averageDissimilaritiesOwn[node]);
				_silhouettes[node] =
						(_averageDissimilaritiesForeign[node] == 0 && _averageDissimilaritiesOwn[node] == 0) ? //Both a(i) and b(i) are 0
								0 : //Special case: We set it to 0 -> Avoid division by 0
						(
							(_averageDissimilaritiesForeign[node] -_averageDissimilaritiesOwn[node])
							/
							max
						);
				if (verbosity > 4){
					cout << "s("<< node << ")= " << _silhouettes[node] << endl;
				}
			}
		}

		//Calculate silhouette value

		double factor = 1.0/_instance->getSize();
		double sum = 0.0;

		for (auto const &cluster : _solution){
			for (auto const &node : cluster){
				sum += _silhouettes[node];
			}
		}

		return factor*sum;
	}
}
