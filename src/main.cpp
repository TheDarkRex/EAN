#include "app.h"

#define MPFR_USE_NO_MACRO
#define MPFR_USE_INTMAX_T

int main() {
	App app;
	if(app.init())
		return 1; 
	app.run(); 
	app.term();  
	return 0;
}