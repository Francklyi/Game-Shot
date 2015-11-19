#ifndef _GLU_H_
#define _GLU_H_

#include <stdio.h>
#include <jni.h>
#include <math.h>
#include <GLES/gl.h>
#include "Vector3.h"

#define GLdouble float
class FVector3
{
public:
	union
	{
		struct
		{
			GLdouble X, Y, Z;
		};
		struct
		{
			GLdouble x, y, z;
		};
		GLdouble v[3];
	};
public:
	FVector3(){}
	FVector3(GLdouble x1,GLdouble y1,GLdouble z1):x(x1),y(y1),z(z1)
	{}
	FVector3(const FVector3& InV);

	FVector3 operator^(const FVector3& V) const;
	FVector3& Normalize();

};

inline
	FVector3::FVector3(const FVector3& InV)
{
	x = InV.x;
	y = InV.y;
	z = InV.z;
}

inline GLdouble appInvSqrt(GLdouble f) { return 1.f/sqrt(f); }

inline FVector3&
	FVector3::Normalize()
{
	GLdouble SquareSum = X*X + Y*Y + Z*Z;
	if( SquareSum < 0.00000001 )
		return *this;
	GLdouble Scale = appInvSqrt(SquareSum);
	X *= Scale;
	Y *= Scale;
	Z *= Scale;
	return *this;
}

inline FVector3
	FVector3::operator^( const FVector3& V ) const
{
	return FVector3(Y * V.Z - Z * V.Y, Z * V.X - X * V.Z, X * V.Y - Y * V.X );
}


static void suLookAt(GLdouble eyeX,GLdouble eyeY,GLdouble eyeZ,GLdouble centerX,GLdouble centerY,GLdouble centerZ,GLdouble upX,GLdouble upY,GLdouble upZ)
{
	GLdouble directMat[16];

	for (int i = 0 ;i<16;i++)
	{
		directMat[i] = 0;
	}

	directMat[15]= 1;
	FVector3 fvDirect(eyeX-centerX,eyeY-centerY,eyeZ-centerZ);
	//(centerX-eyeX,centerY-eyeY,centerZ-eyeZ);
	fvDirect.Normalize();
	FVector3 fvUpD(upX,upY,upZ);
	fvUpD.Normalize();
	FVector3 fvC = fvDirect^fvUpD;
	fvC.Normalize();

	FVector3 fvUp = fvC^fvDirect;
	fvUp.Normalize();

	fvDirect.x = -fvDirect.x;
	fvDirect.y = -fvDirect.y;
	fvDirect.z = -fvDirect.z;

	directMat[0] = fvC.x;
	directMat[4] = fvC.y;
	directMat[8] = fvC.z;
	directMat[1] = fvUp.x;
	directMat[5] = fvUp.y;
	directMat[9] = fvUp.z;
	directMat[2] = fvDirect.x;
	directMat[6] = fvDirect.y;
	directMat[10] = fvDirect.z;

	glLoadMatrixf(directMat);

	glTranslatef(-eyeX,-eyeY,-eyeZ);
}

static void lyuLookAt(float eyeX, float eyeY, float eyeZ,
		              float centerX, float centerY, float centerZ,
		              float upX, float upY, float upZ)
{
	float directMat[16];

	for (int i = 0 ;i<16;i++)
	{
		directMat[i] = 0;
	}

	directMat[15]= 1;
	Vector3 fvDirect(centerX-eyeX,centerY-eyeY,centerZ-eyeZ);
	fvDirect.normalize();
	Vector3 fvUpD(upX,upY,upZ);
	fvUpD.normalize();
	Vector3 fvC = fvDirect.crossProduct(fvUpD);
	fvC.normalize();

	Vector3 fvUp = fvC.crossProduct(fvDirect);
	fvUp.normalize();

	fvDirect.x = -fvDirect.x;
	fvDirect.y = -fvDirect.y;
	fvDirect.z = -fvDirect.z;

	directMat[0] = fvC.x;
	directMat[4] = fvC.y;
	directMat[8] = fvC.z;
	directMat[1] = fvUp.x;
	directMat[5] = fvUp.y;
	directMat[9] = fvUp.z;
	directMat[2] = fvDirect.x;
	directMat[6] = fvDirect.y;
	directMat[10] = fvDirect.z;

	glLoadMatrixf(directMat);

	glTranslatef(-eyeX,-eyeY,-eyeZ);
}




static void gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
	              GLfloat centerx, GLfloat centery, GLfloat centerz,
	              GLfloat upx, GLfloat upy, GLfloat upz)
{
    GLfloat m[16];
    GLfloat x[3], y[3], z[3];
    GLfloat mag;

    /* Make rotation matrix */

    /* Z vector */
    z[0] = eyex - centerx;
    z[1] = eyey - centery;
    z[2] = eyez - centerz;
    mag = (float)sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) {			/* mpichler, 19950515 */
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }

    /* Y vector */
    y[0] = upx;
    y[1] = upy;
    y[2] = upz;

    /* X vector = Y cross Z */
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];

    /* Recompute Y = Z cross X */
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];

    /* mpichler, 19950515 */
    /* cross product gives area of parallelogram, which is < 1.0 for
     * non-perpendicular unit-length vectors; so normalize x, y here
     */

    mag = (float)sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }

    mag = (float)sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }

#define M(row,col)  m[col*4+row]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0;
    M(3, 0) = 0.0;
    M(3, 1) = 0.0;
    M(3, 2) = 0.0;
    M(3, 3) = 1.0;
#undef M
    {
        int a;
        GLfixed fixedM[16];
        for (a = 0; a < 16; ++a)
            fixedM[a] = (GLfixed)(m[a] * 65536);
        glMultMatrixx(fixedM);
    }

    /* Translate Eye to Origin */
    glTranslatex((GLfixed)(-eyex * 65536),
                 (GLfixed)(-eyey * 65536),
                 (GLfixed)(-eyez * 65536));
}






	//matrix will receive the calculated perspective matrix.
	//You would have to upload to your shader
	// or use glLoadMatrixf if you aren't using shaders.

static	void glhFrustumf2(float *matrix, float left, float right, float bottom, float top,
		float znear, float zfar)
	{
		float temp, temp2, temp3, temp4;
		temp = 2.0 * znear;
		temp2 = right - left;
		temp3 = top - bottom;
		temp4 = zfar - znear;
		matrix[0] = temp / temp2;
		matrix[1] = 0.0;
		matrix[2] = 0.0;
		matrix[3] = 0.0;
		matrix[4] = 0.0;
		matrix[5] = temp / temp3;
		matrix[6] = 0.0;
		matrix[7] = 0.0;
		matrix[8] = (right + left) / temp2;
		matrix[9] = (top + bottom) / temp3;
		matrix[10] = (-zfar - znear) / temp4;
		matrix[11] = -1.0;
		matrix[12] = 0.0;
		matrix[13] = 0.0;
		matrix[14] = (-temp * zfar) / temp4;
		matrix[15] = 0.0;
	}

static	void glhPerspectivef2(float fovyInDegrees, float aspectRatio,
		float znear, float zfar)
	{
		float  matrix[16],ymax, xmax;
		const float pi=3.1415926f;
		ymax = znear * tanf(fovyInDegrees * (pi) / 360.0f);
		//ymin = -ymax;
		//xmin = -ymax * aspectRatio;
		xmax = ymax * aspectRatio;
		glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
		glMultMatrixf(matrix);
	}

static	void xgluPerspective( float fovy, float aspect, float near_clip, float far_clip )
		{
			const double PI = 3.1415926f;
			double TWOPI_OVER_360 = 2.0 * PI / 360.0f;
			float half_height = near_clip * (float)tan( fovy * 0.5f * TWOPI_OVER_360 );
			float half_width = half_height * aspect;

//	#ifdef WIN32
//			glFrustum( -half_width, half_width, -half_height, half_height, near_clip, far_clip );
//	#elif defined(CK_ANDROID)
			glFrustumf( -half_width, half_width, -half_height, half_height, near_clip, far_clip );
//	#endif
}

#endif //_GLU_H_
