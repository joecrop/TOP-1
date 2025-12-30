/* ------------------------------------------------------------
name: "wavetable-synth"
Code generated with Faust 2.37.3 (https://faust.grame.fr)
Compilation options: -a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __faust_wavetable_synth_H__
#define  __faust_wavetable_synth_H__

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

class faust_wavetable_synthSIG0 {
	
  private:
	
	int iVec1[2];
	int iRec0[2];
	
  public:
	
	int getNumInputsfaust_wavetable_synthSIG0() {
		return 0;
	}
	int getNumOutputsfaust_wavetable_synthSIG0() {
		return 1;
	}
	
	void instanceInitfaust_wavetable_synthSIG0(int sample_rate) {
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			iVec1[l1] = 0;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			iRec0[l2] = 0;
		}
	}
	
	void fillfaust_wavetable_synthSIG0(int count, float* table) {
		for (int i1 = 0; (i1 < count); i1 = (i1 + 1)) {
			iVec1[0] = 1;
			iRec0[0] = ((iVec1[1] + iRec0[1]) % 65536);
			table[i1] = std::sin((9.58738019e-05f * float(iRec0[0])));
			iVec1[1] = iVec1[0];
			iRec0[1] = iRec0[0];
		}
	}

};

static faust_wavetable_synthSIG0* newfaust_wavetable_synthSIG0() { return (faust_wavetable_synthSIG0*)new faust_wavetable_synthSIG0(); }
static void deletefaust_wavetable_synthSIG0(faust_wavetable_synthSIG0* dsp) { delete dsp; }

static float ftbl0faust_wavetable_synthSIG0[65536];
static float faust_wavetable_synth_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS faust_wavetable_synth
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class faust_wavetable_synth : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fHslider1;
	int iVec0[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider2;
	float fRec1[2];
	float fConst2;
	float fConst3;
	float fConst4;
	float fRec3[2];
	float fVec2[2];
	int IOTA;
	float fVec3[4096];
	float fConst5;
	float fRec2[2];
	float fRec4[2];
	FAUSTFLOAT fHslider3;
	float fRec6[2];
	float fRec8[2];
	float fVec4[2];
	float fVec5[4096];
	float fRec7[2];
	float fRec9[2];
	float fRec11[2];
	float fRec13[2];
	float fVec6[2];
	float fVec7[4096];
	float fRec12[2];
	float fRec14[2];
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fButton0;
	float fVec8[2];
	float fRec16[2];
	float fConst6;
	FAUSTFLOAT fHslider5;
	int iRec17[2];
	
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
		m->declare("filename", "wavetable-synth.dsp");
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
		m->declare("name", "wavetable-synth");
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
		faust_wavetable_synthSIG0* sig0 = newfaust_wavetable_synthSIG0();
		sig0->instanceInitfaust_wavetable_synthSIG0(sample_rate);
		sig0->fillfaust_wavetable_synthSIG0(65536, ftbl0faust_wavetable_synthSIG0);
		deletefaust_wavetable_synthSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (440.0f / fConst0);
		fConst2 = (1760.0f / fConst0);
		fConst3 = (0.25f * fConst0);
		fConst4 = (1.0f / fConst0);
		fConst5 = (0.5f * fConst0);
		fConst6 = (0.200000003f / std::max<float>(1.0f, (0.100000001f * fConst0)));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(1.0f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(69.0f);
		fHslider3 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(0.01f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider5 = FAUSTFLOAT(0.29999999999999999f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec1[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec3[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fVec2[l5] = 0.0f;
		}
		IOTA = 0;
		for (int l6 = 0; (l6 < 4096); l6 = (l6 + 1)) {
			fVec3[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec2[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec4[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec6[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec8[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fVec4[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 4096); l12 = (l12 + 1)) {
			fVec5[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec7[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec9[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec11[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec13[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fVec6[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 4096); l18 = (l18 + 1)) {
			fVec7[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec12[l19] = 0.0f;
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec14[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fVec8[l21] = 0.0f;
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec16[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			iRec17[l23] = 0;
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
	
	virtual faust_wavetable_synth* clone() {
		return new faust_wavetable_synth();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("wavetable-synth");
		ui_interface->addHorizontalSlider("ATTACK", &fHslider4, FAUSTFLOAT(0.00999999978f), FAUSTFLOAT(0.00100000005f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.00100000005f));
		ui_interface->addHorizontalSlider("DETUNE", &fHslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.5f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("KEY", &fHslider2, FAUSTFLOAT(69.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(127.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("POSITION", &fHslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("RELEASE", &fHslider5, FAUSTFLOAT(0.300000012f), FAUSTFLOAT(0.00999999978f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addButton("TRIGGER", &fButton0);
		ui_interface->addHorizontalSlider("VELOCITY", &fHslider0, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = (0.400000006f * float(fHslider0));
		float fSlow1 = (3.0f * float(fHslider1));
		int iSlow2 = int(fSlow1);
		float fSlow3 = float(iSlow2);
		float fSlow4 = (fSlow3 + (1.0f - fSlow1));
		int iSlow5 = (iSlow2 >= 2);
		int iSlow6 = (iSlow2 >= 1);
		float fSlow7 = std::pow(2.0f, (0.0833333358f * (float(fHslider2) + -69.0f)));
		float fSlow8 = (fConst1 * fSlow7);
		float fSlow9 = (fConst2 * fSlow7);
		float fSlow10 = (440.0f * fSlow7);
		float fSlow11 = std::max<float>(fSlow10, 23.4489498f);
		float fSlow12 = std::max<float>(20.0f, std::fabs(fSlow11));
		float fSlow13 = (fConst3 / fSlow12);
		float fSlow14 = (fConst4 * fSlow12);
		float fSlow15 = std::max<float>(0.0f, std::min<float>(2047.0f, (fConst5 / fSlow11)));
		float fSlow16 = std::floor(fSlow15);
		float fSlow17 = (fSlow16 + (1.0f - fSlow15));
		int iSlow18 = int(fSlow15);
		float fSlow19 = (fSlow15 - fSlow16);
		int iSlow20 = (iSlow18 + 1);
		int iSlow21 = (iSlow2 >= 3);
		float fSlow22 = std::max<float>(1.1920929e-07f, std::fabs(fSlow10));
		float fSlow23 = (fConst4 * fSlow22);
		float fSlow24 = (1.0f - (fConst0 / fSlow22));
		float fSlow25 = (fSlow1 - fSlow3);
		int iSlow26 = std::min<int>((iSlow2 + 1), 3);
		int iSlow27 = (iSlow26 >= 2);
		int iSlow28 = (iSlow26 >= 1);
		int iSlow29 = (iSlow26 >= 3);
		float fSlow30 = float(fHslider3);
		float fSlow31 = (0.5f * (fSlow30 * float((fSlow30 > 0.0f))));
		float fSlow32 = (0.0199999996f * fSlow30);
		float fSlow33 = (fSlow7 * (fSlow32 + 1.0f));
		float fSlow34 = (fConst1 * fSlow33);
		float fSlow35 = (fConst2 * fSlow33);
		float fSlow36 = (440.0f * fSlow33);
		float fSlow37 = std::max<float>(fSlow36, 23.4489498f);
		float fSlow38 = std::max<float>(20.0f, std::fabs(fSlow37));
		float fSlow39 = (fConst3 / fSlow38);
		float fSlow40 = (fConst4 * fSlow38);
		float fSlow41 = std::max<float>(0.0f, std::min<float>(2047.0f, (fConst5 / fSlow37)));
		float fSlow42 = std::floor(fSlow41);
		float fSlow43 = (fSlow42 + (1.0f - fSlow41));
		int iSlow44 = int(fSlow41);
		float fSlow45 = (fSlow41 - fSlow42);
		int iSlow46 = (iSlow44 + 1);
		float fSlow47 = std::max<float>(1.1920929e-07f, std::fabs(fSlow36));
		float fSlow48 = (fConst4 * fSlow47);
		float fSlow49 = (1.0f - (fConst0 / fSlow47));
		float fSlow50 = (fSlow7 * (1.0f - fSlow32));
		float fSlow51 = (fConst1 * fSlow50);
		float fSlow52 = (fConst2 * fSlow50);
		float fSlow53 = (440.0f * fSlow50);
		float fSlow54 = std::max<float>(fSlow53, 23.4489498f);
		float fSlow55 = std::max<float>(20.0f, std::fabs(fSlow54));
		float fSlow56 = (fConst3 / fSlow55);
		float fSlow57 = (fConst4 * fSlow55);
		float fSlow58 = std::max<float>(0.0f, std::min<float>(2047.0f, (fConst5 / fSlow54)));
		float fSlow59 = std::floor(fSlow58);
		float fSlow60 = (fSlow59 + (1.0f - fSlow58));
		int iSlow61 = int(fSlow58);
		float fSlow62 = (fSlow58 - fSlow59);
		int iSlow63 = (iSlow61 + 1);
		float fSlow64 = std::max<float>(1.1920929e-07f, std::fabs(fSlow53));
		float fSlow65 = (fConst4 * fSlow64);
		float fSlow66 = (1.0f - (fConst0 / fSlow64));
		float fSlow67 = std::max<float>(1.0f, (fConst0 * float(fHslider4)));
		float fSlow68 = (1.0f / fSlow67);
		float fSlow69 = float(fButton0);
		float fSlow70 = (1.0f / std::max<float>(1.0f, (fConst0 * float(fHslider5))));
		int iSlow71 = (fSlow69 == 0.0f);
		for (int i0 = 0; (i0 < count); i0 = (i0 + 1)) {
			iVec0[0] = 1;
			fRec1[0] = (fSlow8 + (fRec1[1] - std::floor((fSlow8 + fRec1[1]))));
			float fTemp0 = ftbl0faust_wavetable_synthSIG0[int((65536.0f * fRec1[0]))];
			float fTemp1 = float(iVec0[1]);
			fRec3[0] = (fSlow14 + (fRec3[1] - std::floor((fSlow14 + fRec3[1]))));
			float fTemp2 = faust_wavetable_synth_faustpower2_f(((2.0f * fRec3[0]) + -1.0f));
			fVec2[0] = fTemp2;
			float fTemp3 = (fSlow13 * (fTemp1 * (fTemp2 - fVec2[1])));
			fVec3[(IOTA & 4095)] = fTemp3;
			float fTemp4 = ((fSlow17 * fVec3[((IOTA - iSlow18) & 4095)]) + (fSlow19 * fVec3[((IOTA - iSlow20) & 4095)]));
			fRec2[0] = (((0.999000013f * fRec2[1]) + fTemp3) - fTemp4);
			float fTemp5 = (fSlow9 * fRec2[0]);
			float fTemp6 = (fSlow23 + (fRec4[1] + -1.0f));
			int iTemp7 = (fTemp6 < 0.0f);
			float fTemp8 = (fSlow23 + fRec4[1]);
			fRec4[0] = (iTemp7 ? fTemp8 : fTemp6);
			float fThen2 = (fSlow23 + (fRec4[1] + (fSlow24 * fTemp6)));
			float fRec5 = (iTemp7 ? fTemp8 : fThen2);
			float fTemp9 = ((2.0f * fRec5) + -1.0f);
			float fTemp10 = (fTemp3 - fTemp4);
			float fThen4 = (iSlow6 ? fTemp5 : fTemp0);
			float fElse4 = (iSlow21 ? fTemp10 : fTemp9);
			float fThen7 = (iSlow28 ? fTemp5 : fTemp0);
			float fElse7 = (iSlow29 ? fTemp10 : fTemp9);
			fRec6[0] = (fSlow34 + (fRec6[1] - std::floor((fSlow34 + fRec6[1]))));
			float fTemp11 = ftbl0faust_wavetable_synthSIG0[int((65536.0f * fRec6[0]))];
			fRec8[0] = (fSlow40 + (fRec8[1] - std::floor((fSlow40 + fRec8[1]))));
			float fTemp12 = faust_wavetable_synth_faustpower2_f(((2.0f * fRec8[0]) + -1.0f));
			fVec4[0] = fTemp12;
			float fTemp13 = (fSlow39 * (fTemp1 * (fTemp12 - fVec4[1])));
			fVec5[(IOTA & 4095)] = fTemp13;
			float fTemp14 = ((fSlow43 * fVec5[((IOTA - iSlow44) & 4095)]) + (fSlow45 * fVec5[((IOTA - iSlow46) & 4095)]));
			fRec7[0] = (((0.999000013f * fRec7[1]) + fTemp13) - fTemp14);
			float fTemp15 = (fSlow35 * fRec7[0]);
			float fTemp16 = (fSlow48 + (fRec9[1] + -1.0f));
			int iTemp17 = (fTemp16 < 0.0f);
			float fTemp18 = (fSlow48 + fRec9[1]);
			fRec9[0] = (iTemp17 ? fTemp18 : fTemp16);
			float fThen10 = (fSlow48 + (fRec9[1] + (fSlow49 * fTemp16)));
			float fRec10 = (iTemp17 ? fTemp18 : fThen10);
			float fTemp19 = ((2.0f * fRec10) + -1.0f);
			float fTemp20 = (fTemp13 - fTemp14);
			float fThen12 = (iSlow6 ? fTemp15 : fTemp11);
			float fElse12 = (iSlow21 ? fTemp20 : fTemp19);
			float fThen15 = (iSlow28 ? fTemp15 : fTemp11);
			float fElse15 = (iSlow29 ? fTemp20 : fTemp19);
			fRec11[0] = (fSlow51 + (fRec11[1] - std::floor((fSlow51 + fRec11[1]))));
			float fTemp21 = ftbl0faust_wavetable_synthSIG0[int((65536.0f * fRec11[0]))];
			fRec13[0] = (fSlow57 + (fRec13[1] - std::floor((fSlow57 + fRec13[1]))));
			float fTemp22 = faust_wavetable_synth_faustpower2_f(((2.0f * fRec13[0]) + -1.0f));
			fVec6[0] = fTemp22;
			float fTemp23 = (fSlow56 * (fTemp1 * (fTemp22 - fVec6[1])));
			fVec7[(IOTA & 4095)] = fTemp23;
			float fTemp24 = ((fSlow60 * fVec7[((IOTA - iSlow61) & 4095)]) + (fSlow62 * fVec7[((IOTA - iSlow63) & 4095)]));
			fRec12[0] = (((0.999000013f * fRec12[1]) + fTemp23) - fTemp24);
			float fTemp25 = (fSlow52 * fRec12[0]);
			float fTemp26 = (fSlow65 + (fRec14[1] + -1.0f));
			int iTemp27 = (fTemp26 < 0.0f);
			float fTemp28 = (fSlow65 + fRec14[1]);
			fRec14[0] = (iTemp27 ? fTemp28 : fTemp26);
			float fThen18 = (fSlow65 + (fRec14[1] + (fSlow66 * fTemp26)));
			float fRec15 = (iTemp27 ? fTemp28 : fThen18);
			float fTemp29 = ((2.0f * fRec15) + -1.0f);
			float fTemp30 = (fTemp23 - fTemp24);
			float fThen20 = (iSlow6 ? fTemp25 : fTemp21);
			float fElse20 = (iSlow21 ? fTemp30 : fTemp29);
			float fThen23 = (iSlow28 ? fTemp25 : fTemp21);
			float fElse23 = (iSlow29 ? fTemp30 : fTemp29);
			fVec8[0] = fSlow69;
			fRec16[0] = (fSlow69 + (fRec16[1] * float((fVec8[1] >= fSlow69))));
			iRec17[0] = (iSlow71 * (iRec17[1] + 1));
			output0[i0] = FAUSTFLOAT((fSlow0 * ((((fSlow4 * (iSlow5 ? fElse4 : fThen4)) + (fSlow25 * (iSlow27 ? fElse7 : fThen7))) + (fSlow31 * (((fSlow4 * (iSlow5 ? fElse12 : fThen12)) + (fSlow25 * (iSlow27 ? fElse15 : fThen15))) + ((fSlow4 * (iSlow5 ? fElse20 : fThen20)) + (fSlow25 * (iSlow27 ? fElse23 : fThen23)))))) * std::max<float>(0.0f, (std::min<float>((fSlow68 * fRec16[0]), std::max<float>(((fConst6 * (fSlow67 - fRec16[0])) + 1.0f), 0.800000012f)) * (1.0f - (fSlow70 * float(iRec17[0]))))))));
			iVec0[1] = iVec0[0];
			fRec1[1] = fRec1[0];
			fRec3[1] = fRec3[0];
			fVec2[1] = fVec2[0];
			IOTA = (IOTA + 1);
			fRec2[1] = fRec2[0];
			fRec4[1] = fRec4[0];
			fRec6[1] = fRec6[0];
			fRec8[1] = fRec8[0];
			fVec4[1] = fVec4[0];
			fRec7[1] = fRec7[0];
			fRec9[1] = fRec9[0];
			fRec11[1] = fRec11[0];
			fRec13[1] = fRec13[0];
			fVec6[1] = fVec6[0];
			fRec12[1] = fRec12[0];
			fRec14[1] = fRec14[0];
			fVec8[1] = fVec8[0];
			fRec16[1] = fRec16[0];
			iRec17[1] = iRec17[0];
		}
	}

};

#endif
