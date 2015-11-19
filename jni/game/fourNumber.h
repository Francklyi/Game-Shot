#ifndef _4NUMBER_H_
#define _4NUMBER_H_
class FourNumber{
public:
	float x, y, z, w;
	FourNumber(float xx, float yy, float zz){
		x=xx;
		y=yy;
		z=zz;
		w=1;
	}

	void productMatrix(float * floats , float *matrix){
		floats[0]=x*matrix[0]  + y*matrix[1]  + z*matrix[2]  + w*matrix[3];
		floats[1]=x*matrix[4]  + y*matrix[5]  + z*matrix[6]  + w*matrix[7];
		floats[2]=x*matrix[8]  + y*matrix[9]  + z*matrix[10] + w*matrix[11];
	}

	void productFloat(float f){
		x=x*f;
		y=y*f;
		z=z*f;
		w=w*f;
	}

	void productFloat3(float f){
			x=x*f;
			y=y*f;
			z=z*f;
		}

	void add3(FourNumber fN){
		x=x+fN.x;
		y=y+fN.y;
		z=z+fN.z;
	}

	void minus(FourNumber fN){
	         x=x-fN.x;
	         y=y-fN.y;
	         z=z-fN.z;
	  }

	void getXYZ(float* xyz){
		xyz[0]=x;
		xyz[1]=y;
		xyz[2]=z;
	}
};
#endif //_4NUMBER_H_
