/* ------------------------------------------------------------
name: "metronome"
Code generated with Faust 2.37.3 (https://faust.grame.fr)
Compilation options: -a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __faust_metronome_H__
#define  __faust_metronome_H__

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

static float faust_metronome_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS faust_metronome
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class faust_metronome : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fHslider1;
	int iVec0[2];
	float fConst2;
	float fRec0[2];
	float fVec1[2];
	int IOTA;
	float fVec2[1024];
	float fConst3;
	float fConst4;
	float fConst5;
	FAUSTFLOAT fButton0;
	float fVec3[2];
	int iRec1[2];
	float fConst6;
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0");
		m->declare("envelopes.lib/ar:author", "Yann Orlarey, Stéphane Letz");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.1");
		m->declare("filename", "metronome.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "metronome");
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
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (0.25f * fConst0);
		fConst2 = (1.0f / fConst0);
		fConst3 = (0.5f * fConst0);
		fConst4 = std::max<float>(1.0f, (1.00000001e-07f * fConst0));
		fConst5 = (1.0f / fConst4);
		fConst6 = (1.0f / std::max<float>(1.0f, (0.0900000036f * fConst0)));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(1.0f);
		fHslider1 = FAUSTFLOAT(12.0f);
		fButton0 = FAUSTFLOAT(0.0f);
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
		for (int l3 = 0; (l3 < 1024); l3 = (l3 + 1)) {
			fVec2[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fVec3[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			iRec1[l5] = 0;
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
	
	virtual faust_metronome* clone() {
		return new faust_metronome();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("metronome");
		ui_interface->addHorizontalSlider("GAIN", &fHslider0, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("TONE", &fHslider1, FAUSTFLOAT(12.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(24.0f), FAUSTFLOAT(1.0f));
		ui_interface->addButton("TRIGGER", &fButton0);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fHslider0);
		float fSlow1 = std::max<float>((110.0f * std::pow(2.0f, (0.0833333358f * float(fHslider1)))), 23.4489498f);
		float fSlow2 = std::max<float>(20.0f, std::fabs(fSlow1));
		float fSlow3 = (fConst1 / fSlow2);
		float fSlow4 = (fConst2 * fSlow2);
		float fSlow5 = std::max<float>(0.0f, std::min<float>(2047.0f, (fConst3 / fSlow1)));
		float fSlow6 = std::floor(fSlow5);
		float fSlow7 = (fSlow6 + (1.0f - fSlow5));
		int iSlow8 = int(fSlow5);
		float fSlow9 = (fSlow5 - fSlow6);
		int iSlow10 = (iSlow8 + 1);
		float fSlow11 = float(fButton0);
		for (int i0 = 0; (i0 < count); i0 = (i0 + 1)) {
			iVec0[0] = 1;
			fRec0[0] = (fSlow4 + (fRec0[1] - std::floor((fSlow4 + fRec0[1]))));
			float fTemp0 = faust_metronome_faustpower2_f(((2.0f * fRec0[0]) + -1.0f));
			fVec1[0] = fTemp0;
			float fTemp1 = (fSlow3 * (float(iVec0[1]) * (fTemp0 - fVec1[1])));
			fVec2[(IOTA & 1023)] = fTemp1;
			fVec3[0] = fSlow11;
			iRec1[0] = (((iRec1[1] + (iRec1[1] > 0)) * (fSlow11 <= fVec3[1])) + (fSlow11 > fVec3[1]));
			float fTemp2 = float(iRec1[0]);
			output0[i0] = FAUSTFLOAT((fSlow0 * ((fTemp1 - ((fSlow7 * fVec2[((IOTA - iSlow8) & 1023)]) + (fSlow9 * fVec2[((IOTA - iSlow10) & 1023)]))) * std::max<float>(0.0f, std::min<float>((fConst5 * fTemp2), ((fConst6 * (fConst4 - fTemp2)) + 1.0f))))));
			iVec0[1] = iVec0[0];
			fRec0[1] = fRec0[0];
			fVec1[1] = fVec1[0];
			IOTA = (IOTA + 1);
			fVec3[1] = fVec3[0];
			iRec1[1] = iRec1[0];
		}
	}

};

#endif
