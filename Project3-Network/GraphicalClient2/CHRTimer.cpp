#include "CHRTimer.h"
#include "windows.h"	// For high freq

// Written by Michael Feeney, Fanshawe College, 2010
// mfeeney@fanshawec.on.ca
// It may be distributed under the terms of the General Public License:
// http://www.fsf.org/licenses/gpl.html
// Use this code at your own risk. It is indented only as a learning aid.
//

//__int64 start_count;
//__int64 end_count;
//__int64 freq;
//
//// Get the frequency and save it, it shouldn't change
//QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
//QueryPerformanceCounter((LARGE_INTEGER*)&start_count);
//// do some stuff that takes up time
//QueryPerformanceCounter((LARGE_INTEGER*)&end_count);
////find the time
//float time = (float)(end_count - start_count) / (float)freq;


CHRTimer::CHRTimer()
{
	//this->m_elapsedTime = 0;
	//this->m_startTime = 0;
	//this->m_stopTime = 0;

	//this->m_fLongDurationSeconds = 0.0f;
	//this->m_fLongDurationMinutes = 0.0f;
	this->Reset();

	LARGE_INTEGER tempTime;
	
	QueryPerformanceFrequency( &tempTime );   // 8 MHz

	this->m_frequency = static_cast<unsigned long long>( tempTime.QuadPart );

	this->m_ResetNumberOfSamples( DEFAULTNUMBEROFSAMPLES );
	this->m_bFirstSampleTaken = false;	// Used to pre-load the average sample thing

	return;
}

CHRTimer::~CHRTimer()
{
	return;
}

void CHRTimer::Start(void)
{
	LARGE_INTEGER tempTime;
	QueryPerformanceCounter( &tempTime ); 
	this->m_startTime = static_cast<unsigned long long>( tempTime.QuadPart ) ;
	this->m_bIsRunning = true;
}

void CHRTimer::Stop(void)
{
	LARGE_INTEGER tempTime;
	QueryPerformanceCounter( &tempTime );
	this->m_stopTime = static_cast<unsigned long long>( tempTime.QuadPart ) ;
	this->UpdateLongDuration();
	this->m_bIsRunning = false;
}

void CHRTimer::Reset(bool bStopTimerToo /*= false*/)
{
	LARGE_INTEGER tempTime;
	QueryPerformanceCounter( &tempTime ); 
	this->m_elapsedTime = 0;
	this->m_startTime = this->m_startTime = static_cast<unsigned long long>( tempTime.QuadPart ) ;
	this->m_stopTime = this->m_startTime;

	//this->m_fLongDurationMinutes = 0.0f;
	//this->m_fLongDurationSeconds = 0.0f;
	//this->m_dLongDurationTotalSeconds = 0.0;
	this->ResetLongDuration();
	
	if ( bStopTimerToo )
	{
		this->m_bIsRunning = false;
	}

	return;
}

void CHRTimer::ResetAndStart(void)
{
	this->Reset();
	this->Start();
	return;
}

float CHRTimer::GetElapsedSeconds(bool bAndResetTimer /*= false*/)
{
	// Update current time
	LARGE_INTEGER tempTime;
	QueryPerformanceCounter( &tempTime );

	if ( this->m_bIsRunning )
	{
		this->m_stopTime = static_cast<unsigned long long>( tempTime.QuadPart ) ;
	}

	// Calculate elapsed time
	this->m_elapsedTime = this->m_stopTime - this->m_startTime;
	float elapsedTimeSec = static_cast<float>( this->m_elapsedTime ) / static_cast<float>(this->m_frequency );

	//if ( this->m_bIsRunning )
	//{
	//	// Reset it
	//	this->m_startTime = this->m_stopTime;
	//}
	if ( bAndResetTimer )
	{
		this->Reset(false);
	}

	// Update the long duration timer
	if ( this->m_bIsRunning )
	{
		this->m_AppendLondDurationTimer(elapsedTimeSec);
	}

	return elapsedTimeSec;
}

void CHRTimer::SetNumberOfAverageSamples(unsigned int numSamples)
{
	this->m_ResetNumberOfSamples( numSamples );
	return;
}

float CHRTimer::GetElapsedSecondsAverage(bool bAndResetTimer /*= false*/)
{
	float curSample = this->GetElapsedSeconds( bAndResetTimer );

	if ( this->m_vecLastXSamples.empty() )
	{
		return 0.0f;
	}

	this->m_vecLastXSamples[this->m_nextSampleIndex] = curSample;

	this->m_nextSampleIndex++;
	if ( this->m_nextSampleIndex >= this->m_vecLastXSamples.size() )
	{
		this->m_nextSampleIndex = 0;
	}

	float sumSamples = 0.0f;
	for ( int index = 0; index != this->m_numberOfSamples; index++ )
	{
		sumSamples += this->m_vecLastXSamples[index];
	}

	return sumSamples / static_cast<float>( this->m_numberOfSamples );
}

void CHRTimer::m_ResetNumberOfSamples(unsigned int newNumberOfSamples)
{
	// Create a new vector of samples and pre-load it with the average of the old vector

	// Save old average
	float oldAverage = this->GetElapsedSecondsAverage( true );

	this->m_numberOfSamples = newNumberOfSamples;

	this->m_vecLastXSamples.clear();
	this->m_vecLastXSamples.reserve( this->m_numberOfSamples );

	for ( int index = 0; index != this->m_numberOfSamples; index++ )
	{
		this->m_vecLastXSamples.push_back(oldAverage);
	}
	// Start at the beginning of the sample vector
	this->m_nextSampleIndex = 0;
	
	return;
}

unsigned long long CHRTimer::GetFrequency(void)
{
	return this->m_frequency;
}


void CHRTimer::m_AppendLondDurationTimer(float deltaSeconds)
{
	this->m_dLongDurationTotalSeconds += deltaSeconds;
	//this->m_fLongDurationSeconds += deltaSeconds;
	//if ( this->m_fLongDurationSeconds >= 60.0f )
	//{
	//	float numMinutes = this->m_fLongDurationSeconds / 60.0f;
	//	this->m_fLongDurationSeconds -= ( numMinutes * 60.0f );
	//	this->m_fLongDurationMinutes += numMinutes;
	//}
	return;
}

void CHRTimer::UpdateLongDuration(void)
{
	// GetElapsedSeconds() updates m_elapsedTime;
	float deltaTime = this->GetElapsedSeconds();
	this->m_AppendLondDurationTimer( deltaTime );
	return;
}

//float CHRTimer::get_fLongDurationSeconds(void)
//{
//	return this->m_fLongDurationSeconds;
//}
//
//float CHRTimer::get_fLongDurationMinutes(void)
//{
//	return this->m_fLongDurationMinutes;
//}

void CHRTimer::getLongDurationValues(float &seconds, float &minutes )
{
	double dSeconds = this->m_dLongDurationTotalSeconds;
	double dMinutes = 0.0f;
	if ( this->m_dLongDurationTotalSeconds >= 60.0 )
	{
		dMinutes = dSeconds / 60.0;
		dSeconds -= ( dMinutes * 60.0f );
	}
	seconds = static_cast<float>(dSeconds);
	minutes = static_cast<float>(dMinutes);
	return;
}



float CHRTimer::get_fLondDurationTotalSeconds(void)
{
	return static_cast<float>( this->m_dLongDurationTotalSeconds );
}

double CHRTimer::get_dLondDurationTotalSeconds(void)
{
	return this->m_dLongDurationTotalSeconds;
}

void CHRTimer::ResetLongDuration(void)
{
	this->m_dLongDurationTotalSeconds = 0.0;
	return;
}




//#include "CHRTimer.h"
//#include "windows.h"	// For high freq
//
//// Written by Michael Feeney, Fanshawe College, 2010
//// mfeeney@fanshawec.on.ca
//// It may be distributed under the terms of the General Public License:
//// http://www.fsf.org/licenses/gpl.html
//// Use this code at your own risk. It is indented only as a learning aid.
////
//
////__int64 start_count;
////__int64 end_count;
////__int64 freq;
////
////// Get the frequency and save it, it shouldn't change
////QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
////QueryPerformanceCounter((LARGE_INTEGER*)&start_count);
////// do some stuff that takes up time
////QueryPerformanceCounter((LARGE_INTEGER*)&end_count);
//////find the time
////float time = (float)(end_count - start_count) / (float)freq;
//
//
//CHRTimer::CHRTimer()
//{
//	this->m_elapsedTime = 0;
//	this->m_startTime = 0;
//	this->m_stopTime = 0;
//
//	LARGE_INTEGER tempTime;
//	
//	QueryPerformanceFrequency( &tempTime );   // 8 MHz
//
//	this->m_frequency = static_cast<unsigned long long>( tempTime.QuadPart );
//}
//
//CHRTimer::~CHRTimer()
//{
//}
//
//void CHRTimer::Start(void)
//{
//	LARGE_INTEGER tempTime;
//	QueryPerformanceCounter( &tempTime ); 
//	this->m_startTime = static_cast<unsigned long long>( tempTime.QuadPart ) ;
//}
//
//void CHRTimer::Stop(void)
//{
//	LARGE_INTEGER tempTime;
//	QueryPerformanceCounter( &tempTime );
//	this->m_stopTime = static_cast<unsigned long long>( tempTime.QuadPart ) ;
//}
//
//void CHRTimer::Reset(void)
//{
//	this->m_elapsedTime = 0;
//	this->m_startTime = 0;
//	this->m_stopTime = 0;
//}
//
//float CHRTimer::GetElapsedSeconds(void)
//{
//	// Update current time
//	LARGE_INTEGER tempTime;
//	QueryPerformanceCounter( &tempTime );
//	this->m_stopTime = static_cast<unsigned long long>( tempTime.QuadPart ) ;
//
//	// Calculate elapsed time
//	this->m_elapsedTime = this->m_stopTime - this->m_startTime;
//	float elapsedTimeSec = static_cast<float>( this->m_elapsedTime ) / static_cast<float>(this->m_frequency );
//	return elapsedTimeSec;
//}
//
//unsigned long long CHRTimer::GetFrequency(void)
//{
//	return this->m_frequency;
//}
//
