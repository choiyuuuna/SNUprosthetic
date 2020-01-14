#pragma once

#include <Arduino.h>
#include "filters_defs.h"

using namespace IIR;

class Filter 
{
public:

	Filter(float_t hz_, float_t ts_, ORDER od_, TYPE ty_ = TYPE::LOWPASS);
	~Filter();

	float_t filterIn(float_t input);

	void flush();
	void init(bool doFlush = true);

	void setSamplingTime(float_t ts_, bool doFlush = true) { ts = ts_; init(doFlush); }
	void setCutoffFreqHZ(float_t hz_, bool doFlush = true) { hz = hz_; init(doFlush); }
	void setOrder(ORDER od_, bool doFlush = true) { od = od_; init(doFlush); }

	bool isInErrorState() { return f_err; }
	bool isInWarnState() { return f_warn; }
	void dumpParams();

private:

	float_t ts;
	float_t hz;
	ORDER od;
	TYPE  ty;

	float_t a, b, c, d, e;
	float_t b0, b1, b2, b3, b4, a0, a1, a2;
	float_t k0, k1, k2, k3, k4, k5;
	float_t j0, j1, j2;
	float_t y[MAX_ORDER], u[MAX_ORDER];

	bool f_err, f_warn; 

	float_t ap(float_t p); 

	inline float_t computeLowPass(float_t input);
	inline float_t computeHighPass(float_t input);

	inline void  initLowPass();
	inline void  initHighPass();
};