/*
 * Updatable.h
 *
 *  Created on: Feb 9, 2016
 *      Author: matt
 */

#ifndef UPDATABLE_H
#define UPDATABLE_H


class Updatable{
public:
	virtual void Update(double delta) = 0;
	virtual ~Updatable(){}
};


#endif /* UPDATABLE_H_ */
