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
	}RTP_DT_T;		//时间结构

	enum PlayBackType
	{
		CAPDATAPB,			//从采集端调数据回放
		SEVDATAPB,			//从服务器调数据回放
	};

	struct SDK_NodeInfo
	{
		char sNodeID[50];// 节点ID ,
		char sNodeName[50];// 节点名称，
		char sParentID[50];// 父节点ID,
		long lActive;// 节点状态 1在线，0离线
		long lSubCameraCount;// 下级摄像头数	
		long lSubNodeCount;//下级节点数
		int	 nMaxOpenCameraCount;//最多一次同时打开的摄像头数（0为全部打开）
	};
//摄像头内容

	struct SDK_CameraInfo
	{
		char sCameraID[50];			//摄像头ID
		char sCameraName[50];		//,摄像头名称
		char sNodeID[50];		//节点ID
		char sRemtoeIP[20];		//,转发层IP
		long lRemotePort;		//,转发层端口
		bool IsLate;			//是否过期
		long lActive;			// 摄像头状态		1在线，0离线
		long lDeviceType;		// 摄像头设备类型
		long lIsPTZ;			// 是否是云台		1表示是，0表示不是
		long lAudioFlag;		//音频开关状态		1：开，0：关
	};

	//登录返回的内容
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

	//回调视频数据类型
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
		bool			bFlipUD;		//是否需要上下翻转
		bool			bFlipUV;		//是否需要UV分量翻转(YUV中的U、V)
		unsigned int	nDataLen;
	};

	struct Video_PACK
	{
		VideoHead sVideoHead;		//视频头
		unsigned char* pVideoData;	//视频数据
	};

	//当前的状态回调(caller:动态库的调用对象，nType：动作类型，nParam：该动作的参数默认为0)
	typedef void __stdcall ProcStatus(void *caller,long lRealPlayID,int nType,long nParam);

	typedef void __stdcall DataCallBack(void *pCall,unsigned char* EnDate,unsigned frameSize,
		int iskey,int nVideoSize,int frameRate,int type,__int64 pts,long frameCount);

	typedef void __stdcall DataCallBackEX(void *pCall,Video_PACK sVideo);

	//回调函数的nType值：
	enum StatusValue
	{
		VIEW_START = 1,						//开始实时视频
		//nParam:-2表示连接人数超过限制,-1表示初始化参数失败，0表示等待视频接收，1表示接收到关键帧开始播放

		VIEW_STOP,							//停止实时视频
		//nParam:0表示初始值（无意义）
		VIEW_INITAUDIO,						//初始化音频设备
		//nParam:0表示初始化失败
	};	

	enum PlayBackStatus
	{
		PLAY_ST,			//开始状体提示（所跟参数1为开始，0为结束）
		PAUSE_ST,				//暂停状体提示（所跟参数1为暂停，0为开始）
		CHANGEAVI_ST,			//保存AVI状态提示（所跟参数1为开始，0为结束）
		CHANGESPEED_ST,			//改变播放速度（所跟参数1为常速，+2为2倍加速，-2为两倍减速）（2、4、8，-2、-4）

		SETSLDMAXVALUE_ST,		//设置进度条最大值（文件个数，一个文件最长录制一分钟,采集端调录像模式试用）
		SETSLIDERPOS_ST,		//设置进度条位置
		SETTIMES_ST,			//设置当前播放时间

		CAPTURE_ST,				//截图返回状态
		CLOSE_ST,				//关闭界面

		PBPlayRet,				//录像回放开始返回(参数,0:前端没有文件，1：成功，2：观看人数上限，3：采集端不在线，
		//	4:控制服务器拒绝,5:采集端连接录像转发失败,6:浏览端连接转发失败)
	};	


	//设置操作结果回调，以及连接控制服务器
	bool _stdcall SZY_VIEW_Init(ProcStatus *pCallback,void *pCalled);

	//创建登录对象（vServerIP：所有的总控服务器的ip集合，nIPNum：多少个IP，nServerPort：总控服务器端口，strUser：登录账号）
	long _stdcall SZY_VIEW_Connect(char * vServerIP[],int nIPNum,int nServerPort,char * strUser);

	//退出连接（lUserID为SZY_VIEW_Login的正确返回值，以下接口的lUserID同样）
	void _stdcall SZY_VIEW_DisConnect(long &lUserID);

	//登录服务器（lUserID同上，strUser为账号，strPassWord为密码，sLoginBackData为登录返回信息）
	bool _stdcall SZY_VIEW_Login(long lUserID,char * strUser,char * strPassWord,SDK_LoginBackData &sLoginBackData);

	//获取某节点下的节点（lUserID同上，strUserID为账号ID，strNodeID为查询的节点ＩＤ，nNodeNum为返回下级节点数）
	bool _stdcall SZY_VIEW_GetNodeInfo(long lUserID,char *strUserID,char *strNodeID,int &nNodeNum);

	//根据SZY_VIEW_GetNodeInfo函数返回的下级节点数，获取节点信息内容
	void _stdcall SZY_VIEW_GetNodeValue(long lUserID,SDK_NodeInfo *vNodeList,int nNodeNum);

	//根据SZY_VIEW_GetNodeInfo函数返回的下级节点数，获取节点信息内容
	bool _stdcall SZY_VIEW_GetNodeInfoValue(long lUserID,int nIndex,SDK_NodeInfo &sNodeInfo);

	//获取某节点下的摄像头集合（lUserID同上，strUserID为账号ID，strNodeID为查询的节点ＩＤ，nNodeNum为返回下级摄像头数）
	bool _stdcall SZY_VIEW_GetCameraInfo(long lUserID,char *strUserID,char *strNodeID,int &nCameraNum);

	//根据SZY_VIEW_GetCameraInfo函数返回的下级摄像头数，获取摄像头信息集合
	void _stdcall SZY_VIEW_GetCameraValue(long lUserID,SDK_CameraInfo *vCameraList,int nCameraNum);

	bool _stdcall SZY_VIEW_GetCameraInfoValue(long lUserID,int nIndex,SDK_CameraInfo &vCameraInfo);
	
	//实例化视频播放对象(保持旧的能浏览，但是不回调数据)
	long _stdcall SZY_VIEW_InitRealPlay(long lUserID,void *pDataCallBack,void *pCalled);

	//要实现数据回调，则要调用本接口进行初始化
	long _stdcall SZY_VIEW_InitRealPlayEX(long lUserID,void *pDataCallBack,VideoType nVideoType,void *pCalled);

	//开始实时播放视频（lUserID同上，sRemoteIP，nRemotePort为视频服务器IP跟端口，pPlayWnd为播放的窗口对象，sCameraID为摄像头的ID）
	bool _stdcall SZY_VIEW_RealPlay(long lUserID,long lRealPlayID,char *sRemoteIP,int nRemotePort,void *pPlayWnd,char *sCameraID);
	
	//停止实时播放（lUserID同上）
	void _stdcall SZY_VIEW_StopRealPlay(long lUserID,long lRealPlayID);
	
	//云台控制（lUserID同上，bModules为功能模块标识，bPTZCommand为当前操作命令，bValue当前操作值）
	bool _stdcall SZY_VIEW_PTZControl(long lUserID,long lRealPlayID,char bModules,char bPTZCommand,char bValue); 

	//开启讲话功能（lUserID同上）
	bool _stdcall SZY_VIEW_OpenAudioTalk(long lUserID,long lRealPlayID);

	//关闭讲话功能（lUserID同上）
	bool _stdcall SZY_VIEW_CloseAudioTalk(long lUserID,long lRealPlayID);

	//开启语音收听功能（lUserID同上）
	bool _stdcall SZY_VIEW_OpenAudioListen(long lUserID,long lRealPlayID);
	
	//关闭语音收听功能（lUserID同上）
	bool _stdcall SZY_VIEW_CloseAudioListen(long lUserID,long lRealPlayID);

	//录像回放初始化（lUserID同上）,返回录像回放对象句柄:lPBID

	long _stdcall SZY_VIEW_InitPlayBack(long lUserID,void *hPlayWnd,ProcStatus *pCallback,void *lpCalled);
	//释放录像回放句柄
	void _stdcall SZY_VIEW_ReleasePlayBack(long lPBID);
	//设置要回放的摄像头信息
	bool _stdcall SZY_VIEW_SetPBCameraInfo(long lPBID,char *strNodeID,char *strCameraID,char *strRemoteIP,int nRemotePort);
	
	//设置播放类型，以及文件存储路径（AVI文件以及抓图）
	bool _stdcall SZY_VIEW_SetPlayParam(long lPBID,long nType,char *sFileDir);

	//开始播放指定时间区域的录像文件,时间格式为:2012-02-04 09:05:00
	bool _stdcall SZY_VIEW_StartPlayBack(long lPBID,char * strOriStartTime, char * strOriStopTime);

	//停止回放
	bool _stdcall SZY_VIEW_StopPlayBack(long lPBID);

	//暂停、恢复回放
	bool _stdcall SZY_VIEW_Pause(long lPBID,bool bOperate);

	//改变播放的速度
	bool _stdcall SZY_VIEW_ChangeSpeed(long lPBID,bool bOperate);

	//抓图
	bool _stdcall SZY_VIEW_Capture(long lPBID);

	//拖动滚动条
	bool _stdcall SZY_VIEW_HScroll(long lPBID,long nPos);
	
	//录像文件实时保存为AVI文件
	bool _stdcall SZY_VIEW_SaveToAVI(long lPBID,bool bOperate);

#ifdef __cplusplus 
	
}

#endif

#endif

