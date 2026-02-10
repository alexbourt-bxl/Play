#ifndef CONTEXT_H
#define CONTEXT_H

/*	A context should provide an interface which combines the exposed controls
	of its chosen constituent operators, based on some logic. The interface
	should be context sensitive and flexible, so that it would be possible to
	spend most time in the Color context (color correction), but occasionally
	jump into the paint context to mark something or draw something, or to clean
	up some visual mistake. This would just add a paint operator to the end of the
	chain, after which the user goes back to the Color context and continues working,
	color ops being happily added after the paint op (or before, depending how
	it will work best, but probably after). 
*/

#endif