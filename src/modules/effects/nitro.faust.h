/* ------------------------------------------------------------
name: "nitro"
Code generated with Faust 2.37.3 (https://faust.grame.fr)
Compilation options: -a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __faust_nitro_H__
#define  __faust_nitro_H__

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

/* link with : "" */
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>


#ifndef FAUSTCLASS 
#define FAUSTCLASS faust_nitro
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class faust_nitro : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	int fSampleRate;
	float fConst0;
	FAUSTFLOAT fHslider3;
	float fRec2[2];
	float fRec1[2];
	float fRec0[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "0.1");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.2");
		m->declare("compile_options", "-a /home/joe/JOE/TOP-1/scripts/faust-template.h -lang cpp -es 1 -single -ftz 0");
		m->declare("compressors.lib/compression_gain_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/compression_gain_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/compression_gain_mono:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/compressor_lad_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/compressor_lad_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/compressor_lad_mono:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/compressor_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/compressor_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/compressor_mono:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/name", "Faust Compressor Effect Library");
		m->declare("compressors.lib/version", "0.1");
		m->declare("filename", "nitro.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "nitro");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.1");
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
		fConst0 = (1.0f / std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate))));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(4.0f);
		fHslider1 = FAUSTFLOAT(0.5f);
		fHslider2 = FAUSTFLOAT(0.01f);
		fHslider3 = FAUSTFLOAT(0.10000000000000001f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec2[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec1[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec0[l2] = 0.0f;
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
	
	virtual faust_nitro* clone() {
		return new faust_nitro();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("nitro");
		ui_interface->addHorizontalSlider("ATTACK", &fHslider2, FAUSTFLOAT(0.00999999978f), FAUSTFLOAT(0.00100000005f), FAUSTFLOAT(0.100000001f), FAUSTFLOAT(0.00100000005f));
		ui_interface->addHorizontalSlider("DRIVE", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("RATIO", &fHslider0, FAUSTFLOAT(4.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(20.0f), FAUSTFLOAT(0.5f));
		ui_interface->addHorizontalSlider("RELEASE", &fHslider3, FAUSTFLOAT(0.100000001f), FAUSTFLOAT(0.00999999978f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fHslider0);
		float fSlow1 = ((0.100000001f * (fSlow0 + -1.0f)) + 1.0f);
		float fSlow2 = float(fHslider1);
		float fSlow3 = ((3.0f * fSlow2) + 1.0f);
		float fSlow4 = float(fHslider2);
		float fSlow5 = (0.5f * fSlow4);
		int iSlow6 = (std::fabs(fSlow5) < 1.1920929e-07f);
		float fThen1 = std::exp((0.0f - (fConst0 / (iSlow6 ? 1.0f : fSlow5))));
		float fSlow7 = (iSlow6 ? 0.0f : fThen1);
		float fSlow8 = ((1.0f / std::max<float>(1.1920929e-07f, fSlow0)) + -1.0f);
		int iSlow9 = (std::fabs(fSlow4) < 1.1920929e-07f);
		float fThen3 = std::exp((0.0f - (fConst0 / (iSlow9 ? 1.0f : fSlow4))));
		float fSlow10 = (iSlow9 ? 0.0f : fThen3);
		float fSlow11 = float(fHslider3);
		int iSlow12 = (std::fabs(fSlow11) < 1.1920929e-07f);
		float fThen5 = std::exp((0.0f - (fConst0 / (iSlow12 ? 1.0f : fSlow11))));
		float fSlow13 = (iSlow12 ? 0.0f : fThen5);
		float fSlow14 = (10.0f * fSlow2);
		float fSlow15 = (1.0f - fSlow7);
		for (int i0 = 0; (i0 < count); i0 = (i0 + 1)) {
			float fTemp0 = float(tanhf(float((fSlow3 * float(input0[i0])))));
			float fTemp1 = std::fabs(fTemp0);
			float fTemp2 = ((fRec1[1] > fTemp1) ? fSlow13 : fSlow10);
			fRec2[0] = ((fRec2[1] * fTemp2) + (fTemp1 * (1.0f - fTemp2)));
			fRec1[0] = fRec2[0];
			fRec0[0] = ((fRec0[1] * fSlow7) + (fSlow8 * (std::max<float>(((20.0f * (std::log10(std::max<float>(1.17549435e-38f, fRec1[0])) + 1.0f)) - fSlow14), 0.0f) * fSlow15)));
			output0[i0] = FAUSTFLOAT(float(tanhf(float((fSlow1 * (fTemp0 * std::pow(10.0f, (0.0500000007f * fRec0[0]))))))));
			fRec2[1] = fRec2[0];
			fRec1[1] = fRec1[0];
			fRec0[1] = fRec0[0];
		}
	}

};

#endif
