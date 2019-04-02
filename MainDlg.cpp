#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <mmsystem.h>
#include <commctrl.h>//��������
#include "resource.h"
#include "MainDlg.h"
#include "shellapi.h"//������ͷ�ļ�
#define  Pause FALSE
#define  Play  TRUE
#define LOADPATH "C:\\list.txt"
//////////��Ƥ���й�/////////
#include "SkinMagicLib.h"
#pragma comment(lib, "SkinMagicTrial.lib")
/////////////////////
#include "shellapi.h"//NOTIFYICONDATA��
#define WM_MYMESSAGE WM_USER+100//�Զ�����Ϣ

void Minimized(HWND hwnd,int flag); //������С�����ں���

/////////////////ȫ�ֱ���/////////////
bool PlayMusic_or_PauseMusic=Play;//����bool���� ����ʵ�ֲ���/��ͣһ����ť�Ĺ���
TCHAR songLength[256];//������ʱ�� �������������й�
TCHAR szFile[MAX_PATH];//�����ļ����ַ�����
TCHAR ShortName[256];//�����ļ��·��
int curListIndex;//��ǰ�������
int shensuo=0; //���ƴ��ڵ�������0��ʾ��������1��ʾ���� 
TCHAR curFile[MAX_PATH];

int ListIndex;
VERIFY(InitSkinMagicLib(AfxGetInstanceHandle(), "MonitorDir", NULL, NULL));
//////////////////////////////
/************************************************************************/
/* mp3������Ϣ��ǩID3V1��mp3�ļ�β��128���ֽڴ洢������Ϣ               */
/************************************************************************/

///////////////���ú���//////////////

void CALLBACK MyTimer(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime)/*ʱ�亯��*/
{
	SYSTEMTIME stLocal;
	GetLocalTime(&stLocal);
	TCHAR strTime[256];
	wsprintf(strTime,"%02d:%02d:%02d",stLocal.wHour,stLocal.wMinute,stLocal.wSecond);
    SetDlgItemText(hwnd,IDC_TIME,strTime);
}
/*
void ShowCurPlay(HWND hwnd)//��ʾ��ǰ���ŵ���Ŀ
{
	TCHAR PlayInf[256]; 
	wsprintf(PlayInf,"��ǰ��Ŀ   %s",szFile);
	SetWindowText(GetDlgItem(hwnd,IDC_PLAYINFOR),TEXT(PlayInf));
}
*/
////////////��С�������̺���///////////
void Minimized(HWND hwnd,int flag)
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = IDI_ICON1;
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nid.uCallbackMessage = WM_MYMESSAGE;//�Զ�����Ϣ
	HINSTANCE hin = (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE);//��ó�����
	nid.hIcon = LoadIcon(hin,MAKEINTRESOURCE(IDI_ICON1));
	lstrcpy(nid.szTip,TEXT("��ϡ������"));             //��Ϣ��ʾ��
	switch(flag)
	{
	case 0:
		{//�������ͼ�꣬���ش���
			Shell_NotifyIcon(NIM_ADD,&nid);
			ShowWindow(hwnd,SW_HIDE);//���ش���
		}
		break;
	case 1:
		{//ɾ������ͼ��
			ShowWindow(hwnd,SW_SHOWNORMAL);
			Shell_NotifyIcon(NIM_DELETE,&nid);
			SetForegroundWindow(hwnd);
		}
		break;
	default:
		break;
	}	
}

//////////////����//////////////

/////////////����////////////////////////////

/////////////�͸����йصİ�ť����/////////////////

//////////////////��ø�������///////////

/////////////////��̬��ʾ//////////////


void PlayMusic(HWND hwnd)
{
	TCHAR shortPath[MAX_PATH];
	TCHAR cmd[MAX_PATH+30];
	ZeroMemory(curFile,sizeof(curFile));//ÿ�����ǰ��CurFile���㣬���ں���Ĳ���
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
    SetWindowText(GetDlgItem(hwnd,IDC_CUR),TEXT("ֹͣ����"));
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
	SetWindowText(GetDlgItem(hwnd,IDC_CUR),TEXT("������"));
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
	SetWindowText(GetDlgItem(hwnd,IDC_CUR),TEXT("������"));
}

void Add_Music(HWND hwnd)//��Ӹ���
{
	OPENFILENAME ofn;//�ļ��򿪽ṹ
	TCHAR szOpenFileName[MAX_PATH * 80];//Ҫ���ļ����ļ���
	TCHAR szPath[MAX_PATH];//�ļ�·��
	TCHAR szFileName[MAX_PATH * 80];
	TCHAR *p;//���ڱ����ļ�
	int len = 0;
	ZeroMemory(&ofn , sizeof(ofn));//���ļ��ṹ����
	ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;//�ļ��򿪵ķ��
	ofn.lStructSize = sizeof(ofn);//�ļ��ṹ�Ĵ�С
	ofn.lpstrFile = szOpenFileName;//�ļ���
	ofn.nMaxFile = sizeof(szOpenFileName);//Ҫ�򿪵��ļ��Ĵ�С
	ofn.lpstrFile[0] = '\0';//�ļ���β�ı�־
	ofn.lpstrFilter = TEXT("mp3\0*.mp3\0WMA\0*.wma\0�����ļ�\0*.*\0");//������
	if(GetOpenFileName(&ofn))
	{
			
		lstrcpyn(szPath, szOpenFileName, ofn.nFileOffset);	/*�ѵ�һ���ļ������Ƶ�szPath���ֻѡ��һ���ļ�,�͸��Ƶ����һ��'\'
			���ѡ�˶���ļ�,�͸��Ƶ���һ��NULL�ַ�,��ֻѡ��һ���ļ�ʱ,�����NULL�Ǳ����	���ǲ������ѡ��һ���Ͷ���ļ����	*/					
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
			lstrcat(szFileName, szPath);//���ļ�������·��
			//lstrcat(szFileName, p);//�����ļ���
			ListBox_InsertString(hwndList, -1, p);
			lstrcat(szFileName, TEXT("\n"));//����
			p += lstrlen(p) + 1;//�Ƶ���һ���ļ�
		}
	}
}

void Delete_Music(HWND hwnd)//ɾ������
{
	TCHAR line[MAX_PATH];//�������
	HWND hwndList = GetDlgItem(hwnd, IDC_LIST1);//�õ�ListBox�ľ��	
	int CurSel = ListBox_GetCurSel(hwndList);//�õ�Listbox������	
	ListBox_GetText(hwndList, CurSel, line);//�õ�ѡ���ĸ���	
	TCHAR ShortPath[MAX_PATH];//�����ļ��Ķ�·��	
	TCHAR cmd[MAX_PATH + 5];//����: �ļ��� + ����	
	GetShortPathName(line, ShortPath, sizeof(ShortPath));//�õ��ļ��Ķ�·��	
	wsprintf(cmd,"stop %s",ShortPath);//���ļ��Ķ�·���Ͳ�����ӡ�� cmd��	
	mciSendString(cmd, "", 0, NULL);//ֹͣ	
	ListBox_DeleteString(hwndList, CurSel);//ɾ������
}


//////////////////////////��������///////////////////

////////////////////////////
void showmode(int i,HWND hwnd)	//���ò˵��е���ʾģʽ��ʾ 
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
				{//������ͼ����˫��					
					Minimized(hWnd,1);//ɾ������ͼ��					
				}
				else if(lParam == WM_RBUTTONDOWN)
				{//������ͼ�����һ�					
					Minimized(hWnd,1);//ɾ������ͼ��				
				}
			}
			break; 
	}
    return FALSE;
}

///////////////���ڳ�ʼ��////////////////
BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{   ///////��ʱ��///////
    SetTimer(hwnd,NULL,1000,MyTimer);    
    //////ͼ��//////////
	HICON hIcon=LoadIcon((HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(hwnd,WM_SETICON,TRUE,(LPARAM)hIcon);
	SendMessage(hwnd,WM_SETICON,FALSE,(LPARAM)hIcon);
   
	HWND hwndCombo = GetDlgItem(hwnd, IDC_CURPLAYS);//�õ���Ͽ�ľ��
    SetWindowText(GetDlgItem(hwnd,IDC_CURPLAYS),TEXT("˳�򲥷�"));//��ʼ������ģʽ
	ComboBox_InsertString(hwndCombo, -1, TEXT("��������"));
	ComboBox_InsertString(hwndCombo, -1, TEXT("�������"));
	ComboBox_InsertString(hwndCombo, -1, TEXT("ѭ������"));

/////////////////��ʼ������//////////////////////

///////////////////////////////δʵ��/////////////////////
	//���벥���б�

/////////////�ӶԻ���/////////////////
//	SetDlgItemText(hwnd,IDM_NORMAL,"|>");

//////��ʼ������//////////////
//	ComboBox_SetCurSel(ComboHwnd,0);//Ĭ��Ϊ˳�򲥷�
//	SkinH_Attach();
	HWND hwnd_http = GetDlgItem(hwnd,IDC_COMHTTP);
	ComboBox_InsertString(hwnd_http,-1,TEXT("�ٶ�����"));
	ComboBox_InsertString(hwnd_http,-1,TEXT("�������� "));
	ComboBox_InsertString(hwnd_http,-1,TEXT("һ������ "));
	ComboBox_InsertString(hwnd_http,-1,TEXT("�ṷ����"));
	ComboBox_InsertString(hwnd_http,-1,TEXT("����̨"));
	ComboBox_InsertString(hwnd_http,-1,TEXT("qq����"));
	ComboBox_InsertString(hwnd_http,-1,TEXT("�ѹ�����"));
	ComboBox_InsertString(hwnd_http,-1,TEXT("��Ҫ������"));
	ComboBox_InsertString(hwnd_http,-1,TEXT("��Ʒ������̳"));
	ComboBox_SetCurSel(hwnd_http,3);//Ĭ��Ϊ�ṷ����
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
			case LBN_DBLCLK:          //����szFile��curFile��˳��ֻ��PlayMusic()�н�szFileд��curFile�У���
				{                     //�����������ļ���˫�����ļ�������һ����δ�رյ�����
					ListIndex=ListBox_GetCurSel(hwndList);
					StopMusic(hwnd);
                    StopMusic(hwnd);
					ZeroMemory(szFile,sizeof(szFile));
					ListBox_SetCurSel(hwndList,ListIndex);
					ListBox_GetText(hwndList,ListIndex,szFile);
				    PlayMusic(hwnd);					
					PlayMusic_or_PauseMusic=Pause;
					SetWindowText(GetDlgItem(hwnd,IDC_PLAY),TEXT("||")); 
					SetWindowText(GetDlgItem(hwnd,IDC_CUR),TEXT("������"));
			        
				
				}
				break;
			default:
				break;
			}
		}
		break;
       case IDC_SEARCH2:  //�ٶ��Ѹ�
        {
        TCHAR musiconline[256];
		GetDlgItemText(hwnd,IDC_EDT5,musiconline,sizeof(musiconline)/sizeof(TCHAR));
		TCHAR musiconline1[256];
		wsprintf(musiconline1,"http://mp3.baidu.com/m?f=ms&tn=baidump3&ct=134217728&lf=&rn=&word=%s&lm=-1",musiconline);
        ShellExecute(hwnd,"open",musiconline1,NULL,NULL,SW_NORMAL);	
        }
        break;
/////////////////////////����̨///////////////////////          
		case IDC_SEARCH1://����
		{
			HWND hwnd_http=GetDlgItem(hwnd,IDC_COMHTTP);
			int Http_CurSel=ComboBox_GetCurSel(hwnd_http);
			switch(Http_CurSel)
			{
			case 0://�ٶ�����
				{
					ShellExecute(hwnd,"open","http://mp3.baidu.com/",NULL,NULL,SW_SHOWNORMAL);
				}
				break;
			case 1://��������
				{
					ShellExecute(hwnd,"open","http://music.sina.com.cn/yueku/",NULL,NULL,SW_SHOWNORMAL);
				}
				break;
			case 2://һ������
				{
					ShellExecute(hwnd,"open","http://www.1ting.com",NULL,NULL,SW_SHOWNORMAL);
				}
				break;
			case 3://�ṷ����
				{
					ShellExecute(hwnd,"open","http://www.kugou.com/",NULL,NULL,SW_SHOWNORMAL);
				}
				break;
			case 4://����̨
				{
					ShellExecute(hwnd,"open","http://www.yinyuetai.com/",NULL,NULL,SW_SHOWNORMAL);
				}
				break;
			case 5://qq����
				{
					ShellExecute(hwnd,"open","http://music.qq.com/",NULL,NULL,SW_SHOWNORMAL);
				}
				break;
			case 6://�ѹ�����
				{
					ShellExecute(hwnd,"open","http://mp3.sogou.com/",NULL,NULL,SW_SHOWNORMAL);
				}
				break;
			case 7://��Ҫ��������
				{
					ShellExecute(hwnd,"open","http://www.51t.net",NULL,NULL,SW_SHOWNORMAL);
				}
				break;
			case 8://��Ʒ������̳
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
////////////////////////////////������Ƶδʵ��/////////////////////////////////////////
			    
/////////////// ����ģʽ////////////
    	case IDM_MINI ://����������ʾģʽ 
			{	SetWindowPos(hwnd,0,150,200,360,180,SWP_NOMOVE|SWP_NOZORDER);
				showmode(0,hwnd);   //��������������ģʽ�ı�ǩ 
			}break;	
		
		case IDM_NORMAL://����������ʾģʽ 
            {
				SetWindowPos(hwnd,0,150,200,500,500,SWP_NOMOVE|SWP_NOZORDER);
				showmode(1,hwnd);
			}break;
		
//////////////����//////////////////

/////////////����///////////////////
  		case IDC_CHECK:  // ����CheckBox�������Ƿ���
		 { 
            HWND hwCheck = GetDlgItem(hwnd,IDC_CHECK);//�õ�CheckBox�ľ��
			HWND hwndList = GetDlgItem(hwnd,IDC_LIST1);//����ListBox�ľ��
			int CurSel = ListBox_GetCurSel(hwndList);//�õ�ListBox������	
			TCHAR line[MAX_PATH];//�������
			ListBox_GetText(hwndList, CurSel, line);//�õ�ѡ���ĸ���
			TCHAR ShortPath[MAX_PATH];//�����ļ��Ķ�·��
			TCHAR cmd[MAX_PATH + 5];	//����: �ļ��� + ����
			GetShortPathName(line, ShortPath, sizeof(ShortPath));//�õ��ļ��Ķ�·��	

			if(BST_CHECKED == Button_GetCheck(hwCheck))  // �����ѡ�У�����
			{
				wsprintf(cmd, TEXT("setaudio %s off"), ShortPath);
				mciSendString(cmd, NULL, 0, NULL);
			}
			if (BST_UNCHECKED == Button_GetCheck(hwCheck))  // δ��ѡ�У��ظ�����
			{
				wsprintf(cmd, TEXT("setaudio %s on"), ShortPath);
				mciSendString(cmd, NULL, 0, NULL);
			}
		}
			break;
/////////////////////////////���ſ���////////////////////////////////////
		case IDC_PLAY://����/��ͣ
			{	if(PlayMusic_or_PauseMusic==Play)
					{
						PlayMusic(hwnd);	
						PlayMusic_or_PauseMusic=Pause;
						SetWindowText(GetDlgItem(hwnd,IDC_PLAY),TEXT("||")); 
						SetWindowText(GetDlgItem(hwnd,IDC_CUR),TEXT("������"));	
					}
					else if(PlayMusic_or_PauseMusic==Pause)
					{							
						PauseMusic(hwnd);
						PlayMusic_or_PauseMusic=Play;
						SetWindowText(GetDlgItem(hwnd,IDC_PLAY),TEXT("OK")); 
						SetWindowText(GetDlgItem(hwnd,IDC_CUR),TEXT("��ͣ"));
					}

			}
		     break;	
		case IDC_ADD://��Ӹ���
			{		
				Add_Music(hwnd);
			}
			break;		
		case IDC_DELETE://ɾ������
			{
				Delete_Music(hwnd);
			}
			break;		
		case IDC_STOP://ֹͣ
			{
				StopMusic(hwnd);
			}
			break;	    
		case IDC_UPMUSIC://��һ��
			{
				UpMusic(hwnd);
			}
			break;
		case IDC_DOWNMUSIC:	//��һ��
			{
				DownMusic(hwnd);
			}
			break;
    	case IDC_DELDETALL://����б����
				{
					int ilistNumber=0;
					HWND hwndList=GetDlgItem(hwnd,IDC_LIST1);//����б���
					StopMusic(hwnd);
					ilistNumber=ListBox_GetCount(hwndList);
					for (int i=0;i<ilistNumber;i++)
					{
						 SendDlgItemMessage(hwnd,IDC_LIST1,LB_DELETESTRING,0,0);//����б�
					}
				}
				break;
//////////////�˵�/////////////////
	case ID_MENUABOUT:
		{
			MessageBox(hwnd,TEXT("��ӭ��ʹ�ò�����2.1�汾��QQ:1369873960 ��ϡ"),"����",MB_OK);
		}break;
    case ID_MENUAUTHOR:
		{
			MessageBox(hwnd,TEXT("���ߣ���Чͤ"),TEXT("QQ:1369873930"),MB_OK);
		}break;    	
    case ID_MCLOSEW://��ʾ������
		{
			SendMessage(hwnd,WM_SYSCOMMAND,SC_MONITORPOWER,2);
		}
		break;
    case ID_MCLOSEPC://�ػ�                     
		{
		 int II=MessageBox(hwnd,TEXT("ȷ�Ϲػ���"),TEXT("ѯ��"),MB_YESNO|MB_ICONQUESTION);
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
    int ret=MessageBox(hwnd,TEXT("��С������̨������"),"��ʾ",MB_YESNOCANCEL|MB_ICONQUESTION);
	if(IDYES==ret)
	{
        Minimized(hwnd,0);
	}
	else if(IDNO==ret)
    {   

		////////////�����б����δʵ��/////////
		EndDialog(hwnd, 0);
	}
    
}