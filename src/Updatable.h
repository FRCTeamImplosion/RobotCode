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
	virtual void Initialize() {}	// When robot is initialized
	virtual void Disable() {}		// When robot is disabled
	virtual void Enable() {}		// When autonomous, teleop, test mode is entered
	virtual void Stop() {}			// When autonomous, teleop, test mode is exited
	virtual void Update(double delta) = 0;
	virtual ~Updatable(){}
};


#endif /* UPDATABLE_H_ */
