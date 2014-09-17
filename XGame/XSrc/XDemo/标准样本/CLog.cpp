#include"CLog.h"
#include"windows.h"
#include <time.h>
CLog::CLog(){

	fp=fopen(LOGFILENAME,"w+");
	if(fp==NULL){
		MessageBox(NULL,"抱歉，打开日志文件失败","提示",MB_OK);
		return;
	}
}

CLog::CLog(char *fileName){

	fp=fopen(fileName,"w+");
	if(fp==NULL)
	{
		MessageBox(NULL,"抱歉，打开日志文件失败","提示",MB_OK);
		return;
	}
}
char *cutEnter(char *buf)
{
	buf[strlen(buf)-1]='\0';
	return buf;
}
bool CLog::Write(char *prefix,char *Info){
	char *info=NULL;
	time_t ti;
	time(&ti);
	try{
		if(fp==NULL)
			return false;
		
		info=new char[strlen(Info)+strlen(prefix)+128];//申请比原来更多的空间
		
		sprintf(info,"(%s)   %s: %s",cutEnter(ctime(&ti)),prefix,Info);
		
		//fputs(info,fp);
		
		if(fprintf(fp,"%s\n",info)==EOF)
		{
			delete info;
			info=NULL;
			return false;
		}
		delete info;
		info=NULL;
	}catch(...){
		if(info)
		{
			delete info;
			info=NULL;
		}
		MessageBox(NULL,"写入日志失败","提示",MB_OK);	
		return false;
	}
	fflush(fp);
	return true;
}

bool CLog::WriteError(char *errorInfo){
// 	char *info=NULL;
// 	try{
// 		if(fp==NULL)
// 			return false;
// 		
// 		info=new char[strlen(errorInfo)+32];//申请比原来更多的空间
// 
// 		sprintf(info,"\t错误提示: %s\r",errorInfo);
// 	
// 		//fputs(info,fp);
// 
// 		if(fprintf(fp,"%s\n",info)==EOF)
// 		{
// 			delete info;
// 			info=NULL;
// 			return false;
// 		}
// 		delete info;
// 		info=NULL;
// 	}catch(...){
// 		if(info)
// 		{
// 			delete info;
// 			info=NULL;
// 		}
// 		MessageBox(NULL,"写入日志失败","提示",MB_OK);	
// 		return false;
// 	}
// 
// 	return true;
	return Write("错误提示",errorInfo);

}

/*bool CLog::WriteErrorW(wchar_t *errorInfo){
	if(fp==NULL)
		return false;
	wchar_t info[512];
	swprintf(info,L"\t错误提示: %s!\r\n",errorInfo);
	fputws(info,fp);
	return true;
}
*/
bool CLog::WriteRight(char *rightInfo){
// 	if(fp==NULL)
// 		return false;
// 	char info[512];
// 	sprintf(info,"\t正确提示: %s\r\n",rightInfo);
// 	fputs(info,fp);
// 	char *info=NULL;
// 	try{
// 		if(fp==NULL)
// 			return false;
// 		
// 		info=new char[strlen(rightInfo)+32];//申请比原来更多的空间
// 		
// 		sprintf(info,"\t正确提示: %s\r",rightInfo);
// 		
// 		//fputs(info,fp);
// 		
// 		if(fprintf(fp,"%s\n",info)==EOF){
// 			delete info;
// 			info=NULL;
// 			return false;
// 		}
// 
// 		delete info;
// 		info=NULL;
// 	}catch(...){
// 		if(info)
// 		{
// 			delete info;
// 			info=NULL;
// 		}
// 		MessageBox(NULL,"写入日志失败","提示",MB_OK);	
// 		return false;
// 	}
//	return true;
	return Write("正确提示",rightInfo);
}

bool CLog::WriteInfo(char *normalInfo){
// 	if(fp==NULL)
// 		return false;
// 	char info[512];
// 	sprintf(info,"\t普通信息: %s\r\n",normalInfo);
// 	fputs(info,fp);
// 	char *info=NULL;
// 	try{
// 		if(fp==NULL)
// 			return false;
// 		
// 		info=new char[strlen(normalInfo)+32];//申请比原来更多的空间
// 		
// 		sprintf(info,"\t错误提示: %s\r",normalInfo);
// 		
// 		//fputs(info,fp);
// 		
// 		if(fprintf(fp,"%s\n",info)==EOF){
// 			delete info;
// 			info=NULL;
// 			return false;
// 		}
// 
// 		delete info;
// 		info=NULL;
// 	}catch(...){
// 
// 		if(info){
// 			delete info;
// 			info=NULL;
// 		}
// 
// 		MessageBox(NULL,"写入日志失败","提示",MB_OK);	
// 		return false;
// 	}
// 	return true;
	return Write("普通信息",normalInfo);
}

bool CLog::WriteSeg(char *segInfo){
// 	if(fp==NULL)
// 		return false;
// 	char info[512];
// 	sprintf(info,"当前阶段: %s\r\n",segInfo);
// 	fputs(info,fp);
// 	char *info=NULL;
// 	try{
// 		if(fp==NULL)
// 			return false;
// 		
// 		info=new char[strlen(segInfo)+32];//申请比原来更多的空间
// 		
// 		sprintf(info,"\t当前阶段: %s\r",segInfo);
// 		
// 		//fputs(info,fp);
// 		
// 		if(fprintf(fp,"%s\n",info)==EOF){
// 			delete info;
// 			info=NULL;
// 			return false;
// 		}
// 
// 		delete info;
// 		info=NULL;
// 	}catch(...)
// 	{
// 		if(info){
// 			delete info;
// 			info=NULL;
// 		}
// 		MessageBox(NULL,"写入日志失败","提示",MB_OK);	
// 	}	
// 	return true;

	return Write("当前阶段",segInfo);
}

bool CLog::WriteWarr(char *warronInfo){
// 	if(fp==NULL)
// 		return false;
// 	char info[512];
// 	sprintf(info,"\t警告: %s\r\n",warronInfo);
// 	fputs(info,fp);

// 	char *info=NULL;
// 	try{
// 		if(fp==NULL)
// 			return false;
// 		
// 		info=new char[strlen(warronInfo)+32];//申请比原来更多的空间
// 		
// 		sprintf(info,"\t警告: %s\r",warronInfo);
// 		
// 		//fputs(info,fp);
// 		
// 		if(fprintf(fp,"%s\n",info)==EOF){
// 			delete info;
// 			info=NULL;
// 			return false;
// 		}
// 		
// 		delete info;
// 		info=NULL;
// 	}catch(...)
// 	{
// 		if(info){
// 			delete info;
// 			info=NULL;
// 		}
// 		MessageBox(NULL,"写入日志失败","提示",MB_OK);	
// 	}
// 	return true;

	return Write("警告",warronInfo);
}

bool CLog::WriteInfo(float *floatNormalInfo,int length){
	if(fp==NULL)
		return false;
	char info[64];
	sprintf(info,"\t普通信息: ");

	fputs(info,fp);
	
	fwrite(floatNormalInfo,sizeof(float),length,fp);
	fputs("\n",fp);
	return true;
}

bool CLog::WriteInfo(byte *byteNormalInfo,int length){
	if(fp==NULL)
		return false;
	char info[64];
	sprintf(info,"\t普通信息: ");
	
	fputs(info,fp);
	
	fwrite(byteNormalInfo,sizeof(byte),length,fp);
	fputs("\r\n",fp);
	return true;
}

CLog::~CLog(){
	if(fp)
		fclose(fp);
}