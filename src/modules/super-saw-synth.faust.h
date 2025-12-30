/* ------------------------------------------------------------
name: "super-saw-synth"
Code generated with Faust 2.37.3 (https://faust.grame.fr)
Compilation options: -a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __faust_super_saw_synth_H__
#define  __faust_super_saw_synth_H__

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

static float faust_super_saw_synth_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS faust_super_saw_synth
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class faust_super_saw_synth : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	float fConst2;
	FAUSTFLOAT fHslider3;
	float fRec1[2];
	float fRec0[3];
	FAUSTFLOAT fHslider4;
	float fRec4[2];
	float fRec3[3];
	float fRec7[2];
	float fRec6[3];
	float fRec10[2];
	float fRec9[3];
	FAUSTFLOAT fVslider0;
	FAUSTFLOAT fButton0;
	float fVec0[2];
	float fRec12[2];
	FAUSTFLOAT fVslider1;
	FAUSTFLOAT fVslider2;
	FAUSTFLOAT fVslider3;
	int iRec13[2];
	
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
		m->declare("filename", "super-saw-synth.dsp");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/resonlp:author", "Julius O. Smith III");
		m->declare("filters.lib/resonlp:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/resonlp:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "super-saw-synth");
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
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (3.14159274f / fConst0);
		fConst2 = (1.0f / fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(15000.0f);
		fHslider1 = FAUSTFLOAT(1.0f);
		fHslider2 = FAUSTFLOAT(1.0f);
		fHslider3 = FAUSTFLOAT(69.0f);
		fHslider4 = FAUSTFLOAT(0.0f);
		fVslider0 = FAUSTFLOAT(0.0f);
		fButton0 = FAUSTFLOAT(0.0f);
		fVslider1 = FAUSTFLOAT(1.0f);
		fVslider2 = FAUSTFLOAT(0.0f);
		fVslider3 = FAUSTFLOAT(0.20000000000000001f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec1[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 3); l1 = (l1 + 1)) {
			fRec0[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec4[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 3); l3 = (l3 + 1)) {
			fRec3[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec7[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 3); l5 = (l5 + 1)) {
			fRec6[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec10[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 3); l7 = (l7 + 1)) {
			fRec9[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fVec0[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec12[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			iRec13[l10] = 0;
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
	
	virtual faust_super_saw_synth* clone() {
		return new faust_super_saw_synth();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("super-saw-synth");
		ui_interface->addHorizontalSlider("DETUNE", &fHslider4, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->openHorizontalBox("ENVELOPE");
		ui_interface->addVerticalSlider("ATTACK", &fVslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.0199999996f));
		ui_interface->addVerticalSlider("DECAY", &fVslider2, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.0199999996f));
		ui_interface->addVerticalSlider("RELEASE", &fVslider3, FAUSTFLOAT(0.200000003f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.0199999996f));
		ui_interface->addVerticalSlider("SUSTAIN", &fVslider1, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
		ui_interface->addHorizontalSlider("FILTER", &fHslider0, FAUSTFLOAT(15000.0f), FAUSTFLOAT(10.0f), FAUSTFLOAT(15000.0f), FAUSTFLOAT(10.0f));
		ui_interface->addHorizontalSlider("KEY", &fHslider3, FAUSTFLOAT(69.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(127.0f), FAUSTFLOAT(0.100000001f));
		ui_interface->addButton("TRIGGER", &fButton0);
		ui_interface->addHorizontalSlider("VELOCITY", &fHslider1, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("VOICES", &fHslider2, FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fHslider0);
		float fSlow1 = (0.125f / (1.125f - (6.66666674e-05f * fSlow0)));
		float fSlow2 = std::tan((fConst1 * (float(fHslider1) * fSlow0)));
		float fSlow3 = (1.0f / fSlow2);
		float fSlow4 = (1.0f / (((fSlow1 + fSlow3) / fSlow2) + 1.0f));
		float fSlow5 = float(fHslider2);
		float fThen1 = ((fSlow5 < 0.0f) ? 0.0f : fSlow5);
		float fSlow6 = ((fSlow5 > 1.0f) ? 1.0f : fThen1);
		float fSlow7 = std::pow(2.0f, (0.0833333358f * (float(fHslider3) + -69.0f)));
		float fSlow8 = std::max<float>(1.1920929e-07f, std::fabs((440.0f * fSlow7)));
		float fSlow9 = (fConst2 * fSlow8);
		float fSlow10 = (1.0f - (fConst0 / fSlow8));
		float fSlow11 = (((fSlow3 - fSlow1) / fSlow2) + 1.0f);
		float fSlow12 = (2.0f * (1.0f - (1.0f / faust_super_saw_synth_faustpower2_f(fSlow2))));
		float fSlow13 = (fSlow5 + -1.0f);
		float fThen5 = ((fSlow13 < 0.0f) ? 0.0f : fSlow13);
		float fSlow14 = ((fSlow13 > 1.0f) ? 1.0f : fThen5);
		float fSlow15 = float(fHslider4);
		float fSlow16 = std::max<float>(1.1920929e-07f, std::fabs((220.0f * (fSlow7 * ((0.0416666679f * fSlow15) + 1.0f)))));
		float fSlow17 = (fConst2 * fSlow16);
		float fSlow18 = (1.0f - (fConst0 / fSlow16));
		float fSlow19 = (fSlow5 + -2.0f);
		float fThen9 = ((fSlow19 < 0.0f) ? 0.0f : fSlow19);
		float fSlow20 = ((fSlow19 > 1.0f) ? 1.0f : fThen9);
		float fSlow21 = std::max<float>(1.1920929e-07f, std::fabs((110.0f * (fSlow7 * ((0.0416666679f * (0.0f - fSlow15)) + 1.0f)))));
		float fSlow22 = (fConst2 * fSlow21);
		float fSlow23 = (1.0f - (fConst0 / fSlow21));
		float fSlow24 = (fSlow5 + -3.0f);
		float fThen13 = ((fSlow24 < 0.0f) ? 0.0f : fSlow24);
		float fSlow25 = ((fSlow24 > 1.0f) ? 1.0f : fThen13);
		float fSlow26 = std::max<float>(1.1920929e-07f, std::fabs((55.0f * fSlow7)));
		float fSlow27 = (fConst2 * fSlow26);
		float fSlow28 = (1.0f - (fConst0 / fSlow26));
		float fSlow29 = std::max<float>(1.0f, (fConst0 * float(fVslider0)));
		float fSlow30 = (1.0f / fSlow29);
		float fSlow31 = float(fButton0);
		float fSlow32 = (100.0f * float(fVslider1));
		float fSlow33 = ((1.0f - fSlow32) / std::max<float>(1.0f, (fConst0 * float(fVslider2))));
		float fSlow34 = (1.0f / std::max<float>(1.0f, (fConst0 * float(fVslider3))));
		int iSlow35 = (fSlow31 == 0.0f);
		for (int i0 = 0; (i0 < count); i0 = (i0 + 1)) {
			float fTemp0 = (fSlow9 + (fRec1[1] + -1.0f));
			int iTemp1 = (fTemp0 < 0.0f);
			float fTemp2 = (fSlow9 + fRec1[1]);
			fRec1[0] = (iTemp1 ? fTemp2 : fTemp0);
			float fThen3 = (fSlow9 + (fRec1[1] + (fSlow10 * fTemp0)));
			float fRec2 = (iTemp1 ? fTemp2 : fThen3);
			fRec0[0] = ((2.0f * fRec2) + (-1.0f - (fSlow4 * ((fSlow11 * fRec0[2]) + (fSlow12 * fRec0[1])))));
			float fTemp3 = (fSlow17 + (fRec4[1] + -1.0f));
			int iTemp4 = (fTemp3 < 0.0f);
			float fTemp5 = (fSlow17 + fRec4[1]);
			fRec4[0] = (iTemp4 ? fTemp5 : fTemp3);
			float fThen7 = (fSlow17 + (fRec4[1] + (fSlow18 * fTemp3)));
			float fRec5 = (iTemp4 ? fTemp5 : fThen7);
			fRec3[0] = ((2.0f * fRec5) + (-1.0f - (fSlow4 * ((fSlow11 * fRec3[2]) + (fSlow12 * fRec3[1])))));
			float fTemp6 = (fSlow22 + (fRec7[1] + -1.0f));
			int iTemp7 = (fTemp6 < 0.0f);
			float fTemp8 = (fSlow22 + fRec7[1]);
			fRec7[0] = (iTemp7 ? fTemp8 : fTemp6);
			float fThen11 = (fSlow22 + (fRec7[1] + (fSlow23 * fTemp6)));
			float fRec8 = (iTemp7 ? fTemp8 : fThen11);
			fRec6[0] = ((2.0f * fRec8) + (-1.0f - (fSlow4 * ((fSlow11 * fRec6[2]) + (fSlow12 * fRec6[1])))));
			float fTemp9 = (fSlow27 + (fRec10[1] + -1.0f));
			int iTemp10 = (fTemp9 < 0.0f);
			float fTemp11 = (fSlow27 + fRec10[1]);
			fRec10[0] = (iTemp10 ? fTemp11 : fTemp9);
			float fThen15 = (fSlow27 + (fRec10[1] + (fSlow28 * fTemp9)));
			float fRec11 = (iTemp10 ? fTemp11 : fThen15);
			fRec9[0] = ((2.0f * fRec11) + (-1.0f - (fSlow4 * ((fSlow11 * fRec9[2]) + (fSlow12 * fRec9[1])))));
			fVec0[0] = fSlow31;
			fRec12[0] = (fSlow31 + (fRec12[1] * float((fVec0[1] >= fSlow31))));
			iRec13[0] = (iSlow35 * (iRec13[1] + 1));
			output0[i0] = FAUSTFLOAT((fSlow4 * (((((fSlow6 * (fRec0[2] + (fRec0[0] + (2.0f * fRec0[1])))) + (fSlow14 * (fRec3[2] + (fRec3[0] + (2.0f * fRec3[1]))))) + (fSlow20 * (fRec6[2] + (fRec6[0] + (2.0f * fRec6[1]))))) + (fSlow25 * (fRec9[2] + (fRec9[0] + (2.0f * fRec9[1]))))) * std::max<float>(0.0f, (std::min<float>((fSlow30 * fRec12[0]), std::max<float>(((fSlow33 * (fSlow29 - fRec12[0])) + 1.0f), fSlow32)) * (1.0f - (fSlow34 * float(iRec13[0]))))))));
			fRec1[1] = fRec1[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fRec4[1] = fRec4[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec7[1] = fRec7[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fRec10[1] = fRec10[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			fVec0[1] = fVec0[0];
			fRec12[1] = fRec12[0];
			iRec13[1] = iRec13[0];
		}
	}

};

#endif
