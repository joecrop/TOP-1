/* ------------------------------------------------------------
name: "spring"
Code generated with Faust 2.37.3 (https://faust.grame.fr)
Compilation options: -a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __faust_spring_H__
#define  __faust_spring_H__

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

static float faust_spring_faustpower2_f(float value) {
	return (value * value);
}
static float faust_spring_faustpower3_f(float value) {
	return ((value * value) * value);
}
static float faust_spring_faustpower4_f(float value) {
	return (((value * value) * value) * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS faust_spring
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class faust_spring : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider1;
	int IOTA;
	float fVec0[2048];
	FAUSTFLOAT fHslider2;
	float fRec5[2];
	float fVec1[2048];
	float fRec3[2];
	FAUSTFLOAT fHslider3;
	float fVec2[8192];
	float fRec1[2];
	float fRec2[3];
	float fRec0[2];
	float fVec3[8192];
	float fRec8[2];
	float fRec9[3];
	float fRec7[2];
	float fVec4[16384];
	float fRec11[2];
	float fRec12[3];
	float fRec10[2];
	float fVec5[16384];
	float fRec14[2];
	float fRec15[3];
	float fRec13[2];
	float fConst2;
	float fConst3;
	float fConst4;
	float fConst5;
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst9;
	float fVec6[1024];
	int iConst10;
	float fRec17[2];
	float fRec18[2];
	float fConst11;
	float fConst12;
	float fConst13;
	float fConst14;
	float fRec16[5];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "spring.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/bandpass0_bandstop1:author", "Julius O. Smith III");
		m->declare("filters.lib/bandpass0_bandstop1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/bandpass0_bandstop1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/bandpass:author", "Julius O. Smith III");
		m->declare("filters.lib/bandpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/bandpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/fb_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/fb_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fb_comb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
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
		m->declare("filters.lib/tf2sb:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2sb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2sb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "spring");
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
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (3.14159274f / fConst0);
		fConst2 = (1.0f / fConst0);
		fConst3 = faust_spring_faustpower2_f(fConst2);
		fConst4 = (0.150000006f * fConst3);
		fConst5 = (2.0f * fConst0);
		fConst6 = (12566.3711f / fConst0);
		fConst7 = (0.5f / fConst0);
		fConst8 = (4.0f * faust_spring_faustpower2_f(fConst0));
		fConst9 = (11.3137083f / fConst0);
		iConst10 = std::min<int>(1024, std::max<int>(0, (std::max<int>(1, int((0.00300000003f * fConst0))) + -1)));
		fConst11 = faust_spring_faustpower3_f(fConst2);
		fConst12 = (4.0f * fConst2);
		fConst13 = (22.6274166f / fConst0);
		fConst14 = (6.0f * fConst3);
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.5f);
		fHslider1 = FAUSTFLOAT(0.5f);
		fHslider2 = FAUSTFLOAT(0.5f);
		fHslider3 = FAUSTFLOAT(0.59999999999999998f);
	}
	
	virtual void instanceClear() {
		IOTA = 0;
		for (int l0 = 0; (l0 < 2048); l0 = (l0 + 1)) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec5[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 2048); l2 = (l2 + 1)) {
			fVec1[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec3[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 8192); l4 = (l4 + 1)) {
			fVec2[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec1[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 3); l6 = (l6 + 1)) {
			fRec2[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec0[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 8192); l8 = (l8 + 1)) {
			fVec3[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec8[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 3); l10 = (l10 + 1)) {
			fRec9[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec7[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 16384); l12 = (l12 + 1)) {
			fVec4[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec11[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 3); l14 = (l14 + 1)) {
			fRec12[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec10[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 16384); l16 = (l16 + 1)) {
			fVec5[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec14[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 3); l18 = (l18 + 1)) {
			fRec15[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec13[l19] = 0.0f;
		}
		for (int l20 = 0; (l20 < 1024); l20 = (l20 + 1)) {
			fVec6[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec17[l21] = 0.0f;
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec18[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 5); l23 = (l23 + 1)) {
			fRec16[l23] = 0.0f;
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
	
	virtual faust_spring* clone() {
		return new faust_spring();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("spring");
		ui_interface->addHorizontalSlider("DECAY", &fHslider3, FAUSTFLOAT(0.600000024f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("MIX", &fHslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("TENSION", &fHslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("TONE", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fHslider0);
		float fSlow1 = (1.0f - fSlow0);
		float fSlow2 = (1.0f / std::tan((fConst1 * ((6000.0f * float(fHslider1)) + 2000.0f))));
		float fSlow3 = (1.0f / (fSlow2 + 1.0f));
		float fSlow4 = (1.0f - fSlow2);
		float fSlow5 = float(fHslider2);
		int iSlow6 = std::min<int>(4096, std::max<int>(0, (std::max<int>(1, int((fConst0 * ((0.00200000009f * fSlow5) + 0.00469999993f)))) + -1)));
		int iSlow7 = std::min<int>(4096, std::max<int>(0, (std::max<int>(1, int((fConst0 * ((0.00300000003f * fSlow5) + 0.00710000005f)))) + -1)));
		float fSlow8 = ((0.25f * float(fHslider3)) + 0.699999988f);
		float fSlow9 = (0.0f - (0.879999995f * fSlow8));
		int iSlow10 = std::min<int>(8192, std::max<int>(0, (std::max<int>(1, int((fConst0 * ((0.00999999978f * fSlow5) + 0.0189999994f)))) + -1)));
		float fSlow11 = (0.0f - (0.860000014f * fSlow8));
		int iSlow12 = std::min<int>(8192, std::max<int>(0, (std::max<int>(1, int((fConst0 * ((0.0120000001f * fSlow5) + 0.023f)))) + -1)));
		float fSlow13 = (0.0f - (0.839999974f * fSlow8));
		int iSlow14 = std::min<int>(8192, std::max<int>(0, (std::max<int>(1, int((fConst0 * ((0.0149999997f * fSlow5) + 0.0309999995f)))) + -1)));
		float fSlow15 = (0.0f - (0.819999993f * fSlow8));
		int iSlow16 = std::min<int>(8192, std::max<int>(0, (std::max<int>(1, int((fConst0 * ((0.0179999992f * fSlow5) + 0.0370000005f)))) + -1)));
		float fSlow17 = std::tan((fConst6 * (fSlow5 + 1.0f)));
		float fSlow18 = std::sqrt((fConst8 * (std::tan((fConst1 * ((2000.0f * fSlow5) + 1500.0f))) * fSlow17)));
		float fSlow19 = faust_spring_faustpower2_f(fSlow18);
		float fSlow20 = ((fConst5 * fSlow17) - (fConst7 * (fSlow19 / fSlow17)));
		float fSlow21 = faust_spring_faustpower2_f(fSlow20);
		float fSlow22 = (4.0f * fSlow21);
		float fSlow23 = (fSlow22 + (8.0f * fSlow19));
		float fSlow24 = (fConst2 * fSlow19);
		float fSlow25 = (2.82842708f * fSlow20);
		float fSlow26 = (fConst9 * fSlow20);
		float fSlow27 = (((fConst3 * (fSlow23 + (fConst2 * (fSlow19 * (fSlow24 + fSlow25))))) + fSlow26) + 16.0f);
		float fSlow28 = (fConst4 / fSlow27);
		float fSlow29 = (8.0f * fSlow21);
		float fSlow30 = (0.0f - fSlow29);
		float fSlow31 = (1.0f / fSlow27);
		float fSlow32 = (5.65685415f * fSlow20);
		float fSlow33 = (fConst12 * fSlow19);
		float fSlow34 = (fConst13 * fSlow20);
		float fSlow35 = ((fConst11 * (fSlow19 * (fSlow32 + fSlow33))) + (-64.0f - fSlow34));
		float fSlow36 = ((fConst3 * ((0.0f - (fSlow29 + (16.0f * fSlow19))) + (fConst14 * faust_spring_faustpower4_f(fSlow18)))) + 96.0f);
		float fSlow37 = ((fSlow34 + (fConst11 * (fSlow19 * (fSlow33 - fSlow32)))) + -64.0f);
		float fSlow38 = ((fConst3 * (fSlow23 + (fConst2 * (fSlow19 * (fSlow24 - fSlow25))))) + (16.0f - fSlow26));
		for (int i0 = 0; (i0 < count); i0 = (i0 + 1)) {
			float fTemp0 = float(input0[i0]);
			float fTemp1 = (fTemp0 - (0.5f * fRec5[1]));
			fVec0[(IOTA & 2047)] = fTemp1;
			fRec5[0] = fVec0[((IOTA - iSlow6) & 2047)];
			float fRec6 = (0.5f * fTemp1);
			float fTemp2 = ((fRec6 + fRec5[1]) - (0.5f * fRec3[1]));
			fVec1[(IOTA & 2047)] = fTemp2;
			fRec3[0] = fVec1[((IOTA - iSlow7) & 2047)];
			float fRec4 = (0.5f * fTemp2);
			float fTemp3 = (fRec4 + fRec3[1]);
			float fTemp4 = (fTemp3 - (fSlow9 * fRec1[1]));
			fVec2[(IOTA & 8191)] = fTemp4;
			fRec1[0] = fVec2[((IOTA - iSlow10) & 8191)];
			fRec2[0] = fTemp4;
			fRec0[0] = (0.0f - (fSlow3 * ((fSlow4 * fRec0[1]) - (fRec2[1] + fRec2[2]))));
			float fTemp5 = (fTemp3 - (fSlow11 * fRec8[1]));
			fVec3[(IOTA & 8191)] = fTemp5;
			fRec8[0] = fVec3[((IOTA - iSlow12) & 8191)];
			fRec9[0] = fTemp5;
			fRec7[0] = (0.0f - (fSlow3 * ((fSlow4 * fRec7[1]) - (fRec9[1] + fRec9[2]))));
			float fTemp6 = (fTemp3 - (fSlow13 * fRec11[1]));
			fVec4[(IOTA & 16383)] = fTemp6;
			fRec11[0] = fVec4[((IOTA - iSlow14) & 16383)];
			fRec12[0] = fTemp6;
			fRec10[0] = (0.0f - (fSlow3 * ((fSlow4 * fRec10[1]) - (fRec12[1] + fRec12[2]))));
			float fTemp7 = (fTemp3 - (fSlow15 * fRec14[1]));
			fVec5[(IOTA & 16383)] = fTemp7;
			fRec14[0] = fVec5[((IOTA - iSlow16) & 16383)];
			fRec15[0] = fTemp7;
			fRec13[0] = (0.0f - (fSlow3 * ((fSlow4 * fRec13[1]) - (fRec15[1] + fRec15[2]))));
			float fTemp8 = ((0.300000012f * fTemp0) - (0.200000003f * fRec17[1]));
			fVec6[(IOTA & 1023)] = fTemp8;
			fRec17[0] = fVec6[((IOTA - iConst10) & 1023)];
			fRec18[0] = fTemp8;
			fRec16[0] = ((0.300000012f * fRec18[1]) - (fSlow31 * ((((fSlow35 * fRec16[1]) + (fSlow36 * fRec16[2])) + (fSlow37 * fRec16[3])) + (fSlow38 * fRec16[4]))));
			output0[i0] = FAUSTFLOAT(((fSlow1 * fTemp0) + (fSlow0 * ((0.212500006f * (((fRec0[0] + fRec7[0]) + fRec10[0]) + fRec13[0])) + (fSlow28 * (((fSlow30 * fRec16[2]) + (fSlow22 * fRec16[0])) + (fSlow22 * fRec16[4])))))));
			IOTA = (IOTA + 1);
			fRec5[1] = fRec5[0];
			fRec3[1] = fRec3[0];
			fRec1[1] = fRec1[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fRec0[1] = fRec0[0];
			fRec8[1] = fRec8[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			fRec7[1] = fRec7[0];
			fRec11[1] = fRec11[0];
			fRec12[2] = fRec12[1];
			fRec12[1] = fRec12[0];
			fRec10[1] = fRec10[0];
			fRec14[1] = fRec14[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fRec13[1] = fRec13[0];
			fRec17[1] = fRec17[0];
			fRec18[1] = fRec18[0];
			for (int j0 = 4; (j0 > 0); j0 = (j0 - 1)) {
				fRec16[j0] = fRec16[(j0 - 1)];
			}
		}
	}

};

#endif
