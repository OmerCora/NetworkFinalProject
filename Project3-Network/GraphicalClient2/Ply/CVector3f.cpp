//
// Written by Michael Feeney, Fanshawe College, (c) 2008-2014, mfeeney(at)fanshawec.ca
// For the 'Game Development - Advanced Programming' curriculum:  www.fanshawec.ca/en/GDP1
// It may be distributed under the terms of the MIT License: http://opensource.org/licenses/MIT
// Use this code at your own risk. It is indented only as a learning aid.
//
#include "CVector3f.h"

#include <math.h>
#include <float.h>		// FLT_EPSILON



CVector3f::CVector3f() : x(0.0f), y(0.0f), z(0.0f) 
{
	// Christer Ericson suggests to use the sqrt() of this value in "Real-Time Collision Detection"
	this->m_comparisonEpsilon = FLT_EPSILON;
	return;
};

CVector3f::CVector3f( float all )
{
	this->x = this->y = this->z = all;
	return;
}

CVector3f::CVector3f( float x, float y, float z )
{
	this->x = x; this->y = y; this->z = z;
	this->m_comparisonEpsilon = FLT_EPSILON;
	return;
}

CVector3f::CVector3f( const CVector3f &v )
{
	this->x = v.x; this->y = v.y; this->z = v.z;
	return;
}

// Fully defined in header...
//CVector3f::~CVector3f() {};

CVector3f& CVector3f::operator=(const CVector3f &v)
{
	// Check for self-assignment *IMPORTANT*
	if ( this == &v )	return *this;
	// Else...copy values
	this->x = v.x; this->y = v.y; this->z = v.z;
	return *this;
}

const CVector3f CVector3f::operator+(const CVector3f &v) const
{
	CVector3f result = *this;
	result += v;
	return result;
}

const CVector3f CVector3f::operator-(const CVector3f &v) const
{
	CVector3f result = *this;
	result -= v;
	return result;
}

CVector3f& CVector3f::operator+=(const CVector3f &v)
{
	this->x += v.x; this->y += v.y; this->z += v.z;
	return *this;
}

CVector3f& CVector3f::operator-=(const CVector3f &v)
{
	this->x -= v.x; this->y -= v.y; this->z -= v.z;
	return *this;
}

// Times a scalar
CVector3f& CVector3f::operator*=(float scalar)
{
	this->x *= scalar; this->y *= scalar; this->z *= scalar;
	return *this;
}

// Divided by a scalar
CVector3f& CVector3f::operator/=(float scalar)
{
	this->x /= scalar; this->y /= scalar; this->z /= scalar;
	return *this;
}
	
CVector3f CVector3f::operator-()	// Negative on all directions
{
	return CVector3f( -x, -y, -z );
}

void CVector3f::SetComparisonEpsilon(float comparisonEpsilon)
{
	// Christer Ericson suggests to use the sqrt() of this value in "Real-Time Collision Detection"
	this->m_comparisonEpsilon = comparisonEpsilon;
	return;
}

float CVector3f::zzz_m_internalFPMax( const float &num1, const float &num2, const float &num3 ) const
{
	float curMax = num1;	// Assume num1 is max
	if ( num2 > curMax )	{ curMax = num2; }
	if ( num3 > curMax )	{ curMax = num3; }
	return curMax;
}


// Used by the comparison functions
bool CVector3f::zzz_m_internalFPComparison( const float &num1, const float &num2 ) const
{
#ifdef CVECTOR3F_USE_ABSOLUTE_COMPARISON_FASTEST
	if ( abs(num1 - num2 ) <= this->m_comparisonEpsilon )
	{
		return true;
	}
#endif

#ifdef CVECTOR3F_USE_APPROXIMATE_RELATIVE_COMPARISON_SLOWER
	// From Christer Ericson's "Real-Time Collision Detection" book
	if ( fabs(num1 - num2) <= ( this->m_comparisonEpsilon * ( fabs(num1) + fabs(num2) + 1.0f ) ) )
	{
		return true;
	}
	return false;
#endif

#ifdef CVECTOR3F_USE_RELATIVE_COMPARISON_SLOWEST
	// From Christer Ericson's "Real-Time Collision Detection" book
	if ( abs(num1 - num2) <= ( this->m_comparisonEpsilon * this->zzz_m_internalFPMax(num1, num2, 1.0f) ) )
	{
		return true;
	}
	return false;

#endif
}

bool CVector3f::operator==(const CVector3f &other) const 
{
	if ( this->zzz_m_internalFPComparison( this->x, other.x ) &&
		 this->zzz_m_internalFPComparison( this->y, other.y ) && 
		 this->zzz_m_internalFPComparison( this->z, other.z ) )
		return true;
	return false;
}

bool CVector3f::operator!=(const CVector3f &other) const
{
	return !(*this == other);
}

/*static*/ // Subtracts one vector from another and returns in Sub (by reference)
void CVector3f::Subtract(const float &x0, const float &y0, const float &z0, 
                       const float &x1, const float &y1, const float &z1,
                       float &xSub, float &ySub, float &zSub)
{
	xSub = x0 - x1; ySub = y0 - y1; zSub = z0 - z1;
	return;
}

/*static*/ // Subtracts one vector from another and returns in Sub (by reference)
void CVector3f::Subtract( const CVector3f &v1, const CVector3f &v2, CVector3f &vSub )
{
	CVector3f::Subtract( v1.x, v1.y, v1.z, 
		               v2.x, v2.y, v2.z, 
					   vSub.x, vSub.y, vSub.z );
	return;
}

/*static*/ // Subtracts returns one vector from another 
CVector3f CVector3f::Subtract( const CVector3f &v1, const CVector3f &v2 )
{
	CVector3f vTemp;
	CVector3f::Subtract( v1, v2, vTemp );
	return vTemp;
}

/*static*/
CVector3f CVector3f::ScalarMultiply( const CVector3f &v, float s)
{
	CVector3f vTemp(v);
	vTemp *= s;
	return vTemp;
}

/*static*/
CVector3f CVector3f::ScalarDivide( const CVector3f &v, float s)
{
	CVector3f vTemp(v);
	vTemp /= s;
	return vTemp;
}

/*static*/
// Takes cross of vec0 (x0, y0, z0) and vec1 (x1, y1, z1) and 
//	returns cross (xCross, yCross, zCross)
void CVector3f::CrossProduct( const float &x0, const float &y0, const float &z0, 
                            const float &x1, const float &y1, const float &z1,
                            float &xCross, float &yCross, float &zCross)
{
    /* find cross-product between these vectors */
    xCross = y0 * z1 - z0 * y1;
    yCross = z0 * x1 - x0 * z1;
    zCross = x0 * y1 - y0 * x1;
	return;
}

/*static*/
CVector3f CVector3f::CrossProduct( const float &x0, const float &y0, const float &z0, 
                            const float &x1, const float &y1, const float &z1 )
                            //float &xCross, float &yCross, float &zCross)
{
    /* find cross-product between these vectors */
	CVector3f retVal;
    retVal.x = y0 * z1 - z0 * y1;
    retVal.y = z0 * x1 - x0 * z1;
    retVal.z = x0 * y1 - y0 * x1;
	return retVal;
}

/*static*/
void CVector3f::CrossProduct( const CVector3f &v1, const CVector3f &v2, CVector3f &vSub )
{
	CVector3f::CrossProduct( v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, vSub.x, vSub.y, vSub.z );
	return;
}

/*static*/
CVector3f CVector3f::CrossProduct( const CVector3f &v1, const CVector3f &v2 )
{
	CVector3f vSub;
	CVector3f::CrossProduct( v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, vSub.x, vSub.y, vSub.z );
	return vSub;
}

/*static*/
float CVector3f::DotProduct( const float &x1, const float &y1, const float &z1, 
                           const float &x2, const float &y2, const float &z2 )
{
	float dot = x1 * x2 + y1 * y2 + z1 * z2;
	return dot;
}

/*static*/
float CVector3f::DotProduct( const CVector3f &v1, const CVector3f &v2 )
{
	return DotProduct( v1.x, v1.y, v1.z, v2.x, v2.y, v2.z );
}

/*static*/	
// Set vector to unit size (length of 1.0)
void CVector3f::Normalize( float &x, float &y, float &z)
{
	// Convert to double so there is no loss...
	double x2 = static_cast<double>(x) * static_cast<double>(x);
	double y2 = static_cast<double>(y) * static_cast<double>(y);
	double z2 = static_cast<double>(z) * static_cast<double>(z);
	double length = x2 + y2 + z2;
	if ( length == 0 )
	{
		x = y = z = 0.0f;
	} 
	else
	{	// Multiply by reciprical as multiplies are faster than divides (usually)
		double reciprical = 1 / sqrt( length );
		x = static_cast<float>( static_cast<double>(x) * reciprical   );
		y = static_cast<float>( static_cast<double>(y) * reciprical  );
		z = static_cast<float>( static_cast<double>(z) * reciprical  );
	}
	return;
}

/*static*/	
void CVector3f::Normalize( CVector3f &v )
{
	CVector3f::Normalize( v.x, v.y, v.z );
	return;
}

void CVector3f::Normalize(void)
{
	CVector3f::Normalize( *this );
	return;
}

/*static*/
float CVector3f::Magnitude( const CVector3f &v )
{
	return static_cast<float>(sqrt( v.x*v.x + v.y*v.y + v.z*v.z ));
}

float CVector3f::Magnitude( void )
{
	return CVector3f::Magnitude( *this );
}
/*static*/
// Same as magnatude, but without the square root...
float CVector3f::MagnitudeSquared( const CVector3f &v )
{
	return static_cast<float>( v.x*v.x + v.y*v.y + v.z*v.z );
}

float CVector3f::MagnitudeSquared( void )
{
	return CVector3f::Magnitude( *this );
}

/*static*/
float CVector3f::Distance( const CVector3f &v1, const CVector3f &v2)
{
	double deltaX = static_cast<double>( v1.x - v2.x );
	double deltaY = static_cast<double>( v1.y - v2.y );
	double deltaZ = static_cast<double>( v1.z - v2.z );
	return static_cast<float>(sqrt( deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ ));
}

/*static*/
// Same as distance, but without the square root...
float CVector3f::DistanceSquared( const CVector3f &v1, const CVector3f &v2)
{
	double deltaX = static_cast<double>( v1.x - v2.x );
	double deltaY = static_cast<double>( v1.y - v2.y );
	double deltaZ = static_cast<double>( v1.z - v2.z );
	return static_cast<float>( deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ );
}

void CVector3f::Reverse(void)
{
	this->x = -(this->x);
	this->y = -(this->y);
	this->z = -(this->z);
	return;
}