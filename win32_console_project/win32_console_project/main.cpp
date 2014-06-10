#include <iostream>
#include "C:\Users\Ivan\Documents\GitHub\Fluxx\qFluxx\fluxxControl_code\fluxxcontrol.h"
#include "C:\Users\Ivan\Documents\GitHub\Fluxx\qFluxx\cardLib_code\cardLib.h"

int main() {
	CardLib& test = CardLib::getLib();
	fluxxRules rule(test);
	fluxxControl FC(rule);
	FC.fluxxRun();
	return 0;
}