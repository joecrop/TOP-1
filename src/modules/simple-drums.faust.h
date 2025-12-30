/* ------------------------------------------------------------
name: "simple-drums"
Code generated with Faust 2.37.3 (https://faust.grame.fr)
Compilation options: -a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __faust_simple_drums_H__
#define  __faust_simple_drums_H__

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

static float faust_simple_drums_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS faust_simple_drums
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class faust_simple_drums : public dsp {
	
 private:
	
	FAUSTFLOAT fVslider0;
	int iVec0[2];
	int fSampleRate;
	float fConst0;
	FAUSTFLOAT fVslider1;
	FAUSTFLOAT fButton0;
	float fVec1[2];
	int iRec0[2];
	FAUSTFLOAT fVslider2;
	FAUSTFLOAT fCheckbox0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fConst2;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	float fConst3;
	float fConst4;
	FAUSTFLOAT fHbargraph0;
	float fRec1[2];
	float fVec2[2];
	int IOTA;
	float fVec3[4096];
	float fConst5;
	int iRec2[2];
	FAUSTFLOAT fCheckbox1;
	float fConst6;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fHslider5;
	FAUSTFLOAT fHslider6;
	FAUSTFLOAT fHbargraph1;
	float fRec4[2];
	float fVec4[2];
	float fVec5[4096];
	float fRec3[3];
	FAUSTFLOAT fHslider7;
	float fRec5[3];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0");
		m->declare("envelopes.lib/ar:author", "Yann Orlarey, Stéphane Letz");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.1");
		m->declare("filename", "simple-drums.dsp");
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
		m->declare("name", "simple-drums");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "0.1");
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
		fConst1 = (0.25f * fConst0);
		fConst2 = (1.0f / fConst0);
		fConst3 = std::max<float>(1.0f, (9.99999975e-05f * fConst0));
		fConst4 = (1.0f / fConst3);
		fConst5 = (0.5f * fConst0);
		fConst6 = (3.14159274f / fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(1.0f);
		fVslider1 = FAUSTFLOAT(0.0f);
		fButton0 = FAUSTFLOAT(0.0f);
		fVslider2 = FAUSTFLOAT(0.20000000000000001f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(0.20000000000000001f);
		fHslider1 = FAUSTFLOAT(500.0f);
		fHslider2 = FAUSTFLOAT(0.5f);
		fCheckbox1 = FAUSTFLOAT(0.0f);
		fHslider3 = FAUSTFLOAT(1000.0f);
		fHslider4 = FAUSTFLOAT(0.20000000000000001f);
		fHslider5 = FAUSTFLOAT(500.0f);
		fHslider6 = FAUSTFLOAT(0.5f);
		fHslider7 = FAUSTFLOAT(1000.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fVec1[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			iRec0[l2] = 0;
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec1[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fVec2[l4] = 0.0f;
		}
		IOTA = 0;
		for (int l5 = 0; (l5 < 4096); l5 = (l5 + 1)) {
			fVec3[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			iRec2[l6] = 0;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec4[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fVec4[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 4096); l9 = (l9 + 1)) {
			fVec5[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 3); l10 = (l10 + 1)) {
			fRec3[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 3); l11 = (l11 + 1)) {
			fRec5[l11] = 0.0f;
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
	
	virtual faust_simple_drums* clone() {
		return new faust_simple_drums();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("simple-drums");
		ui_interface->openVerticalBox("D1");
		ui_interface->addHorizontalSlider("CUTOFF", &fHslider3, FAUSTFLOAT(1000.0f), FAUSTFLOAT(5.0f), FAUSTFLOAT(10000.0f), FAUSTFLOAT(0.00100000005f));
		ui_interface->addHorizontalBargraph("DECAY_GRAPH", &fHbargraph1, FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addCheckButton("FILTER_SWITCH", &fCheckbox1);
		ui_interface->addHorizontalSlider("FREQ", &fHslider5, FAUSTFLOAT(500.0f), FAUSTFLOAT(10.0f), FAUSTFLOAT(500.0f), FAUSTFLOAT(2.9000001f));
		ui_interface->addHorizontalSlider("NOISE", &fHslider4, FAUSTFLOAT(0.200000003f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("TONE_DECAY", &fHslider6, FAUSTFLOAT(0.5f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
		ui_interface->openVerticalBox("D2");
		ui_interface->addHorizontalSlider("CUTOFF", &fHslider7, FAUSTFLOAT(1000.0f), FAUSTFLOAT(5.0f), FAUSTFLOAT(10000.0f), FAUSTFLOAT(0.00100000005f));
		ui_interface->addHorizontalBargraph("DECAY_GRAPH", &fHbargraph0, FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addCheckButton("FILTER_SWITCH", &fCheckbox0);
		ui_interface->addHorizontalSlider("FREQ", &fHslider1, FAUSTFLOAT(500.0f), FAUSTFLOAT(10.0f), FAUSTFLOAT(500.0f), FAUSTFLOAT(2.9000001f));
		ui_interface->addHorizontalSlider("NOISE", &fHslider0, FAUSTFLOAT(0.200000003f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("TONE_DECAY", &fHslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("ENVELOPE");
		ui_interface->addVerticalSlider("ATTACK", &fVslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.0199999996f));
		ui_interface->addVerticalSlider("RELEASE", &fVslider2, FAUSTFLOAT(0.200000003f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.0199999996f));
		ui_interface->addVerticalSlider("SUSTAIN", &fVslider0, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.0199999996f));
		ui_interface->closeBox();
		ui_interface->addButton("TRIGGER", &fButton0);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fVslider0);
		float fSlow1 = std::max<float>(1.0f, (fConst0 * (float(fVslider1) + 0.00100000005f)));
		float fSlow2 = (1.0f / fSlow1);
		float fSlow3 = float(fButton0);
		float fSlow4 = float(fVslider2);
		float fSlow5 = (1.0f / std::max<float>(1.0f, (fConst0 * (fSlow4 + 0.00100000005f))));
		float fSlow6 = float(fCheckbox0);
		float fSlow7 = (1.0f - fSlow6);
		float fSlow8 = float(fHslider0);
		float fSlow9 = (fConst1 * (1.0f - fSlow8));
		float fSlow10 = float(fHslider1);
		float fSlow11 = float(fHslider2);
		float fSlow12 = (1.0f / std::max<float>(1.0f, (fConst0 * (fSlow4 * std::fabs(fSlow11)))));
		float fSlow13 = (4.65661287e-10f * fSlow8);
		float fSlow14 = float(fCheckbox1);
		float fSlow15 = std::tan((fConst6 * float(fHslider3)));
		float fSlow16 = (1.0f / fSlow15);
		float fSlow17 = (((fSlow16 + 0.333333343f) / fSlow15) + 1.0f);
		float fSlow18 = (fSlow14 / fSlow17);
		float fSlow19 = float(fHslider4);
		float fSlow20 = (fConst1 * (1.0f - fSlow19));
		float fSlow21 = float(fHslider5);
		float fSlow22 = float(fHslider6);
		float fSlow23 = (1.0f / std::max<float>(1.0f, (fConst0 * (fSlow4 * std::fabs(fSlow22)))));
		float fSlow24 = (4.65661287e-10f * fSlow19);
		float fSlow25 = (1.0f / fSlow17);
		float fSlow26 = (((fSlow16 + -0.333333343f) / fSlow15) + 1.0f);
		float fSlow27 = (2.0f * (1.0f - (1.0f / faust_simple_drums_faustpower2_f(fSlow15))));
		float fSlow28 = (1.0f - fSlow14);
		float fSlow29 = std::tan((fConst6 * float(fHslider7)));
		float fSlow30 = (1.0f / fSlow29);
		float fSlow31 = (((fSlow30 + 0.333333343f) / fSlow29) + 1.0f);
		float fSlow32 = (fSlow6 / fSlow31);
		float fSlow33 = (1.0f / fSlow31);
		float fSlow34 = (((fSlow30 + -0.333333343f) / fSlow29) + 1.0f);
		float fSlow35 = (2.0f * (1.0f - (1.0f / faust_simple_drums_faustpower2_f(fSlow29))));
		for (int i0 = 0; (i0 < count); i0 = (i0 + 1)) {
			iVec0[0] = 1;
			fVec1[0] = fSlow3;
			iRec0[0] = ((fSlow3 > fVec1[1]) + ((fSlow3 <= fVec1[1]) * (iRec0[1] + (iRec0[1] > 0))));
			float fTemp0 = float(iRec0[0]);
			float fTemp1 = float(iVec0[1]);
			float fTemp2 = (fConst4 * fTemp0);
			float fTemp3 = (fConst3 - fTemp0);
			fHbargraph0 = FAUSTFLOAT((fSlow11 * std::max<float>(0.0f, std::min<float>(fTemp2, ((fSlow12 * fTemp3) + 1.0f)))));
			float fTemp4 = std::max<float>((fSlow10 * (fHbargraph0 + 1.0f)), 23.4489498f);
			float fTemp5 = std::max<float>(20.0f, std::fabs(fTemp4));
			float fTemp6 = (fRec1[1] + (fConst2 * fTemp5));
			fRec1[0] = (fTemp6 - std::floor(fTemp6));
			float fTemp7 = faust_simple_drums_faustpower2_f(((2.0f * fRec1[0]) + -1.0f));
			fVec2[0] = fTemp7;
			float fTemp8 = ((fTemp1 * (fTemp7 - fVec2[1])) / fTemp5);
			fVec3[(IOTA & 4095)] = fTemp8;
			float fTemp9 = std::max<float>(0.0f, std::min<float>(2047.0f, (fConst5 / fTemp4)));
			int iTemp10 = int(fTemp9);
			float fTemp11 = std::floor(fTemp9);
			iRec2[0] = ((1103515245 * iRec2[1]) + 12345);
			float fTemp12 = float(iRec2[0]);
			float fTemp13 = ((fSlow9 * ((fTemp8 - (fVec3[((IOTA - iTemp10) & 4095)] * (fTemp11 + (1.0f - fTemp9)))) - ((fTemp9 - fTemp11) * fVec3[((IOTA - (iTemp10 + 1)) & 4095)]))) + (fSlow13 * fTemp12));
			fHbargraph1 = FAUSTFLOAT((fSlow22 * std::max<float>(0.0f, std::min<float>(fTemp2, ((fSlow23 * fTemp3) + 1.0f)))));
			float fTemp14 = std::max<float>((fSlow21 * (fHbargraph1 + 1.0f)), 23.4489498f);
			float fTemp15 = std::max<float>(20.0f, std::fabs(fTemp14));
			float fTemp16 = (fRec4[1] + (fConst2 * fTemp15));
			fRec4[0] = (fTemp16 - std::floor(fTemp16));
			float fTemp17 = faust_simple_drums_faustpower2_f(((2.0f * fRec4[0]) + -1.0f));
			fVec4[0] = fTemp17;
			float fTemp18 = ((fTemp1 * (fTemp17 - fVec4[1])) / fTemp15);
			fVec5[(IOTA & 4095)] = fTemp18;
			float fTemp19 = std::max<float>(0.0f, std::min<float>(2047.0f, (fConst5 / fTemp14)));
			int iTemp20 = int(fTemp19);
			float fTemp21 = std::floor(fTemp19);
			float fTemp22 = ((fSlow20 * ((fTemp18 - (fVec5[((IOTA - iTemp20) & 4095)] * (fTemp21 + (1.0f - fTemp19)))) - ((fTemp19 - fTemp21) * fVec5[((IOTA - (iTemp20 + 1)) & 4095)]))) + (fSlow24 * fTemp12));
			fRec3[0] = (fTemp22 - (fSlow25 * ((fSlow26 * fRec3[2]) + (fSlow27 * fRec3[1]))));
			fRec5[0] = (fTemp13 - (fSlow33 * ((fSlow34 * fRec5[2]) + (fSlow35 * fRec5[1]))));
			output0[i0] = FAUSTFLOAT((fSlow0 * (std::max<float>(0.0f, std::min<float>((fSlow2 * fTemp0), ((fSlow5 * (fSlow1 - fTemp0)) + 1.0f))) * ((fSlow7 * fTemp13) + (((fSlow18 * (fRec3[2] + (fRec3[0] + (2.0f * fRec3[1])))) + (fSlow28 * fTemp22)) + (fSlow32 * (fRec5[2] + (fRec5[0] + (2.0f * fRec5[1])))))))));
			iVec0[1] = iVec0[0];
			fVec1[1] = fVec1[0];
			iRec0[1] = iRec0[0];
			fRec1[1] = fRec1[0];
			fVec2[1] = fVec2[0];
			IOTA = (IOTA + 1);
			iRec2[1] = iRec2[0];
			fRec4[1] = fRec4[0];
			fVec4[1] = fVec4[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
		}
	}

};

#endif
