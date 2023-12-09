#include "Low_pass.h"

static float Low_pass_output_last[2];
static float p = 0.1;
float Low_pass_output(float _input, char _i)
{
	float _low_pass;
	_low_pass = _input*p + Low_pass_output_last[_i] * (1-p);
	Low_pass_output_last[_i] = _low_pass;
	return _low_pass;
}


