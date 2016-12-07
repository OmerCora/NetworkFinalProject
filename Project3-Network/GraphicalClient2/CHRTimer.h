#ifndef _CHRTimer_HG_
#define _CHRTimer_HG_

// Used to do high resulution timing
// Used QueryPerformanceTimer() 
// http://msdn.microsoft.com/en-us/library/ms644905(VS.85).aspx

// Written by Michael Feeney, Fanshawe College, 2010
// mfeeney@fanshawec.on.ca
// It may be distributed under the terms of the General Public License:
// http://www.fsf.org/licenses/gpl.html
// Use this code at your own risk. It is indented only as a learning aid.
//

#include <vector>

class CHRTimer
{
public:
	CHRTimer();
	virtual ~CHRTimer();
	void Start(void);
	void Stop(void);		// Will update the long duration timer, too
	void Reset(bool bStopTimerToo = false);
	void ResetAndStart(void);
	// Note: this "resets" the timer

	// This will 'reset' the regular (short duration) timer and update the long duration
	float GetElapsedSeconds(bool bAndResetTimer = false);	// Can also use __int64 (win32 specific)

	void SetNumberOfAverageSamples(unsigned int numSamples);
	float GetElapsedSecondsAverage(bool bAndResetTimer = false);

	unsigned long long GetFrequency(void);	

	void ResetLongDuration(void);
	// Updates the long duration (and resets the 'short' or regular duration timer)
	// The idea is that if you know you are timing a long thing, call this often
	void UpdateLongDuration(void);
	float get_fLondDurationTotalSeconds(void);
	double get_dLondDurationTotalSeconds(void);
	void getLongDurationValues(float &seconds, float &minutes );
private:
	unsigned long long m_elapsedTime;	// Can also use __int64 (win32 specific)
	unsigned long long m_startTime;		// Can also use __int64 (win32 specific)
	unsigned long long m_stopTime;		// Can also use __int64 (win32 specific)
	unsigned long long m_frequency;		// Can also use __int64 (win32 specific)
	// The timer seems to screw up on long (over a second) evens on some computers
	// GetElapsedSeconds updates this
	void m_AppendLondDurationTimer(float deltaTime);
	double m_dLongDurationTotalSeconds;

	void m_ResetNumberOfSamples(unsigned int newNumberOfSamples);
	std::vector< float > m_vecLastXSamples;
	unsigned int m_nextSampleIndex;
	unsigned int m_numberOfSamples;
	static const unsigned int DEFAULTNUMBEROFSAMPLES = 20;
	bool m_bFirstSampleTaken;	// Used to pre-load the average sample thing

	bool m_bIsRunning;
};

#endif // _CHRTimer_HG_



//#ifndef _CHRTimer_HG_
//#define _CHRTimer_HG_
//
//// Used to do high resulution timing
//// Used QueryPerformanceTimer() 
//// http://msdn.microsoft.com/en-us/library/ms644905(VS.85).aspx
//
//// Written by Michael Feeney, Fanshawe College, 2010
//// mfeeney@fanshawec.on.ca
//// It may be distributed under the terms of the General Public License:
//// http://www.fsf.org/licenses/gpl.html
//// Use this code at your own risk. It is indented only as a learning aid.
////
//
//class CHRTimer
//{
//public:
//	CHRTimer();
//	virtual ~CHRTimer();
//	void Start(void);
//	void Stop(void);
//	void Reset(void);
//	float GetElapsedSeconds(void);	// Can also use __int64 (win32 specific)
//	unsigned long long GetFrequency(void);	
//private:
//	unsigned long long m_elapsedTime;	// Can also use __int64 (win32 specific)
//	unsigned long long m_startTime;		// Can also use __int64 (win32 specific)
//	unsigned long long m_stopTime;		// Can also use __int64 (win32 specific)
//	unsigned long long m_frequency;		// Can also use __int64 (win32 specific)
//};
//
//#endif // _CHRTimer_HG_
