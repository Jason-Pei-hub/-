///////////////////////////////////////////////////////////////////////////////////
//
//
//2022.01.17
//
//
///////////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

///////////////////////////////////////////////////////////////////////////////////
//
//
//  8bit
//
//
///////////////////////////////////////////////////////////////////////////////////
BYTE *Rmw_Read8BitBmpFile2Img(const char *filename,int *width,int *height)
{   //������*width���ص�����ʵͼ����(ȥ����4�������չ)
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER dataHeader;
	BYTE *pGryImg;
	FILE *fp;
	int w, h, ext, y;
	int begin, end;
	BYTE tmp[4];

	//step.0-----------------��ʼ��-------------------------//
	*width = *height = 0;
	//step.1-----------------���ļ�-----------------------//
	if(!(fp=fopen(filename,"rb"))) return NULL;
	//step.2-----------------��ȡ�ļ�������Ϣ---------------//
	if (fread((void *)&fileHeader, sizeof(BITMAPFILEHEADER),1,fp)!=1)
	{
		fclose(fp);
		return NULL;
	}
	//����Ƿ���bmp�ļ�
	if (fileHeader.bfType!=((WORD)('M'<<8)|'B'))
	{
		fclose(fp);
		return NULL;
	}
	//step.3-----------------������������Ϣ---------------//
	if (fread((void *)&dataHeader, sizeof(BITMAPINFOHEADER), 1,fp)!=1)
	{
		fclose(fp);
		return NULL;
	}
	//����Ƿ�8Bit�ĻҶ�ͼ��
	if (dataHeader.biBitCount!=8)
	{
		fclose(fp);
		return NULL;
	}
	//step.4-----------------��ȡͼ������-------------------//
 	w=dataHeader.biWidth;
	h=dataHeader.biHeight;
    ext=(w+3)/4*4-w; //��Ϊbmp�ļ������ÿ���ֽڸ�����4�ı���
	//����β��λ��
	fseek(fp, 0L, SEEK_END);
    end = ftell(fp);
	//��λ��������ʼλ��
	fseek(fp, fileHeader.bfOffBits, SEEK_SET);
	begin = ftell(fp);
	//��֤�ļ�����
	if (end-begin<(w+ext)*h)
	{
		fclose(fp);
		return NULL;
	}
	//������
	if ( (pGryImg=new BYTE[w*h])!=NULL)
	{
		for(y=0;y<h;y++)
		{
			//����w���ֽ�
			fread(pGryImg+(h-1-y)*w, sizeof(BYTE), w, fp);
 			//�������������
			fread(tmp, sizeof(BYTE), ext, fp);
		}
	}
	//step.5-----------------����ͼ������-------------------//
	fclose(fp);
	*width = w;
	*height = h;
	return pGryImg;
}

bool Rmw_Read8BitBmpFile2Img(const char *filename, int *width, int *height, BYTE *pGryImg)
{   //pGryImg���ڸú���������õ��ڴ棬Ҫ�㹻��Ҫ�ܵ����¼�����ȡ��ͼ������
	//������*width���ص�����ʵͼ����(ȥ����4�������չ)
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER dataHeader;
	FILE *fp;
	int w, h, ext, y;
	int begin, end;
	BYTE tmp[4];

	//step.0-----------------��ʼ��-------------------------//
	*width = *height = 0;
	//step.1-----------------���ļ�-----------------------//
	if (!(fp = fopen(filename, "rb"))) return NULL;
	//step.2-----------------��ȡ�ļ�������Ϣ---------------//
	if (fread((void *)&fileHeader, sizeof(BITMAPFILEHEADER),1,fp)!=1)
	{
		fclose(fp);
		return false;
	}
	//����Ƿ���bmp�ļ�
	if (fileHeader.bfType!=((WORD)('M'<<8)|'B'))
	{
		fclose(fp);
		return false;
	}
	//step.3-----------------������������Ϣ---------------//
	if (fread((void *)&dataHeader, sizeof(BITMAPINFOHEADER), 1,fp)!=1)
	{
		fclose(fp);
		return false;
	}
	//����Ƿ�8Bit�ĻҶ�ͼ��
	if (dataHeader.biBitCount!=8)
	{
		fclose(fp);
		return false;
	}
	//step.4-----------------��ȡͼ������-------------------//
	w = dataHeader.biWidth;
	h = dataHeader.biHeight;
	ext = (w+3)/4*4-w; //��Ϊbmp�ļ������ÿ���ֽڸ�����4�ı���
	//����β��λ��
	fseek(fp, 0L, SEEK_END);
	end = ftell(fp);
	//��λ��������ʼλ��
	fseek(fp, fileHeader.bfOffBits, SEEK_SET);
	begin = ftell(fp);
	//��֤�ļ�����
	if (end-begin<(w+ext)*h)
	{
		fclose(fp);
		return false;
	}
	//������
	for (y = 0; y<h; y++)
	{
		//����w���ֽ�
		fread(pGryImg+(h-1-y)*w, sizeof(BYTE), w, fp);
		//�������������
		fread(tmp, sizeof(BYTE), ext, fp);
	}
	//step.5-----------------����ͼ������-------------------//
	fclose(fp);
	*width = w;
	*height = h;
	return true;
}

bool Rmw_Write8BitImg2BmpFile(BYTE *pGryImg,int width,int height,const char *filename)
{   
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER dataHeader;
	FILE *fp;
	int i, ext;
	BYTE p[4],*pCur;

	//step.0-----------------�����ļ�-----------------------//
    if( (fp=fopen(filename,"w+b"))==NULL ) return false; 
	//step.1-----------------д�ļ�������Ϣ-----------------//
	fileHeader.bfType= ((WORD) ('M' << 8) | 'B');
	fileHeader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+256*4L; //�Ҷ�ֵ��ʼλ��,��ɫ���СΪ256*4
    fileHeader.bfSize=fileHeader.bfOffBits+width*height ;
    fileHeader.bfReserved1=0;
    fileHeader.bfReserved2=0;
	fwrite((void *)&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	//step.2-----------------д����������Ϣ-----------------//
	memset((void *)&dataHeader, 0, sizeof(BITMAPINFOHEADER));
	dataHeader.biSize = 40;
	dataHeader.biWidth = width;
	dataHeader.biHeight = height;
	dataHeader.biPlanes = 1;
	dataHeader.biBitCount = 8;
	fwrite((void *)&dataHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	//step.3-----------------д��ɫ��-----------------------//
	p[3] = 0;
	for (i=0;i<256;i++)
    {  
		p[0] = p[1] = p[2] = i; //�Ҷȵ�ɫ��,blue=green=red
		fwrite((void *)p, sizeof(BYTE), 4, fp);
 	}
	//step.4-----------------дͼ������---------------------//
	ext=(width+3)/4*4-width; //ÿ���ֽڸ�����Ҫ��4�ı�������
	if (ext==0)
	{   
		for(pCur=pGryImg+(height-1)*width;pCur>=pGryImg;pCur-=width) //��д
		{   
			fwrite((void *)pCur, sizeof(BYTE), width, fp);
		}
	}
	else
	{
		p[0] = p[1] = p[2] = p[3] = 0; //�����������Ϊ0
		for(pCur=pGryImg+(height-1)*width;pCur>=pGryImg;pCur-=width) //��д
		{   
			fwrite((void *)pCur, sizeof(BYTE), width, fp); //ͼ������
			fwrite((void *)p, sizeof(BYTE), ext, fp); //��������
		}
	}
	//step.5-----------------�˳�--------------------------//
	fclose(fp); //�ر��ļ�
	return true;
}

bool Rmw_Write8BitImg2BmpFileRed(BYTE *pGryImg,int width,int height,const char *filename)
{   
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER dataHeader;
	FILE *fp;
	int i, ext;
	BYTE p[4],*pCur;

	//step.0-----------------�����ļ�-----------------------//
    if( (fp=fopen(filename,"w+b"))==NULL ) return false; 
	//step.1-----------------д�ļ�������Ϣ-----------------//
	fileHeader.bfType= ((WORD) ('M' << 8) | 'B');
	fileHeader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+256*4L; //�Ҷ�ֵ��ʼλ��,��ɫ���СΪ256*4
    fileHeader.bfSize=fileHeader.bfOffBits+width*height ;
    fileHeader.bfReserved1=0;
    fileHeader.bfReserved2=0;
	fwrite((void *)&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	//step.2-----------------д����������Ϣ-----------------//
	memset((void *)&dataHeader, 0, sizeof(BITMAPINFOHEADER));
	dataHeader.biSize = 40;
	dataHeader.biWidth = width;
	dataHeader.biHeight = height;
	dataHeader.biPlanes = 1;
	dataHeader.biBitCount = 8;
	fwrite((void *)&dataHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	//step.3-----------------д��ɫ��-----------------------//
	p[0] = p[1] = p[3] = 0;
	for (i=0;i<256;i++)
    {  
		p[2] = i; //��ɫ��ɫ��,blue=green=0
		fwrite((void *)p, sizeof(BYTE), 4, fp);
 	}
	//step.4-----------------дͼ������---------------------//
	ext=(width+3)/4*4-width; //ÿ���ֽڸ�����Ҫ��4�ı�������
	if (ext==0)
	{   
		for(pCur=pGryImg+(height-1)*width;pCur>=pGryImg;pCur-=width) //��д
		{   
			fwrite((void *)pCur, sizeof(BYTE), width, fp);
		}
	}
	else
	{
		p[0] = p[1] = p[2] = p[3] = 0; //�����������Ϊ0
		for(pCur=pGryImg+(height-1)*width;pCur>=pGryImg;pCur-=width) //��д
		{   
			fwrite((void *)pCur, sizeof(BYTE), width, fp); //ͼ������
			fwrite((void *)p, sizeof(BYTE), ext, fp); //��������
		}
	}
	//step.5-----------------�˳�--------------------------//
	fclose(fp); //�ر��ļ�
	return true;
}

bool Rmw_Write8BitImg2BmpFileMark(BYTE *pGryImg, int width, int height, const char *filename)
{   //��Ҷ�ֵ255��ʾʱΪ��ɫ,254��ʾʱΪ��ɫ,253��ʾʱΪ��ɫ,253��ʾʱΪ��ɫ
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER dataHeader;
	FILE *fp;
	int i, ext;
	BYTE p[4], *pCur;

	//step.0-----------------�����ļ�-----------------------//
	if ((fp = fopen(filename, "w+b"))==NULL) return false;
	//step.1-----------------д�ļ�������Ϣ-----------------//
	fileHeader.bfType = ((WORD)('M'<<8)|'B');
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+256*4L; //�Ҷ�ֵ��ʼλ��,��ɫ���СΪ256*4
	fileHeader.bfSize = fileHeader.bfOffBits+width*height;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fwrite((void *)&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	//step.2-----------------д����������Ϣ-----------------//
	memset((void *)&dataHeader, 0, sizeof(BITMAPINFOHEADER));
	dataHeader.biSize = 40;
	dataHeader.biWidth = width;
	dataHeader.biHeight = height;
	dataHeader.biPlanes = 1;
	dataHeader.biBitCount = 8;
	fwrite((void *)&dataHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	//step.3-----------------д��ɫ��-----------------------//
	p[3] = 0;
	for (i = 0; i<252; i++) //[0..251]ʹ�������ĻҶȵ�ɫ��
	{
		p[0] = p[1] = p[2] = i; //�Ҷȵ�ɫ��,blue=green=red
		fwrite((void *)p, sizeof(BYTE), 4, fp);
	}
	p[0] = 0;   p[1] = 255;  p[2] = 255; fwrite((void *)p, sizeof(BYTE), 4, fp); //252��ɫ
	p[0] = 255; p[1] = 0;    p[2] = 0;   fwrite((void *)p, sizeof(BYTE), 4, fp); //253��ɫ
	p[0] = 0;   p[1] = 255;  p[2] = 0;   fwrite((void *)p, sizeof(BYTE), 4, fp); //254��ɫ
	p[0] = 0;   p[1] = 0;    p[2] = 255; fwrite((void *)p, sizeof(BYTE), 4, fp); //255��ɫ
	//step.4-----------------дͼ������---------------------//
	ext = (width+3)/4*4-width; //ÿ���ֽڸ�����Ҫ��4�ı�������
	if (ext==0)
	{
		for (pCur = pGryImg+(height-1)*width; pCur>=pGryImg; pCur -= width) //��д
		{
			fwrite((void *)pCur, sizeof(BYTE), width, fp);
		}
	}
	else
	{
		p[0] = p[1] = p[2] = p[3] = 0; //�����������Ϊ0
		for (pCur = pGryImg+(height-1)*width; pCur>=pGryImg; pCur -= width) //��д
		{
			fwrite((void *)pCur, sizeof(BYTE), width, fp); //ͼ������
			fwrite((void *)p, sizeof(BYTE), ext, fp); //��������
		}
	}
	//step.5-----------------�˳�--------------------------//
	fclose(fp); //�ر��ļ�
	return true;
}
///////////////////////////////////////////////////////////////////////////////////
//
//
//  24bit
//
//
///////////////////////////////////////////////////////////////////////////////////
BYTE *Rmw_Read24BitBmpFile2Img(const char *filename, int *width, int *height)
{   //������*width���ص�����ʵͼ����(ȥ����4�������չ)
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER dataHeader;
	BYTE *pRGBImg;
	FILE *fp;
	int w, h, ext, y;
	int begin, end;
	BYTE tmp[4];

	//step.0-----------------��ʼ��-------------------------//
	*width = *height = 0;
	//step.1-----------------���ļ�-----------------------//
	if (!(fp = fopen(filename, "rb"))) return NULL;
	//step.2-----------------��ȡ�ļ�������Ϣ---------------//
	if (fread((void *)&fileHeader, sizeof(BITMAPFILEHEADER),1,fp)!=1)
	{
		fclose(fp);
		return NULL;
	}
	//����Ƿ���bmp�ļ�
	if (fileHeader.bfType!=((WORD)('M'<<8)|'B'))
	{
		fclose(fp);
		return NULL;
	}
	//step.3-----------------������������Ϣ---------------//
	if (fread((void *)&dataHeader, sizeof(BITMAPINFOHEADER), 1,fp)!=1)
	{
		fclose(fp);
		return NULL;
	}
	//����Ƿ�24Bit�ĻҶ�ͼ��
	if (dataHeader.biBitCount!=24)
	{
		fclose(fp);
		return NULL;
	}
	//step.4-----------------��ȡͼ������-------------------//
	w = dataHeader.biWidth;
	h = dataHeader.biHeight;
	ext = ((w*3+3)/4*4)-w*3; //��Ϊbmp�ļ������ÿ���ֽڸ�����4�ı���
	//����β��λ��
	fseek(fp, 0L, SEEK_END);
	end = ftell(fp);
	//��λ��������ʼλ��
	fseek(fp, fileHeader.bfOffBits, SEEK_SET);
	begin = ftell(fp);
	//��֤�ļ�����
	if (end-begin<w*h*3+ext)
	{
		fclose(fp);
		return NULL;
	}
	//������
	if ((pRGBImg = new BYTE[w*h*3])!=NULL)
	{
		for (y = 0; y<h; y++)
		{
			//����w���ֽ�
			fread(pRGBImg+(h-1-y)*w*3, sizeof(BYTE), w*3, fp);
			//�������������
			fread(tmp, sizeof(BYTE), ext, fp);
		}
	}
	//step.5-----------------����ͼ������-------------------//
	fclose(fp);
	*width = w;
	*height = h;
	return pRGBImg;
}

bool Rmw_Read24BitBmpFile2Img(const char *filename, int *width, int *height, BYTE *pRGBImg)
{   //pRGBImg���ڸú���������õ��ڴ棬Ҫ�㹻��Ҫ�ܵ����¼�����ȡ��ͼ������
	//������*width���ص�����ʵͼ����(ȥ����4�������չ)
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER dataHeader;
	FILE *fp;
	int w, h, ext, y;
	int begin, end;
	BYTE tmp[4];

	//step.0-----------------��ʼ��-------------------------//
	*width = *height = 0;
	//step.1-----------------���ļ�-----------------------//
	if (!(fp = fopen(filename, "rb"))) return NULL;
	//step.2-----------------��ȡ�ļ�������Ϣ---------------//
	if (fread((void *)&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp)!=1)
	{
		fclose(fp);
		return false;
	}
	//����Ƿ���bmp�ļ�
	if (fileHeader.bfType!=((WORD)('M'<<8)|'B'))
	{
		fclose(fp);
		return false;
	}
	//step.3-----------------������������Ϣ---------------//
	if (fread((void *)&dataHeader, sizeof(BITMAPINFOHEADER), 1,fp)!=1)
	{
		fclose(fp);
		return false;
	}
	//����Ƿ�24Bit�ĻҶ�ͼ��
	if (dataHeader.biBitCount!=24)
	{
		fclose(fp);
		return false;
	}
	//step.4-----------------��ȡͼ������-------------------//
	w = dataHeader.biWidth;
	h = dataHeader.biHeight;
	ext = ((w*3+3)/4*4)-w*3; //��Ϊbmp�ļ������ÿ���ֽڸ�����4�ı���
	//����β��λ��
	fseek(fp, 0L, SEEK_END);
	end = ftell(fp);
	//��λ��������ʼλ��
	fseek(fp, fileHeader.bfOffBits, SEEK_SET);
	begin = ftell(fp);
	//��֤�ļ�����
	if (end-begin<w*h*3+ext)
	{
		fclose(fp);
		return false;
	}
	//������
	for (y = 0; y<h; y++)
	{
		//����w���ֽ�
		fread(pRGBImg+(h-1-y)*w*3, sizeof(BYTE), w*3, fp);
		//�������������
		fread(tmp, sizeof(BYTE), ext, fp);
	}
	//step.5-----------------����ͼ������-------------------//
	fclose(fp);
	*width = w;
	*height = h;
	return true;
}

bool Rmw_Write24BitImg2BmpFile(BYTE *pRGBImg, int width, int height, const char *filename)
{
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER dataHeader;
	FILE *fp;
	int ext;
	BYTE p[4], *pCur;

	//step.0-----------------�����ļ�-----------------------//
	if ((fp = fopen(filename, "w+b"))==NULL) return false;
	//step.1-----------------д�ļ�������Ϣ-----------------//
	fileHeader.bfType = ((WORD)('M'<<8)|'B');
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER); //�Ҷ�ֵ��ʼλ��
	fileHeader.bfSize = fileHeader.bfOffBits+width*height+0L; //��ɫͼ��û�е�ɫ��
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fwrite((void *)&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	//step.2-----------------д����������Ϣ-----------------//
	memset((void *)&dataHeader, 0, sizeof(BITMAPINFOHEADER));
	dataHeader.biSize = 40;
	dataHeader.biWidth = width;
	dataHeader.biHeight = height;
	dataHeader.biPlanes = 1;
	dataHeader.biBitCount = 8*3; //RGB��3������
 	fwrite((void *)&dataHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	//step.4-----------------дͼ������---------------------//
	ext = ((width*3+3)/4*4)-width*3; //��Ϊbmp�ļ������ÿ���ֽڸ�����4�ı���
	if (ext==0)
	{
		for (pCur = pRGBImg+(height-1)*width*3; pCur>=pRGBImg; pCur -= width*3) //��д
		{
			fwrite((void *)pCur, sizeof(BYTE), width*3, fp);
		}
	}
	else
	{
		p[0] = p[1] = p[2] = p[3] = 0; //�����������Ϊ0
		for (pCur = pRGBImg+(height-1)*width*3; pCur>=pRGBImg; pCur -= width*3) //��д
		{
			fwrite((void *)pCur, sizeof(BYTE), width*3, fp); //ͼ������
			fwrite((void *)p, sizeof(BYTE), ext, fp); //��������
		}
	}
	//step.5-----------------�˳�--------------------------//
	fclose(fp); //�ر��ļ�
	return true;
}
///////////////////////////////////////////////////////////////////////////////////
//
//
//  8/24bit
//
//
///////////////////////////////////////////////////////////////////////////////////
BYTE *Rmw_ReadBmpFile2Img(const char *filename, int *width, int *height, bool *isRGB)
{   //�Զ��ж���8bit����24Bit��bmpͼ���ļ�
	//����������ʵ�Ŀ��,ȥ����4�������չ.
	FILE *fp;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER dataHeader;

	//step.0-----------------��ʼ��-------------------------//
	*width = *height = 0;
	//step.1-----------------���ļ�-----------------------//
	if (!(fp = fopen(filename, "rb"))) return NULL;
	//step.2-----------------��ȡ�ļ�������Ϣ---------------//
	if (fread((void *)&fileHeader, sizeof(BITMAPFILEHEADER),1,fp)!=1)
	{
		fclose(fp);
		return NULL;
	}
	//����Ƿ���bmp�ļ�
	if (fileHeader.bfType!=((WORD)('M'<<8)|'B'))
	{
		fclose(fp);
		return NULL;
	}
	//step.3-----------------������������Ϣ---------------//
	if (fread((void *)&dataHeader, sizeof(BITMAPINFOHEADER), 1,fp)!=1)
	{
		fclose(fp);
		return NULL;
	}
	fclose(fp);
	//step.4-----------------��ȡͼ������-------------------//
	if (dataHeader.biBitCount==8)
	{
		*isRGB = false;
		return Rmw_Read8BitBmpFile2Img(filename, width, height);
	}
	else if (dataHeader.biBitCount==24)
	{
		*isRGB = true;
		return Rmw_Read24BitBmpFile2Img(filename, width, height);
	}
	//step.5-----------------����ͼ������-------------------//
	return NULL;
}
