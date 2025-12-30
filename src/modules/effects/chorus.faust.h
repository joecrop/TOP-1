/* ------------------------------------------------------------
name: "chorus"
Code generated with Faust 2.37.3 (https://faust.grame.fr)
Compilation options: -a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __faust_chorus_H__
#define  __faust_chorus_H__

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

class faust_chorusSIG0 {
	
  private:
	
	int iVec1[2];
	int iRec0[2];
	
  public:
	
	int getNumInputsfaust_chorusSIG0() {
		return 0;
	}
	int getNumOutputsfaust_chorusSIG0() {
		return 1;
	}
	
	void instanceInitfaust_chorusSIG0(int sample_rate) {
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			iVec1[l1] = 0;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			iRec0[l2] = 0;
		}
	}
	
	void fillfaust_chorusSIG0(int count, float* table) {
		for (int i1 = 0; (i1 < count); i1 = (i1 + 1)) {
			iVec1[0] = 1;
			iRec0[0] = ((iVec1[1] + iRec0[1]) % 65536);
			table[i1] = std::sin((9.58738019e-05f * float(iRec0[0])));
			iVec1[1] = iVec1[0];
			iRec0[1] = iRec0[0];
		}
	}

};

static faust_chorusSIG0* newfaust_chorusSIG0() { return (faust_chorusSIG0*)new faust_chorusSIG0(); }
static void deletefaust_chorusSIG0(faust_chorusSIG0* dsp) { delete dsp; }

static float ftbl0faust_chorusSIG0[65536];

#ifndef FAUSTCLASS 
#define FAUSTCLASS faust_chorus
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class faust_chorus : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int IOTA;
	float fVec0[4096];
	FAUSTFLOAT fHslider1;
	int fSampleRate;
	float fConst0;
	FAUSTFLOAT fHslider2;
	float fConst1;
	FAUSTFLOAT fHslider3;
	float fRec1[2];
	float fConst2;
	float fRec2[2];
	float fConst3;
	float fRec3[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.2");
		m->declare("compile_options", "-a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "chorus.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "chorus");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.1");
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
		faust_chorusSIG0* sig0 = newfaust_chorusSIG0();
		sig0->instanceInitfaust_chorusSIG0(sample_rate);
		sig0->fillfaust_chorusSIG0(65536, ftbl0faust_chorusSIG0);
		deletefaust_chorusSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (1.0f / fConst0);
		fConst2 = (1.10000002f / fConst0);
		fConst3 = (0.899999976f / fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.5f);
		fHslider1 = FAUSTFLOAT(2.0f);
		fHslider2 = FAUSTFLOAT(3.0f);
		fHslider3 = FAUSTFLOAT(1.5f);
	}
	
	virtual void instanceClear() {
		IOTA = 0;
		for (int l0 = 0; (l0 < 4096); l0 = (l0 + 1)) {
			fVec0[l0] = 0.0f;
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec1[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec2[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec3[l5] = 0.0f;
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
	
	virtual faust_chorus* clone() {
		return new faust_chorus();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("chorus");
		ui_interface->addHorizontalSlider("depth", &fHslider2, FAUSTFLOAT(3.0f), FAUSTFLOAT(0.5f), FAUSTFLOAT(10.0f), FAUSTFLOAT(0.100000001f));
		ui_interface->addHorizontalSlider("mix", &fHslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("rate", &fHslider3, FAUSTFLOAT(1.5f), FAUSTFLOAT(0.100000001f), FAUSTFLOAT(5.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("voices", &fHslider1, FAUSTFLOAT(2.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(1.0f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fHslider0);
		float fSlow1 = (1.0f - fSlow0);
		float fSlow2 = float(fHslider1);
		float fSlow3 = (fSlow0 / fSlow2);
		float fSlow4 = (0.00100000005f * float(fHslider2));
		float fSlow5 = float(fHslider3);
		float fSlow6 = (fConst1 * fSlow5);
		float fSlow7 = float((fSlow2 > 1.0f));
		float fSlow8 = (fConst2 * fSlow5);
		float fSlow9 = float((fSlow2 > 2.0f));
		float fSlow10 = (fConst3 * fSlow5);
		float fSlow11 = float((fSlow2 > 3.0f));
		for (int i0 = 0; (i0 < count); i0 = (i0 + 1)) {
			float fTemp0 = float(input0[i0]);
			fVec0[(IOTA & 4095)] = fTemp0;
			fRec1[0] = (fSlow6 + (fRec1[1] - std::floor((fSlow6 + fRec1[1]))));
			float fTemp1 = (fSlow4 * ftbl0faust_chorusSIG0[int((65536.0f * fRec1[0]))]);
			float fTemp2 = (fConst0 * (fTemp1 + 0.0149999997f));
			int iTemp3 = int(fTemp2);
			float fTemp4 = std::floor(fTemp2);
			fRec2[0] = (fSlow8 + (fRec2[1] - std::floor((fSlow8 + fRec2[1]))));
			float fTemp5 = (fConst0 * ((fSlow4 * ftbl0faust_chorusSIG0[int((65536.0f * fRec2[0]))]) + 0.0149999997f));
			int iTemp6 = int(fTemp5);
			float fTemp7 = std::floor(fTemp5);
			fRec3[0] = (fSlow10 + (fRec3[1] - std::floor((fSlow10 + fRec3[1]))));
			float fTemp8 = (fConst0 * ((fSlow4 * ftbl0faust_chorusSIG0[int((65536.0f * fRec3[0]))]) + 0.0149999997f));
			int iTemp9 = int(fTemp8);
			float fTemp10 = std::floor(fTemp8);
			float fTemp11 = (fConst0 * (0.0149999997f - fTemp1));
			int iTemp12 = int(fTemp11);
			float fTemp13 = std::floor(fTemp11);
			output0[i0] = FAUSTFLOAT(((fSlow1 * fTemp0) + (fSlow3 * (((((fVec0[((IOTA - int(std::min<float>(2401.0f, float(std::max<int>(0, iTemp3))))) & 4095)] * (fTemp4 + (1.0f - fTemp2))) + ((fTemp2 - fTemp4) * fVec0[((IOTA - int(std::min<float>(2401.0f, float(std::max<int>(0, (iTemp3 + 1)))))) & 4095)])) + (fSlow7 * ((fVec0[((IOTA - int(std::min<float>(2401.0f, float(std::max<int>(0, iTemp6))))) & 4095)] * (fTemp7 + (1.0f - fTemp5))) + ((fTemp5 - fTemp7) * fVec0[((IOTA - int(std::min<float>(2401.0f, float(std::max<int>(0, (iTemp6 + 1)))))) & 4095)])))) + (fSlow9 * ((fVec0[((IOTA - int(std::min<float>(2401.0f, float(std::max<int>(0, iTemp9))))) & 4095)] * (fTemp10 + (1.0f - fTemp8))) + ((fTemp8 - fTemp10) * fVec0[((IOTA - int(std::min<float>(2401.0f, float(std::max<int>(0, (iTemp9 + 1)))))) & 4095)])))) + (fSlow11 * ((fVec0[((IOTA - int(std::min<float>(2401.0f, float(std::max<int>(0, iTemp12))))) & 4095)] * (fTemp13 + (1.0f - fTemp11))) + ((fTemp11 - fTemp13) * fVec0[((IOTA - int(std::min<float>(2401.0f, float(std::max<int>(0, (iTemp12 + 1)))))) & 4095)])))))));
			IOTA = (IOTA + 1);
			fRec1[1] = fRec1[0];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
		}
	}

};

#endif
