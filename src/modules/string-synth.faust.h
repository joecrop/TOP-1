/* ------------------------------------------------------------
name: "string-synth"
Code generated with Faust 2.37.3 (https://faust.grame.fr)
Compilation options: -a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __faust_string_synth_H__
#define  __faust_string_synth_H__

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

class faust_string_synthSIG0 {
	
  private:
	
	int iVec3[2];
	int iRec14[2];
	
  public:
	
	int getNumInputsfaust_string_synthSIG0() {
		return 0;
	}
	int getNumOutputsfaust_string_synthSIG0() {
		return 1;
	}
	
	void instanceInitfaust_string_synthSIG0(int sample_rate) {
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			iVec3[l7] = 0;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			iRec14[l8] = 0;
		}
	}
	
	void fillfaust_string_synthSIG0(int count, float* table) {
		for (int i1 = 0; (i1 < count); i1 = (i1 + 1)) {
			iVec3[0] = 1;
			iRec14[0] = ((iVec3[1] + iRec14[1]) % 65536);
			table[i1] = std::sin((9.58738019e-05f * float(iRec14[0])));
			iVec3[1] = iVec3[0];
			iRec14[1] = iRec14[0];
		}
	}

};

static faust_string_synthSIG0* newfaust_string_synthSIG0() { return (faust_string_synthSIG0*)new faust_string_synthSIG0(); }
static void deletefaust_string_synthSIG0(faust_string_synthSIG0* dsp) { delete dsp; }

static float ftbl0faust_string_synthSIG0[65536];

#ifndef FAUSTCLASS 
#define FAUSTCLASS faust_string_synth
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class faust_string_synth : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fHslider1;
	int iVec0[2];
	int IOTA;
	float fRec7[2048];
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider2;
	float fConst2;
	FAUSTFLOAT fButton0;
	float fVec1[2];
	FAUSTFLOAT fHslider3;
	int iRec11[2];
	float fConst3;
	float fConst4;
	int iRec12[2];
	float fConst5;
	float fConst6;
	float fConst8;
	float fConst9;
	int iVec2[2];
	float fRec13[2];
	float fConst10;
	float fRec15[2];
	float fConst11;
	float fVec4[2048];
	float fRec3[2];
	float fRec0[3];
	float fRec1[3];
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fHslider5;
	float fConst12;
	float fRec23[2048];
	float fVec5[2048];
	float fRec19[2];
	float fRec16[3];
	float fRec17[3];
	float fRec34[2048];
	float fVec6[2048];
	float fRec30[2];
	float fRec27[3];
	float fRec28[3];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.2");
		m->declare("compile_options", "-a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("envelopes.lib/ar:author", "Yann Orlarey, Stéphane Letz");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.1");
		m->declare("filename", "string-synth.dsp");
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
		m->declare("name", "string-synth");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "0.1");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.1");
		m->declare("physmodels.lib/name", "Faust Physical Models Library");
		m->declare("physmodels.lib/version", "0.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
		faust_string_synthSIG0* sig0 = newfaust_string_synthSIG0();
		sig0->instanceInitfaust_string_synthSIG0(sample_rate);
		sig0->fillfaust_string_synthSIG0(65536, ftbl0faust_string_synthSIG0);
		deletefaust_string_synthSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (0.00147058826f * fConst0);
		fConst2 = (0.00882352982f * fConst0);
		fConst3 = std::max<float>(1.0f, (0.00100000005f * fConst0));
		fConst4 = (1.0f / fConst3);
		fConst5 = std::max<float>(1.0f, (0.00999999978f * fConst0));
		fConst6 = (1.0f / fConst5);
		float fConst7 = (1.0f / std::tan((6283.18555f / fConst0)));
		fConst8 = (1.0f / (fConst7 + 1.0f));
		fConst9 = (1.0f - fConst7);
		fConst10 = (880.0f / fConst0);
		fConst11 = (1.0f / std::max<float>(1.0f, (0.0500000007f * fConst0)));
		fConst12 = (0.800000012f * fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(1.0f);
		fHslider1 = FAUSTFLOAT(0.5f);
		fHslider2 = FAUSTFLOAT(69.0f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider3 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(2.0f);
		fHslider5 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
		}
		IOTA = 0;
		for (int l1 = 0; (l1 < 2048); l1 = (l1 + 1)) {
			fRec7[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fVec1[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			iRec11[l3] = 0;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			iRec12[l4] = 0;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			iVec2[l5] = 0;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec13[l6] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec15[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 2048); l10 = (l10 + 1)) {
			fVec4[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec3[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 3); l12 = (l12 + 1)) {
			fRec0[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 3); l13 = (l13 + 1)) {
			fRec1[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 2048); l14 = (l14 + 1)) {
			fRec23[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 2048); l15 = (l15 + 1)) {
			fVec5[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec19[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 3); l17 = (l17 + 1)) {
			fRec16[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 3); l18 = (l18 + 1)) {
			fRec17[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 2048); l19 = (l19 + 1)) {
			fRec34[l19] = 0.0f;
		}
		for (int l20 = 0; (l20 < 2048); l20 = (l20 + 1)) {
			fVec6[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec30[l21] = 0.0f;
		}
		for (int l22 = 0; (l22 < 3); l22 = (l22 + 1)) {
			fRec27[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 3); l23 = (l23 + 1)) {
			fRec28[l23] = 0.0f;
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
	
	virtual faust_string_synth* clone() {
		return new faust_string_synth();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("string-synth");
		ui_interface->addHorizontalSlider("DAMP", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("DECAY", &fHslider4, FAUSTFLOAT(2.0f), FAUSTFLOAT(0.100000001f), FAUSTFLOAT(10.0f), FAUSTFLOAT(0.100000001f));
		ui_interface->addHorizontalSlider("EXCITE", &fHslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.100000001f));
		ui_interface->addHorizontalSlider("KEY", &fHslider2, FAUSTFLOAT(69.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(127.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("SYMP", &fHslider5, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.300000012f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addButton("TRIGGER", &fButton0);
		ui_interface->addHorizontalSlider("VELOCITY", &fHslider0, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = (0.5f * float(fHslider0));
		float fSlow1 = float(fHslider1);
		float fSlow2 = (0.5f * ((0.180000007f * fSlow1) + 0.800000012f));
		float fSlow3 = std::pow(2.0f, (0.0833333358f * (float(fHslider2) + -69.0f)));
		float fSlow4 = (fConst1 * ((440.0f * fSlow3) + -0.0500000007f));
		float fSlow5 = (fSlow4 + -1.49999499f);
		float fSlow6 = std::floor(fSlow5);
		float fSlow7 = (fSlow4 + (-1.0f - fSlow6));
		float fSlow8 = (fSlow4 + (-2.0f - fSlow6));
		float fSlow9 = (fSlow4 + (-3.0f - fSlow6));
		float fSlow10 = (fSlow4 + (-4.0f - fSlow6));
		float fSlow11 = ((((0.0f - fSlow7) * (0.0f - (0.5f * fSlow8))) * (0.0f - (0.333333343f * fSlow9))) * (0.0f - (0.25f * fSlow10)));
		int iSlow12 = int(fSlow5);
		int iSlow13 = (int(std::min<float>(fConst2, float(std::max<int>(0, iSlow12)))) + 1);
		float fSlow14 = (fSlow4 - fSlow6);
		float fSlow15 = (((0.0f - fSlow8) * (0.0f - (0.5f * fSlow9))) * (0.0f - (0.333333343f * fSlow10)));
		int iSlow16 = (int(std::min<float>(fConst2, float(std::max<int>(0, (iSlow12 + 1))))) + 1);
		float fSlow17 = (0.5f * ((fSlow7 * (0.0f - fSlow9)) * (0.0f - (0.5f * fSlow10))));
		int iSlow18 = (int(std::min<float>(fConst2, float(std::max<int>(0, (iSlow12 + 2))))) + 1);
		float fSlow19 = (fSlow7 * fSlow8);
		float fSlow20 = (0.166666672f * (fSlow19 * (0.0f - fSlow10)));
		int iSlow21 = (int(std::min<float>(fConst2, float(std::max<int>(0, (iSlow12 + 3))))) + 1);
		float fSlow22 = (0.0416666679f * (fSlow19 * fSlow9));
		int iSlow23 = (int(std::min<float>(fConst2, float(std::max<int>(0, (iSlow12 + 4))))) + 1);
		float fSlow24 = float(fButton0);
		float fSlow25 = (10.0f * fSlow24);
		int iSlow26 = int(float(fHslider3));
		int iSlow27 = (iSlow26 >= 2);
		int iSlow28 = (iSlow26 >= 1);
		float fSlow29 = (fConst10 * fSlow3);
		float fSlow30 = float(fHslider4);
		float fSlow31 = (1.0f / std::max<float>(1.0f, (fConst0 * fSlow30)));
		float fSlow32 = float(fHslider5);
		float fSlow33 = (0.300000012f * fSlow32);
		float fSlow34 = (1.0f / std::max<float>(1.0f, (fConst12 * fSlow30)));
		float fSlow35 = (0.5f * ((0.189999998f * fSlow1) + 0.800000012f));
		float fSlow36 = (0.00999999978f * fSlow32);
		float fSlow37 = (fConst1 * ((440.0f * (fSlow3 * (fSlow36 + 1.0f))) + -0.0500000007f));
		float fSlow38 = (fSlow37 + -1.49999499f);
		float fSlow39 = std::floor(fSlow38);
		float fSlow40 = (fSlow37 + (-1.0f - fSlow39));
		float fSlow41 = (fSlow37 + (-2.0f - fSlow39));
		float fSlow42 = (fSlow37 + (-3.0f - fSlow39));
		float fSlow43 = (fSlow37 + (-4.0f - fSlow39));
		float fSlow44 = ((((0.0f - fSlow40) * (0.0f - (0.5f * fSlow41))) * (0.0f - (0.333333343f * fSlow42))) * (0.0f - (0.25f * fSlow43)));
		int iSlow45 = int(fSlow38);
		int iSlow46 = (int(std::min<float>(fConst2, float(std::max<int>(0, iSlow45)))) + 1);
		float fSlow47 = (fSlow37 - fSlow39);
		float fSlow48 = (((0.0f - fSlow41) * (0.0f - (0.5f * fSlow42))) * (0.0f - (0.333333343f * fSlow43)));
		int iSlow49 = (int(std::min<float>(fConst2, float(std::max<int>(0, (iSlow45 + 1))))) + 1);
		float fSlow50 = (0.5f * ((fSlow40 * (0.0f - fSlow42)) * (0.0f - (0.5f * fSlow43))));
		int iSlow51 = (int(std::min<float>(fConst2, float(std::max<int>(0, (iSlow45 + 2))))) + 1);
		float fSlow52 = (fSlow40 * fSlow41);
		float fSlow53 = (0.166666672f * (fSlow52 * (0.0f - fSlow43)));
		int iSlow54 = (int(std::min<float>(fConst2, float(std::max<int>(0, (iSlow45 + 3))))) + 1);
		float fSlow55 = (0.0416666679f * (fSlow52 * fSlow42));
		int iSlow56 = (int(std::min<float>(fConst2, float(std::max<int>(0, (iSlow45 + 4))))) + 1);
		float fSlow57 = (5.0f * fSlow24);
		float fSlow58 = (fConst1 * ((440.0f * (fSlow3 * (1.0f - fSlow36))) + -0.0500000007f));
		float fSlow59 = (fSlow58 + -1.49999499f);
		float fSlow60 = std::floor(fSlow59);
		float fSlow61 = (fSlow58 + (-1.0f - fSlow60));
		float fSlow62 = (fSlow58 + (-2.0f - fSlow60));
		float fSlow63 = (fSlow58 + (-3.0f - fSlow60));
		float fSlow64 = (fSlow58 + (-4.0f - fSlow60));
		float fSlow65 = ((((0.0f - fSlow61) * (0.0f - (0.5f * fSlow62))) * (0.0f - (0.333333343f * fSlow63))) * (0.0f - (0.25f * fSlow64)));
		int iSlow66 = int(fSlow59);
		int iSlow67 = (int(std::min<float>(fConst2, float(std::max<int>(0, iSlow66)))) + 1);
		float fSlow68 = (fSlow58 - fSlow60);
		float fSlow69 = (((0.0f - fSlow62) * (0.0f - (0.5f * fSlow63))) * (0.0f - (0.333333343f * fSlow64)));
		int iSlow70 = (int(std::min<float>(fConst2, float(std::max<int>(0, (iSlow66 + 1))))) + 1);
		float fSlow71 = (0.5f * ((fSlow61 * (0.0f - fSlow63)) * (0.0f - (0.5f * fSlow64))));
		int iSlow72 = (int(std::min<float>(fConst2, float(std::max<int>(0, (iSlow66 + 2))))) + 1);
		float fSlow73 = (fSlow61 * fSlow62);
		float fSlow74 = (0.166666672f * (fSlow73 * (0.0f - fSlow64)));
		int iSlow75 = (int(std::min<float>(fConst2, float(std::max<int>(0, (iSlow66 + 3))))) + 1);
		float fSlow76 = (0.0416666679f * (fSlow73 * fSlow63));
		int iSlow77 = (int(std::min<float>(fConst2, float(std::max<int>(0, (iSlow66 + 4))))) + 1);
		for (int i0 = 0; (i0 < count); i0 = (i0 + 1)) {
			iVec0[0] = 1;
			float fTemp0 = (fSlow2 * (fRec1[1] + fRec1[2]));
			fRec7[(IOTA & 2047)] = fTemp0;
			float fRec8 = ((fSlow11 * fRec7[((IOTA - iSlow13) & 2047)]) + (fSlow14 * ((((fSlow15 * fRec7[((IOTA - iSlow16) & 2047)]) + (fSlow17 * fRec7[((IOTA - iSlow18) & 2047)])) + (fSlow20 * fRec7[((IOTA - iSlow21) & 2047)])) + (fSlow22 * fRec7[((IOTA - iSlow23) & 2047)]))));
			fVec1[0] = fSlow24;
			iRec11[0] = ((1103515245 * iRec11[1]) + 12345);
			iRec12[0] = ((fSlow24 > fVec1[1]) + ((fSlow24 <= fVec1[1]) * (iRec12[1] + (iRec12[1] > 0))));
			float fTemp1 = float(iRec12[0]);
			float fTemp2 = (fConst4 * fTemp1);
			int iTemp3 = (1 - iVec0[1]);
			iVec2[0] = iTemp3;
			fRec13[0] = (0.0f - (fConst8 * ((fConst9 * fRec13[1]) - (float(iTemp3) + float(iVec2[1])))));
			float fThen0 = (4.65661287e-10f * (float(iRec11[0]) * std::max<float>(0.0f, std::min<float>(fTemp2, (1.0f - (fConst6 * (fTemp1 - fConst3)))))));
			float fElse0 = (fSlow24 * fRec13[0]);
			fRec15[0] = (fSlow29 + (fRec15[1] - std::floor((fSlow29 + fRec15[1]))));
			float fThen1 = (iSlow28 ? fElse0 : fThen0);
			float fElse1 = (0.300000012f * (ftbl0faust_string_synthSIG0[int((65536.0f * fRec15[0]))] * std::max<float>(0.0f, std::min<float>((fConst6 * fTemp1), (1.0f - (fConst11 * (fTemp1 - fConst5)))))));
			float fTemp4 = (iSlow27 ? fElse1 : fThen1);
			float fTemp5 = (fSlow25 * fTemp4);
			float fTemp6 = (fRec0[2] + fTemp5);
			fVec4[(IOTA & 2047)] = fTemp6;
			float fTemp7 = (fSlow11 * fVec4[((IOTA - iSlow13) & 2047)]);
			float fTemp8 = (fSlow14 * ((((fSlow15 * fVec4[((IOTA - iSlow16) & 2047)]) + (fSlow17 * fVec4[((IOTA - iSlow18) & 2047)])) + (fSlow20 * fVec4[((IOTA - iSlow21) & 2047)])) + (fSlow22 * fVec4[((IOTA - iSlow23) & 2047)])));
			float fRec9 = (fTemp7 + fTemp8);
			float fRec10 = (fTemp8 + (fTemp0 + fTemp7));
			fRec3[0] = fRec8;
			float fRec4 = (fTemp5 + fRec3[1]);
			float fRec5 = fRec9;
			float fRec6 = fRec10;
			fRec0[0] = fRec4;
			fRec1[0] = fRec5;
			float fRec2 = fRec6;
			float fTemp9 = (fConst3 - fTemp1);
			float fTemp10 = (fSlow35 * (fRec17[1] + fRec17[2]));
			fRec23[(IOTA & 2047)] = fTemp10;
			float fRec24 = ((fSlow44 * fRec23[((IOTA - iSlow46) & 2047)]) + (fSlow47 * ((((fSlow48 * fRec23[((IOTA - iSlow49) & 2047)]) + (fSlow50 * fRec23[((IOTA - iSlow51) & 2047)])) + (fSlow53 * fRec23[((IOTA - iSlow54) & 2047)])) + (fSlow55 * fRec23[((IOTA - iSlow56) & 2047)]))));
			float fTemp11 = (fSlow57 * fTemp4);
			float fTemp12 = (fRec16[2] + fTemp11);
			fVec5[(IOTA & 2047)] = fTemp12;
			float fTemp13 = (fSlow44 * fVec5[((IOTA - iSlow46) & 2047)]);
			float fTemp14 = (fSlow47 * ((((fSlow48 * fVec5[((IOTA - iSlow49) & 2047)]) + (fSlow50 * fVec5[((IOTA - iSlow51) & 2047)])) + (fSlow53 * fVec5[((IOTA - iSlow54) & 2047)])) + (fSlow55 * fVec5[((IOTA - iSlow56) & 2047)])));
			float fRec25 = (fTemp13 + fTemp14);
			float fRec26 = (fTemp14 + (fTemp10 + fTemp13));
			fRec19[0] = fRec24;
			float fRec20 = (fTemp11 + fRec19[1]);
			float fRec21 = fRec25;
			float fRec22 = fRec26;
			fRec16[0] = fRec20;
			fRec17[0] = fRec21;
			float fRec18 = fRec22;
			float fTemp15 = (fSlow35 * (fRec28[1] + fRec28[2]));
			fRec34[(IOTA & 2047)] = fTemp15;
			float fRec35 = ((fSlow65 * fRec34[((IOTA - iSlow67) & 2047)]) + (fSlow68 * ((((fSlow69 * fRec34[((IOTA - iSlow70) & 2047)]) + (fSlow71 * fRec34[((IOTA - iSlow72) & 2047)])) + (fSlow74 * fRec34[((IOTA - iSlow75) & 2047)])) + (fSlow76 * fRec34[((IOTA - iSlow77) & 2047)]))));
			float fTemp16 = (fTemp11 + fRec27[2]);
			fVec6[(IOTA & 2047)] = fTemp16;
			float fTemp17 = (fSlow65 * fVec6[((IOTA - iSlow67) & 2047)]);
			float fTemp18 = (fSlow68 * ((((fSlow69 * fVec6[((IOTA - iSlow70) & 2047)]) + (fSlow71 * fVec6[((IOTA - iSlow72) & 2047)])) + (fSlow74 * fVec6[((IOTA - iSlow75) & 2047)])) + (fSlow76 * fVec6[((IOTA - iSlow77) & 2047)])));
			float fRec36 = (fTemp17 + fTemp18);
			float fRec37 = (fTemp18 + (fTemp15 + fTemp17));
			fRec30[0] = fRec35;
			float fRec31 = (fTemp11 + fRec30[1]);
			float fRec32 = fRec36;
			float fRec33 = fRec37;
			fRec27[0] = fRec31;
			fRec28[0] = fRec32;
			float fRec29 = fRec33;
			output0[i0] = FAUSTFLOAT((fSlow0 * ((fRec2 * std::max<float>(0.0f, std::min<float>(fTemp2, ((fSlow31 * fTemp9) + 1.0f)))) + (fSlow33 * (std::max<float>(0.0f, std::min<float>(fTemp2, ((fSlow34 * fTemp9) + 1.0f))) * (fRec18 + fRec29))))));
			iVec0[1] = iVec0[0];
			IOTA = (IOTA + 1);
			fVec1[1] = fVec1[0];
			iRec11[1] = iRec11[0];
			iRec12[1] = iRec12[0];
			iVec2[1] = iVec2[0];
			fRec13[1] = fRec13[0];
			fRec15[1] = fRec15[0];
			fRec3[1] = fRec3[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec19[1] = fRec19[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fRec30[1] = fRec30[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec28[2] = fRec28[1];
			fRec28[1] = fRec28[0];
		}
	}

};

#endif
