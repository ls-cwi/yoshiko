/*
 * JavaInformer.h
 *
 *  Created on: Sep 3, 2017
 *      Author: philipp spohr
 */

#ifndef SRC_CPLEXINFORMER_H_
#define SRC_CPLEXINFORMER_H_

namespace yskLib{
	class CplexInformer {
		public:
			virtual void updateGap(double gap){};
			virtual bool continueOnTimeout(){
				std::cout << "[WARNING:] Falling back to default implementation of CplexInformer!" << std::endl;
				return false;
			};
			virtual ~CplexInformer(){};
	};
}
#endif /* SRC_CPLEXINFORMER_H_ */
