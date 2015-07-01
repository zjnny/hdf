#ifndef _SATE_LLITE_H
#define _SATE_LLITE_H

#include <string>
#include <vector>
#include <iostream>
#include "common.h"
using namespace std;

#include "stdio.h"
#include "memory.h"
#include "hdf5.h"
// #ifdef WIN32
// #include "h5lt.h"
// #else
// #include "H5LT.h" 
// #endif



const char* const MERSIL1AttriGlobe[] = 
{
		"Satellite Name",//line 0
		"Sensor Name",
		"Sensor Identification Code",
		"Dataset Name",
		"File Name",
		"File Alias Name",
		"Responser",
		"Version Of Software",
		"Software Revision Date",
		"Version Of Coefficient Index",//line 9
		"Coefficient Index Revision Date",
		"Observing Beginning Date",
		"Observing Beginning Time",
		"Observing Ending Date",
		"Observing Ending Time",
		"Data Creating Date",
		"Data Creating Time",
		"Day Or Night Flag",
		"Orbit Number",
		"Orbit Period",//line 19
		"Orbit Direction",
		"Data Quality",
		"Number Of Scans",
		"Number Of Day mode scans",
		"Number of Night mode scans",
		"Incomplete Scans",
		"QA_Scan_Flag",
		"QA_Pixel_Flag",
		"Begin Line Number",
		"End Line Number",//line 29
		"Begin Pixel Number",
		"End Pixel Number",
		"Reference Ellipsoid Model ID",
		"Earth/Sun Distance Ratio",
		"WGS-84 a",
		"WGS-84 b",
		"WGS-84 Oblateness",
		"Satid",
		"Orbit",
		"Decay",//line 39
		"EpochTime",
		"EQUATORCROSSINGDATE",
		"EQUATORCROSSINGTIME",
		"Orbit Point Latitude",
		"Orbit Point Longitude",
		"MeanAnomaly",
		"Equatorcrossinglongitude",
		"MeanMotion",
		"Max Total Frames",
		"Max Earth Frames",//line 49
		"Max VOC Frames",
		"Max BB Frames",
		"Max SV Frames",
		"Missing Packets",
		"Discarded packets",
		"Scan number",
		"Frame count arry",
		"Scan Type",
		"VOC start time",
		"BB start time",//line 59
		"SV start time",
		"EV start time",
		"Packet scan count",
		"Engineering BB DN data",
		"Telemetry BB PRT data",
		"EV center time",//250m锟睫达拷锟斤拷锟斤拷
		"VOC Sun azimuth",//250m锟睫达拷锟斤拷锟斤拷
		"VOC Sun zenith",//250m锟睫达拷锟斤拷锟斤拷
		"VOC frames",
		"SV frames",//line 69
		"IR_Cal_Coeff",
		"VIR_Cal_Coeff",
		"TimeCode_QC_Flag",
		"Instrment_State_QC_Flag",
		"IR_DN_Normalized_Coeff",
		"VIS_250m_DN_Normalized_Coeff",
		"VIS_1km_DN_Normalized_Coeff",//250m锟睫达拷锟斤拷锟斤拷
		"Eccentricity",
		"PerigeeArgument",
		"AscendingNodeLongitude",//line 79
		"Orbitallnclination",		
		"AdditionalAnotation"//lilne 81
};

/**
 *	数据集属性
 */
typedef struct  attrOne{
	attrOne()
	{
		strName = "";
		iDataType = iArraySize = iByteLen = -1;
		pData = NULL;
	}

	~attrOne()
	{
		if (pData!=NULL)
		{
			delete []pData;
			pData = NULL;
		}
	}

	attrOne(const attrOne &f)
	{
		if (this == &f) return ; 
	
		if(f.pData!=NULL)
		{
//			if (pData!=NULL)
//			{
//				delete []pData;
//			}
					
			int iLen = f.iByteLen;			
			pData = new char[iLen];			
			memcpy(pData,f.pData,iLen);
		}

		this->iArraySize = f.iArraySize;
		this->iDataType = f.iDataType;
		this->iByteLen = f.iByteLen;
		this->strName = f.strName;
	}
	
	attrOne& operator = (const attrOne &f)
	{
		if (this == &f) return *this; 

		if(f.pData!=NULL)
		{
			delete[] pData;
			
			int iLen = f.iByteLen;
			pData = new char[iLen];			
			memcpy(pData,f.pData,iLen);
		}

		this->iArraySize = f.iArraySize;
		this->iDataType = f.iDataType;
		this->strName = f.strName;
		this->iByteLen = f.iByteLen;

		return *this;
	}

	string strName;		// 数据属性名字(Name)
	int iDataType;		// 数据类型的标号(Type)
	int iArraySize;		// 数据集的数组的个数
	int iByteLen;		// 总字节数
	void *pData;		// 指向数据集指针
}attrOne;

typedef struct attrGlobeMersiL1{
	attrGlobeMersiL1();
	~attrGlobeMersiL1();
	void applyMem();
	void deleteMem();
	
	string	strSatName;        //line 0    
	string	strSensorName;
	string	strSensorIdentiCode;     
	string	strDatasetName;              
	string	strFileName;                   
	string	strFileAliasName;                
	string	strResponser;                  
	string	strSoftVersion;            
	string	strSoftRevisionDate;         
	string	strCoeffIndexVersion;  //line 9 
	string	strCoeffIndexRevisionDate;
	string	strObBegDate;
	string	strObBegTime;       
	string	strObEndDate;          
	string	strObEndTime;          
	string	strDataCreDate;             
	string	strDataCreTime;             
	string	strDayOrNightFlag;         
	int	iOrbitNumber;             
	unsigned short int	ustOrbigPeriod; //line 19                  
	string	strOrbitDirection;                
	unsigned char	usDataQuality;               
	unsigned int	uNumberOfScans;        
	unsigned int	uNumberOfDayModeScans;       
	unsigned int	uNumberOfNightModeScans;     
	int	iIncompleteScans;       
	unsigned char	*psQAScanFlag;             //200 
	unsigned short int	*pustQAPixelFlag;   //200               
	unsigned short int	ustBeginLineNumber;              
	unsigned short int	ustEndLineNumber;    //line 29          
	unsigned short int	ustBeginPixelNumber;             
	unsigned short int	ustEndPixelNumber;              
	string	strRefEllipModID; 
	double	dEarthSunDistanceRatio;       
	double	dWGS84a;                      
	double	dWGS84b;                       
	double	dWGS84Oblateness;              
	string	strSatid;                        
	string	strOrbit;                          
	string	strDecay;       //line 39                   
	string	strEpochTime;                     
	string	strEQUATORCROSSINGDATE;            
	string	strEQUATORCROSSINGTIME;            
	float	*pfOrbitPointLat;   //4  
	float	*pfOrbitPointLon;      //4    
	string	strMeanAnomaly;                  
	double	dEquatorCrossingLon;       
	string	strMeanMotion;             
	int	iMaxTotalFrames;               
	int	iMaxEarthFrames;  //line 49             
	int	iMaxVOCFrames;               
	int	iMaxBBFrames;                 
	int	iMaxSVFrames;                  
	int	iMissPackets;                
	int	iDiscardPackets;              
	int	*piScanNumber;    //2000            
	int	*piFrameCountArray;     //200         
	unsigned char	*pusScanType;    //200           
	double	*pdVOCStartTime;     //200            
	double	*pdBBStartTime;           //200     line 59  
	double	*pdSVStartTime;                //200  
	double	*pdEVStartTime;                  //200
	int	*piPacketScanCount;      //2000        
	float	*pfEngBBDNData;     //2000
	float	*pfTelemetryBBPRTData;    //2000      
	float	*pfEVCenterTime;     //200      
	float	*pfVOSSunAzimuth;    //200            
	float	*pfVOCSunZenith;     //200           
	float	*pfVOCframes;       //2000           
	float	*pfSVFrames;        //2000  line 69            
	double	*pdIRCalCoeff;      //800             
	double	*pdVIRCalCoeff;     //57             
	unsigned char *pusTimeCodeQCFlag;   //2000           
	unsigned int	*puInstrmentStateQCFlag;  //200       
	float	*pfIRDNNormalCoeff;    //16000
	float	*pfVIS250mDNNormalCoeff;   //320
	float	*pfVIS1kmDNNormalCoeff;    //300
	string	strEccentricity;            
	string	strPerigeeArgument;                
	string	strAscendingNodeLon;  //line 79       
	string	strOrbitallnclination;             
	string	strAdditionalAnotation;    
}attrGlobeMersiL1;

#endif