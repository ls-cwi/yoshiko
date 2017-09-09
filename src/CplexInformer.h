/*
 * JavaInformer.h
 *
 *  Created on: Sep 3, 2017
 *      Author: philipp spohr
 */

#ifndef SRC_CPLEXINFORMER_H_
#define SRC_CPLEXINFORMER_H_

namespace yskLib{

	enum YoshikoState{
		PARSING_INPUT = 0,
		REDUCTION_AC = 1,
		REDUCTION_CR = 2,
		REDUCTION_CC = 3,
		REDUCTION_PD = 4,
		REDUCTION_HE = 5,
		REDUCTION_MR = 6,
		REDUCTION_SN = 7,
		SOLVING_HEURISTIC = 8,
		SOLVING_ILP = 9,
		PROCESS_SOLUTION = 10
	};

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
