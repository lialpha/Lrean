#ifndef _SZYVIEWSDK_H

#define _SZYVIEWSDK_H

#ifdef __cplusplus

extern"C" {
	
#endif
	typedef struct{
		unsigned int		year:6;
		unsigned int		month:4;
		unsigned int		day:5;
		unsigned int		hour:5;
		unsigned int		minute:6;
		unsigned int		second:6;
	}RTP_DT_T;		//ʱ��ṹ

	enum PlayBackType
	{
		CAPDATAPB,			//�Ӳɼ��˵����ݻط�
		SEVDATAPB,			//�ӷ����������ݻط�
	};

	struct SDK_NodeInfo
	{
		char sNodeID[50];// �ڵ�ID ,
		char sNodeName[50];// �ڵ����ƣ�
		char sParentID[50];// ���ڵ�ID,
		long lActive;// �ڵ�״̬ 1���ߣ�0����
		long lSubCameraCount;// �¼�����ͷ��	
		long lSubNodeCount;//�¼��ڵ���
		int	 nMaxOpenCameraCount;//���һ��ͬʱ�򿪵�����ͷ����0Ϊȫ���򿪣�
	};
//����ͷ����

	struct SDK_CameraInfo
	{
		char sCameraID[50];			//����ͷID
		char sCameraName[50];		//,����ͷ����
		char sNodeID[50];		//�ڵ�ID
		char sRemtoeIP[20];		//,ת����IP
		long lRemotePort;		//,ת����˿�
		bool IsLate;			//�Ƿ����
		long lActive;			// ����ͷ״̬		1���ߣ�0����
		long lDeviceType;		// ����ͷ�豸����
		long lIsPTZ;			// �Ƿ�����̨		1��ʾ�ǣ�0��ʾ����
		long lAudioFlag;		//��Ƶ����״̬		1������0����
	};

	//��¼���ص�����
	struct  SDK_LoginBackData
	{	
		char strUserID[50];
		char sClubName[50];
		char sUpdateIP[20];
		int	 nUserType;
		char sVersionID[50];
		char sVersionName[50];
		int	 nResult;
	};

	//�ص���Ƶ��������
	enum VideoType
	{
		SZY_H264,			
		SZY_RGB24,			
	};

	struct VideoHead
	{
		unsigned int	nWidth;
		unsigned int	nHeight;
		unsigned int	nFrameRate;
		long			nTS;
		long			nFrameCount;
		int				nVideoType;
		bool			bKeyFlag;
		bool			bFlipUD;		//�Ƿ���Ҫ���·�ת
		bool			bFlipUV;		//�Ƿ���ҪUV������ת(YUV�е�U��V)
		unsigned int	nDataLen;
	};

	struct Video_PACK
	{
		VideoHead sVideoHead;		//��Ƶͷ
		unsigned char* pVideoData;	//��Ƶ����
	};

	//��ǰ��״̬�ص�(caller:��̬��ĵ��ö���nType���������ͣ�nParam���ö����Ĳ���Ĭ��Ϊ0)
	typedef void __stdcall ProcStatus(void *caller,long lRealPlayID,int nType,long nParam);

	typedef void __stdcall DataCallBack(void *pCall,unsigned char* EnDate,unsigned frameSize,
		int iskey,int nVideoSize,int frameRate,int type,__int64 pts,long frameCount);

	typedef void __stdcall DataCallBackEX(void *pCall,Video_PACK sVideo);

	//�ص�������nTypeֵ��
	enum StatusValue
	{
		VIEW_START = 1,						//��ʼʵʱ��Ƶ
		//nParam:-2��ʾ����������������,-1��ʾ��ʼ������ʧ�ܣ�0��ʾ�ȴ���Ƶ���գ�1��ʾ���յ��ؼ�֡��ʼ����

		VIEW_STOP,							//ֹͣʵʱ��Ƶ
		//nParam:0��ʾ��ʼֵ�������壩
		VIEW_INITAUDIO,						//��ʼ����Ƶ�豸
		//nParam:0��ʾ��ʼ��ʧ��
	};	

	enum PlayBackStatus
	{
		PLAY_ST,			//��ʼ״����ʾ����������1Ϊ��ʼ��0Ϊ������
		PAUSE_ST,				//��ͣ״����ʾ����������1Ϊ��ͣ��0Ϊ��ʼ��
		CHANGEAVI_ST,			//����AVI״̬��ʾ����������1Ϊ��ʼ��0Ϊ������
		CHANGESPEED_ST,			//�ı䲥���ٶȣ���������1Ϊ���٣�+2Ϊ2�����٣�-2Ϊ�������٣���2��4��8��-2��-4��

		SETSLDMAXVALUE_ST,		//���ý��������ֵ���ļ�������һ���ļ��¼��һ����,�ɼ��˵�¼��ģʽ���ã�
		SETSLIDERPOS_ST,		//���ý�����λ��
		SETTIMES_ST,			//���õ�ǰ����ʱ��

		CAPTURE_ST,				//��ͼ����״̬
		CLOSE_ST,				//�رս���

		PBPlayRet,				//¼��طſ�ʼ����(����,0:ǰ��û���ļ���1���ɹ���2���ۿ��������ޣ�3���ɼ��˲����ߣ�
		//	4:���Ʒ������ܾ�,5:�ɼ�������¼��ת��ʧ��,6:���������ת��ʧ��)
	};	


	//���ò�������ص����Լ����ӿ��Ʒ�����
	bool _stdcall SZY_VIEW_Init(ProcStatus *pCallback,void *pCalled);

	//������¼����vServerIP�����е��ܿط�������ip���ϣ�nIPNum�����ٸ�IP��nServerPort���ܿط������˿ڣ�strUser����¼�˺ţ�
	long _stdcall SZY_VIEW_Connect(char * vServerIP[],int nIPNum,int nServerPort,char * strUser);

	//�˳����ӣ�lUserIDΪSZY_VIEW_Login����ȷ����ֵ�����½ӿڵ�lUserIDͬ����
	void _stdcall SZY_VIEW_DisConnect(long &lUserID);

	//��¼��������lUserIDͬ�ϣ�strUserΪ�˺ţ�strPassWordΪ���룬sLoginBackDataΪ��¼������Ϣ��
	bool _stdcall SZY_VIEW_Login(long lUserID,char * strUser,char * strPassWord,SDK_LoginBackData &sLoginBackData);

	//��ȡĳ�ڵ��µĽڵ㣨lUserIDͬ�ϣ�strUserIDΪ�˺�ID��strNodeIDΪ��ѯ�Ľڵ�ɣģ�nNodeNumΪ�����¼��ڵ�����
	bool _stdcall SZY_VIEW_GetNodeInfo(long lUserID,char *strUserID,char *strNodeID,int &nNodeNum);

	//����SZY_VIEW_GetNodeInfo�������ص��¼��ڵ�������ȡ�ڵ���Ϣ����
	void _stdcall SZY_VIEW_GetNodeValue(long lUserID,SDK_NodeInfo *vNodeList,int nNodeNum);

	//����SZY_VIEW_GetNodeInfo�������ص��¼��ڵ�������ȡ�ڵ���Ϣ����
	bool _stdcall SZY_VIEW_GetNodeInfoValue(long lUserID,int nIndex,SDK_NodeInfo &sNodeInfo);

	//��ȡĳ�ڵ��µ�����ͷ���ϣ�lUserIDͬ�ϣ�strUserIDΪ�˺�ID��strNodeIDΪ��ѯ�Ľڵ�ɣģ�nNodeNumΪ�����¼�����ͷ����
	bool _stdcall SZY_VIEW_GetCameraInfo(long lUserID,char *strUserID,char *strNodeID,int &nCameraNum);

	//����SZY_VIEW_GetCameraInfo�������ص��¼�����ͷ������ȡ����ͷ��Ϣ����
	void _stdcall SZY_VIEW_GetCameraValue(long lUserID,SDK_CameraInfo *vCameraList,int nCameraNum);

	bool _stdcall SZY_VIEW_GetCameraInfoValue(long lUserID,int nIndex,SDK_CameraInfo &vCameraInfo);
	
	//ʵ������Ƶ���Ŷ���(���־ɵ�����������ǲ��ص�����)
	long _stdcall SZY_VIEW_InitRealPlay(long lUserID,void *pDataCallBack,void *pCalled);

	//Ҫʵ�����ݻص�����Ҫ���ñ��ӿڽ��г�ʼ��
	long _stdcall SZY_VIEW_InitRealPlayEX(long lUserID,void *pDataCallBack,VideoType nVideoType,void *pCalled);

	//��ʼʵʱ������Ƶ��lUserIDͬ�ϣ�sRemoteIP��nRemotePortΪ��Ƶ������IP���˿ڣ�pPlayWndΪ���ŵĴ��ڶ���sCameraIDΪ����ͷ��ID��
	bool _stdcall SZY_VIEW_RealPlay(long lUserID,long lRealPlayID,char *sRemoteIP,int nRemotePort,void *pPlayWnd,char *sCameraID);
	
	//ֹͣʵʱ���ţ�lUserIDͬ�ϣ�
	void _stdcall SZY_VIEW_StopRealPlay(long lUserID,long lRealPlayID);
	
	//��̨���ƣ�lUserIDͬ�ϣ�bModulesΪ����ģ���ʶ��bPTZCommandΪ��ǰ�������bValue��ǰ����ֵ��
	bool _stdcall SZY_VIEW_PTZControl(long lUserID,long lRealPlayID,char bModules,char bPTZCommand,char bValue); 

	//�����������ܣ�lUserIDͬ�ϣ�
	bool _stdcall SZY_VIEW_OpenAudioTalk(long lUserID,long lRealPlayID);

	//�رս������ܣ�lUserIDͬ�ϣ�
	bool _stdcall SZY_VIEW_CloseAudioTalk(long lUserID,long lRealPlayID);

	//���������������ܣ�lUserIDͬ�ϣ�
	bool _stdcall SZY_VIEW_OpenAudioListen(long lUserID,long lRealPlayID);
	
	//�ر������������ܣ�lUserIDͬ�ϣ�
	bool _stdcall SZY_VIEW_CloseAudioListen(long lUserID,long lRealPlayID);

	//¼��طų�ʼ����lUserIDͬ�ϣ�,����¼��طŶ�����:lPBID

	long _stdcall SZY_VIEW_InitPlayBack(long lUserID,void *hPlayWnd,ProcStatus *pCallback,void *lpCalled);
	//�ͷ�¼��طž��
	void _stdcall SZY_VIEW_ReleasePlayBack(long lPBID);
	//����Ҫ�طŵ�����ͷ��Ϣ
	bool _stdcall SZY_VIEW_SetPBCameraInfo(long lPBID,char *strNodeID,char *strCameraID,char *strRemoteIP,int nRemotePort);
	
	//���ò������ͣ��Լ��ļ��洢·����AVI�ļ��Լ�ץͼ��
	bool _stdcall SZY_VIEW_SetPlayParam(long lPBID,long nType,char *sFileDir);

	//��ʼ����ָ��ʱ�������¼���ļ�,ʱ���ʽΪ:2012-02-04 09:05:00
	bool _stdcall SZY_VIEW_StartPlayBack(long lPBID,char * strOriStartTime, char * strOriStopTime);

	//ֹͣ�ط�
	bool _stdcall SZY_VIEW_StopPlayBack(long lPBID);

	//��ͣ���ָ��ط�
	bool _stdcall SZY_VIEW_Pause(long lPBID,bool bOperate);

	//�ı䲥�ŵ��ٶ�
	bool _stdcall SZY_VIEW_ChangeSpeed(long lPBID,bool bOperate);

	//ץͼ
	bool _stdcall SZY_VIEW_Capture(long lPBID);

	//�϶�������
	bool _stdcall SZY_VIEW_HScroll(long lPBID,long nPos);
	
	//¼���ļ�ʵʱ����ΪAVI�ļ�
	bool _stdcall SZY_VIEW_SaveToAVI(long lPBID,bool bOperate);

#ifdef __cplusplus 
	
}

#endif

#endif

