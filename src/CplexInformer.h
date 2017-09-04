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
			virtual void callback(double gap){};
			virtual ~CplexInformer(){};
	};
}
#endif /* SRC_CPLEXINFORMER_H_ */
