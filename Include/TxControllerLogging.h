#ifndef TXCONTROLLERLOGGING_H
#define TXCONTROLLERLOGGING_H

#include <string>
#include <vector>

#include "InputPort.h"
#include "UniversalInput.h"

class TxControllerLogging {
public:

	/* --Configuration Data per Port. */
	struct Configuration {
		UniversalInput type;
		std::string name;
		bool isEnabled;
		/* --Default Constructor. */
		Configuration() :type(DIGITAL_5KOMEGA), name("???"), isEnabled(false) {}
	};

	/* --Default Constructor. */
	TxControllerLogging() :digital(I_NUM), counter(C_NUM) {}

	typedef std::vector<Configuration> Setup;

	/* --Configuration Data for digital input. */
	Setup digital;
	/* --Configuration Data for counter input. */
	Setup counter;
		
};



#endif
