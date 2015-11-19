
#include "matrix.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "GLSL.h"
#include "TrigFuns.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Matrix& getMatrix(){
	static Matrix matrix1;
	return matrix1;
}


//float *Matrix::sTemp=(float *)malloc(32*sizeof(float));


/**
 * Multiply two 4x4 matrices together and store the result in a third 4x4
 * matrix. In matrix notation: result = lhs x rhs. Due to the way
 * matrix multiplication works, the result matrix will have the same
 * effect as first multiplying by the rhs matrix, then multiplying by
 * the lhs matrix. This is the opposite of what you might expect.
 *
 * The same float array may be passed for result, lhs, and/or rhs. However,
 * the result element values are undefined if the result elements overlap
 * either the lhs or rhs elements.
 *
 * @param result The float array that holds the result.
 * @param resultOffset The offset into the result array where the result is
 *        stored.
 * @param lhs The float array that holds the left-hand-side matrix.
 * @param lhsOffset The offset into the lhs array where the lhs is stored
 * @param rhs The float array that holds the right-hand-side matrix.
 * @param rhsOffset The offset into the rhs array where the rhs is stored.
 *
 * @throws IllegalArgumentException if result, lhs, or rhs are null, or if
 * resultOffset + 16 > result.length or lhsOffset + 16 > lhs.length or
 * rhsOffset + 16 > rhs.length.
 */

void Matrix::set(float *m,float *src)
{
	m[0] = src[0];  m[1] = src[1];  m[2] = src[2];  m[3] = src[3];
	m[4] = src[4];  m[5] = src[5];  m[6] = src[6];  m[7] = src[7];
	m[8] = src[8];  m[9] = src[9];  m[10]= src[10]; m[11]= src[11];
	m[12]= src[12]; m[13]= src[13]; m[14]= src[14]; m[15]= src[15];
}

void Matrix::MM(float *Rm, float *n, float *m)
{
	sTemp[0]=m[0]*n[0]  + m[1]*n[4]  + m[2]*n[8]  + m[3]*n[12];
	sTemp[1]=m[0]*n[1]  + m[1]*n[5]  + m[2]*n[9]  + m[3]*n[13];
	sTemp[2]=m[0]*n[2]  + m[1]*n[6]  + m[2]*n[10]  + m[3]*n[14];
	sTemp[3]=m[0]*n[3]  + m[1]*n[7]  + m[2]*n[11]  + m[3]*n[15];
	sTemp[4]=m[4]*n[0]  + m[5]*n[4]  + m[6]*n[8]  + m[7]*n[12];
	sTemp[5]=m[4]*n[1]  + m[5]*n[5]  + m[6]*n[9]  + m[7]*n[13];
	sTemp[6]=m[4]*n[2]  + m[5]*n[6]  + m[6]*n[10]  + m[7]*n[14];
	sTemp[7]=m[4]*n[3]  + m[5]*n[7]  + m[6]*n[11]  + m[7]*n[15];
	sTemp[8]=m[8]*n[0]  + m[9]*n[4]  + m[10]*n[8] + m[11]*n[12];
	sTemp[9]=m[8]*n[1]  + m[9]*n[5]  + m[10]*n[9] + m[11]*n[13];
	sTemp[10]=m[8]*n[2]  + m[9]*n[6]  + m[10]*n[10] + m[11]*n[14];
	sTemp[11]=m[8]*n[3]  + m[9]*n[7]  + m[10]*n[11] + m[11]*n[15];
	sTemp[12]=m[12]*n[0] + m[13]*n[4] + m[14]*n[8] + m[15]*n[12];
	sTemp[13]=m[12]*n[1] + m[13]*n[5] + m[14]*n[9] + m[15]*n[13];
	sTemp[14]=m[12]*n[2] + m[13]*n[6] + m[14]*n[10] + m[15]*n[14];
	sTemp[15]=m[12]*n[3] + m[13]*n[7] + m[14]*n[11] + m[15]*n[15];

	Rm[0] = sTemp[0];  Rm[1] = sTemp[1];  Rm[2] = sTemp[2];  Rm[3] = sTemp[3];
	Rm[4] = sTemp[4];  Rm[5] = sTemp[5];  Rm[6] = sTemp[6];  Rm[7] = sTemp[7];
	Rm[8] = sTemp[8];  Rm[9] = sTemp[9];  Rm[10]= sTemp[10]; Rm[11]= sTemp[11];
	Rm[12]= sTemp[12]; Rm[13]= sTemp[13]; Rm[14]= sTemp[14]; Rm[15]= sTemp[15];
}

void Matrix::multiplyMM(float *result, int resultOffset,
		float *lhs, int lhsOffset, float *rhs, int rhsOffset)
{
	MM( &result[resultOffset], &lhs[lhsOffset], &rhs[rhsOffset]);

	return;
	/*
	int length=16;
	//static float *temp1[16];//=(float *)malloc(length*sizeof(float));
	int row, //��
	col;//��
	for (int i = 0; i < length; i++) {
		row=i/4;
		col=i%4;
		sTemp[i]=   lhs[lhsOffset+col+0]  * rhs[rhsOffset+row*4+0]
		                                        + lhs[lhsOffset+col+4]  * rhs[rhsOffset+row*4+1]
		                                                                      + lhs[lhsOffset+col+8]  * rhs[rhsOffset+row*4+2]
		                                                                                                    + lhs[lhsOffset+col+12] * rhs[rhsOffset+row*4+3];
	}
	for(int i=0; i< length; i++)
	{
		result[i+resultOffset]=sTemp[i];
	}
	*/
	//free(temp1);
	//temp1=0;

}

/**
 * Multiply a 4 element vector by a 4x4 matrix and store the result in a 4
 * element column vector. In matrix notation: result = lhs x rhs
 *
 * The same float array may be passed for resultVec, lhsMat, and/or rhsVec.
 * However, the resultVec element values are undefined if the resultVec
 * elements overlap either the lhsMat or rhsVec elements.
 *
 * @param resultVec The float array that holds the result vector.
 * @param resultVecOffset The offset into the result array where the result
 *        vector is stored.
 * @param lhsMat The float array that holds the left-hand-side matrix.
 * @param lhsMatOffset The offset into the lhs array where the lhs is stored
 * @param rhsVec The float array that holds the right-hand-side vector.
 * @param rhsVecOffset The offset into the rhs vector where the rhs vector
 *        is stored.
 *
 * @throws IllegalArgumentException if resultVec, lhsMat,
 * or rhsVec are null, or if resultVecOffset + 4 > resultVec.length
 * or lhsMatOffset + 16 > lhsMat.length or
 * rhsVecOffset + 4 > rhsVec.length.
 */
void Matrix::multiplyMV(float *resultVec,
		int resultVecOffset, float *lhsMat, int lhsMatOffset,
		float *rhsVec, int rhsVecOffset)
{
	//float *temp1=(float *)malloc(4*sizeof(float));
	sTemp[0]=rhsVec[rhsVecOffset+0]*lhsMat[lhsMatOffset+0]+ rhsVec[rhsVecOffset+1]*lhsMat[lhsMatOffset+4]+rhsVec[rhsVecOffset+2]*lhsMat[lhsMatOffset+8]+rhsVec[rhsVecOffset+3]*lhsMat[lhsMatOffset+12];
	sTemp[1]=rhsVec[rhsVecOffset+0]*lhsMat[lhsMatOffset+1]+ rhsVec[rhsVecOffset+1]*lhsMat[lhsMatOffset+5]+rhsVec[rhsVecOffset+2]*lhsMat[lhsMatOffset+9]+rhsVec[rhsVecOffset+3]*lhsMat[lhsMatOffset+13];
	sTemp[2]=rhsVec[rhsVecOffset+0]*lhsMat[lhsMatOffset+2]+ rhsVec[rhsVecOffset+1]*lhsMat[lhsMatOffset+6]+rhsVec[rhsVecOffset+2]*lhsMat[lhsMatOffset+10]+rhsVec[rhsVecOffset+3]*lhsMat[lhsMatOffset+14];
	sTemp[3]=rhsVec[rhsVecOffset+0]*lhsMat[lhsMatOffset+3]+ rhsVec[rhsVecOffset+1]*lhsMat[lhsMatOffset+7]+rhsVec[rhsVecOffset+2]*lhsMat[lhsMatOffset+11]+rhsVec[rhsVecOffset+3]*lhsMat[lhsMatOffset+15];

	resultVec[resultVecOffset+0]=sTemp[0];
	resultVec[resultVecOffset+1]=sTemp[1];
	resultVec[resultVecOffset+2]=sTemp[2];
	resultVec[resultVecOffset+3]=sTemp[3];

	//free(temp1);
	//temp1=0;
}
/**
 * Transposes a 4 x 4 matrix.
 *
 * @param mTrans the array that holds the output inverted matrix
 * @param mTransOffset an offset into mInv where the inverted matrix is
 *        stored.
 * @param m the input array
 * @param mOffset an offset into m where the matrix is stored.
 */
void Matrix::transposeM(float *mTrans, int mTransOffset, float *m,
		int mOffset) {
	for (int i = 0; i < 4; i++) {
		int mBase = i * 4 + mOffset;
		mTrans[i + mTransOffset] = m[mBase];
		mTrans[i + 4 + mTransOffset] = m[mBase + 1];
		mTrans[i + 8 + mTransOffset] = m[mBase + 2];
		mTrans[i + 12 + mTransOffset] = m[mBase + 3];
	}
}

/**
 * Inverts a 4 x 4 matrix.
 *
 * @param mInv the array that holds the output inverted matrix
 * @param mInvOffset an offset into mInv where the inverted matrix is
 *        stored.
 * @param m the input array
 * @param mOffset an offset into m where the matrix is stored.
 * @return true if the matrix could be inverted, false if it could not.
 */
bool Matrix::invertM(float *mInv, int mInvOffset, float *m,
		int mOffset) {
	// Invert a 4 x 4 matrix using Cramer's Rule

	// transpose matrix
	float src0  = m[mOffset +  0];
	float src4  = m[mOffset +  1];
	float src8  = m[mOffset +  2];
	float src12 = m[mOffset +  3];

	float src1  = m[mOffset +  4];
	float src5  = m[mOffset +  5];
	float src9  = m[mOffset +  6];
	float src13 = m[mOffset +  7];

	float src2  = m[mOffset +  8];
	float src6  = m[mOffset +  9];
	float src10 = m[mOffset + 10];
	float src14 = m[mOffset + 11];

	float src3  = m[mOffset + 12];
	float src7  = m[mOffset + 13];
	float src11 = m[mOffset + 14];
	float src15 = m[mOffset + 15];

	// calculate pairs for first 8 elements (cofactors)
	float atmp0  = src10 * src15;
	float atmp1  = src11 * src14;
	float atmp2  = src9  * src15;
	float atmp3  = src11 * src13;
	float atmp4  = src9  * src14;
	float atmp5  = src10 * src13;
	float atmp6  = src8  * src15;
	float atmp7  = src11 * src12;
	float atmp8  = src8  * src14;
	float atmp9  = src10 * src12;
	float atmp10 = src8  * src13;
	float atmp11 = src9  * src12;

	// calculate first 8 elements (cofactors)
	float dst0  = (atmp0 * src5 + atmp3 * src6 + atmp4  * src7)
                        		  - (atmp1 * src5 + atmp2 * src6 + atmp5  * src7);
	float dst1  = (atmp1 * src4 + atmp6 * src6 + atmp9  * src7)
                        		  - (atmp0 * src4 + atmp7 * src6 + atmp8  * src7);
	float dst2  = (atmp2 * src4 + atmp7 * src5 + atmp10 * src7)
                        		  - (atmp3 * src4 + atmp6 * src5 + atmp11 * src7);
	float dst3  = (atmp5 * src4 + atmp8 * src5 + atmp11 * src6)
                        		  - (atmp4 * src4 + atmp9 * src5 + atmp10 * src6);
	float dst4  = (atmp1 * src1 + atmp2 * src2 + atmp5  * src3)
                        		  - (atmp0 * src1 + atmp3 * src2 + atmp4  * src3);
	float dst5  = (atmp0 * src0 + atmp7 * src2 + atmp8  * src3)
                        		  - (atmp1 * src0 + atmp6 * src2 + atmp9  * src3);
	float dst6  = (atmp3 * src0 + atmp6 * src1 + atmp11 * src3)
                        		  - (atmp2 * src0 + atmp7 * src1 + atmp10 * src3);
	float dst7  = (atmp4 * src0 + atmp9 * src1 + atmp10 * src2)
                        		  - (atmp5 * src0 + atmp8 * src1 + atmp11 * src2);

	// calculate pairs for second 8 elements (cofactors)
	float btmp0  = src2 * src7;
	float btmp1  = src3 * src6;
	float btmp2  = src1 * src7;
	float btmp3  = src3 * src5;
	float btmp4  = src1 * src6;
	float btmp5  = src2 * src5;
	float btmp6  = src0 * src7;
	float btmp7  = src3 * src4;
	float btmp8  = src0 * src6;
	float btmp9  = src2 * src4;
	float btmp10 = src0 * src5;
	float btmp11 = src1 * src4;

	// calculate second 8 elements (cofactors)
	float dst8  = (btmp0  * src13 + btmp3  * src14 + btmp4  * src15)
                        		  - (btmp1  * src13 + btmp2  * src14 + btmp5  * src15);
	float dst9  = (btmp1  * src12 + btmp6  * src14 + btmp9  * src15)
                        		  - (btmp0  * src12 + btmp7  * src14 + btmp8  * src15);
	float dst10 = (btmp2  * src12 + btmp7  * src13 + btmp10 * src15)
                        		  - (btmp3  * src12 + btmp6  * src13 + btmp11 * src15);
	float dst11 = (btmp5  * src12 + btmp8  * src13 + btmp11 * src14)
                        		  - (btmp4  * src12 + btmp9  * src13 + btmp10 * src14);
	float dst12 = (btmp2  * src10 + btmp5  * src11 + btmp1  * src9 )
                        		  - (btmp4  * src11 + btmp0  * src9  + btmp3  * src10);
	float dst13 = (btmp8  * src11 + btmp0  * src8  + btmp7  * src10)
                        		  - (btmp6  * src10 + btmp9  * src11 + btmp1  * src8 );
	float dst14 = (btmp6  * src9  + btmp11 * src11 + btmp3  * src8 )
                        		  - (btmp10 * src11 + btmp2  * src8  + btmp7  * src9 );
	float dst15 = (btmp10 * src10 + btmp4  * src8  + btmp9  * src9 )
                        		  - (btmp8  * src9  + btmp11 * src10 + btmp5  * src8 );

	// calculate determinant
	float det =
			src0 * dst0 + src1 * dst1 + src2 * dst2 + src3 * dst3;

	if (det == 0.0f) {
		return false;
	}

	// calculate matrix inverse
	float invdet = 1.0f / det;
	mInv[     mInvOffset] = dst0  * invdet;
	mInv[ 1 + mInvOffset] = dst1  * invdet;
	mInv[ 2 + mInvOffset] = dst2  * invdet;
	mInv[ 3 + mInvOffset] = dst3  * invdet;

	mInv[ 4 + mInvOffset] = dst4  * invdet;
	mInv[ 5 + mInvOffset] = dst5  * invdet;
	mInv[ 6 + mInvOffset] = dst6  * invdet;
	mInv[ 7 + mInvOffset] = dst7  * invdet;

	mInv[ 8 + mInvOffset] = dst8  * invdet;
	mInv[ 9 + mInvOffset] = dst9  * invdet;
	mInv[10 + mInvOffset] = dst10 * invdet;
	mInv[11 + mInvOffset] = dst11 * invdet;

	mInv[12 + mInvOffset] = dst12 * invdet;
	mInv[13 + mInvOffset] = dst13 * invdet;
	mInv[14 + mInvOffset] = dst14 * invdet;
	mInv[15 + mInvOffset] = dst15 * invdet;

	return true;
}

void Matrix::transposeM3(float *m)
{
	float temp=0;
	temp=m[1];
	m[1]=m[3];
	m[3]=temp;

	temp=m[2];
	m[2]=m[6];
	m[6]=temp;

	temp=m[5];
	m[5]=m[7];
	m[7]=temp;
	//std::swap(m[1], m[3]);
	//std::swap(m[2], m[6]);
	//std::swap(m[5], m[7]);
}

bool Matrix::invertM3(float *m)
{
	float determinant, invDeterminant;
	float tmp[9];

	tmp[0] = m[4] * m[8] - m[5] * m[7];
	tmp[1] = m[2] * m[7] - m[1] * m[8];
	tmp[2] = m[1] * m[5] - m[2] * m[4];
	tmp[3] = m[5] * m[6] - m[3] * m[8];
	tmp[4] = m[0] * m[8] - m[2] * m[6];
	tmp[5] = m[2] * m[3] - m[0] * m[5];
	tmp[6] = m[3] * m[7] - m[4] * m[6];
	tmp[7] = m[1] * m[6] - m[0] * m[7];
	tmp[8] = m[0] * m[4] - m[1] * m[3];

	// check determinant if it is 0
	determinant = m[0] * tmp[0] + m[1] * tmp[3] + m[2] * tmp[6];
	if(fabs(determinant) <= 0.00001f)
	{
		m[0] = m[4] = m[8] = 1.0f;
		m[1] = m[2] = m[3] = m[5] = m[6] = m[7] = 0.0f;
		return true; // cannot inverse, make it idenety matrix
	}

	// divide by the determinant
	invDeterminant = 1.0f / determinant;
	m[0] = invDeterminant * tmp[0];
	m[1] = invDeterminant * tmp[1];
	m[2] = invDeterminant * tmp[2];
	m[3] = invDeterminant * tmp[3];
	m[4] = invDeterminant * tmp[4];
	m[5] = invDeterminant * tmp[5];
	m[6] = invDeterminant * tmp[6];
	m[7] = invDeterminant * tmp[7];
	m[8] = invDeterminant * tmp[8];

	return true;
}


/**
 * Computes an orthographic projection matrix.
 *
 * @param m returns the result
 * @param mOffset
 * @param left
 * @param right
 * @param bottom
 * @param top
 * @param near
 * @param lar
 */
void Matrix::orthoM( float *m, int mOffset, float left, float right, float bottom, float top, float near, float mlar) {
	float r_width  = 1.0f / (right - left);
	float r_height = 1.0f / (top - bottom);
	float r_depth  = 1.0f / ( mlar-near );
	float x =  2.0f * (r_width);
	float y =  2.0f * (r_height);
	float z = -2.0f * (r_depth);
	float tx = -(right + left) * r_width;
	float ty = -(top + bottom) * r_height;
	float tz = -( mlar + near )  * r_depth;
	m[mOffset + 0] = x;
	m[mOffset + 5] = y;
	m[mOffset +10] = z;
	m[mOffset +12] = tx;
	m[mOffset +13] = ty;
	m[mOffset +14] = tz;
	m[mOffset +15] = 1.0f;
	m[mOffset + 1] = 0.0f;
	m[mOffset + 2] = 0.0f;
	m[mOffset + 3] = 0.0f;
	m[mOffset + 4] = 0.0f;
	m[mOffset + 6] = 0.0f;
	m[mOffset + 7] = 0.0f;
	m[mOffset + 8] = 0.0f;
	m[mOffset + 9] = 0.0f;
	m[mOffset + 11] = 0.0f;
}


/**
 * Define a projection matrix in terms of six clip planes
 * @param m the float array that holds the perspective matrix
 * @param offset the offset into float array m where the perspective
 * matrix data is written
 * @param left
 * @param right
 * @param bottom
 * @param top
 * @param near
 * @param lar
 */

void Matrix::frustumM(float *m, int offset,
		float left, float right, float bottom, float top,
		float near, float lar) {
	float r_width  = 1.0f / (right - left);
	float r_height = 1.0f / (top - bottom);
	float r_depth  = 1.0f / (near - lar);
	float x = 2.0f * (near * r_width);
	float y = 2.0f * (near * r_height);
	float A = 2.0f * ((right + left) * r_width);
	float B = (top + bottom) * r_height;
	float C = (lar + near) * r_depth;
	float D = 2.0f * (lar * near * r_depth);
	m[offset + 0] = x;
	m[offset + 5] = y;
	m[offset + 8] = A;
	m[offset +  9] = B;
	m[offset + 10] = C;
	m[offset + 14] = D;
	m[offset + 11] = -1.0f;
	m[offset +  1] = 0.0f;
	m[offset +  2] = 0.0f;
	m[offset +  3] = 0.0f;
	m[offset +  4] = 0.0f;
	m[offset +  6] = 0.0f;
	m[offset +  7] = 0.0f;
	m[offset + 12] = 0.0f;
	m[offset + 13] = 0.0f;
	m[offset + 15] = 0.0f;
}

/**
 * Define a projection matrix in terms of a field of view angle, an
 * aspect ratio, and z clip planes
 * @param m the float array that holds the perspective matrix
 * @param offset the offset into float array m where the perspective
 * matrix data is written
 * @param fovy field of view in y direction, in degrees
 * @param aspect width to height aspect ratio of the viewport
 * @param zNear
 * @param zlar
 */
void Matrix::perspectiveM(float *m, int offset,
		float fovy, float aspect, float zNear, float zlar) {
	float f = 1.0f / (float) tan(fovy * (M_PI / 360.0));
	float rangeReciprocal = 1.0f / (zNear - zlar);

	m[offset + 0] = f / aspect;
	m[offset + 1] = 0.0f;
	m[offset + 2] = 0.0f;
	m[offset + 3] = 0.0f;

	m[offset + 4] = 0.0f;
	m[offset + 5] = f;
	m[offset + 6] = 0.0f;
	m[offset + 7] = 0.0f;

	m[offset + 8] = 0.0f;
	m[offset + 9] = 0.0f;
	m[offset + 10] = (zlar + zNear) * rangeReciprocal;
	m[offset + 11] = -1.0f;

	m[offset + 12] = 0.0f;
	m[offset + 13] = 0.0f;
	m[offset + 14] = 2.0f * zlar * zNear * rangeReciprocal;
	m[offset + 15] = 0.0f;
}

/**
 * Computes the length of a vector
 *
 * @param x x coordinate of a vector
 * @param y y coordinate of a vector
 * @param z z coordinate of a vector
 * @return the length of a vector
 */
float Matrix::length(float x, float y, float z) {
	return (float) sqrt(x * x + y * y + z * z);
}

/**
 * Sets matrix m to the identity matrix.
 * @param sm returns the result
 * @param smOffset index into sm where the result matrix starts
 */
void Matrix::setIdentityM(float *sm, int smOffset) {
	for (int i=0 ; i<16 ; i++) {
		sm[smOffset + i] = 0.0f;
	}
	for(int i = 0; i < 16; i += 5) {
		sm[smOffset + i] = 1.0f;
	}
}

/**
 * Scales matrix  m by x, y, and z, putting the result in sm
 * @param sm returns the result
 * @param smOffset index into sm where the result matrix starts
 * @param m source matrix
 * @param mOffset index into m where the source matrix starts
 * @param x scale factor x
 * @param y scale factor y
 * @param z scale factor z
 */
void Matrix::scaleM(float *sm, int smOffset,
		float *m, int mOffset,
		float x, float y, float z) {
	for (int i=0 ; i<4 ; i++) {
		int smi = smOffset + i;
		int mi = mOffset + i;
		sm[     smi] = m[     mi] * x;
		sm[ 4 + smi] = m[ 4 + mi] * y;
		sm[ 8 + smi] = m[ 8 + mi] * z;
		sm[12 + smi] = m[12 + mi];
	}
}

/**
 * Scales matrix m in place by sx, sy, and sz
 * @param m matrix to scale
 * @param mOffset index into m where the matrix starts
 * @param x scale factor x
 * @param y scale factor y
 * @param z scale factor z
 */
void Matrix::scaleM(float *m, int mOffset,
		float x, float y, float z) {
	for (int i=0 ; i<4 ; i++) {
		int mi = mOffset + i;
		m[     mi] *= x;
		m[ 4 + mi] *= y;
		m[ 8 + mi] *= z;
	}
}

/**
 * Translates matrix m by x, y, and z, putting the result in tm
 * @param tm returns the result
 * @param tmOffset index into sm where the result matrix starts
 * @param m source matrix
 * @param mOffset index into m where the source matrix starts
 * @param x translation factor x
 * @param y translation factor y
 * @param z translation factor z
 */
void Matrix::translateM(float *tm, int tmOffset,
		float *m, int mOffset,
		float x, float y, float z) {
	for (int i=0 ; i<12 ; i++) {
		tm[tmOffset + i] = m[mOffset + i];
	}
	for (int i=0 ; i<4 ; i++) {
		int tmi = tmOffset + i;
		int mi = mOffset + i;
		tm[12 + tmi] = m[mi] * x + m[4 + mi] * y + m[8 + mi] * z +
				m[12 + mi];
	}
}

/**
 * Translates matrix m by x, y, and z in place.
 * @param m matrix
 * @param mOffset index into m where the matrix starts
 * @param x translation factor x
 * @param y translation factor y
 * @param z translation factor z
 */
void Matrix::translateM(
		float *m, int mOffset,
		float x, float y, float z) {
	for (int i=0 ; i<4 ; i++) {
		int mi = mOffset + i;
		m[12 + mi] += m[mi] * x + m[4 + mi] * y + m[8 + mi] * z;
	}
}

/**
 * Rotates matrix m by angle a (in degrees) around the axis (x, y, z)
 * @param rm returns the result
 * @param rmOffset index into rm where the result matrix starts
 * @param m source matrix
 * @param mOffset index into m where the source matrix starts
 * @param a angle to rotate in degrees
 * @param x scale factor x
 * @param y scale factor y
 * @param z scale factor z
 */
void Matrix::rotateM(float *rm, int rmOffset,
		float *m, int mOffset,
		float a, float x, float y, float z) {
	{
		setRotateM(sTemp, 32, a, x, y, z);
		multiplyMM(rm, rmOffset, m, mOffset, sTemp, 32);
	}
}

void Matrix::arrayCopy(float * src, int offset1, float *result, int offset2, int length)
{
	for(int i=0; i< length; i++)
	{
		result[i+offset2] = src[i+offset1];
	}
}

/**
 * Rotates matrix m in place by angle a (in degrees)
 * around the axis (x, y, z)
 * @param m source matrix
 * @param mOffset index into m where the matrix starts
 * @param a angle to rotate in degrees
 * @param x scale factor x
 * @param y scale factor y
 * @param z scale factor z
 */
void Matrix::rotateM(float *m, int mOffset,
		float a, float x, float y, float z) {
	//setIdentityM(sTemp,0);
	//setIdentityM(sTemp,16);
	setRotateM(sTemp, 32, a, x, y, z);

	multiplyMM(m, mOffset, m, mOffset, sTemp, 32);

	//arrayCopy(sTemp, 16, m, mOffset, 16);
}

/**
 * Rotates matrix m by angle a (in degrees) around the axis (x, y, z)
 * @param rm returns the result
 * @param rmOffset index into rm where the result matrix starts
 * @param a angle to rotate in degrees
 * @param x scale factor x
 * @param y scale factor y
 * @param z scale factor z
 */
void Matrix::setRotateM(float *rm, int rmOffset,
		float a, float x, float y, float z) {
	rm[rmOffset + 3] = 0;
	rm[rmOffset + 7] = 0;
	rm[rmOffset + 11]= 0;
	rm[rmOffset + 12]= 0;
	rm[rmOffset + 13]= 0;
	rm[rmOffset + 14]= 0;
	rm[rmOffset + 15]= 1;
	//a *= (float) (M_PI / 180.0f);
	//float s = (float) sin(a);
	//float c = (float) cos(a);

	float s = pTrigFuns->getSin(a);
	float c = pTrigFuns->getCos(a);

	 //LOGI("s,c: %f ,%f, %f",a, s, c);

	if (1.0f == x && 0.0f == y && 0.0f == z) {
		rm[rmOffset + 5] = c;   rm[rmOffset + 10]= c;
		rm[rmOffset + 6] = s;   rm[rmOffset + 9] = -s;
		rm[rmOffset + 1] = 0;   rm[rmOffset + 2] = 0;
		rm[rmOffset + 4] = 0;   rm[rmOffset + 8] = 0;
		rm[rmOffset + 0] = 1;
	} else if (0.0f == x && 1.0f == y && 0.0f == z) {
		rm[rmOffset + 0] = c;   rm[rmOffset + 10]= c;
		rm[rmOffset + 8] = s;   rm[rmOffset + 2] = -s;
		rm[rmOffset + 1] = 0;   rm[rmOffset + 4] = 0;
		rm[rmOffset + 6] = 0;   rm[rmOffset + 9] = 0;
		rm[rmOffset + 5] = 1;
	} else if (0.0f == x && 0.0f == y && 1.0f == z) {
		rm[rmOffset + 0] = c;   rm[rmOffset + 5] = c;
		rm[rmOffset + 1] = s;   rm[rmOffset + 4] = -s;
		rm[rmOffset + 2] = 0;   rm[rmOffset + 6] = 0;
		rm[rmOffset + 8] = 0;   rm[rmOffset + 9] = 0;
		rm[rmOffset + 10]= 1;
	} else {
		float len = length(x, y, z);
		if (1.0f != len) {
			float recipLen = 1.0f / len;
			x *= recipLen;
			y *= recipLen;
			z *= recipLen;
		}
		float nc = 1.0f - c;
		float xy = x * y;
		float yz = y * z;
		float zx = z * x;
		float xs = x * s;
		float ys = y * s;
		float zs = z * s;
		rm[rmOffset +  0] = x*x*nc +  c;
		rm[rmOffset +  4] =  xy*nc - zs;
		rm[rmOffset +  8] =  zx*nc + ys;
		rm[rmOffset +  1] =  xy*nc + zs;
		rm[rmOffset +  5] = y*y*nc +  c;
		rm[rmOffset +  9] =  yz*nc - xs;
		rm[rmOffset +  2] =  zx*nc - ys;
		rm[rmOffset +  6] =  yz*nc + xs;
		rm[rmOffset + 10] = z*z*nc +  c;
	}
}

/*
 *   	float c = cosf(a * DEG2RAD);    // cosine
    	    float s = sinf(a * DEG2RAD);    // sine
    	    float xx = x * x;
    	    float xy = x * y;
    	    float xz = x * z;
    	    float yy = y * y;
    	    float yz = y * z;
    	    float zz = z * z;

    	    // build rotation matrix
    	    rm[rmOffset +0] = xx * (1 - c) + c;
    	    rm[rmOffset +1] = xy * (1 - c) - z * s;
    	    rm[rmOffset +2] = xz * (1 - c) + y * s;
    	    rm[rmOffset +3] = 0;
    	    rm[rmOffset +4] = xy * (1 - c) + z * s;
    	    rm[rmOffset +5] = yy * (1 - c) + c;
    	    rm[rmOffset +6] = yz * (1 - c) - x * s;
    	    rm[rmOffset +7] = 0;
    	    rm[rmOffset +8] = xz * (1 - c) - y * s;
    	    rm[rmOffset +9] = yz * (1 - c) + x * s;
    	    rm[rmOffset +10]= zz * (1 - c) + c;
    	    rm[rmOffset +11]= 0;
    	    rm[rmOffset +12]= 0;
    	    rm[rmOffset +13]= 0;
    	    rm[rmOffset +14]= 0;
    	    rm[rmOffset +15]= 1;
 */


/**
 * Converts Euler angles to a rotation matrix
 * @param rm returns the result
 * @param rmOffset index into rm where the result matrix starts
 * @param x angle of rotation, in degrees
 * @param y angle of rotation, in degrees
 * @param z angle of rotation, in degrees
 */
void Matrix::setRotateEulerM(float *rm, int rmOffset,
		float x, float y, float z) {
	x *= (float) (M_PI / 180.0f);
	y *= (float) (M_PI / 180.0f);
	z *= (float) (M_PI / 180.0f);
	float cx = (float) cos(x);
	float sx = (float) sin(x);
	float cy = (float) cos(y);
	float sy = (float) sin(y);
	float cz = (float) cos(z);
	float sz = (float) sin(z);
	float cxsy = cx * sy;
	float sxsy = sx * sy;

	rm[rmOffset + 0]  =   cy * cz;
	rm[rmOffset + 1]  =  -cy * sz;
	rm[rmOffset + 2]  =   sy;
	rm[rmOffset + 3]  =  0.0f;

	rm[rmOffset + 4]  =  cxsy * cz + cx * sz;
	rm[rmOffset + 5]  = -cxsy * sz + cx * cz;
	rm[rmOffset + 6]  =  -sx * cy;
	rm[rmOffset + 7]  =  0.0f;

	rm[rmOffset + 8]  = -sxsy * cz + sx * sz;
	rm[rmOffset + 9]  =  sxsy * sz + sx * cz;
	rm[rmOffset + 10] =  cx * cy;
	rm[rmOffset + 11] =  0.0f;

	rm[rmOffset + 12] =  0.0f;
	rm[rmOffset + 13] =  0.0f;
	rm[rmOffset + 14] =  0.0f;
	rm[rmOffset + 15] =  1.0f;
}

/**
 * Define a viewing transformation in terms of an eye point, a center of
 * view, and an up vector.
 *
 * @param rm returns the result
 * @param rmOffset index into rm where the result matrix starts
 * @param eyeX eye point X
 * @param eyeY eye point Y
 * @param eyeZ eye point Z
 * @param centerX center of view X
 * @param centerY center of view Y
 * @param centerZ center of view Z
 * @param upX up vector X
 * @param upY up vector Y
 * @param upZ up vector Z
 */
void Matrix::setLookAtM(float *rm, int rmOffset,
		float eyeX, float eyeY, float eyeZ,
		float centerX, float centerY, float centerZ, float upX, float upY,
		float upZ) {

	// See the OpenGL GLUT documentation for gluLookAt for a description
	// of the algorithm. We implement it in a straightforward way:

	float fx = centerX - eyeX;
	float fy = centerY - eyeY;
	float fz = centerZ - eyeZ;

	// Normalize f
	float rlf = 1.0f / Matrix::length(fx, fy, fz);
	fx *= rlf;
	fy *= rlf;
	fz *= rlf;

	// compute s = f x up (x means "cross product")
	float sx = fy * upZ - fz * upY;
	float sy = fz * upX - fx * upZ;
	float sz = fx * upY - fy * upX;

	float sl=Matrix::length(sx, sy, sz);
	if(sl<=0.00001) return;
	// and normalize s
	float rls = 1.0f / sl;
	sx *= rls;
	sy *= rls;
	sz *= rls;

	// compute u = s x f
	float ux = sy * fz - sz * fy;
	float uy = sz * fx - sx * fz;
	float uz = sx * fy - sy * fx;

	rm[rmOffset + 0] = sx;
	rm[rmOffset + 1] = ux;
	rm[rmOffset + 2] = -fx;
	rm[rmOffset + 3] = 0.0f;

	rm[rmOffset + 4] = sy;
	rm[rmOffset + 5] = uy;
	rm[rmOffset + 6] = -fy;
	rm[rmOffset + 7] = 0.0f;

	rm[rmOffset + 8] = sz;
	rm[rmOffset + 9] = uz;
	rm[rmOffset + 10] = -fz;
	rm[rmOffset + 11] = 0.0f;

	rm[rmOffset + 12] = 0.0f;
	rm[rmOffset + 13] = 0.0f;
	rm[rmOffset + 14] = 0.0f;
	rm[rmOffset + 15] = 1.0f;

	translateM(rm, rmOffset, -eyeX, -eyeY, -eyeZ);
}

void Matrix::gluProject(float objX, float objY, float objZ,
		float* model, int modelOffset, float* project, int projectOffset,
		int* view, int viewOffset, float* win, int winOffset) {

	int M_OFFSET = 0; // 0..15
	int V_OFFSET = 16; // 16..19
	int V2_OFFSET = 20; // 20..23
	multiplyMM(sTemp, M_OFFSET, project, projectOffset,
			model, modelOffset);

	sTemp[V_OFFSET + 0] = objX;
	sTemp[V_OFFSET + 1] = objY;
	sTemp[V_OFFSET + 2] = objZ;
	sTemp[V_OFFSET + 3] = 1.0f;

	multiplyMV(sTemp, V2_OFFSET,
			sTemp, M_OFFSET, sTemp, V_OFFSET);

	float w = sTemp[V2_OFFSET + 3];
	if (w == 0.0f) {
		return;
	}

	float rw = 1.0f / w;

	win[winOffset] =
			view[viewOffset] + view[viewOffset + 2]
			                        * (sTemp[V2_OFFSET + 0] * rw + 1.0f)
			                        * 0.5f;
	win[winOffset + 1] =
			view[viewOffset + 1] + view[viewOffset + 3]
			                            * (sTemp[V2_OFFSET + 1] * rw + 1.0f) * 0.5f;
	win[winOffset + 2] = (sTemp[V2_OFFSET + 2] * rw + 1.0f) * 0.5f;
}

void Matrix::gluUnProject(float winX, float winY, float winZ,
		float* model, int modelOffset, float* project, int projectOffset,
		int* view, int viewOffset, float* obj, int objOffset) {
	float scratch[32];
	int PM_OFFSET = 0; // 0..15
	int INVPM_OFFSET = 16; // 16..31
	int V_OFFSET = 0; // 0..3 Reuses PM_OFFSET space
	multiplyMM(scratch, PM_OFFSET, project, projectOffset,
			model, modelOffset);

	if (!invertM(scratch, INVPM_OFFSET, scratch, PM_OFFSET)) {
		return;
	}

	scratch[V_OFFSET + 0] =
			2.0f * (winX - view[viewOffset + 0]) / view[viewOffset + 2]
			                                            - 1.0f;
	scratch[V_OFFSET + 1] =
			2.0f * (winY - view[viewOffset + 1]) / view[viewOffset + 3]
			                                            - 1.0f;
	scratch[V_OFFSET + 2] = 2.0f * winZ - 1.0f;
	scratch[V_OFFSET + 3] = 1.0f;

	multiplyMV(obj, objOffset, scratch, INVPM_OFFSET,
			scratch, V_OFFSET);
}

void Matrix::screenPto3dP(int x, int y, float *obj4)
{
	glGetIntegerv(GL_VIEWPORT, getGL().viewport);
	//LOGI("WH: %d, %d",getGL().viewport[2],getGL().viewport[3]);

	//float modelview[16];
	//float projection[16];
	//GLfloat  winX, winY, winZ;
	//GLdouble posX, posY, posZ;

	//for(int i=0;i<16;i++) modelview[i]=(float)getGL().VMatrix[i];
	//for(int i=0;i<16;i++) projection[i]=(float)getGL().PMatrix[i];

	gluUnProject(x,getGL().viewport[3]-y,0.0, getGL().VMatrix,0, getGL().PMatrix, 0,getGL().viewport,0, obj4, 0);

}

void Matrix::m3dpToScreenp(float *pf3dp, float *pfscreenp)
{
	glGetIntegerv(GL_VIEWPORT, getGL().viewport);
	//LOGI("WH: %d, %d",viewport[2],viewport[3]);

	//float modelview[16];
	//float projection[16];
	//GLfloat  winX, winY, winZ;
	//GLdouble posX, posY, posZ;

	//for(int i=0;i<16;i++) modelview[i]=(float)getGL().VMatrix[i];
	//for(int i=0;i<16;i++) projection[i]=(float)getGL().PMatrix[i];

	gluProject(pf3dp[0],pf3dp[1],pf3dp[2], getGL().VMatrix,0, getGL().PMatrix, 0,getGL().viewport,0,pfscreenp,0 );
	//m_log1f("pfv42 0:  %f",pfv42[0]);m_log1f("pfv42 1:  %f",pfv42[1]);m_log1f("pfv42 2:  %f",pfv42[2]);
}


Matrix *pMatrix=new Matrix();
