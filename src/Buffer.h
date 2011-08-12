/*
 * Buffer.h
 *
 *  Created on: 12.08.2011
 *      Author: outz
 */

#ifndef BUFFER_H_
#define BUFFER_H_

namespace webzavod {

class Buffer {
public:
	Buffer(unsigned aBufferSize);
	virtual ~Buffer();
	const int Size() const;
	const int Pos() const;
	const char * Data() const;
};

}

#endif /* BUFFER_H_ */
