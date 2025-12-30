/* ------------------------------------------------------------
name: "fm-synth"
Code generated with Faust 2.37.3 (https://faust.grame.fr)
Compilation options: -a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __faust_fm_synth_H__
#define  __faust_fm_synth_H__

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

class faust_fm_synthSIG0 {
	
  private:
	
	int iVec1[2];
	int iRec2[2];
	
  public:
	
	int getNumInputsfaust_fm_synthSIG0() {
		return 0;
	}
	int getNumOutputsfaust_fm_synthSIG0() {
		return 1;
	}
	
	void instanceInitfaust_fm_synthSIG0(int sample_rate) {
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			iVec1[l3] = 0;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			iRec2[l4] = 0;
		}
	}
	
	void fillfaust_fm_synthSIG0(int count, float* table) {
		for (int i1 = 0; (i1 < count); i1 = (i1 + 1)) {
			iVec1[0] = 1;
			iRec2[0] = ((iVec1[1] + iRec2[1]) % 65536);
			table[i1] = std::sin((9.58738019e-05f * float(iRec2[0])));
			iVec1[1] = iVec1[0];
			iRec2[1] = iRec2[0];
		}
	}

};

static faust_fm_synthSIG0* newfaust_fm_synthSIG0() { return (faust_fm_synthSIG0*)new faust_fm_synthSIG0(); }
static void deletefaust_fm_synthSIG0(faust_fm_synthSIG0* dsp) { delete dsp; }

static float ftbl0faust_fm_synthSIG0[65536];

#ifndef FAUSTCLASS 
#define FAUSTCLASS faust_fm_synth
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class faust_fm_synth : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int fSampleRate;
	float fConst0;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fButton0;
	float fVec0[2];
	float fRec0[2];
	float fConst1;
	FAUSTFLOAT fHslider2;
	int iRec1[2];
	float fConst2;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	float fConst3;
	FAUSTFLOAT fHslider5;
	float fRec4[2];
	float fRec3[2];
	
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
		m->declare("filename", "fm-synth.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "fm-synth");
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
		faust_fm_synthSIG0* sig0 = newfaust_fm_synthSIG0();
		sig0->instanceInitfaust_fm_synthSIG0(sample_rate);
		sig0->fillfaust_fm_synthSIG0(65536, ftbl0faust_fm_synthSIG0);
		deletefaust_fm_synthSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (0.300000012f / std::max<float>(1.0f, (0.100000001f * fConst0)));
		fConst2 = (1.0f / fConst0);
		fConst3 = (440.0f / fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(1.0f);
		fHslider1 = FAUSTFLOAT(0.01f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(0.29999999999999999f);
		fHslider3 = FAUSTFLOAT(69.0f);
		fHslider4 = FAUSTFLOAT(0.5f);
		fHslider5 = FAUSTFLOAT(2.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec0[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			iRec1[l2] = 0;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec4[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec3[l6] = 0.0f;
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
	
	virtual faust_fm_synth* clone() {
		return new faust_fm_synth();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fm-synth");
		ui_interface->addHorizontalSlider("ATTACK", &fHslider1, FAUSTFLOAT(0.00999999978f), FAUSTFLOAT(0.00100000005f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00100000005f));
		ui_interface->addHorizontalSlider("DEPTH", &fHslider4, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("KEY", &fHslider3, FAUSTFLOAT(69.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(127.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("RATIO", &fHslider5, FAUSTFLOAT(2.0f), FAUSTFLOAT(0.5f), FAUSTFLOAT(8.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("RELEASE", &fHslider2, FAUSTFLOAT(0.300000012f), FAUSTFLOAT(0.00999999978f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addButton("TRIGGER", &fButton0);
		ui_interface->addHorizontalSlider("VELOCITY", &fHslider0, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = (0.5f * float(fHslider0));
		float fSlow1 = std::max<float>(1.0f, (fConst0 * float(fHslider1)));
		float fSlow2 = (1.0f / fSlow1);
		float fSlow3 = float(fButton0);
		float fSlow4 = (1.0f / std::max<float>(1.0f, (fConst0 * float(fHslider2))));
		int iSlow5 = (fSlow3 == 0.0f);
		float fSlow6 = std::pow(2.0f, (0.0833333358f * (float(fHslider3) + -69.0f)));
		float fSlow7 = (fConst2 * fSlow6);
		float fSlow8 = (4400.0f * float(fHslider4));
		float fSlow9 = (fConst3 * (float(fHslider5) * fSlow6));
		for (int i0 = 0; (i0 < count); i0 = (i0 + 1)) {
			fVec0[0] = fSlow3;
			fRec0[0] = (fSlow3 + (fRec0[1] * float((fVec0[1] >= fSlow3))));
			iRec1[0] = (iSlow5 * (iRec1[1] + 1));
			float fTemp0 = std::max<float>(0.0f, (std::min<float>((fSlow2 * fRec0[0]), std::max<float>((1.0f - (fConst1 * (fRec0[0] - fSlow1))), 0.699999988f)) * (1.0f - (fSlow4 * float(iRec1[0])))));
			fRec4[0] = (fSlow9 + (fRec4[1] - std::floor((fSlow9 + fRec4[1]))));
			float fTemp1 = (fRec3[1] + (fSlow7 * ((fSlow8 * (fTemp0 * ftbl0faust_fm_synthSIG0[int((65536.0f * fRec4[0]))])) + 440.0f)));
			fRec3[0] = (fTemp1 - std::floor(fTemp1));
			output0[i0] = FAUSTFLOAT((fSlow0 * (fTemp0 * ftbl0faust_fm_synthSIG0[int((65536.0f * fRec3[0]))])));
			fVec0[1] = fVec0[0];
			fRec0[1] = fRec0[0];
			iRec1[1] = iRec1[0];
			fRec4[1] = fRec4[0];
			fRec3[1] = fRec3[0];
		}
	}

};

#endif
