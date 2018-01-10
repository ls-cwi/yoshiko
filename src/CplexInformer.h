/*
 * JavaInformer.h
 *
 *  Created on: Sep 3, 2017
 *      Author: philipp spohr
 */

#ifndef SRC_CPLEXINFORMER_H_
#define SRC_CPLEXINFORMER_H_

namespace yskLib{

	/**
	 * Describes the different states the algorithm can be in. Might be used to receive feedback about the current state of longer runs.
	 * TODO: Might be implemented elsewhere for better readability / code coherence?
	 */
	enum YoshikoState{
		PARSING_INPUT = 0,    //!< PARSING_INPUT
		REDUCTION_AC = 1,     //!< REDUCTION_AC
		REDUCTION_CR = 2,     //!< REDUCTION_CR
		REDUCTION_CC = 3,     //!< REDUCTION_CC
		REDUCTION_PD = 4,     //!< REDUCTION_PD the algorithm is currently applying the parameter-dependent reduction rule
		REDUCTION_HE = 5,     //!< REDUCTION_HE
		REDUCTION_MR = 6,     //!< REDUCTION_MR
		REDUCTION_SN = 7,     //!< REDUCTION_SN
		SOLVING_HEURISTIC = 8,//!< SOLVING_HEURISTIC the algorithm is currently solving the heuristic
		SOLVING_ILP = 9,      //!< SOLVING_ILP
		PROCESS_SOLUTION = 10 //!< PROCESS_SOLUTION
	};

	/**
	 * A class that is meant to be implemented if live feedback about a run is required. The callbacks will be triggered if appropriate when a CPlexInformer has been registered.
	 */
	class CplexInformer {
		public:
			virtual void updateStatus(yskLib::YoshikoState state){};
			virtual void updateStatus(yskLib::YoshikoState state, double value){}

			virtual bool continueOnTimeout(){
				std::cout << "[WARNING:] Falling back to default implementation of CplexInformer!" << std::endl;
				return false;
			};
			virtual ~CplexInformer(){};
	};


}
#endif /* SRC_CPLEXINFORMER_H_ */
