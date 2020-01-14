#pragma once

 // #define LIBFILTER_USE_DOUBLE

#ifndef LIBFILTER_USE_DOUBLE
typedef float   float_t;
typedef int     int_t;
#else
typedef double  float_t;
typedef int64_t int_t;
#endif 

namespace IIR {
	const uint8_t MAX_ORDER = 5;
	enum class ORDER : uint8_t { OD1 = 0, OD2, OD3, OD4 };//, OD5};
	enum class TYPE : uint8_t { LOWPASS = 0, HIGHPASS = 1 };

	const float_t SQRT2 = sqrt(2.0);
	const float_t SQRT3 = sqrt(3.0);
	const float_t SQRT5 = sqrt(5.0);

	const float_t EPSILON = 0.00001;   
	const float_t WEPSILON = 0.00010;   
	const float_t KM = 100.0;    
}