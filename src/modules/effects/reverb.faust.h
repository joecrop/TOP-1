/* ------------------------------------------------------------
name: "reverb"
Code generated with Faust 2.37.3 (https://faust.grame.fr)
Compilation options: -a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __faust_reverb_H__
#define  __faust_reverb_H__

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


#ifndef FAUSTCLASS 
#define FAUSTCLASS faust_reverb
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class faust_reverb : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int fSampleRate;
	float fConst0;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	float fVec0[2];
	float fRec5[2];
	int IOTA;
	float fVec1[2048];
	FAUSTFLOAT fHslider3;
	float fRec4[2];
	float fVec2[2];
	float fRec7[2];
	float fVec3[2048];
	float fRec6[2];
	float fVec4[2];
	float fRec9[2];
	float fVec5[2048];
	float fRec8[2];
	float fVec6[2];
	float fRec11[2];
	float fVec7[2048];
	float fRec10[2];
	float fVec8[256];
	float fRec2[2];
	float fVec9[1024];
	float fRec0[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "reverb.dsp");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "reverb");
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
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = (25132.7422f / std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate))));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.29999999999999999f);
		fHslider1 = FAUSTFLOAT(0.5f);
		fHslider2 = FAUSTFLOAT(0.5f);
		fHslider3 = FAUSTFLOAT(0.5f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec5[l1] = 0.0f;
		}
		IOTA = 0;
		for (int l2 = 0; (l2 < 2048); l2 = (l2 + 1)) {
			fVec1[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec4[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fVec2[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec7[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2048); l6 = (l6 + 1)) {
			fVec3[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec6[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fVec4[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec9[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 2048); l10 = (l10 + 1)) {
			fVec5[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec8[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fVec6[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec11[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 2048); l14 = (l14 + 1)) {
			fVec7[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec10[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 256); l16 = (l16 + 1)) {
			fVec8[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec2[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 1024); l18 = (l18 + 1)) {
			fVec9[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec0[l19] = 0.0f;
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
	
	virtual faust_reverb* clone() {
		return new faust_reverb();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("reverb");
		ui_interface->addHorizontalSlider("damp", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("decay", &fHslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.100000001f), FAUSTFLOAT(0.949999988f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("mix", &fHslider0, FAUSTFLOAT(0.300000012f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("size", &fHslider3, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.100000001f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fHslider0);
		float fSlow1 = (1.0f - fSlow0);
		float fSlow2 = (1.0f / std::tan((fConst0 * (1.0f - float(fHslider1)))));
		float fSlow3 = (1.0f / (fSlow2 + 1.0f));
		float fSlow4 = (1.0f - fSlow2);
		float fSlow5 = float(fHslider2);
		float fSlow6 = float(fHslider3);
		int iSlow7 = std::min<int>(48000, std::max<int>(0, int((1557.0f * fSlow6))));
		int iSlow8 = std::min<int>(48000, std::max<int>(0, int((1617.0f * fSlow6))));
		int iSlow9 = std::min<int>(48000, std::max<int>(0, int((1491.0f * fSlow6))));
		int iSlow10 = std::min<int>(48000, std::max<int>(0, int((1422.0f * fSlow6))));
		int iSlow11 = std::min<int>(48000, std::max<int>(0, int((225.0f * fSlow6))));
		int iSlow12 = std::min<int>(48000, std::max<int>(0, int((556.0f * fSlow6))));
		for (int i0 = 0; (i0 < count); i0 = (i0 + 1)) {
			float fTemp0 = float(input0[i0]);
			float fTemp1 = (fSlow5 * fRec4[1]);
			fVec0[0] = fTemp1;
			fRec5[0] = (0.0f - (fSlow3 * ((fSlow4 * fRec5[1]) - (fTemp1 + fVec0[1]))));
			fVec1[(IOTA & 2047)] = (fTemp0 + fRec5[0]);
			fRec4[0] = fVec1[((IOTA - iSlow7) & 2047)];
			float fTemp2 = (fSlow5 * fRec6[1]);
			fVec2[0] = fTemp2;
			fRec7[0] = (0.0f - (fSlow3 * ((fSlow4 * fRec7[1]) - (fTemp2 + fVec2[1]))));
			fVec3[(IOTA & 2047)] = (fTemp0 + fRec7[0]);
			fRec6[0] = fVec3[((IOTA - iSlow8) & 2047)];
			float fTemp3 = (fSlow5 * fRec8[1]);
			fVec4[0] = fTemp3;
			fRec9[0] = (0.0f - (fSlow3 * ((fSlow4 * fRec9[1]) - (fTemp3 + fVec4[1]))));
			fVec5[(IOTA & 2047)] = (fTemp0 + fRec9[0]);
			fRec8[0] = fVec5[((IOTA - iSlow9) & 2047)];
			float fTemp4 = (fSlow5 * fRec10[1]);
			fVec6[0] = fTemp4;
			fRec11[0] = (0.0f - (fSlow3 * ((fSlow4 * fRec11[1]) - (fTemp4 + fVec6[1]))));
			fVec7[(IOTA & 2047)] = (fTemp0 + fRec11[0]);
			fRec10[0] = fVec7[((IOTA - iSlow10) & 2047)];
			float fTemp5 = ((0.25f * (((fRec4[0] + fRec6[0]) + fRec8[0]) + fRec10[0])) - (0.5f * fRec2[1]));
			fVec8[(IOTA & 255)] = fTemp5;
			fRec2[0] = fVec8[((IOTA - iSlow11) & 255)];
			float fRec3 = (0.5f * fTemp5);
			float fTemp6 = ((fRec2[0] + fRec3) - (0.5f * fRec0[1]));
			fVec9[(IOTA & 1023)] = fTemp6;
			fRec0[0] = fVec9[((IOTA - iSlow12) & 1023)];
			float fRec1 = (0.5f * fTemp6);
			output0[i0] = FAUSTFLOAT(((fSlow1 * fTemp0) + (fSlow0 * (fRec0[0] + fRec1))));
			fVec0[1] = fVec0[0];
			fRec5[1] = fRec5[0];
			IOTA = (IOTA + 1);
			fRec4[1] = fRec4[0];
			fVec2[1] = fVec2[0];
			fRec7[1] = fRec7[0];
			fRec6[1] = fRec6[0];
			fVec4[1] = fVec4[0];
			fRec9[1] = fRec9[0];
			fRec8[1] = fRec8[0];
			fVec6[1] = fVec6[0];
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			fRec2[1] = fRec2[0];
			fRec0[1] = fRec0[0];
		}
	}

};

#endif
