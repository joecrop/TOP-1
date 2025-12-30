/* ------------------------------------------------------------
name: "cluster-synth"
Code generated with Faust 2.37.3 (https://faust.grame.fr)
Compilation options: -a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __faust_cluster_synth_H__
#define  __faust_cluster_synth_H__

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

class faust_cluster_synthSIG0 {
	
  private:
	
	int iVec0[2];
	int iRec0[2];
	
  public:
	
	int getNumInputsfaust_cluster_synthSIG0() {
		return 0;
	}
	int getNumOutputsfaust_cluster_synthSIG0() {
		return 1;
	}
	
	void instanceInitfaust_cluster_synthSIG0(int sample_rate) {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			iRec0[l1] = 0;
		}
	}
	
	void fillfaust_cluster_synthSIG0(int count, float* table) {
		for (int i1 = 0; (i1 < count); i1 = (i1 + 1)) {
			iVec0[0] = 1;
			iRec0[0] = ((iVec0[1] + iRec0[1]) % 65536);
			table[i1] = std::sin((9.58738019e-05f * float(iRec0[0])));
			iVec0[1] = iVec0[0];
			iRec0[1] = iRec0[0];
		}
	}

};

static faust_cluster_synthSIG0* newfaust_cluster_synthSIG0() { return (faust_cluster_synthSIG0*)new faust_cluster_synthSIG0(); }
static void deletefaust_cluster_synthSIG0(faust_cluster_synthSIG0* dsp) { delete dsp; }

static float ftbl0faust_cluster_synthSIG0[65536];

#ifndef FAUSTCLASS 
#define FAUSTCLASS faust_cluster_synth
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class faust_cluster_synth : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fHslider1;
	float fRec1[2];
	float fConst2;
	FAUSTFLOAT fHslider2;
	float fRec2[2];
	FAUSTFLOAT fHslider3;
	float fRec3[2];
	float fConst3;
	float fRec4[2];
	float fRec5[2];
	float fConst4;
	float fRec6[2];
	float fRec7[2];
	float fConst5;
	float fRec8[2];
	float fRec9[2];
	float fConst6;
	float fRec10[2];
	float fRec11[2];
	float fConst7;
	float fRec12[2];
	float fConst8;
	float fConst9;
	FAUSTFLOAT fButton0;
	float fVec1[2];
	float fRec13[2];
	float fConst10;
	float fConst11;
	int iRec14[2];
	FAUSTFLOAT fHslider4;
	int iRec15[2];
	
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
		m->declare("filename", "cluster-synth.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "cluster-synth");
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
		faust_cluster_synthSIG0* sig0 = newfaust_cluster_synthSIG0();
		sig0->instanceInitfaust_cluster_synthSIG0(sample_rate);
		sig0->fillfaust_cluster_synthSIG0(65536, ftbl0faust_cluster_synthSIG0);
		deletefaust_cluster_synthSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (440.0f / fConst0);
		fConst2 = (1000.0f / fConst0);
		fConst3 = (1075.0f / fConst0);
		fConst4 = (1150.0f / fConst0);
		fConst5 = (1225.0f / fConst0);
		fConst6 = (1099.0f / fConst0);
		fConst7 = (1198.0f / fConst0);
		fConst8 = std::max<float>(1.0f, (0.00999999978f * fConst0));
		fConst9 = (1.0f / fConst8);
		fConst10 = (0.200000003f / std::max<float>(1.0f, (0.100000001f * fConst0)));
		fConst11 = (1.0f / std::max<float>(1.0f, (0.300000012f * fConst0)));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(1.0f);
		fHslider1 = FAUSTFLOAT(69.0f);
		fHslider2 = FAUSTFLOAT(50.0f);
		fHslider3 = FAUSTFLOAT(0.29999999999999999f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(0.5f);
	}
	
	virtual void instanceClear() {
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec1[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec2[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec3[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec4[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec5[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec6[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec7[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec8[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec9[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec10[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec11[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec12[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fVec1[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec13[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			iRec14[l16] = 0;
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			iRec15[l17] = 0;
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
	
	virtual faust_cluster_synth* clone() {
		return new faust_cluster_synth();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("cluster-synth");
		ui_interface->addHorizontalSlider("GRAINSIZE", &fHslider2, FAUSTFLOAT(50.0f), FAUSTFLOAT(5.0f), FAUSTFLOAT(200.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("KEY", &fHslider1, FAUSTFLOAT(69.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(127.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("SPREAD", &fHslider3, FAUSTFLOAT(0.300000012f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("TEXTURE", &fHslider4, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addButton("TRIGGER", &fButton0);
		ui_interface->addHorizontalSlider("VELOCITY", &fHslider0, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = (0.0333333351f * float(fHslider0));
		float fSlow1 = std::pow(2.0f, (0.0833333358f * (float(fHslider1) + -69.0f)));
		float fSlow2 = (fConst1 * fSlow1);
		float fSlow3 = float(fHslider2);
		float fSlow4 = (fConst2 / fSlow3);
		float fSlow5 = float(fHslider3);
		float fSlow6 = (0.0199999996f * fSlow5);
		float fSlow7 = (fConst1 * (fSlow1 * (fSlow6 + 1.0f)));
		float fSlow8 = (fConst3 / fSlow3);
		float fSlow9 = (fConst1 * (fSlow1 * (1.0f - fSlow6)));
		float fSlow10 = (fConst4 / fSlow3);
		float fSlow11 = (0.0399999991f * fSlow5);
		float fSlow12 = (fConst1 * (fSlow1 * (fSlow11 + 1.0f)));
		float fSlow13 = (fConst5 / fSlow3);
		float fSlow14 = (fConst1 * (fSlow1 * (1.0f - fSlow11)));
		float fSlow15 = (fConst6 / fSlow3);
		float fSlow16 = (fConst1 * (fSlow1 * ((0.00999999978f * fSlow5) + 1.0f)));
		float fSlow17 = (fConst7 / fSlow3);
		float fSlow18 = float(fButton0);
		int iSlow19 = (fSlow18 == 0.0f);
		float fSlow20 = float(fHslider4);
		float fSlow21 = (1.0f - fSlow20);
		float fSlow22 = (1.39698392e-10f * fSlow20);
		for (int i0 = 0; (i0 < count); i0 = (i0 + 1)) {
			fRec1[0] = (fSlow2 + (fRec1[1] - std::floor((fSlow2 + fRec1[1]))));
			fRec2[0] = (fSlow4 + (fRec2[1] - std::floor((fSlow4 + fRec2[1]))));
			fRec3[0] = (fSlow7 + (fRec3[1] - std::floor((fSlow7 + fRec3[1]))));
			fRec4[0] = (fSlow8 + (fRec4[1] - std::floor((fSlow8 + fRec4[1]))));
			fRec5[0] = (fSlow9 + (fRec5[1] - std::floor((fSlow9 + fRec5[1]))));
			fRec6[0] = (fSlow10 + (fRec6[1] - std::floor((fSlow10 + fRec6[1]))));
			fRec7[0] = (fSlow12 + (fRec7[1] - std::floor((fSlow12 + fRec7[1]))));
			fRec8[0] = (fSlow13 + (fRec8[1] - std::floor((fSlow13 + fRec8[1]))));
			fRec9[0] = (fSlow14 + (fRec9[1] - std::floor((fSlow14 + fRec9[1]))));
			fRec10[0] = (fSlow15 + (fRec10[1] - std::floor((fSlow15 + fRec10[1]))));
			fRec11[0] = (fSlow16 + (fRec11[1] - std::floor((fSlow16 + fRec11[1]))));
			fRec12[0] = (fSlow17 + (fRec12[1] - std::floor((fSlow17 + fRec12[1]))));
			fVec1[0] = fSlow18;
			fRec13[0] = (fSlow18 + (fRec13[1] * float((fVec1[1] >= fSlow18))));
			iRec14[0] = (iSlow19 * (iRec14[1] + 1));
			float fTemp0 = std::max<float>(0.0f, (std::min<float>((fConst9 * fRec13[0]), std::max<float>(((fConst10 * (fConst8 - fRec13[0])) + 1.0f), 0.800000012f)) * (1.0f - (fConst11 * float(iRec14[0])))));
			iRec15[0] = ((1103515245 * iRec15[1]) + 12345);
			output0[i0] = FAUSTFLOAT((fSlow0 * ((((((((ftbl0faust_cluster_synthSIG0[int((65536.0f * fRec1[0]))] * (ftbl0faust_cluster_synthSIG0[int((65536.0f * fRec2[0]))] + 1.0f)) + (ftbl0faust_cluster_synthSIG0[int((65536.0f * fRec3[0]))] * (ftbl0faust_cluster_synthSIG0[int((65536.0f * fRec4[0]))] + 1.0f))) + (ftbl0faust_cluster_synthSIG0[int((65536.0f * fRec5[0]))] * (ftbl0faust_cluster_synthSIG0[int((65536.0f * fRec6[0]))] + 1.0f))) + (ftbl0faust_cluster_synthSIG0[int((65536.0f * fRec7[0]))] * (ftbl0faust_cluster_synthSIG0[int((65536.0f * fRec8[0]))] + 1.0f))) + (ftbl0faust_cluster_synthSIG0[int((65536.0f * fRec9[0]))] * (ftbl0faust_cluster_synthSIG0[int((65536.0f * fRec10[0]))] + 1.0f))) + (ftbl0faust_cluster_synthSIG0[int((65536.0f * fRec11[0]))] * (ftbl0faust_cluster_synthSIG0[int((65536.0f * fRec12[0]))] + 1.0f))) * fTemp0) * (fSlow21 + ((fSlow22 * (float(iRec15[0]) * fTemp0)) + 1.0f)))));
			fRec1[1] = fRec1[0];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			fRec8[1] = fRec8[0];
			fRec9[1] = fRec9[0];
			fRec10[1] = fRec10[0];
			fRec11[1] = fRec11[0];
			fRec12[1] = fRec12[0];
			fVec1[1] = fVec1[0];
			fRec13[1] = fRec13[0];
			iRec14[1] = iRec14[0];
			iRec15[1] = iRec15[0];
		}
	}

};

#endif
