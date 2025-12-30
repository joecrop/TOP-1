/* ------------------------------------------------------------
name: "cwo"
Code generated with Faust 2.37.3 (https://faust.grame.fr)
Compilation options: -a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __faust_cwo_H__
#define  __faust_cwo_H__

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

class faust_cwoSIG0 {
	
  private:
	
	int iVec1[2];
	int iRec1[2];
	
  public:
	
	int getNumInputsfaust_cwoSIG0() {
		return 0;
	}
	int getNumOutputsfaust_cwoSIG0() {
		return 1;
	}
	
	void instanceInitfaust_cwoSIG0(int sample_rate) {
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			iVec1[l1] = 0;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			iRec1[l2] = 0;
		}
	}
	
	void fillfaust_cwoSIG0(int count, float* table) {
		for (int i1 = 0; (i1 < count); i1 = (i1 + 1)) {
			iVec1[0] = 1;
			iRec1[0] = ((iVec1[1] + iRec1[1]) % 65536);
			table[i1] = std::sin((9.58738019e-05f * float(iRec1[0])));
			iVec1[1] = iVec1[0];
			iRec1[1] = iRec1[0];
		}
	}

};

static faust_cwoSIG0* newfaust_cwoSIG0() { return (faust_cwoSIG0*)new faust_cwoSIG0(); }
static void deletefaust_cwoSIG0(faust_cwoSIG0* dsp) { delete dsp; }

static float ftbl0faust_cwoSIG0[65536];

#ifndef FAUSTCLASS 
#define FAUSTCLASS faust_cwo
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class faust_cwo : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int IOTA;
	float fVec0[8192];
	int fSampleRate;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	float fConst3;
	FAUSTFLOAT fHslider3;
	float fRec2[2];
	float fConst4;
	float fRec3[2];
	float fConst5;
	float fRec4[2];
	float fConst6;
	float fRec5[2];
	float fRec0[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.2");
		m->declare("compile_options", "-a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "cwo.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "cwo");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.1");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
		faust_cwoSIG0* sig0 = newfaust_cwoSIG0();
		sig0->instanceInitfaust_cwoSIG0(sample_rate);
		sig0->fillfaust_cwoSIG0(65536, ftbl0faust_cwoSIG0);
		deletefaust_cwoSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (44.0999985f / fConst0);
		fConst2 = (1.0f - fConst1);
		fConst3 = (1.0f / fConst0);
		fConst4 = (6.30000019f / fConst0);
		fConst5 = (7.0999999f / fConst0);
		fConst6 = (11.6999998f / fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.5f);
		fHslider1 = FAUSTFLOAT(0.29999999999999999f);
		fHslider2 = FAUSTFLOAT(0.20000000000000001f);
		fHslider3 = FAUSTFLOAT(2.0f);
	}
	
	virtual void instanceClear() {
		IOTA = 0;
		for (int l0 = 0; (l0 < 8192); l0 = (l0 + 1)) {
			fVec0[l0] = 0.0f;
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
			fRec0[l7] = 0.0f;
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
	
	virtual faust_cwo* clone() {
		return new faust_cwo();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("cwo");
		ui_interface->addHorizontalSlider("DEPTH", &fHslider1, FAUSTFLOAT(0.300000012f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("FLUTTER", &fHslider2, FAUSTFLOAT(0.200000003f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("MIX", &fHslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("RATE", &fHslider3, FAUSTFLOAT(2.0f), FAUSTFLOAT(0.100000001f), FAUSTFLOAT(10.0f), FAUSTFLOAT(0.100000001f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fHslider0);
		float fSlow1 = (1.0f - fSlow0);
		float fSlow2 = (1920.0f * float(fHslider1));
		float fSlow3 = float(fHslider2);
		float fSlow4 = (0.5f * (1.0f - fSlow3));
		float fSlow5 = float(fHslider3);
		float fSlow6 = (fConst3 * fSlow5);
		float fSlow7 = (fConst4 * fSlow5);
		float fSlow8 = (fConst5 * fSlow5);
		float fSlow9 = (fConst6 * fSlow5);
		for (int i0 = 0; (i0 < count); i0 = (i0 + 1)) {
			float fTemp0 = float(input0[i0]);
			fVec0[(IOTA & 8191)] = fTemp0;
			fRec2[0] = (fSlow6 + (fRec2[1] - std::floor((fSlow6 + fRec2[1]))));
			fRec3[0] = (fSlow7 + (fRec3[1] - std::floor((fSlow7 + fRec3[1]))));
			fRec4[0] = (fSlow8 + (fRec4[1] - std::floor((fSlow8 + fRec4[1]))));
			fRec5[0] = (fSlow9 + (fRec5[1] - std::floor((fSlow9 + fRec5[1]))));
			fRec0[0] = ((fConst2 * fRec0[1]) + (fConst1 * ((fSlow2 * ((fSlow4 * (ftbl0faust_cwoSIG0[int((65536.0f * fRec2[0]))] + 1.0f)) + (fSlow3 * ((0.166666672f * ((ftbl0faust_cwoSIG0[int((65536.0f * fRec3[0]))] + ftbl0faust_cwoSIG0[int((65536.0f * fRec4[0]))]) + ftbl0faust_cwoSIG0[int((65536.0f * fRec5[0]))])) + 0.5f)))) + 2400.0f)));
			int iTemp1 = int(fRec0[0]);
			float fTemp2 = std::floor(fRec0[0]);
			output0[i0] = FAUSTFLOAT(((fSlow1 * fTemp0) + (fSlow0 * ((fVec0[((IOTA - std::min<int>(4801, std::max<int>(0, iTemp1))) & 8191)] * (fTemp2 + (1.0f - fRec0[0]))) + ((fRec0[0] - fTemp2) * fVec0[((IOTA - std::min<int>(4801, std::max<int>(0, (iTemp1 + 1)))) & 8191)])))));
			IOTA = (IOTA + 1);
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec0[1] = fRec0[0];
		}
	}

};

#endif
