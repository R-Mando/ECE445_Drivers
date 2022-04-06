#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) CHOPCONF_register.SETTING = B; write(CHOPCONF_register.address, CHOPCONF_register.sr)

// CHOPCONF
#define GET_REG_2660(SETTING) return CHOPCONF_register.SETTING;

uint32_t TMC2660Stepper::CHOPCONF() { return CHOPCONF_register.sr; }

void TMC2660Stepper::CHOPCONF(uint32_t data) {
  CHOPCONF_register.sr = data;
  write(CHOPCONF_register.address, CHOPCONF_register.sr);
}

void TMC2660Stepper::toff(uint8_t B) 	{
	SET_REG(toff);
	if (B>0) _savedToff = B;
}
void TMC2660Stepper::hstrt(uint8_t B) 	{ SET_REG(hstrt); 	}
void TMC2660Stepper::hend(uint8_t B) 	{ SET_REG(hend);	}
void TMC2660Stepper::hdec(uint8_t B) 	{ SET_REG(hdec);	}
void TMC2660Stepper::rndtf(bool B) 	{ SET_REG(rndtf);	}
void TMC2660Stepper::chm(bool B) 	{ SET_REG(chm);	}
void TMC2660Stepper::tbl(uint8_t B) 	{ SET_REG(tbl);	}

uint8_t TMC2660Stepper::toff() 	{ GET_REG_2660(toff);	}
uint8_t TMC2660Stepper::hstrt() 	{ GET_REG_2660(hstrt);	}
uint8_t TMC2660Stepper::hend() { GET_REG_2660(hend);	}
uint8_t TMC2660Stepper::hdec() 	{ GET_REG_2660(hdec);	}
bool TMC2660Stepper::rndtf() { GET_REG_2660(rndtf);	}
bool TMC2660Stepper::chm() 	{ GET_REG_2660(chm);	}
uint8_t TMC2660Stepper::tbl() { GET_REG_2660(tbl);	}
