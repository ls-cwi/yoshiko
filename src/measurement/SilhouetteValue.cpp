/*
 * SilhouetteValue.cpp
 *
 *  Created on: Jan 15, 2018
 *      Author: philipp
 */

#include "SilhouetteValue.h"

using namespace std;
using namespace lemon;

namespace ysk{

//TODO: Experimental Stuff, makes 0 sense right now as edges are not a distance function

	SilhouetteValue::~SilhouetteValue(){};

	double SilhouetteValue::getValue(){
		//Calculate average dissimilarities to own clusters (a(i))
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
					sum += _instance->getWeight(edge);
				}
				//cout << dissim << " " << sum << endl;
				dissim *= sum;
				_averageDissimilaritiesOwn[node]=dissim;

			}
		}

//		if (verbosity > 5){
//			cout << "Average Dissimilarities to own clusters" << endl << endl;
//			for (auto const &cluster : _solution){
//				for (auto const &node: cluster){
//					cout << node << " : " << _averageDissimilaritiesOwn[node]<< endl;
//				}
//			}
//		}

		//Calculate average dissimilarities to other clusters (b(i))
		map<int,double> _averageDissimilaritiesForeign= map<int,double>();
		for(auto const &cluster : _solution){
			for (auto const &node : cluster){
				double dissim = 1.0/(_instance->getSize()-cluster.size());
				//Sum Factor
				double sum = 0.0;
				for (auto const &otherCluster : _solution){
					for (auto const &otherNode : otherCluster){
						FullGraph::Edge edge = _instance->getOrig().findEdge(
								_instance->getOrig().nodeFromId(node),
								_instance->getOrig().nodeFromId(otherNode),
						INVALID);
						sum += _instance->getWeight(edge);
					}
				}
				dissim *= sum;
				_averageDissimilaritiesForeign[node]=dissim;

			}
		}

		if (verbosity > 4){
			cout << "Average Dissimilarities to other clusters" << endl << endl;
			for (auto const &cluster : _solution){
				for (auto const &node: cluster){
					cout << node << " : " << _averageDissimilaritiesForeign[node]<< endl;
				}
			}
		}

		//Calculate silhouettes
		map<int,double> _silhouettes= map<int,double>();
		for (auto const &cluster: _solution){
			for (auto const &node : cluster){
				_silhouettes[node] =
						(1.0/_instance->getSize())*
						(
							(_averageDissimilaritiesForeign[node] -_averageDissimilaritiesOwn[node])
							/
							(std::max(_averageDissimilaritiesForeign[node],_averageDissimilaritiesOwn[node]))
						);
			}
		}

		//Calculate silhouette value

		double factor = 1.0/_instance->getSize();
		int sum = 0.0;

		for (auto const &cluster : _solution){
			for (auto const &node : cluster){
				sum += _silhouettes[node];
			}
		}

		return factor*sum;
	}
}


