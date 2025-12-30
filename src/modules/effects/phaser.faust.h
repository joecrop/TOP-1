/* ------------------------------------------------------------
name: "phaser"
Code generated with Faust 2.37.3 (https://faust.grame.fr)
Compilation options: -a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __faust_phaser_H__
#define  __faust_phaser_H__

#include <math.h>
#include <algorithm>

#include <faust/gui/UI.h>
#include <faust/gui/meta.h>
#include <faust/dsp/dsp.h>

using std::max;
using std::min;

/********************************
	VECTOR INTRINSICS
*********************************/


/********************************
	ABSTRACT USER INTERFACE
*********************************/

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

class faust_phaserSIG0 {
	
  private:
	
	int iVec0[2];
	int iRec0[2];
	
  public:
	
	int getNumInputsfaust_phaserSIG0() {
		return 0;
	}
	int getNumOutputsfaust_phaserSIG0() {
		return 1;
	}
	
	void instanceInitfaust_phaserSIG0(int sample_rate) {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			iRec0[l1] = 0;
		}
	}
	
	void fillfaust_phaserSIG0(int count, float* table) {
		for (int i1 = 0; (i1 < count); i1 = (i1 + 1)) {
			iVec0[0] = 1;
			iRec0[0] = ((iVec0[1] + iRec0[1]) % 65536);
			table[i1] = std::sin((9.58738019e-05f * float(iRec0[0])));
			iVec0[1] = iVec0[0];
			iRec0[1] = iRec0[0];
		}
	}

};

static faust_phaserSIG0* newfaust_phaserSIG0() { return (faust_phaserSIG0*)new faust_phaserSIG0(); }
static void deletefaust_phaserSIG0(faust_phaserSIG0* dsp) { delete dsp; }

static float ftbl0faust_phaserSIG0[65536];

#ifndef FAUSTCLASS 
#define FAUSTCLASS faust_phaser
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class faust_phaser : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fHslider1;
	float fConst2;
	FAUSTFLOAT fHslider2;
	float fRec1[2];
	float fRec2[2];
	float fConst3;
	float fRec3[2];
	float fConst4;
	float fRec4[2];
	float fConst5;
	float fRec5[2];
	float fConst6;
	float fRec6[2];
	float fConst7;
	float fRec7[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.2");
		m->declare("compile_options", "-a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0");
		m->declare("filename", "phaser.dsp");
		m->declare("filters.lib/allpassnn:author", "Julius O. Smith III");
		m->declare("filters.lib/allpassnn:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpassnn:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "phaser");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
		faust_phaserSIG0* sig0 = newfaust_phaserSIG0();
		sig0->instanceInitfaust_phaserSIG0(sample_rate);
		sig0->fillfaust_phaserSIG0(65536, ftbl0faust_phaserSIG0);
		deletefaust_phaserSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (6.28318548f / fConst0);
		fConst2 = (1.0f / fConst0);
		fConst3 = (8.16814137f / fConst0);
		fConst4 = (10.0530968f / fConst0);
		fConst5 = (12.566371f / fConst0);
		fConst6 = (15.707963f / fConst0);
		fConst7 = (18.849556f / fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.5f);
		fHslider1 = FAUSTFLOAT(0.59999999999999998f);
		fHslider2 = FAUSTFLOAT(0.5f);
	}
	
	virtual void instanceClear() {
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec1[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec2[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec3[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec4[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec5[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec6[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec7[l8] = 0.0f;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual faust_phaser* clone() {
		return new faust_phaser();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("phaser");
		ui_interface->addHorizontalSlider("depth", &fHslider1, FAUSTFLOAT(0.600000024f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("mix", &fHslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("rate", &fHslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0500000007f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fHslider0);
		float fSlow1 = (1.0f - fSlow0);
		float fSlow2 = (1400.0f * float(fHslider1));
		float fSlow3 = (fConst2 * float(fHslider2));
		for (int i0 = 0; (i0 < count); i0 = (i0 + 1)) {
			float fTemp0 = float(input0[i0]);
			fRec1[0] = (fSlow3 + (fRec1[1] - std::floor((fSlow3 + fRec1[1]))));
			float fTemp1 = ((fSlow2 * (ftbl0faust_phaserSIG0[int((65536.0f * fRec1[0]))] + 1.0f)) + 200.0f);
			float fTemp2 = (fConst1 * fTemp1);
			float fTemp3 = ((1.0f - std::sin(fTemp2)) / std::cos(fTemp2));
			float fTemp4 = std::sin(fTemp3);
			float fTemp5 = std::cos(fTemp3);
			fRec2[0] = ((fTemp0 * fTemp5) - (fTemp4 * fRec2[1]));
			float fTemp6 = ((fTemp0 * fTemp4) + (fRec2[1] * fTemp5));
			float fTemp7 = (fConst3 * fTemp1);
			float fTemp8 = ((1.0f - std::sin(fTemp7)) / std::cos(fTemp7));
			float fTemp9 = std::sin(fTemp8);
			float fTemp10 = std::cos(fTemp8);
			fRec3[0] = ((fTemp6 * fTemp10) - (fTemp9 * fRec3[1]));
			float fTemp11 = ((fTemp6 * fTemp9) + (fRec3[1] * fTemp10));
			float fTemp12 = (fConst4 * fTemp1);
			float fTemp13 = ((1.0f - std::sin(fTemp12)) / std::cos(fTemp12));
			float fTemp14 = std::sin(fTemp13);
			float fTemp15 = std::cos(fTemp13);
			fRec4[0] = ((fTemp11 * fTemp15) - (fTemp14 * fRec4[1]));
			float fTemp16 = ((fTemp11 * fTemp14) + (fRec4[1] * fTemp15));
			float fTemp17 = (fConst5 * fTemp1);
			float fTemp18 = ((1.0f - std::sin(fTemp17)) / std::cos(fTemp17));
			float fTemp19 = std::sin(fTemp18);
			float fTemp20 = std::cos(fTemp18);
			fRec5[0] = ((fTemp16 * fTemp20) - (fTemp19 * fRec5[1]));
			float fTemp21 = ((fTemp16 * fTemp19) + (fRec5[1] * fTemp20));
			float fTemp22 = (fConst6 * fTemp1);
			float fTemp23 = ((1.0f - std::sin(fTemp22)) / std::cos(fTemp22));
			float fTemp24 = std::sin(fTemp23);
			float fTemp25 = std::cos(fTemp23);
			fRec6[0] = ((fTemp21 * fTemp25) - (fTemp24 * fRec6[1]));
			float fTemp26 = ((fTemp21 * fTemp24) + (fRec6[1] * fTemp25));
			float fTemp27 = (fConst7 * fTemp1);
			float fTemp28 = ((1.0f - std::sin(fTemp27)) / std::cos(fTemp27));
			float fTemp29 = std::sin(fTemp28);
			float fTemp30 = std::cos(fTemp28);
			fRec7[0] = ((fTemp26 * fTemp30) - (fTemp29 * fRec7[1]));
			output0[i0] = FAUSTFLOAT(((fSlow1 * fTemp0) + (fSlow0 * ((fTemp26 * fTemp29) + (fRec7[1] * fTemp30)))));
			fRec1[1] = fRec1[0];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
		}
	}

};

#endif
