/*
*���ڴ洢��γ������ϵ
*/
#ifndef Coordinate_h_
#define Coordinate_h_
class Coordinate
{
public:
	typedef unsigned short ushort;
	Coordinate();
	~Coordinate();
	//��ʼ���������ݼ���pDadaԴ���������ֶ��ͷ�
	void Init(int width,int height,void *pDada,int metre,unsigned char bytes=4);
	//��ȡ����Ӧ���ɵ�ͼ���С
	int GetDestLength();
	ushort GetWidth();
	ushort GetHeight();
	void* GetCoord();
	void Convert();
	float GetResolution(){return m_pixelUnit;}
protected:
	//����������ϵת�����Ƿ񻹱���ԭʼ����ϵ��
	
	void FindMaxAndMin();
private:
	ushort m_width;
	ushort m_height;
	void *m_pSrcData;
	void *m_pData;//ת����������������
	float m_pixelUnit;//ÿ���ض�Ӧ�Ķ���
	ushort m_bytes;//�ֽ�����������������ϵ�Ĵ�Сȷ��ʹ��int ��short
public:
	float m_max_degree;//������
	float m_min_degree;//��С����
};


#endif