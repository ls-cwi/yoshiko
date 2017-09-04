/*
 * JavaInformer.h
 *
 *  Created on: Sep 3, 2017
 *      Author: philipp spohr
 */

#ifndef SRC_JAVAINFORMER_H_
#define SRC_JAVAINFORMER_H_

namespace yskLib{
	class JavaInformer {
		public:
			virtual void callback(){};
			virtual ~JavaInformer(){};
	};
}
#endif /* SRC_JAVAINFORMER_H_ */
