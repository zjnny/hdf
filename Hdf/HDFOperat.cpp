#include "HDFOperat.h"
#include "stdio.h"
#include "xtiffio.h"
int HDFOperat::Data2Raw(const char *filename,void *pData,int width,int height,int bytes)
{
	FILE* fp = fopen(filename, "wb");
	if(fp==NULL)
		return -1;
	fwrite(pData, width*bytes,height, fp);
	fclose(fp);
	return 0;
}
int HDFOperat::Data2Tiff(const char* filename,void *pData,int width,int height,int bytes)
{
	TIFF *tif=NULL;  /* TIFF-level descriptor */
	tif=XTIFFOpen(filename,"w");
	if (tif==NULL)
		return -1;
	TIFFSetDirectory(tif,0);
	TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);//宽度
	TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);//长度=高度
	TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);//方向
	TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, bytes);
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bytes* 8);
	TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(tif, TIFFTAG_PHOTOMETRIC,  PHOTOMETRIC_MINISBLACK);
	TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
	TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_PACKBITS);

	TIFFSetField(tif, TIFFTAG_FILLORDER, FILLORDER_LSB2MSB);
	int rowsperstrip=(uint32) -1;
	rowsperstrip= TIFFDefaultStripSize(tif, rowsperstrip);
	if (rowsperstrip >height) {
		rowsperstrip = height;
	}
	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rowsperstrip );

	unsigned char *buffOut = (unsigned char *)_TIFFmalloc(width *bytes);
	memset(buffOut,0,(size_t)(width * bytes));
	for (int i=0;i<height;i++){
		unsigned char* pTmp = pData + i * width*bytes;

		memcpy(buffOut,pTmp,width* bytes);
		//缓冲区由_TIFFmalloc生成，存储时不用指明长度，如下句
		if (!TIFFWriteScanline(tif, buffOut, i, 0))
			TIFFError("WriteImage","failure in WriteScanline\n");

	}
	if(buffOut!=NULL)
		_TIFFfree(buffOut);
	XTIFFClose(tif);
	return 0;
}
int HDFOperat::Data2GeoTiff(const char* filename,void *pData,int width,int height,int bytes)
{
	if(pData==NULL||width==0||height==0)
		return -1;
	TIFF *tif=NULL;  /* TIFF-level descriptor */
	GTIF *gtif=NULL;
	tif=XTIFFOpen(filename,"w");
	if (tif==NULL)
		return -1;
	gtif = GTIFNew(tif);
	if (gtif==NULL)
	{
		XTIFFClose(tif);
		printf("failed in GTIFNew\n");
		return -1;
	}
	TIFFSetDirectory(tif,0);
	TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);//宽度
	TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);//长度=高度
	TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);//方向
	TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, bytes);
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bytes* 8);
	TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(tif, TIFFTAG_PHOTOMETRIC,  PHOTOMETRIC_MINISBLACK);
	TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
	TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_PACKBITS);

	TIFFSetField(tif, TIFFTAG_FILLORDER, FILLORDER_LSB2MSB);
	int rowsperstrip=(uint32) -1;
	rowsperstrip= TIFFDefaultStripSize(tif, rowsperstrip);
	if (rowsperstrip >height) {
		rowsperstrip = height;
	}
	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rowsperstrip );
	//
	GTIFKeySet(gtif, GTModelTypeGeoKey, TYPE_SHORT, 1, ModelGeographic);
	GTIFKeySet(gtif, GTRasterTypeGeoKey, TYPE_SHORT, 1, RasterPixelIsArea);
	GTIFKeySet(gtif, GTCitationGeoKey, TYPE_ASCII, 0, "GeoImage");
	GTIFKeySet(gtif, GeographicTypeGeoKey, TYPE_SHORT,  1, GCS_WGS_84/*KvUserDefined*/);
	GTIFKeySet(gtif, GeogCitationGeoKey, TYPE_ASCII, 0, "WGS84");
	GTIFKeySet(gtif, GeogAngularUnitsGeoKey, TYPE_SHORT,  1, Angular_Degree);
	//GTIFKeySet(gtif, GeogLinearUnitsGeoKey, TYPE_SHORT,  1, Linear_Meter);
	GTIFKeySet(gtif, GeogGeodeticDatumGeoKey, TYPE_SHORT,     1, KvUserDefined);
	//GTIFKeySet(gtif, GeogEllipsoidGeoKey, TYPE_SHORT,     1, Ellipse_Bessel_1841/*Ellipse_Everest_1830_1967_Definition*/);
	//GTIFKeySet(gtif, GeogSemiMajorAxisGeoKey, TYPE_DOUBLE, 1, (double)6377298.556);
	//GTIFKeySet(gtif, GeogInvFlatteningGeoKey, TYPE_DOUBLE, 1, (double)300.8017);
	//
	unsigned char *buffOut = (unsigned char *)_TIFFmalloc(width *bytes);
	memset(buffOut,0,(size_t)(width * bytes));
	for (int i=0;i<height;i++){
		unsigned char* pTmp = pData + i * width*bytes;

		memcpy(buffOut,pTmp,width* bytes);
		//缓冲区由_TIFFmalloc生成，存储时不用指明长度，如下句
		if (!TIFFWriteScanline(tif, buffOut, i, 0))
			TIFFError("WriteImage","failure in WriteScanline\n");

	}
	//geotif
	GTIFWriteKeys(gtif);
	GTIFFree(gtif);
	//
	if(buffOut!=NULL)
		_TIFFfree(buffOut);
	XTIFFClose(tif);
	return 0;
}
