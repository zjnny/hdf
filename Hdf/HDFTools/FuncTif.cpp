
#include "FuncTif.h"
#include "../Project/DataSet.h"
#include <iostream>
#include <string>
#include "geotiffio.h"
#include "xtiffio.h"
using namespace std;
using namespace HDFTools;
FuncTif::FuncTif()
{
	m_pDataSet=NULL;
}
FuncTif::~FuncTif()
{
}

void FuncTif::SetParam(void * lparam,void *rparam)
{
	m_pDataSet=(DataSet*)lparam;
	m_outfile=*(string*)rparam;
}
int FuncTif::Execute()
{
	
	return DataToTiff();
}
int FuncTif::DataToTiff()
{
	int dimens=m_pDataSet->GetDimensions();
	int x=m_pDataSet->GetX();
	int y=m_pDataSet->GetY();
	int z=m_pDataSet->GetZ();

	hsize_t hval[3]={1,1,1};
	if(dimens==2)
	{
		hval[0]=m_pDataSet->GetY();
		hval[1]=m_pDataSet->GetX();
	}
	else if(dimens==3)
	{
		hval[0]=m_pDataSet->GetZ();
		hval[1]=m_pDataSet->GetY();
		hval[2]=m_pDataSet->GetX();
	}
	//HDF5Helper::SaveDataSet(m_outfile.c_str(),m_pDataSet->GetDataSetName(),m_pDataSet->GetData(),dimens,hval);
	//HDF5Helper::SetAttrInfo(m_outfile.c_str(),m_pDataSet->GetDataSetName().c_str(),*(m_pDataSet->GetAttr()));

	TIFF *tif=(TIFF*)0;  /* TIFF-level descriptor */
	GTIF *gtif=(GTIF*)0; /* GeoKey-level descriptor */
	
	gtif = GTIFNew(tif);
	if (gtif==NULL)
	{
		XTIFFClose(tif);
		printf("failed in GTIFNew\n");
		return -1;
	}


	int width=x,height=y;
	int nChannels=1;
	tif=XTIFFOpen(m_outfile.c_str(),"w");
	if (!tif)
		return 0;
	int dircount=0;
	while (TIFFReadDirectory(tif))
	{
		dircount++;
	}

	unsigned short *buffOut = (unsigned short *)_TIFFmalloc(width *sizeof(unsigned short));
	unsigned short* imageData=NULL;
	for(int i=dircount;i<dircount+z;++i)
	{
		imageData=((unsigned short*)m_pDataSet->GetData())+width*height*(i-dircount);
		TIFFSetDirectory(tif,i);
		TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);//���
		TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);//����=�߶�
		TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);//����
		TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
		TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, sizeof(unsigned short));
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
	
		//TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 3 );
	
		/*double tiepoints[6]={0};
		double pixscale[3]={0};
		TIFFSetField(tif,TIFFTAG_GEOTIEPOINTS, 6,tiepoints);
		TIFFSetField(tif,TIFFTAG_GEOPIXELSCALE, 3,pixscale);*/

	
		//GTIFKeySet(gtif, GeogEllipsoidGeoKey, TYPE_SHORT,     1, Ellipse_Bessel_1841/*Ellipse_Everest_1830_1967_Definition*/);
		//GTIFKeySet(gtif, GeogSemiMajorAxisGeoKey, TYPE_DOUBLE, 1, (double)6377298.556);
		//GTIFKeySet(gtif, GeogInvFlatteningGeoKey, TYPE_DOUBLE, 1, (double)300.8017);

		
		memset(buffOut,0,width * sizeof(unsigned short));
		for (int i=0;i<height;i++){
			unsigned short* pTmp = imageData + i * width;
			//for(int j=0;j<width;j++)
			//{
			//	buffOut[j*nChannels]= pTmp[j*nChannels+2];
			//	buffOut[j*nChannels+1]= pTmp[j*nChannels+1];
			//	buffOut[j*nChannels+2]= pTmp[j*nChannels];
			//}
			memcpy(buffOut,pTmp,width* sizeof(unsigned short));
		//��������_TIFFmalloc���ɣ��洢ʱ����ָ�����ȣ����¾�
			if (!TIFFWriteScanline(tif, buffOut, i, 0))
				TIFFError("WriteImage","failure in WriteScanline\n");

		}
	}

	//TIFFSetDirectory(tif,1);
	//TIFFWriteDirectory(tif);
	//TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);//���
	//TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);//����=�߶�
	//TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);//����
	//TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
	//TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 1 * 8);
	//TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	//TIFFSetField(tif, TIFFTAG_PHOTOMETRIC,  PHOTOMETRIC_MINISBLACK);
	//TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
	//TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_PACKBITS);

	//TIFFSetField(tif, TIFFTAG_FILLORDER, FILLORDER_LSB2MSB);
	//rowsperstrip=(uint32) -1;
	//rowsperstrip= TIFFDefaultStripSize(tif, rowsperstrip);
	//if (rowsperstrip >height) {
	//	rowsperstrip = height;
	//}
	//TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rowsperstrip );
	//for (int i=0;i<height;i++){
	//	unsigned char* pTmp = imageData + i * width;
	//	//for(int j=0;j<width;j++)
	//	//{
	//	//	buffOut[j*nChannels]= pTmp[j*nChannels+2];
	//	//	buffOut[j*nChannels+1]= pTmp[j*nChannels+1];
	//	//	buffOut[j*nChannels+2]= pTmp[j*nChannels];
	//	//}
	//	memcpy(buffOut,pTmp,width* nChannels);
	//	//��������_TIFFmalloc���ɣ��洢ʱ����ָ�����ȣ����¾�
	//	if (!TIFFWriteScanline(tif, buffOut, i, 0))
	//		TIFFError("WriteImage","failure in WriteScanline\n");

	//}
	GTIFKeySet(gtif, GTModelTypeGeoKey, TYPE_SHORT, 1, ModelGeographic);
	GTIFKeySet(gtif, GTRasterTypeGeoKey, TYPE_SHORT, 1, RasterPixelIsArea);
	GTIFKeySet(gtif, GTCitationGeoKey, TYPE_ASCII, 0, "GeoImage");
	GTIFKeySet(gtif, GeographicTypeGeoKey, TYPE_SHORT,  1, GCS_WGS_84/*KvUserDefined*/);
	GTIFKeySet(gtif, GeogCitationGeoKey, TYPE_ASCII, 0, "WGS84");
	GTIFKeySet(gtif, GeogAngularUnitsGeoKey, TYPE_SHORT,  1, Angular_Degree);
	//GTIFKeySet(gtif, GeogLinearUnitsGeoKey, TYPE_SHORT,  1, Linear_Meter);
	GTIFKeySet(gtif, GeogGeodeticDatumGeoKey, TYPE_SHORT,     1, KvUserDefined);
	GTIFWriteKeys(gtif);
	GTIFFree(gtif);
	//_TIFFfree(imageData);
	if(buffOut!=NULL)
		_TIFFfree(buffOut);
	XTIFFClose(tif);


	return 0;
}
