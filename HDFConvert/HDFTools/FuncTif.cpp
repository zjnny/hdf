
#include "FuncTif.h"
#include "../DataSet.h"
#include <iostream>
#include <string>
#include "xtiffio.h"
#include "geotiffio.h"

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
	////
	//debug 版本可能与hdf5有冲突，想同时使用只能用release
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
	{
		cout<<"Can not open tif file"<<endl;
		return 0;
	}

	int dircount=0;
	/*do {
	dircount++;
	} while (TIFFReadDirectory(tif)); */
	/*while (TIFFReadDirectory(tif))
	{
	dircount++;
	}*/
	cout<<"已经存在图层个数："<<dircount<<endl;
	unsigned short *buffOut = (unsigned short *)_TIFFmalloc(width *sizeof(unsigned short));
	unsigned short* imageData=NULL;
	for(int dir=dircount;dir<dircount+z;++dir)
	{
		imageData=((unsigned short*)m_pDataSet->GetData())+width*height*(dir-dircount);
		TIFFSetDirectory(tif,dir);
		//////////////////////////////////////////////////////////////////////////
		if(dir>dircount)
			TIFFWriteDirectory(tif);//注意此句不能在dir起始写时用，要在写第二个dir开始时调用
		//////////////////////////////////////////////////////////////////////////
		cout<<"写dir："<<dir<<endl;
		TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);//宽度
		TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);//长度=高度
		TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);//方向
		TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
		TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8*sizeof(unsigned short));
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
		//缓冲区由_TIFFmalloc生成，存储时不用指明长度，如下句
			if (!TIFFWriteScanline(tif, buffOut, i, 0))
				TIFFError("WriteImage","failure in WriteScanline\n");

		}
		//
	}
	//TIFFFreeDirectory(tif);
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
