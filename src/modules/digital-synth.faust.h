/* ------------------------------------------------------------
name: "digital-synth"
Code generated with Faust 2.37.3 (https://faust.grame.fr)
Compilation options: -a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __faust_digital_synth_H__
#define  __faust_digital_synth_H__

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

static float faust_digital_synth_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS faust_digital_synth
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class faust_digital_synth : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fHslider1;
	int iVec0[2];
	FAUSTFLOAT fHslider2;
	int iRec1[2];
	FAUSTFLOAT fHslider3;
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider4;
	float fRec2[2];
	float fConst2;
	float fRec4[2];
	float fVec1[2];
	int IOTA;
	float fVec2[4096];
	float fConst3;
	float fConst4;
	float fRec5[2];
	int iRec6[2];
	float fRec0[2];
	FAUSTFLOAT fHslider5;
	float fRec7[2];
	float fConst5;
	float fConst6;
	FAUSTFLOAT fButton0;
	float fVec3[2];
	float fRec8[2];
	float fConst7;
	float fConst8;
	int iRec9[2];
	
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
		m->declare("filename", "digital-synth.dsp");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/pole:author", "Julius O. Smith III");
		m->declare("filters.lib/pole:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/pole:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "digital-synth");
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
		fConst1 = (1.0f / fConst0);
		fConst2 = (0.25f * fConst0);
		fConst3 = (0.5f * fConst0);
		fConst4 = (1760.0f / fConst0);
		fConst5 = std::max<float>(1.0f, (0.00999999978f * fConst0));
		fConst6 = (1.0f / fConst5);
		fConst7 = (0.300000012f / std::max<float>(1.0f, (0.100000001f * fConst0)));
		fConst8 = (1.0f / std::max<float>(1.0f, (0.200000003f * fConst0)));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(1.0f);
		fHslider1 = FAUSTFLOAT(8.0f);
		fHslider2 = FAUSTFLOAT(1.0f);
		fHslider3 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(69.0f);
		fHslider5 = FAUSTFLOAT(0.0f);
		fButton0 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			iRec1[l1] = 0;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec2[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec4[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fVec1[l4] = 0.0f;
		}
		IOTA = 0;
		for (int l5 = 0; (l5 < 4096); l5 = (l5 + 1)) {
			fVec2[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec5[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			iRec6[l7] = 0;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec0[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec7[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fVec3[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec8[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			iRec9[l12] = 0;
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
	
	virtual faust_digital_synth* clone() {
		return new faust_digital_synth();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("digital-synth");
		ui_interface->addHorizontalSlider("BITS", &fHslider1, FAUSTFLOAT(8.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(16.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("GLITCH", &fHslider5, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("KEY", &fHslider4, FAUSTFLOAT(69.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(127.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("SRATE", &fHslider2, FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(32.0f), FAUSTFLOAT(1.0f));
		ui_interface->addButton("TRIGGER", &fButton0);
		ui_interface->addHorizontalSlider("VELOCITY", &fHslider0, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("WAVE", &fHslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(3.0f), FAUSTFLOAT(1.0f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = std::pow(2.0f, (float(fHslider1) + -1.0f));
		float fSlow1 = (0.5f * (float(fHslider0) / fSlow0));
		int iSlow2 = int(float(fHslider2));
		int iSlow3 = int(float(fHslider3));
		int iSlow4 = (iSlow3 >= 2);
		int iSlow5 = (iSlow3 >= 1);
		float fSlow6 = std::pow(2.0f, (0.0833333358f * (float(fHslider4) + -69.0f)));
		float fSlow7 = (440.0f * fSlow6);
		float fSlow8 = std::max<float>(1.1920929e-07f, std::fabs(fSlow7));
		float fSlow9 = (fConst1 * fSlow8);
		float fSlow10 = (1.0f - (fConst0 / fSlow8));
		float fSlow11 = std::max<float>(fSlow7, 23.4489498f);
		float fSlow12 = std::max<float>(20.0f, std::fabs(fSlow11));
		float fSlow13 = (fConst2 / fSlow12);
		float fSlow14 = (fConst1 * fSlow12);
		float fSlow15 = std::max<float>(0.0f, std::min<float>(2047.0f, (fConst3 / fSlow11)));
		float fSlow16 = std::floor(fSlow15);
		float fSlow17 = (fSlow16 + (1.0f - fSlow15));
		int iSlow18 = int(fSlow15);
		float fSlow19 = (fSlow15 - fSlow16);
		int iSlow20 = (iSlow18 + 1);
		int iSlow21 = (iSlow3 >= 3);
		float fSlow22 = (fConst4 * fSlow6);
		float fSlow23 = float(fHslider5);
		float fSlow24 = (1.0f - fSlow23);
		float fSlow25 = (1.0f - (0.5f * fSlow23));
		float fSlow26 = (1.0f / fSlow0);
		float fSlow27 = float(fButton0);
		int iSlow28 = (fSlow27 == 0.0f);
		for (int i0 = 0; (i0 < count); i0 = (i0 + 1)) {
			iVec0[0] = 1;
			iRec1[0] = ((iVec0[1] + iRec1[1]) % iSlow2);
			float fTemp0 = (fSlow9 + (fRec2[1] + -1.0f));
			int iTemp1 = (fTemp0 < 0.0f);
			float fTemp2 = (fSlow9 + fRec2[1]);
			fRec2[0] = (iTemp1 ? fTemp2 : fTemp0);
			float fThen1 = (fSlow9 + (fRec2[1] + (fSlow10 * fTemp0)));
			float fRec3 = (iTemp1 ? fTemp2 : fThen1);
			fRec4[0] = (fSlow14 + (fRec4[1] - std::floor((fSlow14 + fRec4[1]))));
			float fTemp3 = faust_digital_synth_faustpower2_f(((2.0f * fRec4[0]) + -1.0f));
			fVec1[0] = fTemp3;
			float fTemp4 = (fSlow13 * (float(iVec0[1]) * (fTemp3 - fVec1[1])));
			fVec2[(IOTA & 4095)] = fTemp4;
			float fTemp5 = ((fSlow17 * fVec2[((IOTA - iSlow18) & 4095)]) + (fSlow19 * fVec2[((IOTA - iSlow20) & 4095)]));
			float fThen2 = ((2.0f * fRec3) + -1.0f);
			float fElse2 = (fTemp4 - fTemp5);
			fRec5[0] = ((fTemp4 + (0.999000013f * fRec5[1])) - fTemp5);
			iRec6[0] = ((1103515245 * iRec6[1]) + 12345);
			float fTemp6 = (4.65661287e-10f * float(iRec6[0]));
			float fThen3 = (fSlow22 * fRec5[0]);
			float fThen4 = (iSlow5 ? fElse2 : fThen2);
			float fElse4 = (iSlow21 ? fTemp6 : fThen3);
			float fElse5 = (iSlow4 ? fElse4 : fThen4);
			fRec0[0] = ((iRec1[0] <= iRec1[1]) ? fElse5 : fRec0[1]);
			float fTemp7 = std::floor((fSlow0 * fRec0[0]));
			float fElse6 = (fSlow26 * fTemp7);
			fRec7[0] = ((std::fabs(fTemp6) > fSlow25) ? fElse6 : fRec7[1]);
			fVec3[0] = fSlow27;
			fRec8[0] = (fSlow27 + (fRec8[1] * float((fVec3[1] >= fSlow27))));
			iRec9[0] = (iSlow28 * (iRec9[1] + 1));
			output0[i0] = FAUSTFLOAT((fSlow1 * ((fTemp7 * (fSlow24 + (fSlow23 * fRec7[0]))) * std::max<float>(0.0f, (std::min<float>((fConst6 * fRec8[0]), std::max<float>(((fConst7 * (fConst5 - fRec8[0])) + 1.0f), 0.699999988f)) * (1.0f - (fConst8 * float(iRec9[0]))))))));
			iVec0[1] = iVec0[0];
			iRec1[1] = iRec1[0];
			fRec2[1] = fRec2[0];
			fRec4[1] = fRec4[0];
			fVec1[1] = fVec1[0];
			IOTA = (IOTA + 1);
			fRec5[1] = fRec5[0];
			iRec6[1] = iRec6[0];
			fRec0[1] = fRec0[0];
			fRec7[1] = fRec7[0];
			fVec3[1] = fVec3[0];
			fRec8[1] = fRec8[0];
			iRec9[1] = iRec9[0];
		}
	}

};

#endif
