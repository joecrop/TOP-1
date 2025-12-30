/* ------------------------------------------------------------
name: "pulse-synth"
Code generated with Faust 2.37.3 (https://faust.grame.fr)
Compilation options: -a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __faust_pulse_synth_H__
#define  __faust_pulse_synth_H__

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

class faust_pulse_synthSIG0 {
	
  private:
	
	int iVec5[2];
	int iRec2[2];
	
  public:
	
	int getNumInputsfaust_pulse_synthSIG0() {
		return 0;
	}
	int getNumOutputsfaust_pulse_synthSIG0() {
		return 1;
	}
	
	void instanceInitfaust_pulse_synthSIG0(int sample_rate) {
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			iVec5[l7] = 0;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			iRec2[l8] = 0;
		}
	}
	
	void fillfaust_pulse_synthSIG0(int count, float* table) {
		for (int i1 = 0; (i1 < count); i1 = (i1 + 1)) {
			iVec5[0] = 1;
			iRec2[0] = ((iVec5[1] + iRec2[1]) % 65536);
			table[i1] = std::sin((9.58738019e-05f * float(iRec2[0])));
			iVec5[1] = iVec5[0];
			iRec2[1] = iRec2[0];
		}
	}

};

static faust_pulse_synthSIG0* newfaust_pulse_synthSIG0() { return (faust_pulse_synthSIG0*)new faust_pulse_synthSIG0(); }
static void deletefaust_pulse_synthSIG0(faust_pulse_synthSIG0* dsp) { delete dsp; }

static float faust_pulse_synth_faustpower2_f(float value) {
	return (value * value);
}
static float ftbl0faust_pulse_synthSIG0[65536];

#ifndef FAUSTCLASS 
#define FAUSTCLASS faust_pulse_synth
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class faust_pulse_synth : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider1;
	int iVec0[2];
	float fConst2;
	float fRec0[2];
	float fVec1[2];
	int IOTA;
	float fVec2[4096];
	float fRec1[2];
	float fVec3[2];
	float fVec4[4096];
	float fConst3;
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fHslider3;
	float fRec3[2];
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fButton0;
	float fVec6[2];
	float fRec4[2];
	float fConst4;
	FAUSTFLOAT fHslider5;
	int iRec5[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.2");
		m->declare("compile_options", "-a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0");
		m->declare("envelopes.lib/adsr:author", "Yann Orlarey and Andrey Bundin");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.1");
		m->declare("filename", "pulse-synth.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "pulse-synth");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
		faust_pulse_synthSIG0* sig0 = newfaust_pulse_synthSIG0();
		sig0->instanceInitfaust_pulse_synthSIG0(sample_rate);
		sig0->fillfaust_pulse_synthSIG0(65536, ftbl0faust_pulse_synthSIG0);
		deletefaust_pulse_synthSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (0.25f * fConst0);
		fConst2 = (1.0f / fConst0);
		fConst3 = (0.5f * fConst0);
		fConst4 = (0.200000003f / std::max<float>(1.0f, (0.100000001f * fConst0)));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(1.0f);
		fHslider1 = FAUSTFLOAT(69.0f);
		fHslider2 = FAUSTFLOAT(0.5f);
		fHslider3 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(0.01f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider5 = FAUSTFLOAT(0.29999999999999999f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec0[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fVec1[l2] = 0.0f;
		}
		IOTA = 0;
		for (int l3 = 0; (l3 < 4096); l3 = (l3 + 1)) {
			fVec2[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec1[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fVec3[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 4096); l6 = (l6 + 1)) {
			fVec4[l6] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec3[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fVec6[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec4[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			iRec5[l12] = 0;
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
	
	virtual faust_pulse_synth* clone() {
		return new faust_pulse_synth();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("pulse-synth");
		ui_interface->addHorizontalSlider("ATTACK", &fHslider4, FAUSTFLOAT(0.00999999978f), FAUSTFLOAT(0.00100000005f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00100000005f));
		ui_interface->addHorizontalSlider("KEY", &fHslider1, FAUSTFLOAT(69.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(127.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("PWM", &fHslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(10.0f), FAUSTFLOAT(0.100000001f));
		ui_interface->addHorizontalSlider("RELEASE", &fHslider5, FAUSTFLOAT(0.300000012f), FAUSTFLOAT(0.00999999978f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addButton("TRIGGER", &fButton0);
		ui_interface->addHorizontalSlider("VELOCITY", &fHslider0, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("WIDTH", &fHslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0500000007f), FAUSTFLOAT(0.949999988f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = (0.400000006f * float(fHslider0));
		float fSlow1 = std::pow(2.0f, (0.0833333358f * (float(fHslider1) + -69.0f)));
		float fSlow2 = std::max<float>((440.0f * fSlow1), 23.4489498f);
		float fSlow3 = std::max<float>(20.0f, std::fabs(fSlow2));
		float fSlow4 = (fConst1 / fSlow3);
		float fSlow5 = (fConst2 * fSlow3);
		float fSlow6 = std::max<float>((220.0f * fSlow1), 23.4489498f);
		float fSlow7 = std::max<float>(20.0f, std::fabs(fSlow6));
		float fSlow8 = (fConst1 / fSlow7);
		float fSlow9 = (fConst2 * fSlow7);
		float fSlow10 = std::max<float>(0.0f, std::min<float>(2047.0f, (fConst3 / fSlow6)));
		float fSlow11 = std::floor(fSlow10);
		float fSlow12 = (fSlow11 + (1.0f - fSlow10));
		int iSlow13 = int(fSlow10);
		float fSlow14 = (fSlow10 - fSlow11);
		int iSlow15 = (iSlow13 + 1);
		float fSlow16 = (fConst0 / fSlow2);
		float fSlow17 = float(fHslider2);
		float fSlow18 = (fConst2 * float(fHslider3));
		float fSlow19 = std::max<float>(1.0f, (fConst0 * float(fHslider4)));
		float fSlow20 = (1.0f / fSlow19);
		float fSlow21 = float(fButton0);
		float fSlow22 = (1.0f / std::max<float>(1.0f, (fConst0 * float(fHslider5))));
		int iSlow23 = (fSlow21 == 0.0f);
		for (int i0 = 0; (i0 < count); i0 = (i0 + 1)) {
			iVec0[0] = 1;
			float fTemp0 = float(iVec0[1]);
			fRec0[0] = (fSlow5 + (fRec0[1] - std::floor((fSlow5 + fRec0[1]))));
			float fTemp1 = faust_pulse_synth_faustpower2_f(((2.0f * fRec0[0]) + -1.0f));
			fVec1[0] = fTemp1;
			float fTemp2 = (fSlow4 * (fTemp0 * (fTemp1 - fVec1[1])));
			fVec2[(IOTA & 4095)] = fTemp2;
			fRec1[0] = (fSlow9 + (fRec1[1] - std::floor((fSlow9 + fRec1[1]))));
			float fTemp3 = faust_pulse_synth_faustpower2_f(((2.0f * fRec1[0]) + -1.0f));
			fVec3[0] = fTemp3;
			float fTemp4 = (fSlow8 * (fTemp0 * (fTemp3 - fVec3[1])));
			fVec4[(IOTA & 4095)] = fTemp4;
			fRec3[0] = (fSlow18 + (fRec3[1] - std::floor((fSlow18 + fRec3[1]))));
			float fTemp5 = std::max<float>(0.0f, std::min<float>(2047.0f, (fSlow16 * std::min<float>(0.949999988f, std::max<float>(0.0500000007f, (fSlow17 + (0.300000012f * ftbl0faust_pulse_synthSIG0[int((65536.0f * fRec3[0]))])))))));
			int iTemp6 = int(fTemp5);
			float fTemp7 = std::floor(fTemp5);
			fVec6[0] = fSlow21;
			fRec4[0] = (fSlow21 + (fRec4[1] * float((fVec6[1] >= fSlow21))));
			iRec5[0] = (iSlow23 * (iRec5[1] + 1));
			output0[i0] = FAUSTFLOAT((fSlow0 * (((fTemp2 + (0.300000012f * (fTemp4 - ((fSlow12 * fVec4[((IOTA - iSlow13) & 4095)]) + (fSlow14 * fVec4[((IOTA - iSlow15) & 4095)]))))) - ((fVec2[((IOTA - iTemp6) & 4095)] * (fTemp7 + (1.0f - fTemp5))) + ((fTemp5 - fTemp7) * fVec2[((IOTA - (iTemp6 + 1)) & 4095)]))) * std::max<float>(0.0f, (std::min<float>((fSlow20 * fRec4[0]), std::max<float>(((fConst4 * (fSlow19 - fRec4[0])) + 1.0f), 0.800000012f)) * (1.0f - (fSlow22 * float(iRec5[0]))))))));
			iVec0[1] = iVec0[0];
			fRec0[1] = fRec0[0];
			fVec1[1] = fVec1[0];
			IOTA = (IOTA + 1);
			fRec1[1] = fRec1[0];
			fVec3[1] = fVec3[0];
			fRec3[1] = fRec3[0];
			fVec6[1] = fVec6[0];
			fRec4[1] = fRec4[0];
			iRec5[1] = iRec5[0];
		}
	}

};

#endif
