#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <mmsystem.h>
#include <commctrl.h>//设置音量
#include "resource.h"
#include "MainDlg.h"
#include "shellapi.h"//上网的头文件
#define  Pause FALSE
#define  Play  TRUE
#define LOADPATH "C:\\list.txt"
//////////与皮肤有关/////////
#include "SkinMagicLib.h"
#pragma comment(lib, "SkinMagicTrial.lib")
/////////////////////
#include "shellapi.h"//NOTIFYICONDATA库
#define WM_MYMESSAGE WM_USER+100//自定义消息

void Minimized(HWND hwnd,int flag); //声明最小化窗口函数

/////////////////全局变量/////////////
bool PlayMusic_or_PauseMusic=Play;//定义bool类型 用来实现播放/暂停一个按钮的功能
TCHAR songLength[256];//歌曲总时间 跟歌曲进度条有关
TCHAR szFile[MAX_PATH];//设置文件名字符数组
TCHAR ShortName[256];//歌曲的简短路径
int curListIndex;//当前歌曲标号
int shensuo=0; //控制窗口的伸缩，0表示不伸缩，1表示伸缩 
TCHAR curFile[MAX_PATH];

int ListIndex;
VERIFY(InitSkinMagicLib(AfxGetInstanceHandle(), "MonitorDir", NULL, NULL));
//////////////////////////////
/************************************************************************/
/* mp3歌曲信息标签ID3V1，mp3文件尾部128个字节存储下列信息               */
/************************************************************************/

///////////////调用函数//////////////

void CALLBACK MyTimer(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime)/*时间函数*/
{
	SYSTEMTIME stLocal;
	GetLocalTime(&stLocal);
	TCHAR strTime[256];
	wsprintf(strTime,"%02d:%02d:%02d",stLocal.wHour,stLocal.wMinute,stLocal.wSecond);
    SetDlgItemText(hwnd,IDC_TIME,strTime);
}
/*
void ShowCurPlay(HWND hwnd)//显示当前播放的曲目
{
	TCHAR PlayInf[256]; 
	wsprintf(PlayInf,"当前曲目   %s",szFile);
	SetWindowText(GetDlgItem(hwnd,IDC_PLAYINFOR),TEXT(PlayInf));
}
*/
////////////最小化到托盘函数///////////
void Minimized(HWND hwnd,int flag)
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = IDI_ICON1;
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nid.uCallbackMessage = WM_MYMESSAGE;//自定义消息
	HINSTANCE hin = (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE);//获得程序句柄
	nid.hIcon = LoadIcon(hin,MAKEINTRESOURCE(IDI_ICON1));
	lstrcpy(nid.szTip,TEXT("依稀播放器"));             //信息提示条
	switch(flag)
	{
	case 0:
		{//添加托盘图标，隐藏窗口
			Shell_NotifyIcon(NIM_ADD,&nid);
			ShowWindow(hwnd,SW_HIDE);//隐藏窗口
		}
		break;
	case 1:
		{//删除托盘图标
			ShowWindow(hwnd,SW_SHOWNORMAL);
			Shell_NotifyIcon(NIM_DELETE,&nid);
			SetForegroundWindow(hwnd);
		}
		break;
	default:
		break;
	}	
}

//////////////音量//////////////

/////////////网络////////////////////////////

/////////////和歌曲有关的按钮函数/////////////////

//////////////////获得歌曲名字///////////

/////////////////动态显示//////////////


void PlayMusic(HWND hwnd)
{
	TCHAR shortPath[MAX_PATH];
	TCHAR cmd[MAX_PATH+30];
	ZeroMemory(curFile,sizeof(curFile));//每次添加前将CurFile清零，便于后面的操作
	wsprintf(curFile,"%s",szFile);
	SetDlgItemText(hwnd,IDC_PLAYINFOR,curFile);
	GetShortPathName(curFile,shortPath,sizeof(shortPath));
	wsprintf(cmd,"play %s",shortPath);
	mciSendString(cmd,"",0,NULL);

	
	SetTimer(hwnd,1,1000,MyTimer);
}

/**************************** StopMusic()************************************************/
void StopMusic(HWND hwnd)
{
	TCHAR shortPath[MAX_PATH];
	TCHAR cmd[MAX_PATH+30];
	GetShortPathName(curFile,shortPath,sizeof(shortPath));
	wsprintf(cmd,"stop %s",shortPath);
	mciSendString(cmd,"",0,NULL);
	SetWindowText(GetDlgItem(hwnd,IDC_PLAY),TEXT("OK"));
    SetWindowText(GetDlgItem(hwnd,IDC_CUR),TEXT("停止播放"));
}

////////////////////////////////////
void PauseMusic(HWND hwnd)
{
	TCHAR shortPath[MAX_PATH];
	TCHAR cmd[MAX_PATH+30];
	GetShortPathName(curFile,shortPath,sizeof(shortPath));
	wsprintf(cmd,"pause %s",shortPath);
	mciSendString(cmd,"",0,NULL);

}

/**************************** UpMusic()************************************************/
void UpMusic(HWND hwnd)
{
	StopMusic(hwnd);
	HWND hwndList=GetDlgItem(hwnd,IDC_LIST1);
	ListIndex=ListBox_GetCurSel(hwndList);
	if(0==ListIndex)
	{
		PlayMusic(hwnd);
	}
	else
	{
		ListIndex--;
		ListBox_SetCurSel(hwndList,ListIndex);
		ZeroMemory(szFile,sizeof(szFile));
		ListBox_GetText(hwndList,ListIndex,szFile);
		PlayMusic(hwnd);
	}
	PlayMusic(hwnd);	
	PlayMusic_or_PauseMusic=Pause;
	SetWindowText(GetDlgItem(hwnd,IDC_PLAY),TEXT("||")); 
	SetWindowText(GetDlgItem(hwnd,IDC_CUR),TEXT("播放中"));
}

/**************************** DownMusic()************************************************/
void DownMusic(HWND hwnd)
{
	StopMusic(hwnd);
	HWND hwndList=GetDlgItem(hwnd,IDC_LIST1);
	ListIndex=ListBox_GetCurSel(hwndList);
	int ListCount=ListBox_GetCount(hwndList);
	if(ListIndex==ListCount)
	{
		PlayMusic(hwnd);
	}
	else
	{
		ListIndex++;
		ListBox_SetCurSel(hwndList,ListIndex);
		ZeroMemory(szFile,sizeof(szFile));
		ListBox_GetText(hwndList,ListIndex,szFile);
		PlayMusic(hwnd);
	}
	PlayMusic(hwnd);	
	PlayMusic_or_PauseMusic=Pause;
	SetWindowText(GetDlgItem(hwnd,IDC_PLAY),TEXT("||")); 
	SetWindowText(GetDlgItem(hwnd,IDC_CUR),TEXT("播放中"));
}

void Add_Music(HWND hwnd)//添加歌曲
{
	OPENFILENAME ofn;//文件打开结构
	TCHAR szOpenFileName[MAX_PATH * 80];//要打开文件的文件名
	TCHAR szPath[MAX_PATH];//文件路径
	TCHAR szFileName[MAX_PATH * 80];
	TCHAR *p;//用于遍历文件
	int len = 0;
	ZeroMemory(&ofn , sizeof(ofn));//对文件结构清零
	ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;//文件打开的风格
	ofn.lStructSize = sizeof(ofn);//文件结构的大小
	ofn.lpstrFile = szOpenFileName;//文件名
	ofn.nMaxFile = sizeof(szOpenFileName);//要打开的文件的大小
	ofn.lpstrFile[0] = '\0';//文件结尾的标志
	ofn.lpstrFilter = TEXT("mp3\0*.mp3\0WMA\0*.wma\0所有文件\0*.*\0");//过滤器
	if(GetOpenFileName(&ofn))
	{
			
		lstrcpyn(szPath, szOpenFileName, ofn.nFileOffset);	/*把第一个文件名复制到szPath如果只选了一个文件,就复制到最后一个'\'
			如果选了多个文件,就复制到第一个NULL字符,当只选了一个文件时,下面的NULL是必须的	这是不区别待选了一个和多个文件情况	*/					
		szPath[ofn.nFileOffset] = '\0';
		len = lstrlen(szPath);
		if(szPath[len - 1] != '\\')
		{
			lstrcat(szPath,TEXT("\\"));
		}
		p = szOpenFileName + ofn.nFileOffset;
		ZeroMemory(szFileName, sizeof(szFileName));
		HWND hwndList = GetDlgItem(hwnd, IDC_LIST1);
		while(*p)
		{
			lstrcat(szFileName, szPath);//给文件名加上路径
			//lstrcat(szFileName, p);//加上文件名
			ListBox_InsertString(hwndList, -1, p);
			lstrcat(szFileName, TEXT("\n"));//换行
			p += lstrlen(p) + 1;//移到下一个文件
		}
	}
}

void Delete_Music(HWND hwnd)//删除歌曲
{
	TCHAR line[MAX_PATH];//保存歌曲
	HWND hwndList = GetDlgItem(hwnd, IDC_LIST1);//得到ListBox的句柄	
	int CurSel = ListBox_GetCurSel(hwndList);//得到Listbox的索引	
	ListBox_GetText(hwndList, CurSel, line);//得到选定的歌曲	
	TCHAR ShortPath[MAX_PATH];//储存文件的短路径	
	TCHAR cmd[MAX_PATH + 5];//储存: 文件名 + 操作	
	GetShortPathName(line, ShortPath, sizeof(ShortPath));//得到文件的段路径	
	wsprintf(cmd,"stop %s",ShortPath);//将文件的段路径和操作打印到 cmd中	
	mciSendString(cmd, "", 0, NULL);//停止	
	ListBox_DeleteString(hwndList, CurSel);//删除歌曲
}


//////////////////////////音量函数///////////////////

////////////////////////////
void showmode(int i,HWND hwnd)	//设置菜单中的显示模式标示 
{
      HMENU hemnu,hsubmenu;
      hemnu=GetMenu(hwnd);
  	  hsubmenu=GetSubMenu(hemnu,0);
	switch (i)
	{
	  case 0:  
		  {CheckMenuItem(hsubmenu,IDM_NORMAL,MF_UNCHECKED);
  	       CheckMenuItem(hsubmenu,IDM_MINI,MF_CHECKED);
		  }break;	  
	  case 1:
		  {CheckMenuItem(hsubmenu,IDM_MINI,MF_UNCHECKED);
   	       CheckMenuItem(hsubmenu,IDM_NORMAL,MF_CHECKED);
		  }break;	  
	  default:
	  break;
	}

}

BOOL WINAPI Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {   
        HANDLE_MSG(hWnd, WM_INITDIALOG, Main_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Main_OnCommand);
		HANDLE_MSG(hWnd,WM_CLOSE, Main_OnClose);
    	case WM_MYMESSAGE:
			{
				if(lParam == WM_LBUTTONDBLCLK)
				{//在托盘图标上双击					
					Minimized(hWnd,1);//删除托盘图标					
				}
				else if(lParam == WM_RBUTTONDOWN)
				{//在托盘图标上右击					
					Minimized(hWnd,1);//删除托盘图标				
				}
			}
			break; 
	}
    return FALSE;
}

///////////////窗口初始化////////////////
BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{   ///////定时器///////
    SetTimer(hwnd,NULL,1000,MyTimer);    
    //////图标//////////
	HICON hIcon=LoadIcon((HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(hwnd,WM_SETICON,TRUE,(LPARAM)hIcon);
	SendMessage(hwnd,WM_SETICON,FALSE,(LPARAM)hIcon);
   
	HWND hwndCombo = GetDlgItem(hwnd, IDC_CURPLAYS);//得到组合框的句柄
    SetWindowText(GetDlgItem(hwnd,IDC_CURPLAYS),TEXT("顺序播放"));//初始化播放模式
	ComboBox_InsertString(hwndCombo, -1, TEXT("单曲播放"));
	ComboBox_InsertString(hwndCombo, -1, TEXT("随机播放"));
	ComboBox_InsertString(hwndCombo, -1, TEXT("循环播放"));

/////////////////初始化音量//////////////////////

///////////////////////////////未实现/////////////////////
	//载入播放列表

/////////////子对话框/////////////////
//	SetDlgItemText(hwnd,IDM_NORMAL,"|>");

//////初始化网络//////////////
//	ComboBox_SetCurSel(ComboHwnd,0);//默认为顺序播放
//	SkinH_Attach();
	HWND hwnd_http = GetDlgItem(hwnd,IDC_COMHTTP);
	ComboBox_InsertString(hwnd_http,-1,TEXT("百度音乐"));
	ComboBox_InsertString(hwnd_http,-1,TEXT("新浪音乐 "));
	ComboBox_InsertString(hwnd_http,-1,TEXT("一听音乐 "));
	ComboBox_InsertString(hwnd_http,-1,TEXT("酷狗音乐"));
	ComboBox_InsertString(hwnd_http,-1,TEXT("音悦台"));
	ComboBox_InsertString(hwnd_http,-1,TEXT("qq音乐"));
	ComboBox_InsertString(hwnd_http,-1,TEXT("搜狗音乐"));
	ComboBox_InsertString(hwnd_http,-1,TEXT("我要听音乐"));
	ComboBox_InsertString(hwnd_http,-1,TEXT("极品音乐论坛"));
	ComboBox_SetCurSel(hwnd_http,3);//默认为酷狗音乐
	return TRUE;
}

void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{  	
  	HWND hwndList=GetDlgItem(hwnd,IDC_LIST1);
	switch(id)
    {   
		case IDC_LIST1:
		{
			switch(codeNotify)
			{
			case LBN_DBLCLK:          //设置szFile和curFile的顺序（只在PlayMusic()中将szFile写入curFile中），
				{                     //解决了添加新文件，双击新文件，而上一首仍未关闭的问题
					ListIndex=ListBox_GetCurSel(hwndList);
					StopMusic(hwnd);
                    StopMusic(hwnd);
					ZeroMemory(szFile,sizeof(szFile));
					ListBox_SetCurSel(hwndList,ListIndex);
					ListBox_GetText(hwndList,ListIndex,szFile);
				    PlayMusic(hwnd);					
					PlayMusic_or_PauseMusic=Pause;
					SetWindowText(GetDlgItem(hwnd,IDC_PLAY),TEXT("||")); 
					SetWindowText(GetDlgItem(hwnd,IDC_CUR),TEXT("播放中"));
			        
				
				}
				break;
			default:
				break;
			}
		}
		break;
       case IDC_SEARCH2:  //百度搜歌
        {
        TCHAR musiconline[256];
		GetDlgItemText(hwnd,IDC_EDT5,musiconline,sizeof(musiconline)/sizeof(TCHAR));
		TCHAR musiconline1[256];
		wsprintf(musiconline1,"http://mp3.baidu.com/m?f=ms&tn=baidump3&ct=134217728&lf=&rn=&word=%s&lm=-1",musiconline);
        ShellExecute(hwnd,"open",musiconline1,NULL,NULL,SW_NORMAL);	
        }
        break;
/////////////////////////音悦台///////////////////////          
		case IDC_SEARCH1://搜索
		{
			HWND hwnd_http=GetDlgItem(hwnd,IDC_COMHTTP);
			int Http_CurSel=ComboBox_GetCurSel(hwnd_http);
			switch(Http_CurSel)
			{
			case 0://百度音乐
				{
					ShellExecute(hwnd,"open","http://mp3.baidu.com/",NULL,NULL,SW_SHOWNORMAL);
				}
				break;
			case 1://新浪音乐
				{
					ShellExecute(hwnd,"open","http://music.sina.com.cn/yueku/",NULL,NULL,SW_SHOWNORMAL);
				}
				break;
			case 2://一听音乐
				{
					ShellExecute(hwnd,"open","http://www.1ting.com",NULL,NULL,SW_SHOWNORMAL);
				}
				break;
			case 3://酷狗音乐
				{
					ShellExecute(hwnd,"open","http://www.kugou.com/",NULL,NULL,SW_SHOWNORMAL);
				}
				break;
			case 4://音悦台
				{
					ShellExecute(hwnd,"open","http://www.yinyuetai.com/",NULL,NULL,SW_SHOWNORMAL);
				}
				break;
			case 5://qq音乐
				{
					ShellExecute(hwnd,"open","http://music.qq.com/",NULL,NULL,SW_SHOWNORMAL);
				}
				break;
			case 6://搜狗音乐
				{
					ShellExecute(hwnd,"open","http://mp3.sogou.com/",NULL,NULL,SW_SHOWNORMAL);
				}
				break;
			case 7://我要听音乐网
				{
					ShellExecute(hwnd,"open","http://www.51t.net",NULL,NULL,SW_SHOWNORMAL);
				}
				break;
			case 8://极品音乐论坛
				{
					ShellExecute(hwnd,"open","http://club.xialala.com",NULL,NULL,SW_SHOWNORMAL);
				}
				break;
			}
		}	
		break;
			case IDC_BAIDU:
				{
				   ShellExecute(hwnd,"open","http://www.baidu.com",NULL,NULL,SW_SHOWNORMAL);
				}break;
			case IDC_SOUGOU:
				{
				   ShellExecute(hwnd,"open","http://www.sogou.com",NULL,NULL,SW_SHOWNORMAL);
				}break;
			case IDC_YAHU:
				{
				   ShellExecute(hwnd,"open","http://cn.yahoo.com",NULL,NULL,SW_SHOWNORMAL);
				}break;
			case IDC_YOUKU:
				{
				   ShellExecute(hwnd,"open","http://www.youku.com",NULL,NULL,SW_SHOWNORMAL);
				}break;
			case IDC_TAOBAO:
				{
				   ShellExecute(hwnd,"open","http://www.taobao.com",NULL,NULL,SW_SHOWNORMAL);
				}break;
			case IDC_NGD:
				{
					ShellExecute(hwnd,"open","http://www.imut.edu.cn",NULL,NULL,SW_SHOWNORMAL);
				}break;
////////////////////////////////播放视频未实现/////////////////////////////////////////
			    
/////////////// 窗口模式////////////
    	case IDM_MINI ://设置迷你显示模式 
			{	SetWindowPos(hwnd,0,150,200,360,180,SWP_NOMOVE|SWP_NOZORDER);
				showmode(0,hwnd);   //设置设置中迷你模式的标签 
			}break;	
		
		case IDM_NORMAL://设置正常显示模式 
            {
				SetWindowPos(hwnd,0,150,200,500,500,SWP_NOMOVE|SWP_NOZORDER);
				showmode(1,hwnd);
			}break;
		
//////////////音量//////////////////

/////////////静音///////////////////
  		case IDC_CHECK:  // 利用CheckBox来设置是否静音
		 { 
            HWND hwCheck = GetDlgItem(hwnd,IDC_CHECK);//得到CheckBox的句柄
			HWND hwndList = GetDlgItem(hwnd,IDC_LIST1);//定义ListBox的句柄
			int CurSel = ListBox_GetCurSel(hwndList);//得到ListBox的索引	
			TCHAR line[MAX_PATH];//保存歌曲
			ListBox_GetText(hwndList, CurSel, line);//得到选定的歌曲
			TCHAR ShortPath[MAX_PATH];//储存文件的短路径
			TCHAR cmd[MAX_PATH + 5];	//储存: 文件名 + 操作
			GetShortPathName(line, ShortPath, sizeof(ShortPath));//得到文件的段路径	

			if(BST_CHECKED == Button_GetCheck(hwCheck))  // 如果被选中，则静音
			{
				wsprintf(cmd, TEXT("setaudio %s off"), ShortPath);
				mciSendString(cmd, NULL, 0, NULL);
			}
			if (BST_UNCHECKED == Button_GetCheck(hwCheck))  // 未被选中，回复正常
			{
				wsprintf(cmd, TEXT("setaudio %s on"), ShortPath);
				mciSendString(cmd, NULL, 0, NULL);
			}
		}
			break;
/////////////////////////////播放控制////////////////////////////////////
		case IDC_PLAY://播放/暂停
			{	if(PlayMusic_or_PauseMusic==Play)
					{
						PlayMusic(hwnd);	
						PlayMusic_or_PauseMusic=Pause;
						SetWindowText(GetDlgItem(hwnd,IDC_PLAY),TEXT("||")); 
						SetWindowText(GetDlgItem(hwnd,IDC_CUR),TEXT("播放中"));	
					}
					else if(PlayMusic_or_PauseMusic==Pause)
					{							
						PauseMusic(hwnd);
						PlayMusic_or_PauseMusic=Play;
						SetWindowText(GetDlgItem(hwnd,IDC_PLAY),TEXT("OK")); 
						SetWindowText(GetDlgItem(hwnd,IDC_CUR),TEXT("暂停"));
					}

			}
		     break;	
		case IDC_ADD://添加歌曲
			{		
				Add_Music(hwnd);
			}
			break;		
		case IDC_DELETE://删除歌曲
			{
				Delete_Music(hwnd);
			}
			break;		
		case IDC_STOP://停止
			{
				StopMusic(hwnd);
			}
			break;	    
		case IDC_UPMUSIC://上一曲
			{
				UpMusic(hwnd);
			}
			break;
		case IDC_DOWNMUSIC:	//下一曲
			{
				DownMusic(hwnd);
			}
			break;
    	case IDC_DELDETALL://清空列表歌曲
				{
					int ilistNumber=0;
					HWND hwndList=GetDlgItem(hwnd,IDC_LIST1);//获得列表句柄
					StopMusic(hwnd);
					ilistNumber=ListBox_GetCount(hwndList);
					for (int i=0;i<ilistNumber;i++)
					{
						 SendDlgItemMessage(hwnd,IDC_LIST1,LB_DELETESTRING,0,0);//清空列表
					}
				}
				break;
//////////////菜单/////////////////
	case ID_MENUABOUT:
		{
			MessageBox(hwnd,TEXT("欢迎是使用播放器2.1版本！QQ:1369873960 依稀"),"关于",MB_OK);
		}break;
    case ID_MENUAUTHOR:
		{
			MessageBox(hwnd,TEXT("作者：逯效亭"),TEXT("QQ:1369873930"),MB_OK);
		}break;    	
    case ID_MCLOSEW://显示器休眠
		{
			SendMessage(hwnd,WM_SYSCOMMAND,SC_MONITORPOWER,2);
		}
		break;
    case ID_MCLOSEPC://关机                     
		{
		 int II=MessageBox(hwnd,TEXT("确认关机吗？"),TEXT("询问"),MB_YESNO|MB_ICONQUESTION);
			if(II==6)
			{
				system("shutdown -s -t 0");
			}
		}break;
    case ID_MENUQUIT:
		{ 
			EndDialog(hwnd, 0);
		}break;
	case ID_TANCHU:
		{
			mciSendString("set cdaudio door open",NULL,0,0);   
		}
		break;
        default:
		break;
    }
}
///////////////////////////////////////
void Main_OnClose(HWND hwnd)
{   
    int ret=MessageBox(hwnd,TEXT("最小化到后台播放吗？"),"提示",MB_YESNOCANCEL|MB_ICONQUESTION);
	if(IDYES==ret)
	{
        Minimized(hwnd,0);
	}
	else if(IDNO==ret)
    {   

		////////////保存列表歌曲未实现/////////
		EndDialog(hwnd, 0);
	}
    
}