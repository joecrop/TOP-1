/* ------------------------------------------------------------
name: "phase-synth"
Code generated with Faust 2.37.3 (https://faust.grame.fr)
Compilation options: -a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __faust_phase_synth_H__
#define  __faust_phase_synth_H__

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

class faust_phase_synthSIG0 {
	
  private:
	
	int iVec1[2];
	int iRec3[2];
	
  public:
	
	int getNumInputsfaust_phase_synthSIG0() {
		return 0;
	}
	int getNumOutputsfaust_phase_synthSIG0() {
		return 1;
	}
	
	void instanceInitfaust_phase_synthSIG0(int sample_rate) {
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			iVec1[l4] = 0;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			iRec3[l5] = 0;
		}
	}
	
	void fillfaust_phase_synthSIG0(int count, float* table) {
		for (int i1 = 0; (i1 < count); i1 = (i1 + 1)) {
			iVec1[0] = 1;
			iRec3[0] = ((iVec1[1] + iRec3[1]) % 65536);
			table[i1] = std::sin((9.58738019e-05f * float(iRec3[0])));
			iVec1[1] = iVec1[0];
			iRec3[1] = iRec3[0];
		}
	}

};

static faust_phase_synthSIG0* newfaust_phase_synthSIG0() { return (faust_phase_synthSIG0*)new faust_phase_synthSIG0(); }
static void deletefaust_phase_synthSIG0(faust_phase_synthSIG0* dsp) { delete dsp; }

static float ftbl0faust_phase_synthSIG0[65536];

#ifndef FAUSTCLASS 
#define FAUSTCLASS faust_phase_synth
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class faust_phase_synth : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fHslider1;
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fHslider2;
	float fRec0[2];
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	float fConst2;
	float fConst3;
	FAUSTFLOAT fButton0;
	float fVec0[2];
	float fRec1[2];
	float fConst4;
	FAUSTFLOAT fHslider5;
	float fConst5;
	int iRec2[2];
	float fRec4[2];
	int iRec5[2];
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst9;
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.2");
		m->declare("compile_options", "-a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0");
		m->declare("envelopes.lib/adsr:author", "Yann Orlarey and Andrey Bundin");
		m->declare("envelopes.lib/ar:author", "Yann Orlarey, Stéphane Letz");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.1");
		m->declare("filename", "phase-synth.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "phase-synth");
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
		faust_phase_synthSIG0* sig0 = newfaust_phase_synthSIG0();
		sig0->instanceInitfaust_phase_synthSIG0(sample_rate);
		sig0->fillfaust_phase_synthSIG0(65536, ftbl0faust_phase_synthSIG0);
		deletefaust_phase_synthSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (440.0f / fConst0);
		fConst2 = std::max<float>(1.0f, (0.00100000005f * fConst0));
		fConst3 = (1.0f / fConst2);
		fConst4 = (0.100000001f * fConst0);
		fConst5 = (0.0500000007f * fConst0);
		fConst6 = std::max<float>(1.0f, (0.00999999978f * fConst0));
		fConst7 = (1.0f / fConst6);
		fConst8 = (0.300000012f / std::max<float>(1.0f, (0.150000006f * fConst0)));
		fConst9 = (1.0f / std::max<float>(1.0f, (0.300000012f * fConst0)));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(1.0f);
		fHslider1 = FAUSTFLOAT(0.5f);
		fHslider2 = FAUSTFLOAT(69.0f);
		fHslider3 = FAUSTFLOAT(0.5f);
		fHslider4 = FAUSTFLOAT(0.29999999999999999f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider5 = FAUSTFLOAT(2.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fVec0[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec1[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			iRec2[l3] = 0;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec4[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			iRec5[l7] = 0;
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
	
	virtual faust_phase_synth* clone() {
		return new faust_phase_synth();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("phase-synth");
		ui_interface->addHorizontalSlider("DISTORT", &fHslider3, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("KEY", &fHslider2, FAUSTFLOAT(69.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(127.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("MODDEPTH", &fHslider4, FAUSTFLOAT(0.300000012f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("MODRATE", &fHslider5, FAUSTFLOAT(2.0f), FAUSTFLOAT(0.100000001f), FAUSTFLOAT(10.0f), FAUSTFLOAT(0.100000001f));
		ui_interface->addHorizontalSlider("RESO", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addButton("TRIGGER", &fButton0);
		ui_interface->addHorizontalSlider("VELOCITY", &fHslider0, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = (0.5f * float(fHslider0));
		float fSlow1 = float(fHslider1);
		float fSlow2 = (0.5f * fSlow1);
		float fSlow3 = (1.0f - fSlow2);
		float fSlow4 = std::pow(2.0f, (0.0833333358f * (float(fHslider2) + -69.0f)));
		float fSlow5 = (fConst1 * fSlow4);
		float fSlow6 = (0.400000006f * float(fHslider3));
		float fSlow7 = float(fHslider4);
		float fSlow8 = float(fButton0);
		float fSlow9 = float(fHslider5);
		float fSlow10 = (0.699999988f / std::max<float>(1.0f, (fConst4 * fSlow9)));
		float fSlow11 = (1.0f / std::max<float>(1.0f, (fConst5 * fSlow9)));
		int iSlow12 = (fSlow8 == 0.0f);
		float fSlow13 = (fConst1 * (fSlow4 * ((3.0f * fSlow1) + 1.0f)));
		float fSlow14 = (1.0f / std::max<float>(1.0f, (fConst4 * (1.0f - fSlow1))));
		for (int i0 = 0; (i0 < count); i0 = (i0 + 1)) {
			fRec0[0] = (fSlow5 + (fRec0[1] - std::floor((fSlow5 + fRec0[1]))));
			fVec0[0] = fSlow8;
			fRec1[0] = (fSlow8 + (fRec1[1] * float((fVec0[1] >= fSlow8))));
			iRec2[0] = (iSlow12 * (iRec2[1] + 1));
			float fTemp0 = float(iRec2[0]);
			float fTemp1 = std::max<float>(0.00999999978f, std::min<float>(0.99000001f, ((fSlow6 * ((fSlow7 * std::max<float>(0.0f, (std::min<float>((fConst3 * fRec1[0]), std::max<float>(((fSlow10 * (fConst2 - fRec1[0])) + 1.0f), 0.300000012f)) * (1.0f - (fSlow11 * fTemp0))))) + 1.0f)) + 0.5f)));
			float fThen0 = (0.5f * ((fRec0[0] - fTemp1) / (1.0f - fTemp1)));
			float fElse0 = (0.5f * ((fRec0[0] / fTemp1) + 1.0f));
			fRec4[0] = (fSlow13 + (fRec4[1] - std::floor((fSlow13 + fRec4[1]))));
			iRec5[0] = (((iRec5[1] + (iRec5[1] > 0)) * (fSlow8 <= fVec0[1])) + (fSlow8 > fVec0[1]));
			float fTemp2 = float(iRec5[0]);
			output0[i0] = FAUSTFLOAT((fSlow0 * (((fSlow3 * std::cos((6.28318548f * ((fRec0[0] < fTemp1) ? fElse0 : fThen0)))) + (fSlow2 * (ftbl0faust_phase_synthSIG0[int((65536.0f * fRec4[0]))] * std::max<float>(0.0f, std::min<float>((fConst3 * fTemp2), ((fSlow14 * (fConst2 - fTemp2)) + 1.0f)))))) * std::max<float>(0.0f, (std::min<float>((fConst7 * fRec1[0]), std::max<float>(((fConst8 * (fConst6 - fRec1[0])) + 1.0f), 0.699999988f)) * (1.0f - (fConst9 * fTemp0)))))));
			fRec0[1] = fRec0[0];
			fVec0[1] = fVec0[0];
			fRec1[1] = fRec1[0];
			iRec2[1] = iRec2[0];
			fRec4[1] = fRec4[0];
			iRec5[1] = iRec5[0];
		}
	}

};

#endif
