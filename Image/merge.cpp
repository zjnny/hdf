/*
rgb通道合并测试
*/
#include <string>
#include <iostream>
//using namespace std;
#include <stdio.h>
#include "jpeglib.h"

typedef unsigned short ushort;
typedef unsigned char uchar;

int main(int argc, char* argv[])
{
	try
	{
		int width=2048;
		int height=2000;
		int len=width*height;
		uchar *Rband=new uchar[len];
		uchar *Gband=new uchar[len];
		uchar *Bband=new uchar[len];
		FILE* fp = fopen("F:\\temp\\out2.raw", "r");
		if(fp==NULL)
		{
			std::cout<<"Can not open file at "<<__LINE__<<std::endl;
			return -1;
		}
		fread(Rband, width,height, fp);
		fclose(fp);
		fp = fopen("F:\\temp\\out1.raw", "r");
		if(fp==NULL)
		{
			std::cout<<"Can not open file at "<<__LINE__<<std::endl;
			return -1;
		}
		fread(Gband, width,height, fp);
		fclose(fp);
		fp = fopen("F:\\temp\\out0.raw", "r");
		if(fp==NULL)
		{
			std::cout<<"Can not open file at "<<__LINE__<<std::endl;
			return -1;
		}
		fread(Bband, width,height, fp);
		fclose(fp);
		uchar *image_buffer=new uchar[width*height*3];
		for(int i=0;i<width*height;++i)
		{
			image_buffer[i*3]=Rband[i];
			image_buffer[i*3+1]=Gband[i];
			image_buffer[i*3+2]=Bband[i];
		}
		//写jpeg
		const char *filename="F:\\temp\\out.jpg";
		struct jpeg_compress_struct cinfo;
		struct jpeg_error_mgr jerr;
		FILE * outfile;		/* target file */
		JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
		int row_stride;
		cinfo.err = jpeg_std_error(&jerr);
		/* Now we can initialize the JPEG compression object. */
		jpeg_create_compress(&cinfo);
		if ((outfile = fopen(filename, "wb")) == NULL) {
			fprintf(stderr, "can't open %s\n", filename);
			exit(1);
		}
		jpeg_stdio_dest(&cinfo, outfile);
		cinfo.image_width = width; 	/* image width and height, in pixels */
		cinfo.image_height = height;
		cinfo.input_components = 3;		/* # of color components per pixel */
		cinfo.in_color_space = JCS_RGB; 
		 jpeg_set_defaults(&cinfo);
		 jpeg_set_quality(&cinfo, 95, TRUE /* limit to baseline-JPEG values */);
		 jpeg_start_compress(&cinfo, TRUE);
		 row_stride = width * 3;
		 while (cinfo.next_scanline < cinfo.image_height) {
			row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
			(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
		}
		jpeg_finish_compress(&cinfo);
		fclose(outfile);
		jpeg_destroy_compress(&cinfo);
		delete []Rband;
		delete []Gband;
		delete[]Bband;
		delete[]image_buffer;
	}
	catch (std::exception* e)
	{
		std::cout<<e->what()<<std::endl;
	}
	catch (...)
	{
		std::cout<<"Unknow error!"<<std::endl;
	}
#ifdef _DEBUG
	system("pause");
#endif
	return 0;
}