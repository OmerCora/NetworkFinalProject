//
// Written by Michael Feeney, Fanshawe College, (c) 2008-2014, mfeeney(at)fanshawec.ca
// For the 'Game Development - Advanced Programming' curriculum:  www.fanshawec.ca/en/GDP1
// It may be distributed under the terms of the MIT License: http://opensource.org/licenses/MIT
// Use this code at your own risk. It is indented only as a learning aid.
//                                                                       
#ifndef _CVector3f_HG_
#define _CVector3f_HG_

// Pick ONE of these
//#define CVECTOR3F_USE_ABSOLUTE_COMPARISON_FASTEST
#define CVECTOR3F_USE_APPROXIMATE_RELATIVE_COMPARISON_SLOWER
//#define CVECTOR3F_USE_RELATIVE_COMPARISON_SLOWEST

class CVector3f
{
public:
	CVector3f();										// Updated: October 2014
	CVector3f( float all );
	CVector3f( float x, float y, float z );
	CVector3f( const CVector3f &v );	// Copy constructor
	~CVector3f() {};
	float x; float y; float z;

	// These are used on vector types
	CVector3f& operator=(const CVector3f &v);							
	const CVector3f operator+(const CVector3f &v) const;			// Fixed: October 2014
	const CVector3f operator-(const CVector3f &v) const;			// Fixed: October 2014
	CVector3f& operator+=(const CVector3f &v);							
	CVector3f& operator-=(const CVector3f &v);							 
	CVector3f& operator*=(float scalar);	// Times a scalar			
	CVector3f& operator/=(float scalar);	// Divided by a scalar		
	CVector3f operator-();	// Negative on all directions

	void SetComparisonEpsilon(float comparisonEpsilon);
	float m_comparisonEpsilon;
	bool operator==(const CVector3f &other) const;		
	bool operator!=(const CVector3f &other) const;		
	
	// Helpful functions
	void Normalize(void);
	float Magnitude(void);
	float MagnitudeSquared(void);
	void Reverse(void);
	
	// Helpful static functions...
	static void Normalize( float &x, float &y, float &z);
	static void Normalize( CVector3f &v );
	static float Magnitude( const CVector3f &v );
	static float MagnitudeSquared( const CVector3f &v );
	static float Distance( const CVector3f &v1, const CVector3f &v2);
	static float DistanceSquared( const CVector3f &v1, const CVector3f &v2);

	static void Subtract( const float &x0, const float &y0, const float &z0, 
                          const float &x1, const float &y1, const float &z1,
                          float &xSub, float &ySub, float &zSub);
	static void Subtract( const CVector3f &v1, const CVector3f &v2, CVector3f &vSub );
	static CVector3f Subtract( const CVector3f &v1, const CVector3f &v2 );
	static CVector3f ScalarMultiply( const CVector3f &v, float s);
	static CVector3f ScalarDivide( const CVector3f &v, float s);

	static void CrossProduct( const float &x0, const float &y0, const float &z0, 
	                          const float &x1, const float &y1, const float &z1,
	                          float &xCross, float &yCross, float &zCross);
	static CVector3f CrossProduct( const float &x0, const float &y0, const float &z0, 
		                           const float &x1, const float &y1, const float &z1 );
	static void CrossProduct( const CVector3f &v1, const CVector3f &v2, CVector3f &vSub );
	static CVector3f CrossProduct( const CVector3f &v1, const CVector3f &v2 );

	static float DotProduct( const float &x1, const float &y1, const float &z1, 
	                         const float &x2, const float &y2, const float &z2 );
	static float DotProduct( const CVector3f &v1, const CVector3f &v2 );

	// Used by the comparison functions
	bool zzz_m_internalFPComparison( const float &num1, const float &num2 ) const;
	float zzz_m_internalFPMax( const float &num1, const float &num2, const float &num3 ) const;
};

#endif