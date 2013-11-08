/*
 * random.h
 *
 *  Created on: Nov 6, 2013
 *      Author: C15Jennae.Steinmille
 */

#ifndef __RAND_H
#define __RAND_H

/* LCG constants */
#define M 49381 // Multiplier
#define I 8643 // Increment

unsigned int rand();
unsigned int prand(unsigned int state);

#endif /* __RAND_H */
