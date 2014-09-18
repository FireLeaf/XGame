#include"CLog.h"
#include"windows.h"
#include <time.h>
CLog::CLog(){

	fp=fopen(LOGFILENAME,"w+");
	if(fp==NULL){
		MessageBox(NULL,"��Ǹ������־�ļ�ʧ��","��ʾ",MB_OK);
		return;
	}
}

CLog::CLog(char *fileName){

	fp=fopen(fileName,"w+");
	if(fp==NULL)
	{
		MessageBox(NULL,"��Ǹ������־�ļ�ʧ��","��ʾ",MB_OK);
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
		
		info=new char[strlen(Info)+strlen(prefix)+128];//�����ԭ������Ŀռ�
		
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
		MessageBox(NULL,"д����־ʧ��","��ʾ",MB_OK);	
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
// 		info=new char[strlen(errorInfo)+32];//�����ԭ������Ŀռ�
// 
// 		sprintf(info,"\t������ʾ: %s\r",errorInfo);
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
// 		MessageBox(NULL,"д����־ʧ��","��ʾ",MB_OK);	
// 		return false;
// 	}
// 
// 	return true;
	return Write("������ʾ",errorInfo);

}

/*bool CLog::WriteErrorW(wchar_t *errorInfo){
	if(fp==NULL)
		return false;
	wchar_t info[512];
	swprintf(info,L"\t������ʾ: %s!\r\n",errorInfo);
	fputws(info,fp);
	return true;
}
*/
bool CLog::WriteRight(char *rightInfo){
// 	if(fp==NULL)
// 		return false;
// 	char info[512];
// 	sprintf(info,"\t��ȷ��ʾ: %s\r\n",rightInfo);
// 	fputs(info,fp);
// 	char *info=NULL;
// 	try{
// 		if(fp==NULL)
// 			return false;
// 		
// 		info=new char[strlen(rightInfo)+32];//�����ԭ������Ŀռ�
// 		
// 		sprintf(info,"\t��ȷ��ʾ: %s\r",rightInfo);
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
// 		MessageBox(NULL,"д����־ʧ��","��ʾ",MB_OK);	
// 		return false;
// 	}
//	return true;
	return Write("��ȷ��ʾ",rightInfo);
}

bool CLog::WriteInfo(char *normalInfo){
// 	if(fp==NULL)
// 		return false;
// 	char info[512];
// 	sprintf(info,"\t��ͨ��Ϣ: %s\r\n",normalInfo);
// 	fputs(info,fp);
// 	char *info=NULL;
// 	try{
// 		if(fp==NULL)
// 			return false;
// 		
// 		info=new char[strlen(normalInfo)+32];//�����ԭ������Ŀռ�
// 		
// 		sprintf(info,"\t������ʾ: %s\r",normalInfo);
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
// 		MessageBox(NULL,"д����־ʧ��","��ʾ",MB_OK);	
// 		return false;
// 	}
// 	return true;
	return Write("��ͨ��Ϣ",normalInfo);
}

bool CLog::WriteSeg(char *segInfo){
// 	if(fp==NULL)
// 		return false;
// 	char info[512];
// 	sprintf(info,"��ǰ�׶�: %s\r\n",segInfo);
// 	fputs(info,fp);
// 	char *info=NULL;
// 	try{
// 		if(fp==NULL)
// 			return false;
// 		
// 		info=new char[strlen(segInfo)+32];//�����ԭ������Ŀռ�
// 		
// 		sprintf(info,"\t��ǰ�׶�: %s\r",segInfo);
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
// 		MessageBox(NULL,"д����־ʧ��","��ʾ",MB_OK);	
// 	}	
// 	return true;

	return Write("��ǰ�׶�",segInfo);
}

bool CLog::WriteWarr(char *warronInfo){
// 	if(fp==NULL)
// 		return false;
// 	char info[512];
// 	sprintf(info,"\t����: %s\r\n",warronInfo);
// 	fputs(info,fp);

// 	char *info=NULL;
// 	try{
// 		if(fp==NULL)
// 			return false;
// 		
// 		info=new char[strlen(warronInfo)+32];//�����ԭ������Ŀռ�
// 		
// 		sprintf(info,"\t����: %s\r",warronInfo);
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
// 		MessageBox(NULL,"д����־ʧ��","��ʾ",MB_OK);	
// 	}
// 	return true;

	return Write("����",warronInfo);
}

bool CLog::WriteInfo(float *floatNormalInfo,int length){
	if(fp==NULL)
		return false;
	char info[64];
	sprintf(info,"\t��ͨ��Ϣ: ");

	fputs(info,fp);
	
	fwrite(floatNormalInfo,sizeof(float),length,fp);
	fputs("\n",fp);
	return true;
}

bool CLog::WriteInfo(byte *byteNormalInfo,int length){
	if(fp==NULL)
		return false;
	char info[64];
	sprintf(info,"\t��ͨ��Ϣ: ");
	
	fputs(info,fp);
	
	fwrite(byteNormalInfo,sizeof(byte),length,fp);
	fputs("\r\n",fp);
	return true;
}

CLog::~CLog(){
	if(fp)
		fclose(fp);
}