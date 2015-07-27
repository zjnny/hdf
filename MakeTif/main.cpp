/*
根据hdf 黑夜或白天属性生成RGB或灰度Geotiff，只生成单张图
*/
#include <iostream>
using namespace std;

#include "hdf5.h"
#include "HDFTools/Analysis.h"
#include "HDFTools/DataSet.h"
#include "HDFTools/ToolFactory.h"
#include "HDFTools/FuncObject.h"
#include "HDFTools/Enhance.h"
#include "xtiffio.h"
#include "geotiffio.h"
#include <string>
#include <iostream>
using namespace std;
using namespace HDFTools;
typedef unsigned short ushort;
typedef unsigned char uchar;
static bool bEnhance;
void ConvertDataset(string &filepath,string &dspath,string &outname);
void ExportFile(DataSet* pDs,string &outname);
//一个数据集一个通道，多个数据集
void MergeToRGB(Analysis*pAn,vector<string> &dslist,string &out);
//一个数据集的多个通道
void MergeToRGB(DataSet* ds,string &out);
void WriteRGB2Tif(unsigned char*data,int width,int height,string &outfile);
void Write16Gray2Tif(unsigned short* data,int width,int height,string &outfile);
//HDF5转化成geotif，目前只处理同一个数据集里的通道,只考虑同一个文件中的数据
//-o out.tif -i xxx.hdf -d ds1
int main(int argc, char* argv[])
{
	
	try
	{
		//分析参数
		Analysis ana;
		ana.SetParam(argc,argv);
		ana.Execute();
		vector<string> flist;
		ana.GetInputFile(flist);
		if(flist.size()!=1)
			return -1;
		string enhance;
		bEnhance=true;
		ana.GetAttr("-e",enhance);
		if(enhance.empty())
			bEnhance=false;
		//判断是白天or黑夜
		string flag;
		HDF5Helper::GetFileDayOrNightFLag(flist[0].c_str(),"Time Of Data Composed",flag);
		//HDF5Helper::GetFileDayOrNightFLag(flist[0].c_str(),"Day Or Night Flag",flag);
		if(flag.empty())
		{
			cout<<"Can not find \"Time Of Data Composed\"flag"<<endl;
			cout<<"Try to find \"Day Or Night Flag\"flag"<<endl;
			HDF5Helper::GetFileDayOrNightFLag(flist[0].c_str(),"Day Or Night Flag",flag);
			if(flag.empty())
				cout<<"Can not find \"Day Or Night Flag\"flag"<<endl;
		}
		//处理获取-d指定的数据集
		vector<string> vc;
		ana.GetConvertedDataSet(vc);
		if(vc.size()==3)//必定是rgb三通道合并
		{
			if(flag!="Day")
				cout<<"DayOrNightFLag maybe error"<<endl;
			MergeToRGB(&ana,vc,ana.GetOutputFile());
			//3个数据集应该都是单通道的
		}
		else if(vc.size()==1)
		{
			if(flag=="Day")
			{
				//rgb通道都在一个数据集里
				DataSet ds;
				string dsFile=ana.GetDataSetFile(vc[0].c_str());
				string dsGroup=ana.GetDataSetGroupPath(vc[0].c_str());
				ds.InitFromFile(dsFile.c_str(),dsGroup);
				if(ds.GetY()<3)
					return -1;
				MergeToRGB(&ds,ana.GetOutputFile());
			}
			else if(flag=="Night")
			{
				//单通道灰度图
				DataSet ds;
				string dsFile=ana.GetDataSetFile(vc[0].c_str());
				string dsGroup=ana.GetDataSetGroupPath(vc[0].c_str());
				ds.InitFromFile(dsFile.c_str(),dsGroup);
				//ConvertDataset(dsFile,dsGroup,ana.GetOutputFile());
				if(bEnhance)
					Enhance::Histogram_Equalization_IgnoreZero((ushort*)ds.GetData(),ds.GetX(),ds.GetY());
				Write16Gray2Tif((ushort*)ds.GetData(),ds.GetX(),ds.GetY(),ana.GetOutputFile());
			}
			else
			{
				//单通道灰度图
				DataSet ds;
				string dsFile=ana.GetDataSetFile(vc[0].c_str());
				string dsGroup=ana.GetDataSetGroupPath(vc[0].c_str());
				ds.InitFromFile(dsFile.c_str(),dsGroup);
				//ConvertDataset(dsFile,dsGroup,ana.GetOutputFile());
				if(bEnhance)
					Enhance::Histogram_Equalization_IgnoreZero((ushort*)ds.GetData(),ds.GetX(),ds.GetY());
				Write16Gray2Tif((ushort*)ds.GetData(),ds.GetX(),ds.GetY(),ana.GetOutputFile());

			}

		}
		else
		{
			cout<<"Something is error"<<endl;
		}
	}
	catch (std::exception* e)
	{
		cout<<e->what()<<endl;
	}
	catch (...)
	{
		cout<<"Unknow error!"<<endl;
	}
#ifdef _DEBUG
	system("pause");
#endif
	return 0;
}
void ConvertDataset(string &filepath,string &dspath,string &outname)
{
	//获取数据集数据
	DataSet ds;
	ds.InitFromFile(filepath.c_str(),dspath);
	//暂时认为全部是ushort型
	if(ds.GetData()!=NULL)
		ExportFile(&ds,outname);

}
void ExportFile(DataSet* pDs,string &outname)
{
	string outfile=outname;
	string ext=outfile.substr(outfile.rfind('.')+1,outfile.length());
	ext=strlwr((char*)ext.c_str());
	//大小写转换
	ToolFactory &fac=ToolFactory::GetInstance();
	FuncObject *fc=NULL;
	if(ext=="jpg")
		fc=fac.Concrete(TOOL_GEN_JPEG);
	else
		fc=fac.Concrete(TOOL_GEN_JPEG);
	if(fc)
	{
		fc->SetParam((void*)pDs,(void*)&outfile);
		fc->Execute();
		delete fc;
		fc=NULL;
	}
	else
		cout<<"Can not output to file"<<endl;

}
void MergeToRGB(Analysis*pAn,vector<string> &dslist,string &out)
{
	//提取并转化每个数据集的单独通道
	if(dslist.size()!=3)
		return ;
	DataSet rds;
	DataSet gds;
	DataSet bds;
	string fname;
	string dsgroup;
	int width=0,height=0;
	unsigned char* pRGB=NULL;
	unsigned char* ptpRGB=NULL;
	unsigned char*pR=NULL;
	unsigned char*pG=NULL;
	unsigned char*pB=NULL;
	unsigned char*ptpR=NULL;
	unsigned char*ptpG=NULL;
	unsigned char*ptpB=NULL;
	//转化成R
	fname=pAn->GetDataSetFile(dslist[0].c_str());
	dsgroup=pAn->GetDataSetGroupPath(dslist[0].c_str());
	rds.InitFromFile(fname.c_str(),dsgroup);
	width=rds.GetX();
	height=rds.GetY();

	pR=new unsigned char[width*height];
	ptpR=pR;
	Enhance::Cast12To8((unsigned short*)rds.GetData(),ptpR,rds.GetX(),rds.GetY());
	rds.Destroy();//避免占用内存
	//转化成G
	fname=pAn->GetDataSetFile(dslist[1].c_str());
	dsgroup=pAn->GetDataSetGroupPath(dslist[1].c_str());
	gds.InitFromFile(fname.c_str(),dsgroup);
	if(width!=gds.GetX()||height!=gds.GetY())
	{
		delete []pR;
		return ;
	}
	pG=new unsigned char[width*height];
	ptpG=pG;
	Enhance::Cast12To8((unsigned short*)gds.GetData(),ptpG,gds.GetX(),gds.GetY());
	gds.Destroy();//避免占用内存
	//转化成B
	fname=pAn->GetDataSetFile(dslist[2].c_str());
	dsgroup=pAn->GetDataSetGroupPath(dslist[2].c_str());
	bds.InitFromFile(fname.c_str(),dsgroup);
	if(width!=gds.GetX()||height!=gds.GetY())
	{
		delete []pR;
		delete []pG;
		return ;
	}
	pB=new unsigned char[width*height];
	ptpB=pB;
	Enhance::Cast12To8((unsigned short*)bds.GetData(),ptpB,bds.GetX(),bds.GetY());
	bds.Destroy();//避免占用内存
	if(bEnhance)
	{
		Enhance::Histogram_Equalization_IgnoreZero(pR,width,height);
		Enhance::Histogram_Equalization_IgnoreZero(pG,width,height);
		Enhance::Histogram_Equalization_IgnoreZero(pB,width,height);
	}
	pRGB=new unsigned char[3*width*height];//3个数据集大小必须相同
	ptpRGB=pRGB;
	for(int i=0;i<width*height;++i)
	{
		*(ptpRGB+i*3)=*ptpR++;
		*(ptpRGB+i*3+1)=*ptpG++;
		*(ptpRGB+i*3+2)=*ptpB++;
	}
	delete []pR;
	delete []pG;
	delete []pB;
	//输出tif
	WriteRGB2Tif(pRGB,width,height,out);
	delete []pRGB;
}
void MergeToRGB(DataSet* ds,string &out)
{
	if(ds->GetZ()<3)
		return;
	//默认2，1，0有序对应RGB
	vector<int> bands;
	bands.push_back(2);
	bands.push_back(1);
	bands.push_back(0);
	unsigned short *pRbuf=NULL;
	unsigned short *pGbuf=NULL;
	unsigned short *pBbuf=NULL;
	unsigned char*pR=NULL;
	unsigned char*pG=NULL;
	unsigned char*pB=NULL;
	unsigned char*ptpR=NULL;
	unsigned char*ptpG=NULL;
	unsigned char*ptpB=NULL;
	unsigned short *pSrc=(unsigned short*)ds->GetData();
	int width=ds->GetX();
	int height=ds->GetY();
	int zcoord=ds->GetZ();
	pRbuf=pSrc+bands[0]*width*height;
	pGbuf=pSrc+bands[1]*width*height;
	pBbuf=pSrc+bands[2]*width*height;
	pR=new unsigned char[width*height];
	pG=new unsigned char[width*height];
	pB=new unsigned char[width*height];
	ptpR=pR;
	ptpG=pG;
	ptpB=pB;
	Enhance::Cast12To8(pRbuf,pR,width,height);
	Enhance::Cast12To8(pGbuf,pG,width,height);
	Enhance::Cast12To8(pBbuf,pB,width,height);
	if(bEnhance)
	{
		Enhance::Histogram_Equalization_IgnoreZero(pR,width,height);
		Enhance::Histogram_Equalization_IgnoreZero(pG,width,height);
		Enhance::Histogram_Equalization_IgnoreZero(pB,width,height);
	}

	ds->Destroy();
	unsigned char *imageData=new unsigned char[width*height*3];
	if(imageData==NULL)
	{
		delete []pR;
		delete[]pG;
		delete[]pB;
		return;
	}
	unsigned char *temp=imageData;
	for(int i=0;i<width*height;++i)
	{
		*(temp+i*3)=*(ptpR++);
		*(temp+i*3+1)=*(ptpG++);
		*(temp+i*3+2)=*(ptpB++);

	}
	delete []pR;
	delete[]pG;
	delete[]pB;

	WriteRGB2Tif(imageData,width,height,out);
	delete []imageData;
}
void WriteRGB2Tif(unsigned char*data,int width,int height,string &outfile)
{
	TIFF *tif=(TIFF*)0;  /* TIFF-level descriptor */
	GTIF *gtif=(GTIF*)0; /* GeoKey-level descriptor */

	gtif = GTIFNew(tif);
	if (gtif==NULL)
	{
		XTIFFClose(tif);
		printf("failed in GTIFNew\n");
		return ;
	}
	tif=XTIFFOpen(outfile.c_str(),"w");
	if (!tif)
	{
		cout<<"Can not open tif file"<<endl;
		return ;
	}
	unsigned char *buffOut = (unsigned char *)_TIFFmalloc(width *3*sizeof(unsigned char));


	TIFFSetDirectory(tif,0);
	cout<<"write tif,width="<<width<<" height="<<height<<endl;
	TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);//宽度
	TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);//长度=高度
	TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);//方向
	TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8*sizeof(unsigned char));
	TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(tif, TIFFTAG_PHOTOMETRIC,  PHOTOMETRIC_RGB);
	TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
	TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_PACKBITS);
	TIFFSetField(tif, TIFFTAG_FILLORDER, FILLORDER_LSB2MSB);
	int rowsperstrip=(uint32) -1;
	rowsperstrip= TIFFDefaultStripSize(tif, rowsperstrip);
	if (rowsperstrip >height) {
		rowsperstrip = height;
	}
	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rowsperstrip );



	memset(buffOut,0,width*3*sizeof(unsigned char));
	for (int i=0;i<height;i++){
		unsigned char* pTmp = data + i * width*3*sizeof(unsigned char);
		//for(int j=0;j<width;j++)
		//{
		//	buffOut[j*nChannels]= pTmp[j*nChannels+2];
		//	buffOut[j*nChannels+1]= pTmp[j*nChannels+1];
		//	buffOut[j*nChannels+2]= pTmp[j*nChannels];
		//}
		memcpy(buffOut,pTmp,width*3* sizeof(unsigned char));
		//缓冲区由_TIFFmalloc生成，存储时不用指明长度，如下句
		if (!TIFFWriteScanline(tif, buffOut, i, 0))
			TIFFError("WriteImage","failure in WriteScanline\n");

	}
	//
	//TIFFFreeDirectory(tif);
	GTIFKeySet(gtif, GTModelTypeGeoKey, TYPE_SHORT, 1, ModelGeographic);
	GTIFKeySet(gtif, GTRasterTypeGeoKey, TYPE_SHORT, 1, RasterPixelIsArea);
	GTIFKeySet(gtif, GTCitationGeoKey, TYPE_ASCII, 0, "GeoImage");
	GTIFKeySet(gtif, GeographicTypeGeoKey, TYPE_SHORT,  1, GCS_WGS_84/*KvUserDefined*/);
	GTIFKeySet(gtif, GeogCitationGeoKey, TYPE_ASCII, 0, "WGS84");
	GTIFKeySet(gtif, GeogAngularUnitsGeoKey, TYPE_SHORT,  1, Angular_Degree);
	//GTIFKeySet(gtif, GeogLinearUnitsGeoKey, TYPE_SHORT,  1, Linear_Meter);
	GTIFKeySet(gtif, GeogGeodeticDatumGeoKey, TYPE_SHORT, 1, KvUserDefined);
	GTIFWriteKeys(gtif);
	GTIFFree(gtif);
	//_TIFFfree(imageData);
	if(buffOut!=NULL)
		_TIFFfree(buffOut);
	XTIFFClose(tif);
}
void Write16Gray2Tif(unsigned short* data,int width,int height,string &outfile)
{
	TIFF *tif=(TIFF*)0;  /* TIFF-level descriptor */
	GTIF *gtif=(GTIF*)0; /* GeoKey-level descriptor */

	gtif = GTIFNew(tif);
	if (gtif==NULL)
	{
		XTIFFClose(tif);
		printf("failed in GTIFNew\n");
		return ;
	}
	tif=XTIFFOpen(outfile.c_str(),"w");
	if (!tif)
	{
		cout<<"Can not open tif file"<<endl;
		return ;
	}
	unsigned short *buffOut = (unsigned short *)_TIFFmalloc(width *sizeof(unsigned short));


	TIFFSetDirectory(tif,0);
	cout<<"write tif"<<endl;
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



	memset(buffOut,0,width*sizeof(unsigned short));
	for (int i=0;i<height;i++){
		unsigned short* pTmp = data + i * width;
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
	//TIFFFreeDirectory(tif);
	GTIFKeySet(gtif, GTModelTypeGeoKey, TYPE_SHORT, 1, ModelGeographic);
	GTIFKeySet(gtif, GTRasterTypeGeoKey, TYPE_SHORT, 1, RasterPixelIsArea);
	GTIFKeySet(gtif, GTCitationGeoKey, TYPE_ASCII, 0, "GeoImage");
	GTIFKeySet(gtif, GeographicTypeGeoKey, TYPE_SHORT,  1, GCS_WGS_84/*KvUserDefined*/);
	GTIFKeySet(gtif, GeogCitationGeoKey, TYPE_ASCII, 0, "WGS84");
	GTIFKeySet(gtif, GeogAngularUnitsGeoKey, TYPE_SHORT,  1, Angular_Degree);
	//GTIFKeySet(gtif, GeogLinearUnitsGeoKey, TYPE_SHORT,  1, Linear_Meter);
	GTIFKeySet(gtif, GeogGeodeticDatumGeoKey, TYPE_SHORT, 1, KvUserDefined);
	GTIFWriteKeys(gtif);
	GTIFFree(gtif);
	//_TIFFfree(imageData);
	if(buffOut!=NULL)
		_TIFFfree(buffOut);
	XTIFFClose(tif);
}