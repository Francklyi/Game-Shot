#ifndef _MTLS_H_
#define _MTLS_H_
#include <GLES2/gl2.h>
class Mtl{
public:
	//char* name;
	int Lmap_Kd;
	int Lmap_Ka;
	char *map_Ka;//��ʾ�洢map_Ka�ĳ��ȵ�ָ�룬��ָ�����һ���ֽھ���map_Ka���׸��ַ��λ�á�
	char *map_Kd;

	float Ns;
	float Ni;
	float d;
	float Tr;

	float
	Tf[4] , Ka[4], Kd[4], Ks[4], Ke[4],
	*v, *vn, *vt;

	int 	illum,
	nv, nvt, nvn,
	w , h;
	GLuint mTextureId;//��ɫ����id
	GLuint mNormalTexId;//��������id
	unsigned active_texture_id;//���ڰ���ɫ����ʹ�õ����?��0��ʼ
	char *Lbp;
	//float ** flp;
	//struct VTNbuffer *VTNp;

	int viL;//number of vi
	unsigned short *vip;
	GLuint uboi[1];// for vi

public:
	//methods

};

#endif //_MTLS_H_
