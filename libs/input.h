#ifndef INPUT_H
#define INPUT_H

#include "err.h"

err input_uint(unsigned*, unsigned, unsigned);
err input_int(int*, int, int);
err txt_input_int(FILE*, int *, int, int);
err txt_input_uint(FILE*, unsigned *, unsigned, unsigned);
err bin_input_int(FILE*, int *, int, int);
err bin_input_uint(FILE*, unsigned *, unsigned, unsigned);


#endif
