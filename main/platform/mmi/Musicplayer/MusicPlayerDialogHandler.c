/*==============================================================================
// 文件：
//        MusicPlayerDialogHandler.c
//        版权所有(c) 2009 Anylook Tech. CO.,LTD.
//        
// 文件说明：
// 
==============================================================================*/

/*==============================================================================
修改历史记录：
时       间      修 改 人     修改内容、位置及原因
-----------      ----------     -----------------------------------------------
==============================================================================*/


/*==============================================================================
                                 本文件包含的外部文件
==============================================================================*/
#include "MusicPlayer_priv.h"
#include "MediaGallery.h"

#ifndef WIN32
#include "Snd.h"
#endif

/*==============================================================================
                                 宏定义和常数
==============================================================================*/
// 画界面底部提示条宏定义
#define MP3_DRAW_BOTTOMBAR(x)                           \
{                                                   \
    BottomBar_Param_type BarParam;                  \
    MEMSET(&BarParam, 0, sizeof(BarParam));         \
    BarParam.eBBarType = x;                         \
    DrawBottomBar(pMe->m_pDisplay, &BarParam);      \
}

/*Macro for add an item into no icon menuctl*/
#define MP3MENU_ADDITEM(pMenu, ITEMID)            \
         IMENUCTL_AddItem((pMenu), (MUSICPLAYER_RES_FILE_LANG), (ITEMID), (ITEMID), NULL, 0)
/*==============================================================================
                                 类型定义
==============================================================================*/


/*==============================================================================
                                 函数声明
==============================================================================*/

/*MP3播放主窗口处理函数*/
static boolean MP3_PlayMusic_Windows_HandleEvent(CMusicPlayer *pMe,
                                                 AEEEvent eCode,
                                                 uint16 wParam,
                                                 uint32 dwParam);
/*MP3主弹菜单事件处理函数*/
static boolean MP3_MainOptsMenu_HandleEvent(CMusicPlayer *pMe,
                                              AEEEvent eCode,
                                              uint16 wParam,
                                              uint32 dwParam);
/*MP3播放列表处理函数*/
static boolean MP3_Playlist_HandleEvent(CMusicPlayer *pMe,
                                        AEEEvent eCode,
                                        uint16 wParam,
                                        uint32 dwParam);
/*把正在播放的歌曲设置为铃声*/
static boolean MP3_SetRingtone_HandleEvent(CMusicPlayer *pMe,
                                             AEEEvent eCode,
                                             uint16 wParam,
                                             uint32 dwParam);
/*设置播放风格模式音量等处理函数*/
static boolean MP3_Settings_HandleEvent(CMusicPlayer *pMe,
                                         AEEEvent eCode,
                                         uint16 wParam,
                                         uint32 dwParam);
/*把当前播放的歌曲设置为铃声*/
static boolean MP3_SetRingtone_HandleEvent(CMusicPlayer *pMe,
                                             AEEEvent eCode,
                                             uint16 wParam,
                                             uint32 dwParam);
/*对播放列表的操作*/
static boolean MP3_PlaylistOpts_HandleEvent(CMusicPlayer *pMe,
                                            AEEEvent eCode,
                                            uint16 wParam,
                                            uint32 dwParam);
/*创建播放列表*/
static boolean MP3_CreatOrRenamelist_HandleEvent(CMusicPlayer *pMe,
                                                AEEEvent eCode,
                                                uint16 wParam,
                                                uint32 dwParam);
/*查看播放列表里面的音乐列表*/
static boolean MP3_Playlist_View_HandleEvent(CMusicPlayer *pMe,
                                              AEEEvent eCode,
                                              uint16 wParam,
                                              uint32 dwParam);
    /*对playlist祐view 进行操作*/
static boolean MP3_View_Opts_HandleEvent(CMusicPlayer *pMe,
                                          AEEEvent eCode,
                                          uint16 wParam,
                                          uint32 dwParam);

/*处理播放界面的按键事件*/
static boolean MP3_MusicPlayerHandleKeyEvent(CMusicPlayer*pMe,
                                            AEEEvent eCode,
                                            uint16  wParam ,
                                            uint32 dwParam);

/*简单播放音乐处理函数供GALLERY调用用的*/
static boolean MP3_SimplePlayer_HandleEvent(CMusicPlayer *pMe,
                                            AEEEvent eCode,
                                            uint16 wParam,
                                            uint32 dwParam);

/*处理提示信息*/
static boolean  CMusicPlayer_HandleMsgBoxDlgEvent( CMusicPlayer  *pMe,
                                                    AEEEvent   eCode,
                                                    uint16     wParam,
                                                    uint32     dwParam);
 /*处理判断提示信息*/
static boolean  CMusicPlayer_HandleYesNoDlgEvent( CMusicPlayer  *pMe,
                                                   AEEEvent   eCode,
                                                   uint16     wParam,
                                                   uint32     dwParam);

/*在指定位置画图*/
static void MP3_DrawImage(CMusicPlayer *pMe, uint16 ResID, int x, int y);

/*画播放器播放窗口*/
static void MP3_DrawPlayerWindows(CMusicPlayer *pMe);

/*画正在播放的歌曲名称*/
static void MP3_DrawMusicName(CMusicPlayer *pMe,int index);

static void MP3_RefreshPlayingTick(CMusicPlayer *pMe);

/*刷新音量条*/
static void MP3_RefreshVolBar(CMusicPlayer *pMe);
/* 设置LIST控件*/
static void CMusicPlayer_Set_CTL(CMusicPlayer *pMe);

static void MP3_RefreshscheduleBar(CMusicPlayer *pMe);

static void MP3_DrawImageWithOffset( CMusicPlayer *pMe);

/*设置是否需要滚动*/
static void MP3_ResetScroll(CMusicPlayer *pMe);

/*判断是否需要滚动显示*/
static boolean IsRequiredScroll(CMusicPlayer *pMe, int nIdx);

static void MP3_DrawRewindImage(CMusicPlayer *pMe);
static void MP3_DrawForwardImage(CMusicPlayer *pMe);
/*==================================================================================
使菜单项成为当前的菜单控件对象选择
===================================================================================*/
void Imenuctl_SetModeSel(CMusicPlayer *pMe);
/*==================================================================================
使菜单项成为当前的菜单控件对象选择
===================================================================================*/
void Imenuctl_SetVolumeSel(CMusicPlayer *pMe);
/*==================================================================================
使菜单项成为当前的菜单控件对象选择
===================================================================================*/
void Imenuctl_SetSortSel(CMusicPlayer *pMe);
/*画箭头*/
static void CMusicPlayer_Draw_Arrow(CMusicPlayer *pMe,int title_hight, int lineSpace,int itemheight);
/*获取当前选中的项*/
static void CMusicPlayer_GetSettings(CMusicPlayer *pMe);
/*保存CFG配置项的值*/
static void CMusicPlayer_SaveCfg(CMusicPlayer *pMe);

static void CMusicPlayer_HandleDialogTimer(void *pUser);
//重绘时建立PLAYLIST菜单项
static int CMusicPlayer_BuildPlaylist(CMusicPlayer *pMe,IMenuCtl *pMenuCtl);

//建立PLAYLIST VIEW LIST
static int CMusicPlayer_BuildViewList(CMusicPlayer *pMe,IMenuCtl *pMenuCtl);

/*添加音乐的回调*/
int MP3_AddMusicCB(void* pv, FileNamesBuf pBuf, uint32 nBufSize);

/*播放选中的播放列表的歌曲*/
static boolean MP3_PlayPlaylist(CMusicPlayer *pMe);
/*添加OPT主菜单*/
static void MP3_Build_MainOpts_Menu(CMusicPlayer *pMe,IMenuCtl *pMenuCtl);

/*build setting menu and set properties*/
static void MP3_Build_Settings_Menu(CMusicPlayer *pMe);

/*set menuctlrect and set menu propoties*/
static void MP3_SetMenuCtlRect_Prop(CMusicPlayer * pMe, IMenuCtl* pMenuCtl,uint32 dwProps);

/*draw settings text*/
static void MP3_Draw_SettingsText(CMusicPlayer *pMe);

static void MP3_Draw_Settings_TitleBar(CMusicPlayer *pMe);

/*draw cliprect*/
static void MP3_drawClipRectWithOffset(CMusicPlayer *pMe,uint32 imageId,AEERect *rect);


#ifdef FEATURE_MUSICPLAYER_LIST_INDEX_DISPLAY
/*display list index in main screen*/
static void MP3_DispListIndex(CMusicPlayer *pMe);
#endif
/*画简单播放器界面*/
static void MP3_DrawSimplePlayerFace(CMusicPlayer *pMe);

static void MP3_RefreshSimpleSchBar(CMusicPlayer *pMe);
static void CMusicPlayer_PlayPlaylistCB(CMusicPlayer *pMe);
static void MP3_DrawNoRecord(CMusicPlayer *pMe);

/*==============================================================================
                                 全局数据
==============================================================================*/


/*==============================================================================
                                 本地（静态）数据
==============================================================================*/


/*==============================================================================
                                 函数定义
==============================================================================*/
/*==============================================================================
函数:
       CMusicPlayer_ShowDialog
说明:
       函数由状态处理函数用来创建相应对话框资源ID的对话框。

参数:
       pMe [In]: 指向MusicPlayer Applet对象结构的指针,该结构包含小程序的特定信息.
       dlgResId [in]：对话框资源ID，函数根据该ID创建对话框。

返回值:
       SUCCESS: 创建对话框成功
       非SUCCESS: 创建对话框失败

备注:
       

==============================================================================*/
int CMusicPlayer_ShowDialog(CMusicPlayer *pMe, uint16  dlgResId)
{
    int nRet;
    // At most one dialog open at once
    if (ISHELL_GetActiveDialog(pMe->m_pShell) != NULL)
    {
        // Looks like there is one dialog already opened.
        // Flag an error an return without doing anything.
#if defined(AEE_STATIC)
        MSG_FATAL("Trying to create a dialog without closing the previous one",0,0,0);
#endif
        return EFAILED;
    }

    nRet = ISHELL_CreateDialog(pMe->m_pShell,MUSICPLAYER_RES_FILE_LANG,dlgResId,NULL);

#if defined(AEE_STATIC)
    if (nRet != SUCCESS)
    {
        MSG_FATAL("Failed to create the dialog in the MusicPlayer applet",0,0,0);
    }
#endif

    return nRet;
}

/*==============================================================================
函数:
       MusicPlayer_RouteDialogEvent
说明:
       函数将BREW事件路由给当前活动对话框的事件处理函数。

参数:
       pMe [In]: 指向MusicPlayer Applet对象结构的指针,该结构包含小程序的特定信息.
       eCode [in]：事件代码。
       wParam [in]：与事件eCode关联的数据。
       dwParam [in]：与事件eCode关联的数据。

返回值:
       TRUE：传入事件被处理。
       FALSE：传入事件没被处理。

备注:

==============================================================================*/
boolean CMusicPlayer_RouteDialogEvent(CMusicPlayer *pMe,
                                     AEEEvent    eCode,
                                     uint16  wParam,
                                     uint32 dwParam)
{
#if defined(AEE_STATIC)
    if (NULL == pMe)
    {
        return FALSE;
    }
#endif

    if (NULL == pMe->m_pActiveDlg)
    {
        return FALSE;
    }

    switch (pMe->m_pActiveDlgID)
    {
        case IDD_WINDOWS:
             return MP3_PlayMusic_Windows_HandleEvent(pMe, eCode,wParam, dwParam);
             
        case IDD_MAINMENU:
             return MP3_MainOptsMenu_HandleEvent(pMe, eCode,wParam, dwParam);
             
        case IDD_PLAYLIST_MAINMENU:
             return MP3_Playlist_HandleEvent(pMe, eCode,wParam, dwParam);
             
        case IDD_SETASRINGTONE:
             return MP3_SetRingtone_HandleEvent(pMe, eCode,wParam, dwParam);
             
        case IDD_SET_MAINMENU:
             return MP3_Settings_HandleEvent(pMe, eCode,wParam, dwParam);
             
        case IDD_MSGBOX:
             return CMusicPlayer_HandleMsgBoxDlgEvent (pMe,eCode, wParam, dwParam);
             
        case IDD_YESNO:
             return CMusicPlayer_HandleYesNoDlgEvent (pMe,eCode, wParam, dwParam);
        case IDD_PLAYLIST_OPTS:
             return MP3_PlaylistOpts_HandleEvent(pMe,eCode, wParam, dwParam);
        case IDD_CREATEORRENAMELIST:
             return MP3_CreatOrRenamelist_HandleEvent(pMe,eCode, wParam, dwParam);
        case IDD_PLAYLIST_VIEW:
             return MP3_Playlist_View_HandleEvent(pMe,eCode, wParam, dwParam);
        case IDD_VIEW_OPTS:
             return MP3_View_Opts_HandleEvent(pMe,eCode, wParam, dwParam);
        case IDD_SIMPLEPLAYER:
             return MP3_SimplePlayer_HandleEvent(pMe,eCode, wParam, dwParam);
        default:
             return FALSE;
    }
}

/*==============================================================================
函数：
       MP3_PlayMusic_Windows_HandleEvent
说明：
       IDD_WINDOWS 对话框事件处理函数
       
参数：
       pMe [in]：指向CMusicPlayer Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。
       
返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。
       
备注：
       
==============================================================================*/

static boolean MP3_PlayMusic_Windows_HandleEvent(CMusicPlayer *pMe,
                                            AEEEvent eCode,
                                            uint16 wParam,
                                            uint32 dwParam)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    MSG_FATAL("MP3_PlayMusic_Windows_HandleEvent Start",0,0,0);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:   
        {   
            return TRUE;
        }
        case EVT_DIALOG_START:
        {
            if(pMe->m_pIAnn != NULL)
            {
            	int nIdx;
            	nIdx  = pMe->m_nAutoScrollIdx;
                MSG_FATAL("IANNUNCIATOR_SetHasTitleText",0,0,0);
                IANNUNCIATOR_SetHasTitleText(pMe->m_pIAnn, FALSE);
				//IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,pMe->m_pMp3FileToPlay+nIdx);
                IANNUNCIATOR_Redraw(pMe->m_pIAnn);
            }
           (void) ISHELL_PostEvent(pMe->m_pShell, 
                                    AEECLSID_APP_MUSICPLAYER,
                                    EVT_USER_REDRAW,
                                    0,
                                    0);          
            return TRUE;
        }
        case EVT_USER_REDRAW:
        {          
           MP3_DrawPlayerWindows(pMe);
            /*if(pMe->m_bPlaying)
            {
              MP3_DRAW_BOTTOMBAR(BTBAR_OPTION_PAUSE_STOP);
            }
            else if(pMe->m_bPaused)
            {
              MP3_DRAW_BOTTOMBAR(BTBAR_OPTION_PLAY_STOP);
            }
            else
            {
                MP3_DRAW_BOTTOMBAR(BTBAR_OPTION_PLAY_BACK)
            }*/
            if(pMe->m_bPaused || pMe->m_bPlaying)
            {
                MP3_DRAW_BOTTOMBAR(BTBAR_OPTION_STOP);
            }
            else
            {
                MP3_DRAW_BOTTOMBAR(BTBAR_OPTION_BACK);
            }
            IDISPLAY_Update(pMe->m_pDisplay); //刷屏
            return TRUE;
            
        }
        case EVT_DIALOG_END:
        {        
            return TRUE;
        }
        //case EVT_GSENSOR_SHAKE:
#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
		case EVT_PEN_UP:
#endif//FEATURE_LCD_TOUCH_ENABLE
        case EVT_KEY:
             return MP3_MusicPlayerHandleKeyEvent(pMe,eCode,wParam,dwParam);
             
        default:
            break;
        }//switch (eCode)

        return FALSE;
}
/*==============================================================================
函数：
       MP3_MainOptsMenu_HandleEvent
说明：
       IDD_MAINMENU 对话框事件处理函数
       
参数：
       pMe [in]：指向CMusicPlayer Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。
       
返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。
       
备注：
       
==============================================================================*/
static boolean MP3_MainOptsMenu_HandleEvent(CMusicPlayer *pMe,
                                            AEEEvent eCode,
                                            uint16 wParam,
                                            uint32 dwParam)
{
  IMenuCtl  *pMenuCtl;
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    MSG_FATAL("MP3_MainOptsMenu_HandleEvent Start",0,0,0);
    pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                              IDC_MAINMENU);
   if (pMenuCtl == NULL)
   {
       return FALSE;
   }
   
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
              MP3_Build_MainOpts_Menu(pMe,pMenuCtl);
              return TRUE;
            
        case EVT_DIALOG_START:
        {            
            IMENUCTL_SetPopMenuRect(pMenuCtl);
            IMENUCTL_SetProperties(pMenuCtl, MP_UNDERLINE_TITLE |MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY);
            IMENUCTL_SetBottomBarType(pMenuCtl,BTBAR_SELECT_BACK);
            // For redraw the dialog
            (void)ISHELL_PostEvent( pMe->m_pShell,
                                    AEECLSID_APP_MUSICPLAYER,
                                    EVT_USER_REDRAW,
                                    0,
                                    0); 
            return TRUE;
        }
        
        // Note: Fall through to the EVT_USER_REDRAW
        //lint -fallthrough
        
        case EVT_USER_REDRAW:
            // Restore the menu select from suspend
            IMENUCTL_SetProperties(pMenuCtl, MP_UNDERLINE_TITLE |MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY);
            //IDISPLAY_Update(pMe->m_pDisplay);  
            IMENUCTL_Redraw(pMenuCtl);
            return TRUE;
            
        case EVT_DIALOG_END:
		{	
            return TRUE;
        }
            
        case EVT_KEY:			
            switch (wParam)
            {
                case AVK_CLR:			
				{
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return TRUE;
                }
                case AVK_BGPLAY:					
                    ISHELL_CloseApplet(pMe->m_pShell, TRUE);
                    return TRUE;
                    
                default:
                    break;                    
            }
            break;

        case EVT_COMMAND:
            switch (wParam)
            {
                case IDS_PLAY:
                    if(pMe->m_bPlaying==FALSE&&pMe->m_bPaused==FALSE)
                    {
                      if(pMe->m_MusicPlayerCfg.eMusicPlayMode == PLAYMODE_RANDOM)
                      {
                         MP3_ResetRandonIdentifier(pMe);
                      }
                      if(pMe->m_pMedia)
                      {
                        (void)IMEDIA_Play(pMe->m_pMedia);
                        pMe->m_bPlaying = TRUE;
                      } 
                    }
                    else if(pMe->m_bPlaying==FALSE&&pMe->m_bPaused==TRUE)
                    {
                        if(pMe->m_pMedia)
                      {
                        (void)IMEDIA_Resume(pMe->m_pMedia);
                        pMe->m_bPlaying = TRUE;
                        pMe->m_bPaused = FALSE;
                      } 
                    }
                   CLOSE_DIALOG(DLGRET_PLAY);
                    return TRUE;

                case IDS_PAUSE:
                    if(pMe->m_pMedia)
                    {               
                      if(SUCCESS == IMEDIA_Pause(pMe->m_pMedia))
                      {
                        pMe->m_bPaused= TRUE;
                        pMe->m_bPlaying=FALSE;                  
                      }
                    }
                    CLOSE_DIALOG(DLGRET_PAUSE);
                    return TRUE;
                    
                case IDS_PLAYLIST:
                    CLOSE_DIALOG(DLGRET_PLAYLIST);
                    return TRUE;
                    
                case IDS_SET_AS_RINGTONE:
                    CLOSE_DIALOG(DLGRET_SETASRINGTONE);
                    return TRUE;
                    
                case IDS_SETTINGS:
                    CLOSE_DIALOG(DLGRET_SETTINGS);
                    return TRUE;
                    
                case IDS_MINIMIZE:					 
                     SetMp3PlayerStatus(pMe, MP3STATUS_RUNONBACKGROUND);
                     ISHELL_CloseApplet(pMe->m_pShell, TRUE);
                    return TRUE;

#ifdef FEATURE_SUPPORT_BT_APP
                case IDS_USEBT_HEADSET:
#ifndef WIN32
                    pMe->m_nBTID = bcmapp_is_ag_connected();
                    if(BCMAPP_AG_BAD_APPID == pMe->m_nBTID)
                    {
                        CLOSE_DIALOG(DLGRET_UNCONNECT_BT);
                        return TRUE;
                    }
                    else
                    {
                       (void)bcmapp_ag_audio_open(pMe->m_nBTID, 
                                                (PFN_AG_NOTIFY)MP3_PlayMusicByBTCallBack,
                                                pMe, 
                                                pMe->m_MusicPlayerCfg.eMusicVolume,
                                                pMe->m_bUse848); 
#else
                       pMe->m_bUseBT = TRUE;
                       SetIsPlayMP3ByBt(TRUE);
                       CLOSE_DIALOG(DLGRET_OPENNING_SCO);
                       return TRUE;
#endif
#ifndef WIN32
                    }
#endif
                case IDS_UNUSE_BT_HEADSET:
#ifndef WIN32
                    if(BCMAPP_AG_SUCCESS == bcmapp_ag_audio_close(pMe->m_nBTID))
#endif
                    {
#ifndef WIN32
                        pMe->m_nBTID = BCMAPP_AG_BAD_APPID;
#endif
                        pMe->m_bUseBT = FALSE;
                        //g_bPlayMP3ByBt = FALSE;
                        SetIsPlayMP3ByBt(FALSE);
                    }
                    CLOSE_DIALOG(DLGRET_CLOSED_BT);
                    return TRUE;
#endif //FEATURE_SUPPORT_BT_APP

                default:
                    break;
                    
            }
            break;
#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
		//case EVT_PEN_DOWN:
		case EVT_PEN_UP:
			{
				AEEDeviceInfo devinfo;
				int nBarH ;
				AEERect rc;
				int16 wXPos = (int16)AEE_GET_X((const char *)dwParam);
				int16 wYPos = (int16)AEE_GET_Y((const char *)dwParam);
				nBarH = GetBottomBarHeight(pMe->m_pDisplay);
				MEMSET(&devinfo, 0, sizeof(devinfo));
				ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
				SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, devinfo.cyScreen);
				if ((wXPos>0)&&(wXPos<devinfo.cxScreen/2)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
				{
					uint16 nEvtCommand = IMENUCTL_GetSel(pMenuCtl);
					boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_SELECT,nEvtCommand);
					return rt;
				}
				else if ((wXPos>devinfo.cxScreen/2)&&(wXPos<devinfo.cxScreen)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
				{
					boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_CLR,0);
					return rt;
				}

			}
			return TRUE;
#endif            
        default:
            break;
            
    }
    
    return FALSE;
}
/*==============================================================================
函数：
       MP3_Playlist_HandleEvent
说明：
       IDD_PLAYLIST_MAINMENU 对话框事件处理函数
       
参数：
       pMe [in]：指向CMusicPlayer Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。
       
返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。
       
备注：
       
==============================================================================*/
static boolean MP3_Playlist_HandleEvent(CMusicPlayer *pMe,
                                            AEEEvent eCode,
                                            uint16 wParam,
                                            uint32 dwParam)
{
  IMenuCtl  *pMenuCtl;
  int i;

#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    MSG_FATAL("MP3_Playlist_HandleEvent Start",0,0,0);
    pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                             IDC_PLAYLIST_MAINMENU);
   if (pMenuCtl == NULL)
   {
       return FALSE;
   }
   
    switch (eCode)
    {
        case EVT_DIALOG_INIT:     
            return TRUE;
            
        case EVT_DIALOG_START:
        {
            MP3_SetMenuCtlRect_Prop(pMe,pMenuCtl,MP_UNDERLINE_TITLE |MP_WRAPSCROLL);
			#if 0
            IMENUCTL_SetTitle(pMenuCtl, MUSICPLAYER_RES_FILE_LANG, IDS_PLAYLIST, NULL);
			#else
		    {
		  		AECHAR WTitle[40] = {0};
				(void)ISHELL_LoadResString(pMe->m_pShell,
                        MUSICPLAYER_RES_FILE_LANG,                                
                        IDS_PLAYLIST,
                        WTitle,
                        sizeof(WTitle));
                IANNUNCIATOR_SetHasTitleText(pMe->m_pIAnn, TRUE);
				IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,WTitle);
		    }
			#endif
   
            if(SUCCESS!=CMusicPlayer_BuildPlaylist(pMe,pMenuCtl))
            {
                return FALSE;
            }
            // For redraw the dialog
            (void)ISHELL_PostEvent( pMe->m_pShell,
                                    AEECLSID_APP_MUSICPLAYER,
                                    EVT_USER_REDRAW,
                                    0,
                                    0);
           return TRUE;
        }
        
        case EVT_USER_REDRAW:
            IMENUCTL_SetSel(pMenuCtl, pMe->m_nCurPlaylistID);
            //IMENUCTL_SetProperties(pMenuCtl, MP_UNDERLINE_TITLE |MP_WRAPSCROLL);
            IMENUCTL_SetOemProperties( pMenuCtl, OEMMP_DISTINGUISH_INFOKEY_SELECTKEY | OEMMP_USE_MENU_STYLE);
            // Draw prompt bar here
            if(pMe->m_nPlaylistNum == 0)
            {
              IMENUCTL_SetActive(pMenuCtl, FALSE);
              MP3_DrawNoRecord(pMe);
              MP3_DRAW_BOTTOMBAR(BTBAR_CREATE_BACK);
              IDISPLAY_Update(pMe->m_pDisplay);  
            }
            else
            {
              MP3_DRAW_BOTTOMBAR(BTBAR_OPTION_BACK);
              IMENUCTL_Redraw(pMenuCtl);
            }
            return TRUE;
            
        case EVT_DIALOG_END:
            if(pMe->m_bSuspending && pMe->m_nPlaylistNum != 0)
            {
                pMe->m_nCurPlaylistID = IMENUCTL_GetSel(pMenuCtl);
            }
            return TRUE;
            
        case EVT_KEY:
            switch (wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return TRUE;

                case AVK_BGPLAY:
                    ISHELL_CloseApplet(pMe->m_pShell, TRUE);
                    return TRUE;

                case AVK_INFO:
                    /*if(pMe->m_nPlaylistNum >= PLAYLIST_MAX_NUM)
                    {
                        CLOSE_DIALOG(DLGRET_CREATELIST_FAILED);
                    }
                    else
                    {
                       CLOSE_DIALOG(DLGRET_CREATEORRENAMELIST);
                    }*/
                    if(pMe->m_nPlaylistNum != 0)
                    {
                        pMe->m_nCurPlaylistID = IMENUCTL_GetSel(pMenuCtl);
                        if(SUCCESS!=CMusicPlayer_ReadMusiclist(pMe,
                                                               pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName,
                                                               FALSE))
                        {
                            for(i=0;i<MUSIC_MAX_FILESNUM;i++)
                           {
                             MEMSET(pMe->m_Musiclist[i].pMusicName,0,MP3NAME_MAX_LEN*sizeof(char));
                           }
                        }
                        if(FALSE == MP3_PlayPlaylist(pMe))
                        {
                            return TRUE;
                        }
                        CLOSE_DIALOG(DLGRET_PLAYLISTPLAY);
                    }
                    return TRUE;
                    
                case AVK_SELECT:
                    DBGPRINTF("AVK_SELECT");
                    if(pMe->m_nPlaylistNum == 0)
                    {
                        CLOSE_DIALOG(DLGRET_CREATEORRENAMELIST);
                    }
                    return TRUE;
                default:
                    break;                    
            }
            break;
            
        case EVT_COMMAND:
            if(pMe->m_nPlaylistNum>0)
            {
                pMe->m_nCurPlaylistID = IMENUCTL_GetSel(pMenuCtl);
                if(SUCCESS!=CMusicPlayer_ReadMusiclist(pMe,
                                                       pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName,
                                                       FALSE))
                {
                    for(i=0;i<MUSIC_MAX_FILESNUM;i++)
                   {
                     MEMSET(pMe->m_Musiclist[i].pMusicName,0,MP3NAME_MAX_LEN*sizeof(char));
                   }
                }
                CLOSE_DIALOG(DLGRET_OK);
            }
            else
            {
               CLOSE_DIALOG(DLGRET_CREATEORRENAMELIST);
            }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
		case AVK_SELECT:
		{
			return ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_SELECT,1);
		}
		return TRUE;
#endif//FEATURE_LCD_TOUCH_ENABLE            
            
        default:
            break;
            
    }
    
 return FALSE; 
}
/*==============================================================================
函数
       MP3_SetRingtone_HandleEvent
说明：
       IDD_SETASRINGTONE 对话框事件处理函数
       
参数：
       pMe [in]：指向CMusicPlayer Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。
       
返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。
       
备注：
       
==============================================================================*/
static boolean MP3_SetRingtone_HandleEvent(CMusicPlayer *pMe,
                                            AEEEvent eCode,
                                            uint16 wParam,
                                            uint32 dwParam)
{
  IMenuCtl  *pMenuCtl;
  static  boolean ringTone = FALSE;
  static  boolean smsTone = FALSE;
  static  boolean alarmTone = FALSE;
    
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    MSG_FATAL("MP3_SetRingtone_HandleEvent Start",0,0,0);
    pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                              IDC_SETASRINGTONE);
     if (pMenuCtl == NULL)
   {
       return FALSE;
   }
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            MP3MENU_ADDITEM(pMenuCtl,IDS_SET_CALL_RINGTONE);
            MP3MENU_ADDITEM(pMenuCtl,IDS_SET_SMS_RINGTONE);
            MP3MENU_ADDITEM(pMenuCtl,IDS_SET_ALARM_RINGTONE);
			#if 0
            IMENUCTL_SetTitle(pMenuCtl, MUSICPLAYER_RES_FILE_LANG, IDS_SET_AS_RINGTONE, NULL);
			#else
		    {
		  		AECHAR WTitle[40] = {0};
				(void)ISHELL_LoadResString(pMe->m_pShell,
                        MUSICPLAYER_RES_FILE_LANG,                                
                        IDS_SET_AS_RINGTONE,
                        WTitle,
                        sizeof(WTitle));
                IANNUNCIATOR_SetHasTitleText(pMe->m_pIAnn, TRUE);
				IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,WTitle);
		    }
			#endif
            IMENUCTL_SetOemProperties(pMenuCtl, OEMMP_USE_MENU_STYLE);
            return TRUE;

        case EVT_DIALOG_START:
            {   
                MP3_SetMenuCtlRect_Prop(pMe,pMenuCtl,
                                        MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_MULTI_SEL);
                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, 0));
                (void) ISHELL_PostEvent( pMe->m_pShell,
                            AEECLSID_APP_MUSICPLAYER,
                            EVT_USER_REDRAW,
                            0,
                            0);
                return TRUE;
            } 
        case EVT_USER_REDRAW:
            {
                byte CurProfileNum;
                ringID ConfigRinger[PROFILENUMBER];
                
                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                CFGI_PROFILE_CUR_NUMBER,
                                &CurProfileNum,
                                sizeof(CurProfileNum));
                // 检查是否为来电铃声
                ICONFIG_GetItem(pMe->m_pConfig,
                                    CFGI_PROFILE_CALL_RINGER,
                                    (void*)ConfigRinger,
                                    sizeof(ConfigRinger));
                
                if(((STRCMP(pMe->m_PlayingMusiclist[pMe->m_MusicPlayerCfg.lastPlayMusicID].pMusicName + 1,
                        ConfigRinger[CurProfileNum].szMusicname) == 0) && 
                        (ConfigRinger[CurProfileNum].ringType == OEMNV_MP3_RINGER)) || ringTone)
                {
                    SetCheckBoxItem(pMenuCtl, IDS_SET_CALL_RINGTONE, TRUE);
                    ringTone = TRUE;
                }
                else
                {
                    ringTone = FALSE;
                }
                // 检查是否为短信铃声 
                ICONFIG_GetItem(pMe->m_pConfig,
                                    CFGI_PROFILE_SMS_RINGER_ID,
                                    (void*)ConfigRinger,
                                    sizeof(ConfigRinger));
                
                if(((STRCMP(pMe->m_PlayingMusiclist[pMe->m_MusicPlayerCfg.lastPlayMusicID].pMusicName + 1,
                        ConfigRinger[CurProfileNum].szMusicname) == 0) && 
                        (ConfigRinger[CurProfileNum].ringType == OEMNV_MP3_RINGER)) || smsTone)

                {
                    SetCheckBoxItem(pMenuCtl, IDS_SET_SMS_RINGTONE, TRUE);
                    smsTone = TRUE;
                }
                else
                {
                    smsTone = FALSE;
                }
                // 检查是否为闹钟铃声 
                ICONFIG_GetItem(pMe->m_pConfig,
                                    CFGI_PROFILE_ALARM_RINGER,
                                    (void*)ConfigRinger,
                                    sizeof(ConfigRinger));
                if(((STRCMP(pMe->m_PlayingMusiclist[pMe->m_MusicPlayerCfg.lastPlayMusicID].pMusicName + 1,
                        ConfigRinger[CurProfileNum].szMusicname) == 0) && 
                        (ConfigRinger[CurProfileNum].ringType == OEMNV_MP3_RINGER)) || alarmTone)

                {
                    SetCheckBoxItem(pMenuCtl, IDS_SET_ALARM_RINGTONE, TRUE);
                    alarmTone = TRUE;
                } 
                else
                {
                    alarmTone = FALSE;
                }
                IMENUCTL_SetOemProperties(pMenuCtl, OEMMP_USE_MENU_STYLE);
                MP3_DRAW_BOTTOMBAR(BTBAR_SAVE_BACK);
                IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
                IMENUCTL_Redraw(pMenuCtl);
                return TRUE;
            }
        case EVT_DIALOG_END:
             if(pMe->m_bSuspending)
             {
                 if(GetCheckBoxVal(pMenuCtl, IDS_SET_CALL_RINGTONE))
                 {
                    ringTone = TRUE;
                 }
                 if(GetCheckBoxVal(pMenuCtl, IDS_SET_ALARM_RINGTONE))
                 {
                    alarmTone = TRUE;
                 }
                 if(GetCheckBoxVal(pMenuCtl, IDS_SET_SMS_RINGTONE))
                 {
                    smsTone = TRUE;
                 }
             }
             else
             {
                ringTone = alarmTone = smsTone = FALSE;
             }
             return TRUE;
             
        case EVT_KEY:
            switch (wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return TRUE;

                case AVK_BGPLAY:
                    ISHELL_CloseApplet(pMe->m_pShell, TRUE);
                    return TRUE;

                case AVK_SELECT:
                case AVK_INFO:
                    {
                        byte CurProfileNum;
                        ringID nNewConfigRinger[PROFILENUMBER];
                        int nSelect = 0;

                        (void) ICONFIG_GetItem(pMe->m_pConfig,
                                                CFGI_PROFILE_CUR_NUMBER,
                                                &CurProfileNum,
                                                sizeof(CurProfileNum));                 

                        // 设置来电铃声
                        {
                            ICONFIG_GetItem(pMe->m_pConfig,CFGI_PROFILE_CALL_RINGER,(void*)nNewConfigRinger,sizeof(nNewConfigRinger));

                            if(GetCheckBoxVal(pMenuCtl, IDS_SET_CALL_RINGTONE))
                            {
                                (void)STRCPY(nNewConfigRinger[CurProfileNum].szMusicname, 
                                                pMe->m_PlayingMusiclist[pMe->m_MusicPlayerCfg.lastPlayMusicID].pMusicName+1);

                                nNewConfigRinger[CurProfileNum].ringType = OEMNV_MP3_RINGER;
                                nSelect++;
                            }
                            else if(ringTone)
                            {
                                nNewConfigRinger[CurProfileNum].ringType = OEMNV_MID_RINGER;
                                nSelect++;
                                ringTone = FALSE;
                            }

                            ICONFIG_SetItem( pMe->m_pConfig, 
                                                CFGI_PROFILE_CALL_RINGER,
                                                (void*)nNewConfigRinger, 
                                                sizeof(nNewConfigRinger));
                            
                        }

                        // 设置闹钟铃声
                        {
                            ICONFIG_GetItem(pMe->m_pConfig, CFGI_PROFILE_ALARM_RINGER, (void*)nNewConfigRinger,sizeof(nNewConfigRinger));

                            if(GetCheckBoxVal(pMenuCtl, IDS_SET_ALARM_RINGTONE))
                            {
                                (void)STRCPY(nNewConfigRinger[CurProfileNum].szMusicname, 
                                                pMe->m_PlayingMusiclist[pMe->m_MusicPlayerCfg.lastPlayMusicID].pMusicName+1);

                                nNewConfigRinger[CurProfileNum].ringType = OEMNV_MP3_RINGER;
                                nSelect++;
                            }
                            else if(alarmTone)
                            {
                                nNewConfigRinger[CurProfileNum].ringType = OEMNV_MID_RINGER;
                                nSelect++;
                                alarmTone = FALSE;
                            }

                            ICONFIG_SetItem( pMe->m_pConfig, 
                                                CFGI_PROFILE_ALARM_RINGER,
                                                (void*)nNewConfigRinger, 
                                                sizeof(nNewConfigRinger));
                            
                        }

                        // 设置短信铃声
                        {
                            ICONFIG_GetItem(pMe->m_pConfig, CFGI_PROFILE_SMS_RINGER_ID, (void*)nNewConfigRinger, sizeof(nNewConfigRinger));

                            if(GetCheckBoxVal(pMenuCtl, IDS_SET_SMS_RINGTONE))
                            {
                                (void)STRCPY(nNewConfigRinger[CurProfileNum].szMusicname, 
                                                 pMe->m_PlayingMusiclist[pMe->m_MusicPlayerCfg.lastPlayMusicID].pMusicName+1);

                                nNewConfigRinger[CurProfileNum].ringType = OEMNV_MP3_RINGER;
                                nSelect++;
                            }
                            else if(smsTone)
                            {
                                nNewConfigRinger[CurProfileNum].ringType = OEMNV_MID_RINGER;
                                nSelect++;
                                smsTone = FALSE;
                            }

                            ICONFIG_SetItem( pMe->m_pConfig, 
                                                CFGI_PROFILE_SMS_RINGER_ID,
                                                (void*)nNewConfigRinger, 
                                                sizeof(nNewConfigRinger));
                         
                        }
                     if(0 == nSelect)
                     {
                        CLOSE_DIALOG(DLGRET_SELECT_NONE);
                     }
                     else
                     {
                       CLOSE_DIALOG(DLGRET_OK);
                     }
                  }
                    return TRUE;
                    
                default:
                    break;
            }
            // App handle all key event
            return TRUE;   
            
        case EVT_COMMAND:
#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
			return ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_SELECT,KEY_PRESS_DWPARM);
#endif//FEATURE_LCD_TOUCH_ENABLE
           return TRUE;
 
        default:
            break;
            
    }
    return FALSE;
 }

/*==============================================================================
函数：
       MP3_Playlist_HandleEvent
说明：
       IDD_SET_MAINMENU 对话框事件处理函数
       
参数：
       pMe [in]：指向CMusicPlayer Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。
       
返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。
       
备注：
       
==============================================================================*/
static boolean MP3_Settings_HandleEvent(CMusicPlayer *pMe,
                                            AEEEvent eCode,
                                            uint16 wParam,
                                            uint32 dwParam)
{
    int nSelect=0;
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    MSG_FATAL("MP3_Settings_HandleEvent Start",0,0,0);
 switch (eCode)
    {
        case EVT_DIALOG_INIT:   
        {   
      
            return TRUE;
        }
        case EVT_DIALOG_START:
        {
            if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                             AEECLSID_LISTCTL,
                             (void **)&pMe->m_pMode))
            {
              return FALSE;
            }
            if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                             AEECLSID_LISTCTL,
                             (void **)&pMe->m_pVolume))
            {
             (void)IMENUCTL_Release(pMe->m_pMode);
             pMe->m_pMode = NULL;
             return FALSE;
            }
            if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                             AEECLSID_LISTCTL,
                             (void **)&pMe->m_pSort))
            {
             (void)IMENUCTL_Release(pMe->m_pMode);
             pMe->m_pMode = NULL;
             (void)IMENUCTL_Release(pMe->m_pVolume);
             pMe->m_pVolume = NULL;
             return FALSE;
            }
            MP3_Build_Settings_Menu(pMe);
            (void) ISHELL_PostEvent(pMe->m_pShell, 
                                    AEECLSID_APP_MUSICPLAYER,
                                    EVT_USER_REDRAW,
                                    0,
                                    0);          
            return TRUE;
        }
        case EVT_USER_REDRAW:
        {    
            MP3_Draw_SettingsText(pMe);
            return TRUE;
         }
        case EVT_DIALOG_END:
        {  
             if(pMe->m_bSuspending)
            {
                CMusicPlayer_GetSettings(pMe);
            }
            //pMe->m_nCtlID = IDC_MUSICPLAY_MODE;   //控件ID
            (void)IMENUCTL_Release(pMe->m_pMode);
            pMe->m_pMode = NULL;
            (void)IMENUCTL_Release(pMe->m_pVolume);
            pMe->m_pVolume = NULL;
            (void)IMENUCTL_Release(pMe->m_pSort);
            pMe->m_pSort = NULL;
            return TRUE;
        }
      case EVT_KEY:
       {    
            if (IMENUCTL_IsActive(pMe->m_pMode))
            {
               if(IMENUCTL_HandleEvent(pMe->m_pMode, eCode, wParam, dwParam))
              {
               return TRUE;
              }
            }
            if (IMENUCTL_IsActive(pMe->m_pVolume))
            {
               if(IMENUCTL_HandleEvent(pMe->m_pVolume, eCode, wParam, dwParam))
               {
                 return TRUE;
               }
           }
             if (IMENUCTL_IsActive(pMe->m_pSort))
            {
               if(IMENUCTL_HandleEvent(pMe->m_pSort, eCode, wParam, dwParam))
               {
                return TRUE;
               }
            }
            switch(wParam)
            {
                case AVK_CLR:
                    pMe->m_nCurrentVolume = pMe->m_MusicPlayerCfg.eMusicVolume *AEE_MAX_VOLUME/5;
                    if(pMe->m_pMedia) 
                    {
                        (void)IMEDIA_SetVolume(pMe->m_pMedia,pMe->m_nCurrentVolume); 
                    } 
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return  TRUE;

               case AVK_BGPLAY:
                    ISHELL_CloseApplet(pMe->m_pShell, TRUE);
                    return TRUE;
                    
            }
            return TRUE;
         }

        case EVT_KEY_PRESS:
        {
          if (IMENUCTL_IsActive(pMe->m_pMode))
          {
            if(IMENUCTL_HandleEvent(pMe->m_pMode, eCode, wParam, dwParam))
            {
              return TRUE;
            }
         }
        if (IMENUCTL_IsActive(pMe->m_pVolume))
        {
          if(IMENUCTL_HandleEvent(pMe->m_pVolume, eCode, wParam, dwParam))
          {
            return TRUE;
          }
        }

        if (IMENUCTL_IsActive(pMe->m_pSort))
        {
          if(IMENUCTL_HandleEvent(pMe->m_pSort, eCode, wParam, dwParam))
          {
            return TRUE;
          }
        }
            //传送给自定义事件EVT_FOCUS_SWITCH处理,是为了避免切换控件的同时
            //控件提前处理方向键事件
            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_APP_MUSICPLAYER,
                                     EVT_FOCUS_SWITCH,
                                     wParam,
                                     0);
            return TRUE;
         }
        case EVT_FOCUS_SWITCH:
        {
            switch(wParam)
            {
              case AVK_DOWN:
                if(pMe->m_nCtlID>=IDC_MUSIC_SORT)
                {
                   pMe->m_nCtlID-=IDC_MUSIC_SORT;
                }
                pMe->m_nCtlID++;
                CMusicPlayer_Set_CTL(pMe);
                return TRUE;
                
            case AVK_UP:
                if(pMe->m_nCtlID<=IDC_MUSICPLAY_MODE)
                {
                  pMe->m_nCtlID+=IDC_MUSIC_SORT;
                }
                pMe->m_nCtlID--;
                CMusicPlayer_Set_CTL(pMe);
                return TRUE;
                
            default:
                break;
            }
            return TRUE;
        }
       case EVT_COMMAND:
           {
              char *musicName;

              musicName=(char *)MALLOC(MP3NAME_MAX_LEN*sizeof(char));
              MEMSET(musicName,0,MP3NAME_MAX_LEN*sizeof(char));
              (void)STRCPY(musicName,pMe->m_PlayingMusiclist[pMe->m_MusicPlayerCfg.lastPlayMusicID].pMusicName);
              CMusicPlayer_SaveCfg(pMe);
              CMusicPlayer_PlayingMusiclistSortBy(pMe);
              MP3_SearchIdByName(pMe,musicName);
              FREEIF(musicName);
              (void) ISHELL_SetPrefs(pMe->m_pShell,
                                       AEECLSID_APP_MUSICPLAYER,
                                       MUSICPLAYERCFG_VERSION,
                                       &pMe->m_MusicPlayerCfg,
                                       sizeof(CMusicPlayerCfg));
              CLOSE_DIALOG(DLGRET_OK); 
              return TRUE;
           }
      case EVT_CTL_SEL_CHANGED:
            if (IMENUCTL_IsActive(pMe->m_pVolume))
            {
                switch(wParam)
                {
                    case IDS_VOLUME_ONE:
                        nSelect = VOLUME_ONE;
                        break;
                    case IDS_VOLUME_TWO:
                        nSelect = VOLUME_TWO;
                        break;
                    case IDS_VOLUME_THREE:
                        nSelect = VOLUME_THREE;
                        break;
                    case IDS_VOLUME_FOUR:
                        nSelect = VOLUME_FOUR;
                        break;
                    case IDS_VOLUME_FIVE:
                        nSelect = VOLUME_FIVE;
                        break;
                    case IDS_VOLUME_OFF:
                        nSelect = VOLUME_OFF;
                        break;
                    default:
                        break;
                }
                pMe->m_nCurrentVolume= nSelect*AEE_MAX_VOLUME/5;
                if(pMe->m_pMedia) 
                {
                    (void)IMEDIA_SetVolume(pMe->m_pMedia,pMe->m_nCurrentVolume); 
                } 
            }
             return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
	  case EVT_PEN_UP:
		  {
			  int i;
			  AEEDeviceInfo devinfo;
			  int nBarH ;
			  AEERect rc;
			  int16 wXPos = (int16)AEE_GET_X((const char *)dwParam);
			  int16 wYPos = (int16)AEE_GET_Y((const char *)dwParam);



			  //播放
			  SETAEERECT(&rc,PLAY_X,PLAY_Y,PLAY_W, PLAY_H);
			  if(MUSICPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
			  {
				  return  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_INFO,0);
			  }
			  //前一首
			  SETAEERECT(&rc,PREVIOUSPRESS_X,PREVIOUSPRESS_Y,PREVIOUSPRESS_W, PREVIOUSPRESS_H);
			  if(MUSICPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
			  {
				  return  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_INFO,0);
			  }
			  //下一首
			  SETAEERECT(&rc,NEXTPRESS_X,NEXTPRESS_Y,NEXTPRESS_W, NEXTPRESS_H);
			  if(MUSICPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
			  {
				  return  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_INFO,0);
			  }
			  //增加音量
			  SETAEERECT(&rc,ADDVOLUMEPRESS_X,ADDVOLUMEPRESS_Y,ADDVOLUMEPRESS_W, ADDVOLUMEPRESS_H);
			  if(MUSICPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
			  {
				  return  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_INFO,0);
			  }
			  //降低音量
			  SETAEERECT(&rc,DECREASEVOLUMEPRESS_X,DECREASEVOLUMEPRESS_Y,DECREASEVOLUMEPRESS_W, DECREASEVOLUMEPRESS_H);
			  if(MUSICPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
			  {
				  return  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_INFO,0);
			  }
			  //底部操作栏
			  nBarH = GetBottomBarHeight(pMe->m_pDisplay);
			  MEMSET(&devinfo, 0, sizeof(devinfo));
			  ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
			  SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, devinfo.cyScreen);
			  if ((wXPos>0)&&(wXPos<devinfo.cxScreen/3)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
			  {
				  boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_SELECT,1);
				  return rt;
			  }
			  else if ((wXPos>devinfo.cxScreen/3)&&(wXPos<(devinfo.cxScreen/3)*2)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
			  {
				  boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_INFO,0);
				  return rt;
			  }
			  else if ((wXPos>wXPos<(devinfo.cxScreen/3)*2)&&(wXPos<devinfo.cxScreen)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
			  {
				  boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_CLR,0);
				  return rt;
			  }
		  }
		  return TRUE;
#endif//FEATURE_LCD_TOUCH_ENABLE

        default:
            break;
    }

        return FALSE;
 
 }
/*==============================================================================
函数：
       MP3_PlaylistOpts_HandleEvent
说明：
       IDD_PLAYLIST_OPTS 对话框事件处理函数
       
参数：
       pMe [in]：指向CMusicPlayer Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。
       
返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。
       
备注：
       
==============================================================================*/
static boolean MP3_PlaylistOpts_HandleEvent(CMusicPlayer *pMe,
                                            AEEEvent eCode,
                                            uint16 wParam,
                                            uint32 dwParam)
{
  IMenuCtl  *pMenuCtl;
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
  MSG_FATAL("MP3_PlaylistOpts_HandleEvent Start",0,0,0);
  pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                        IDC_PLAYLIST_OPTS);

  if (pMenuCtl == NULL)
 {
     return FALSE;
 }
 
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
              MP3MENU_ADDITEM(pMenuCtl,IDS_PLAYLIST_PLAY);
              MP3MENU_ADDITEM(pMenuCtl,IDS_PLAYLIST_CREATELIST);
              MP3MENU_ADDITEM(pMenuCtl,IDS_PLAYLIST_VIEW);
              MP3MENU_ADDITEM(pMenuCtl,IDS_PLAYLIST_ADDMUSIC);
              MP3MENU_ADDITEM(pMenuCtl,IDS_PLAYLIST_RENAME);
              MP3MENU_ADDITEM(pMenuCtl,IDS_PLAYLIST_DELETE);              
              return TRUE;
            
        case EVT_DIALOG_START:
        {
            
//#if defined(FEATURE_DISP_160X128)          
          {
              AEERect rc = {0};
              IMENUCTL_SetPopMenuRect(pMenuCtl); 
              IMENUCTL_GetRect(pMenuCtl, &rc);
              MSG_FATAL("rc.y = %d; rc.dy = %d",rc.y, rc.dy, 0);
              if(rc.y < 0)
              {
                  int temp = -(rc.y);
                  rc.y += temp;
                  rc.dy -= temp;
                  IMENUCTL_SetRect(pMenuCtl, &rc);
              }
              MSG_FATAL("rc.y = %d; rc.dy = %d",rc.y, rc.dy, 0);
          }
//#endif  

           IMENUCTL_SetBottomBarType(pMenuCtl,BTBAR_SELECT_BACK);
            
           // For redraw the dialog
          (void)ISHELL_PostEvent( pMe->m_pShell,
                                  AEECLSID_APP_MUSICPLAYER,
                                  EVT_USER_REDRAW,
                                  0,
                                  0);
           return TRUE;
                                    
        }
        
        case EVT_USER_REDRAW:
            // Restore the menu select from suspend
            IMENUCTL_SetProperties(pMenuCtl, MP_UNDERLINE_TITLE |MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY);
            // Draw prompt bar here 
            IDISPLAY_Update(pMe->m_pDisplay); 
            IMENUCTL_Redraw(pMenuCtl);
            return TRUE;
            
        case EVT_DIALOG_END:
            return TRUE;
            
        case EVT_KEY:
            switch (wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return TRUE;

                case AVK_BGPLAY:
                    ISHELL_CloseApplet(pMe->m_pShell, TRUE);
                    return TRUE;
                    
                default:
                    break;                    
            }
            break;
            
        case EVT_COMMAND:
         
            switch (wParam)
            {
                case IDS_PLAYLIST_CREATELIST:
                    if(pMe->m_nPlaylistNum>=PLAYLIST_MAX_NUM)
                    {
                        CLOSE_DIALOG(DLGRET_CREATELIST_FAILED);
                    }
                    else
                    {
                       CLOSE_DIALOG(DLGRET_CREATEORRENAMELIST);
                    }
                    return TRUE;
                    
                case IDS_PLAYLIST_PLAY:
                    if(FALSE == MP3_PlayPlaylist(pMe))
                    {
                        return TRUE;
                    }
                    CLOSE_DIALOG(DLGRET_PLAYLISTPLAY);
                    return TRUE;
                
                case IDS_PLAYLIST_VIEW:
                    CLOSE_DIALOG(DLGRET_PLAYLISTVIEW);
                    return TRUE;
                    
                case IDS_PLAYLIST_ADDMUSIC:
                   if(pMe->m_nPlaylistMusicNum >= MUSIC_MAX_FILESNUM)
                   { 
                      CLOSE_DIALOG(DLGRET_ADDMUSIC_FAILED);
                   }
                   else
                   {
#ifndef WIN32
                       CMediaGallery_RegisterCallback((PFNMGSELECTEDCB)MP3_AddMusicCB, pMe);
                       CMediaGallery_FileExplorer(GALLERY_MUSIC_ADDLIST, NULL);
#endif//WIN32
                   }
                    return TRUE;
                    
                case IDS_PLAYLIST_RENAME:
                    if(pMe->m_nPlaylistNum>=PLAYLIST_MAX_NUM 
                        && STRCMP(DEFAULT_PLAYLIST,pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName)== 0)
                    {
                        CLOSE_DIALOG(DLGRET_RENAMELIST_FAILED);
                    }
                    else
                    {
                       pMe->m_bRenameOpt = TRUE;
                       CLOSE_DIALOG(DLGRET_CREATEORRENAMELIST);
                    }
                    return TRUE;

                case IDS_PLAYLIST_DELETE:
                    if(STRCMP(pMe->m_MusicPlayerCfg.lastPlayPlaylist,pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName)==0)
                    {
                        if(pMe->m_bPlaying || pMe->m_bPaused)
                        {
                            CLOSE_DIALOG(DLGRET_DELPLAYLISTWARN);
                        }
                        else
                        {
                            CLOSE_DIALOG(DLGRET_PLAYLISTDELETE);
                        }
                    }
                    else
                    {
                       CLOSE_DIALOG(DLGRET_PLAYLISTDELETE);
                    }
                    return TRUE;
                    
                default:
                    break;
                    
            }
            break;
#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
		//case EVT_PEN_DOWN:
		case EVT_PEN_UP:
			{
				AEEDeviceInfo devinfo;
				int nBarH ;
				AEERect rc;
				int16 wXPos = (int16)AEE_GET_X((const char *)dwParam);
				int16 wYPos = (int16)AEE_GET_Y((const char *)dwParam);
				nBarH = GetBottomBarHeight(pMe->m_pDisplay);
				MEMSET(&devinfo, 0, sizeof(devinfo));
				ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
				SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, devinfo.cyScreen);
				if ((wXPos>0)&&(wXPos<devinfo.cxScreen/3)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
				{
					uint16 nEvtCommand = IMENUCTL_GetSel(pMenuCtl);
					boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_SELECT,nEvtCommand);
					return rt;
				}
				else if ((wXPos>devinfo.cxScreen/3)&&(wXPos<(devinfo.cxScreen/3)*2)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
				{
					boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_INFO,0);
					return rt;
				}
				else if ((wXPos>(devinfo.cxScreen/3)*2)&&(wXPos<devinfo.cxScreen)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
				{
					boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_CLR,0);
					return rt;
				}
			}
			return TRUE;
#endif//FEATURE_LCD_TOUCH_ENABLE            
        default:
            break;
            
    }
    
    return FALSE;
}

  /*==============================================================================
函数：
       MP3_Playlist_View_HandleEvent
说明：
       IDD_PLAYLIST_VIEW 对话框事件处理函数
       
参数：
       pMe [in]：指向CMusicPlayer Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。
       
返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。
       
备注：
       
==============================================================================*/
 static boolean MP3_Playlist_View_HandleEvent(CMusicPlayer *pMe,
                                            AEEEvent eCode,
                                            uint16 wParam,
                                            uint32 dwParam)
 {
   IMenuCtl  *pMenuCtl;
    
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    MSG_FATAL("MP3_Playlist_View_HandleEvent Start",0,0,0);
    pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                              IDC_PLAYLIST_VIEWCTL);
    
    if (pMenuCtl == NULL)
    {
         return FALSE;
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
     
            return TRUE;
            
        case EVT_DIALOG_START:
        {
           MP3_SetMenuCtlRect_Prop(pMe,pMenuCtl,MP_UNDERLINE_TITLE |MP_WRAPSCROLL);
		   #if 0
           IMENUCTL_SetTitle(pMenuCtl, MUSICPLAYER_RES_FILE_LANG, IDS_PLAYLIST, NULL);
		   #else
		    {
		  		AECHAR WTitle[40] = {0};
				(void)ISHELL_LoadResString(pMe->m_pShell,
                        MUSICPLAYER_RES_FILE_LANG,                                
                        IDS_PLAYLIST,
                        WTitle,
                        sizeof(WTitle));
                IANNUNCIATOR_SetHasTitleText(pMe->m_pIAnn, TRUE);
				IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,WTitle);
		    }
			#endif
   
           if(SUCCESS!=CMusicPlayer_BuildViewList(pMe,pMenuCtl))
            {
             return FALSE;
            }
            // For redraw the dialog
            (void)ISHELL_PostEvent( pMe->m_pShell,
                                    AEECLSID_APP_MUSICPLAYER,
                                    EVT_USER_REDRAW,
                                    0,
                                    0);
           return TRUE;
                                    
        }
        
        case EVT_USER_REDRAW:
            //set select 
            if(STRCMP(pMe->m_MusicPlayerCfg.lastPlayPlaylist,pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName)==0
                && pMe->m_bPlaying)
            {
                IMENUCTL_SetSel(pMenuCtl, pMe->m_MusicPlayerCfg.lastPlayMusicID);
            }
            else
            {
                IMENUCTL_SetSel(pMenuCtl, pMe->m_nCurMusiclistID);
            }
            //IMENUCTL_SetProperties(pMenuCtl, MP_UNDERLINE_TITLE |MP_WRAPSCROLL);
            IMENUCTL_SetOemProperties( pMenuCtl, OEMMP_DISTINGUISH_INFOKEY_SELECTKEY | OEMMP_USE_MENU_STYLE);
            // Draw prompt bar here
            if(pMe->m_nPlaylistMusicNum == 0)
            {
                IMENUCTL_SetActive(pMenuCtl, FALSE);
                MP3_DrawNoRecord(pMe);
                MP3_DRAW_BOTTOMBAR(BTBAR_ADD_BACK);
                IDISPLAY_Update(pMe->m_pDisplay);  
            }
            else
            {
                MP3_DRAW_BOTTOMBAR(BTBAR_OPTION_BACK);
                IMENUCTL_Redraw(pMenuCtl);
            }
           
            //IMENUCTL_Redraw(pMenuCtl);

            return TRUE;
            
        case EVT_DIALOG_END:
            if(pMe->m_bSuspending && pMe->m_nPlaylistMusicNum != 0)
            {
                pMe->m_nCurMusiclistID = IMENUCTL_GetSel(pMenuCtl);
            }
            return TRUE;
            
        case EVT_KEY:
            switch (wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return TRUE;

                case AVK_BGPLAY:
                    ISHELL_CloseApplet(pMe->m_pShell, TRUE);
                    return TRUE;

                case AVK_INFO:
                    if(pMe->m_nPlaylistMusicNum>0)
                    {
                        pMe->m_nCurMusiclistID=IMENUCTL_GetSel(pMenuCtl);
                        if(FALSE == MP3_PlayPlaylist(pMe))
                        {
                            return TRUE;
                        }
                        CLOSE_DIALOG(DLGRET_VIEWPLAY);
                    }
                    return TRUE;

                case AVK_SELECT:
                    if(pMe->m_nPlaylistMusicNum == 0)
                    {
#ifndef WIN32
                       CMediaGallery_RegisterCallback((PFNMGSELECTEDCB)MP3_AddMusicCB, pMe);
                       CMediaGallery_FileExplorer(GALLERY_MUSIC_ADDLIST, NULL);
#endif
                    }
                    return TRUE;
                    
                default:
                    break;                    
            }
            break;
            
        case EVT_COMMAND:
            if(pMe->m_nPlaylistMusicNum>0)
            {
                pMe->m_nCurMusiclistID = IMENUCTL_GetSel(pMenuCtl);
                CLOSE_DIALOG(DLGRET_OK);
            }
            else
            {  
#ifndef WIN32
               CMediaGallery_RegisterCallback((PFNMGSELECTEDCB)MP3_AddMusicCB, pMe);
               CMediaGallery_FileExplorer(GALLERY_MUSIC_ADDLIST, NULL);
#endif//WIN32
            }
            return TRUE;
            
        default:
            break;
            
    }
    
    return FALSE; 
 }
   /*==============================================================================
函数：
       MP3_View_Opts_HandleEvent
说明：
       IDD_VIEW_OPTS 对话框事件处理函数
       
参数：
       pMe [in]：指向CMusicPlayer Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。
       
返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。
       
备注：
       
==============================================================================*/
 static boolean MP3_View_Opts_HandleEvent(CMusicPlayer *pMe,
                                            AEEEvent eCode,
                                            uint16 wParam,
                                            uint32 dwParam)
{
   IMenuCtl  *pMenuCtl;
    
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
   
    MSG_FATAL("MP3_View_Opts_HandleEvent Start",0,0,0);
    pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                              IDC_VIEW_OPTS_MENU);
    if (pMenuCtl == NULL)
     {
         return FALSE;
     }
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            MP3MENU_ADDITEM(pMenuCtl,IDS_MUSICLIST_PLAY);
            MP3MENU_ADDITEM(pMenuCtl,IDS_MUSICLIST_ADD);
            MP3MENU_ADDITEM(pMenuCtl,IDS_MUSICLIST_DEL);
            MP3MENU_ADDITEM(pMenuCtl,IDS_MUSICLIST_DELALL);
            return TRUE;
            
        case EVT_DIALOG_START:
        {
            
            IMENUCTL_SetPopMenuRect(pMenuCtl);
            IMENUCTL_SetBottomBarType(pMenuCtl,BTBAR_OPTION_BACK);
            
            // For redraw the dialog
            (void)ISHELL_PostEvent( pMe->m_pShell,
                                    AEECLSID_APP_MUSICPLAYER,
                                    EVT_USER_REDRAW,
                                    0,
                                    0);
            return TRUE;
                                    
        }
     
        case EVT_USER_REDRAW:
            // Restore the menu select from suspend
            IMENUCTL_SetProperties(pMenuCtl, MP_UNDERLINE_TITLE |MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY);
           
            IDISPLAY_Update(pMe->m_pDisplay); 
            IMENUCTL_Redraw(pMenuCtl);

            return TRUE;
            
        case EVT_DIALOG_END:

            return TRUE;
            
        case EVT_KEY:
            switch (wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return TRUE;

                case AVK_BGPLAY:
                    ISHELL_CloseApplet(pMe->m_pShell, TRUE);
                    return TRUE;
                    
                default:
                    break;                    
            }
            break;
            
        case EVT_COMMAND:
         
            switch (wParam)
            {
                case IDS_MUSICLIST_PLAY:
                    if(FALSE == MP3_PlayPlaylist(pMe))
                    {
                        return TRUE;
                    }
                    CLOSE_DIALOG(DLGRET_VIEWPLAY);
                    return TRUE;
                   
                case IDS_MUSICLIST_ADD:
                    if(pMe->m_nPlaylistMusicNum >= MUSIC_MAX_FILESNUM)
                    {
                        CLOSE_DIALOG(DLGRET_ADDMUSIC_FAILED);
                    }
                    else
                    {
#ifndef WIN32
                        CMediaGallery_RegisterCallback((PFNMGSELECTEDCB)MP3_AddMusicCB, pMe);
                        CMediaGallery_FileExplorer(GALLERY_MUSIC_ADDLIST, NULL);
#endif
                    }
                     return TRUE;
                    
                case IDS_MUSICLIST_DEL:
                    //if(STRCMP(pMe->m_PlayingMusiclist[pMe->m_MusicPlayerCfg.lastPlayMusicID].pMusicName,
                    //          pMe->m_Musiclist[pMe->m_nCurMusiclistID].pMusicName)==0 && 
                    //          STRCMP(pMe->m_MusicPlayerCfg.lastPlayPlaylist,pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName)==0)
                    if(pMe->m_MusicPlayerCfg.lastPlayMusicID == pMe->m_nCurMusiclistID &&
                        STRCMP(pMe->m_MusicPlayerCfg.lastPlayPlaylist,pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName)==0)
                    {
                        if(pMe->m_bPlaying || pMe->m_bPaused)
                        {
                            CLOSE_DIALOG(DLGRET_DELPLAYLISTWARN);
                        }
                        else
                        {
                            CLOSE_DIALOG(DLGRET_VIEWDELETE);
                        }
                    }
                    else
                    {   
                        CLOSE_DIALOG(DLGRET_VIEWDELETE);
                    }
                    return TRUE;
                    
                case IDS_MUSICLIST_DELALL:
                    if(STRCMP(pMe->m_MusicPlayerCfg.lastPlayPlaylist,pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName)==0)
                    {
                         if(pMe->m_bPlaying || pMe->m_bPaused)
                        {
                            CLOSE_DIALOG(DLGRET_DELPLAYLISTWARN);
                        }
                        else
                        {
                            CLOSE_DIALOG(DLGRET_VIEWDELETEALL);
                        }
                    }
                    else
                    {
                      CLOSE_DIALOG(DLGRET_VIEWDELETEALL);
                    }
                    return TRUE;
                    
                default:
                    break;
                    
            }
            break;
            
        default:
            break;
            
    }
    
    return FALSE;
}

   /*==============================================================================
函数：
       MP3_CreatOrRenamelist_HandleEvent
说明：
       IDD_RENAME 对话框事件处理函数
       
参数：
       pMe [in]：指向CMusicPlayer Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。
       
返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。
       
备注：
       
==============================================================================*/   
static boolean MP3_CreatOrRenamelist_HandleEvent(CMusicPlayer *pMe,
                                            AEEEvent eCode,
                                            uint16 wParam,
                                            uint32 dwParam)
{
    ITextCtl *pTextCtl;
    char srcFileName[MP3_MAX_FILE_NAME];
    char destFileName[MP3_MAX_FILE_NAME];
    int i;
    IFile  *pFile;
    
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    MSG_FATAL("MP3_CreatOrRenamelist_HandleEvent Start",0,0,0);
    pTextCtl = (ITextCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                             IDC_CREATEORRENAMELIST_TEXTCTL);
    if (pTextCtl == NULL)
    {
       return FALSE;
    }
   
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            return TRUE;
            
        case EVT_DIALOG_START:
        {
            AEERect        rc;
            AEEDeviceInfo devinfo;

            AECHAR temp_list_name[16] = {0};

            ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
            rc = pMe->m_rc;
            rc.dy = devinfo.cyScreen;
            rc.dy -= GetBottomBarHeight(pMe->m_pDisplay);
            ITEXTCTL_SetRect(pTextCtl, &rc);

            ITEXTCTL_SetProperties( pTextCtl,TP_MULTILINE | TP_FRAME | TP_STARKEY_SWITCH |TP_DISPLAY_COUNT | TP_NOUPDATE | TP_FIXSETRECT|TP_FOCUS_NOSEL|TP_GRAPHIC_BG);// | TP_FIXSETRECT
            (void)ITEXTCTL_SetInputMode(pTextCtl, AEE_TM_CURRENT);
            if(pMe->m_bRenameOpt)
            {
             (void)ITEXTCTL_SetTitle( pTextCtl,
                                     MUSICPLAYER_RES_FILE_LANG,
                                     IDS_PLAYLIST_RENAME,
                                     NULL);
            }
            else
            {
                (void)ITEXTCTL_SetTitle( pTextCtl,
                                     MUSICPLAYER_RES_FILE_LANG,
                                     IDS_PLAYLIST_CREATELIST,
                                     NULL);
            }
            ITEXTCTL_SetMaxSize( pTextCtl, MAX_INPUT_NAME);
            if(pMe->m_bRenameOpt)
            {
                //UTF8 to WSTR
                UTF8TOWSTR((const byte*)pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName, 
                                    STRLEN(pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName),
                                    temp_list_name,MG_MAX_FILE_NAME);
                if (WSTRLEN(temp_list_name) != 0)
                {
                    (void)ITEXTCTL_SetText(pTextCtl, temp_list_name, -1);
                }
            }
            if (pMe->m_pInputListName)
            {
             (void)ITEXTCTL_SetText(pTextCtl, pMe->m_pInputListName, -1);
            }

            // For redraw the dialog
            (void)ISHELL_PostEvent( pMe->m_pShell,
                                    AEECLSID_APP_MUSICPLAYER,
                                    EVT_USER_REDRAW,
                                    0,
                                    0);
            return TRUE;
                                    
        }
        
        case EVT_KEY_RELEASE:
        case EVT_USER_REDRAW:
            {
                AEETextInputMode nInputMode; 
                nInputMode = ITEXTCTL_GetInputMode(pTextCtl,NULL);
                if (nInputMode == AEE_TM_SYMBOLS)
                {
                    return TRUE;
                }
            
                if ( WSTRLEN(ITEXTCTL_GetTextPtr(pTextCtl)) > 0 && ITEXTCTL_GetT9End(pTextCtl) != TC_CURSORSTART )
                {
                    MP3_DRAW_BOTTOMBAR(BTBAR_OK_DELETE);
                }
                else
                {
                    MP3_DRAW_BOTTOMBAR(BTBAR_BACK);            
                }

                IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);  
            }
            return TRUE;
            
        case EVT_DIALOG_END:
            if (pMe->m_bSuspending)
            {
                FREEIF(pMe->m_pInputListName);
                pMe->m_pInputListName= WSTRDUP(ITEXTCTL_GetTextPtr(pTextCtl));
            }
            return TRUE;
            
        case EVT_KEY:
            switch (wParam)
            {
                case AVK_CLR:
                    FREEIF(pMe->m_pInputListName);
                    if(pMe->m_bRenameOpt)
                    {
                        pMe->m_bRenameOpt = FALSE;
                    }
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return TRUE;

               case AVK_INFO:
               case AVK_SELECT:
                    FREEIF(pMe->m_pInputListName);
                    pMe->m_pInputListName = WSTRDUP(ITEXTCTL_GetTextPtr(pTextCtl));
                    FREEIF(pMe->m_pBuffer);
                    pMe->m_pBuffer = (AECHAR*)MALLOC(PLAYLISTNAME_MAX_LEN * sizeof(AECHAR));
                    if(WSTRLEN(pMe->m_pInputListName) > 0)
                    {    
                        for(i=0;i<pMe->m_nPlaylistNum;i++)
                        {
                          (void)UTF8TOWSTR((const byte *)pMe->m_Playlist[i].pPlaylistName,
                                             STRLEN(pMe->m_Playlist[i].pPlaylistName),
                                             pMe->m_pBuffer,
                                             PLAYLISTNAME_MAX_LEN * sizeof(AECHAR));
                          if(0==WSTRCMP(pMe->m_pBuffer,pMe->m_pInputListName))
                          {
                            CLOSE_DIALOG(DLGRET_REPEAT_PLAYLISTNAME);
                            return TRUE;
                          }
                          MEMSET(pMe->m_pBuffer,0,PLAYLISTNAME_MAX_LEN * sizeof(AECHAR));
                        }
                        if(WSTRRCHR(pMe->m_pInputListName,(AECHAR)'/')!=NULL)
                        {
                            CLOSE_DIALOG(DLGRET_INVALID_PLAYLISTNAME);
                            return TRUE;
                        }
                        if(pMe->m_bRenameOpt)
                        {
                            boolean bPlayinglist = FALSE;
                            if(0==STRCMP(pMe->m_MusicPlayerCfg.lastPlayPlaylist,pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName))
                            {
                                bPlayinglist =TRUE;
                            }
                            pMe->m_bRenameOpt = FALSE;
                            (void)STRCPY(srcFileName,MUSICPLAYLIST_DIR);
                            (void)STRCAT(srcFileName,"/");
                            (void)STRCAT(srcFileName,pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName);
                            (void)STRCAT(srcFileName,".txt");
                            MEMSET(pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName,0,PLAYLISTNAME_MAX_LEN*sizeof(char));
                            (void)WSTRTOUTF8(pMe->m_pInputListName,
                                             WSTRLEN(pMe->m_pInputListName),
                                            (byte *)pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName,
                                             sizeof(pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName));
                            if(bPlayinglist)
                            {
                                (void)STRCPY(pMe->m_MusicPlayerCfg.lastPlayPlaylist,
                                             pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName);
                            }
                            (void)STRCPY(destFileName,MUSICPLAYLIST_DIR);
                            (void)STRCAT(destFileName,"/");
                            (void)STRCAT(destFileName,pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName);
                            (void)STRCAT(destFileName,".txt");
                            (void)IFILEMGR_Rename (pMe->m_pFileMgr, srcFileName, destFileName);
                        }
                        else
                        {
                            (void)WSTRTOUTF8(pMe->m_pInputListName,
                                             WSTRLEN(pMe->m_pInputListName),
                                            (byte*)pMe->m_Playlist[pMe->m_nPlaylistNum].pPlaylistName,
                                             sizeof(pMe->m_Playlist[pMe->m_nPlaylistNum].pPlaylistName));
                            FREEIF(pMe->m_pInputListName);
                            (void)STRCPY(destFileName,MUSICPLAYLIST_DIR);
                            (void)STRCAT(destFileName,"/");
                            (void)STRCAT(destFileName,pMe->m_Playlist[pMe->m_nPlaylistNum].pPlaylistName);
                            (void)STRCAT(destFileName,".txt");
                            pFile=IFILEMGR_OpenFile(pMe->m_pFileMgr, destFileName, _OFM_CREATE);
                            if(NULL != pFile)
                            {
                              IFILE_Release(pFile);
                              pFile = NULL;
                            }
                        }
                        CLOSE_DIALOG(DLGRET_OK);
                        FREEIF(pMe->m_pInputListName);
                    }
                    else
                    {
                       // CLOSE_DIALOG(DLGRET_INPUT_NULL);
                    }
                    return TRUE; 
                    
                default:
                    break;
            }
            break;

        case EVT_KEY_PRESS:
            switch(wParam)
            {
                 case AVK_BGPLAY:
                    ISHELL_CloseApplet(pMe->m_pShell, TRUE);
                    return TRUE;

                default:
                    break;
            }
            break;
            
        case EVT_COMMAND:
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
		case EVT_PEN_UP:
			{
				AEEDeviceInfo devinfo;
				int nBarH ;
				AEERect rc;
				int16 wXPos = (int16)AEE_GET_X((const char *)dwParam);
				int16 wYPos = (int16)AEE_GET_Y((const char *)dwParam);
				nBarH = GetBottomBarHeight(pMe->m_pDisplay);
				MEMSET(&devinfo, 0, sizeof(devinfo));
				ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
				SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, devinfo.cyScreen);
				if ((wXPos>0)&&(wXPos<devinfo.cxScreen/3)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
				{
					boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_SELECT,0);
					return rt;
				}
				else if ((wXPos>devinfo.cxScreen/3)&&(wXPos<(devinfo.cxScreen/3)*2)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
				{
					boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_INFO,0);
					return rt;
				}
				else if ((wXPos>(devinfo.cxScreen/3)*2)&&(wXPos<devinfo.cxScreen)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
				{
					boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_CLR,0);
					return rt;
				}


			}
			return TRUE;
#endif//FEATURE_LCD_TOUCH_ENABLE    
        default:
            break;
            
    }

    return FALSE;
}
/*===========================================================================
  播放音乐时key事件处理函数
===========================================================================*/
static boolean MP3_MusicPlayerHandleKeyEvent(CMusicPlayer*pMe,
                                            AEEEvent eCode,
                                            uint16  wParam ,
                                            uint32 dwParam)
{

#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
	//if ((eCode == EVT_PEN_UP)||(eCode == EVT_PEN_DOWN))
	if (eCode == EVT_PEN_UP)
	{
		int i;
		AEEDeviceInfo devinfo;
		int nBarH ;
		AEERect rc;
		int16 wXPos = (int16)AEE_GET_X((const char *)dwParam);
		int16 wYPos = (int16)AEE_GET_Y((const char *)dwParam);


		//播放
		SETAEERECT(&rc,PLAY_X,PLAY_Y,PLAY_W, PLAY_H);
		if(MUSICPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
		{
			wParam = AVK_INFO;
		}
		//前一首
		SETAEERECT(&rc,PREVIOUSPRESS_X,PREVIOUSPRESS_Y,PREVIOUSPRESS_W, PREVIOUSPRESS_H);
		if(MUSICPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
		{
			wParam = AVK_LEFT;
		}
		//下一首
		SETAEERECT(&rc,NEXTPRESS_X,NEXTPRESS_Y,NEXTPRESS_W, NEXTPRESS_H);
	    if(MUSICPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
		{
			wParam = AVK_RIGHT;
		}
		//增加音量
		SETAEERECT(&rc,ADDVOLUMEPRESS_X,ADDVOLUMEPRESS_Y,ADDVOLUMEPRESS_W, ADDVOLUMEPRESS_H);
		if(MUSICPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
		{
			wParam = AVK_UP;
		}
		//降低音量
		SETAEERECT(&rc,DECREASEVOLUMEPRESS_X,DECREASEVOLUMEPRESS_Y,DECREASEVOLUMEPRESS_W, DECREASEVOLUMEPRESS_H);
		if(MUSICPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
		{
			wParam = AVK_DOWN;
		}
		//底部操作栏
		nBarH = GetBottomBarHeight(pMe->m_pDisplay);
		MEMSET(&devinfo, 0, sizeof(devinfo));
		ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
		SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, devinfo.cyScreen);
		if ((wXPos>0)&&(wXPos<devinfo.cxScreen/3)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
		{
			wParam = AVK_SELECT;
		}
		else if ((wXPos>devinfo.cxScreen/3)&&(wXPos<(devinfo.cxScreen/3)*2)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
		{
			wParam = AVK_INFO;
		}
		else if ((wXPos>wXPos<(devinfo.cxScreen/3)*2)&&(wXPos<devinfo.cxScreen)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
		{
			wParam = AVK_CLR;
		}
	}
#endif//FEATURE_LCD_TOUCH_ENABLE

  switch(wParam)
  {    
     case AVK_SELECT:
        ISHELL_CancelTimer(pMe->m_pShell,(PFNNOTIFY)MP3_MusicNameAutoScroll,pMe);
        pMe->m_rtype = TYPE_PLAYER;//wlh 20090415 mod 为了区别播放区域，加音量，减音量的刷新，加了个参数
        ISHELL_CancelTimer(pMe->m_pShell, (PFNNOTIFY)MP3_DrawImageWithOffset,pMe);
        ISHELL_CancelTimer(pMe->m_pShell, (PFNNOTIFY)MP3_DrawForwardImage, pMe);
        ISHELL_CancelTimer(pMe->m_pShell, (PFNNOTIFY)MP3_DrawRewindImage, pMe);
        CLOSE_DIALOG(DLGRET_OK);
        return TRUE;
     case AVK_INFO://播放 
        if((!pMe->m_bPlaying)&&(!pMe->m_bPaused))
        {
            if(pMe->m_MusicPlayerCfg.eMusicPlayMode == PLAYMODE_RANDOM)
            {
              MP3_ResetRandonIdentifier(pMe);
            }
            if(pMe->m_pMedia)
            {
                IMEDIA_Play(pMe->m_pMedia);
                pMe->m_bPlaying=TRUE;
                pMe->m_bPaused = FALSE;
                (void) ISHELL_PostEvent(pMe->m_pShell, 
                                        AEECLSID_APP_MUSICPLAYER,
                                        EVT_USER_REDRAW,
                                        0,
                                        0);       
            }   
        }
        else if(pMe->m_bPaused)
        {
            
            if(pMe->m_pMedia)
            {  
                DBGPRINTF("pMe->m_pMedia is not null imediaresume");
                (void)IMEDIA_Resume(pMe->m_pMedia);
                pMe->m_bPaused=FALSE;
                pMe->m_bPlaying = TRUE;
                (void) ISHELL_PostEvent(pMe->m_pShell, 
                                    AEECLSID_APP_MUSICPLAYER,
                                    EVT_USER_REDRAW,
                                    0,
                                    0);       
            }
        }
       else if(pMe->m_bPlaying)
        {
          //当开始播放和临近结束时暂停会有问题，在这里进行出错处理
          if(pMe->m_nCurrentTime == pMe->m_nTotalTime || pMe->m_nCurrentTime == 0)
          {
            return TRUE;
          }
          if(pMe->m_pMedia)
          { 
            if(SUCCESS == IMEDIA_Pause(pMe->m_pMedia))
            {  
               pMe->m_bPaused= TRUE;
               pMe->m_bPlaying = FALSE;                  
              (void) ISHELL_PostEvent(pMe->m_pShell, 
                                    AEECLSID_APP_MUSICPLAYER,
                                    EVT_USER_REDRAW,
                                    0,
                                    0);       
            }
         }     
      }
       return TRUE;
       
    case AVK_UP:
         MP3_DrawImage( pMe,IDI_ADDVOLUME_PRESS, ADDVOLUMEPRESS_X, ADDVOLUMEPRESS_Y);
         IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
		 pMe->m_rtype = TYPE_ADDVOLUME;//wlh 20090415 mod 为了区别播放区域，加音量，减音量的刷新，加了个参数
         ISHELL_SetTimer(pMe->m_pShell,50,(PFNNOTIFY)MP3_DrawImageWithOffset, pMe);
         if(pMe->m_MusicPlayerCfg.eMusicVolume < VOLUME_FIVE)
         {
            pMe->m_MusicPlayerCfg.eMusicVolume++;
            pMe->m_nCurrentVolume= pMe->m_MusicPlayerCfg.eMusicVolume * AEE_MAX_VOLUME/5;
            if(pMe->m_pMedia) 
            {    
                (void)IMEDIA_SetVolume(pMe->m_pMedia,pMe->m_nCurrentVolume); 
            } 
                //重新刷新音量bar  
                MP3_RefreshVolBar(pMe);
                
         }
        // IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);//wlh 20090415 mod true -> false
         return TRUE;
         
    case AVK_DOWN:
          MP3_DrawImage( pMe,IDI_DECREASEVOLUME_PRESS, DECREASEVOLUMEPRESS_X, DECREASEVOLUMEPRESS_Y);
          IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
		  pMe->m_rtype = TYPE_DECVOLUME;//wlh 20090415 mod 为了区别播放区域，加音量，减音量的刷新，加了个参数
          ISHELL_SetTimer(pMe->m_pShell,50,(PFNNOTIFY)MP3_DrawImageWithOffset, pMe);
          if(pMe->m_MusicPlayerCfg.eMusicVolume > VOLUME_OFF)
         {
           pMe->m_MusicPlayerCfg.eMusicVolume--;
           pMe->m_nCurrentVolume= pMe->m_MusicPlayerCfg.eMusicVolume * AEE_MAX_VOLUME/5;
            if(pMe->m_pMedia) 
            {  
                (void)IMEDIA_SetVolume(pMe->m_pMedia,pMe->m_nCurrentVolume); 
            } 
                //重新刷新音量bar  
                MP3_RefreshVolBar(pMe);
                
         }
        IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);//wlh 20090415 mod true -> false
        return  TRUE;
    //case AVK_GSENSOR_FORWARD:    
    case AVK_LEFT:
        MP3_DrawImage( pMe,IDI_PREVIOUS_PRESS, PREVIOUSPRESS_X, PREVIOUSPRESS_Y);
        IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
		pMe->m_rtype = TYPE_PREVIOUS;//wlh 20090415 mod 为了区别播放区域，加音量，减音量的刷新，加了个参数
        ISHELL_SetTimer(pMe->m_pShell,50,(PFNNOTIFY)MP3_DrawImageWithOffset, pMe);
        CMusicPlayer_PlayNext(pMe,FALSE );//播放上一首
        return TRUE;
        
    //case AVK_GSENSOR_BACKWARD:   
    case AVK_RIGHT:
        MP3_DrawImage( pMe,IDI_NEXT_PRESS,NEXTPRESS_X, NEXTPRESS_Y);
        IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
		pMe->m_rtype = TYPE_NEXT;//wlh 20090415 mod 为了区别播放区域，加音量，减音量的刷新，加了个参数
        ISHELL_SetTimer(pMe->m_pShell,50,(PFNNOTIFY)MP3_DrawImageWithOffset, pMe);
        pMe->m_bUserPressNext = TRUE;
        CMusicPlayer_PlayNext(pMe,TRUE);//播放下一首
        pMe->m_bUserPressNext = FALSE;
        return TRUE;
        
    case AVK_CLR:
       if(pMe->m_bPlaying||pMe->m_bPaused)
       {
            ISHELL_CancelTimer(pMe->m_pShell,(PFNNOTIFY)CMusicPlayer_InitMusic,pMe);
            ISHELL_CancelTimer(pMe->m_pShell,(PFNNOTIFY) CMusicPlayer_PlayMusic,pMe);
			  pMe->m_rtype = TYPE_PLAYER;//wlh 20090415 mod 为了区别播放区域，加音量，减音量的刷新，加了个参数
            ISHELL_CancelTimer(pMe->m_pShell, (PFNNOTIFY)MP3_DrawImageWithOffset,pMe);
            ISHELL_CancelTimer(pMe->m_pShell, (PFNNOTIFY)MP3_DrawForwardImage, pMe);
            ISHELL_CancelTimer(pMe->m_pShell, (PFNNOTIFY)MP3_DrawRewindImage, pMe);
            //停止
            if(pMe->m_pMedia)
            {  
                if(SUCCESS == IMEDIA_Stop(pMe->m_pMedia))
                {
                    pMe->m_bUserStopped= TRUE;
                    pMe->m_nCurrentTime = 0;
                    pMe->m_bPlaying = FALSE;
                    pMe->m_bPaused= FALSE;
                    (void) ISHELL_PostEvent(pMe->m_pShell, 
                                            AEECLSID_APP_MUSICPLAYER,
                                            EVT_USER_REDRAW,
                                            0,
                                            0);       
                }
            }
         }
       else
       {
          CLOSE_DIALOG(DLGRET_CANCELED);
       }
       return TRUE;

    case AVK_POUND:
        if(pMe->m_bPlaying && pMe->m_pMedia)
        {
            MP3_DrawImage(pMe, IDI_FORWARD_PRESS, 40,140);
            IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
            ISHELL_SetTimer(pMe->m_pShell,50,(PFNNOTIFY)MP3_DrawForwardImage, pMe);
            if((pMe->m_nTotalTime - pMe->m_nCurrentTime) < MS_FASTFORWARDREWIND_TIME/1000)
            {
                IMEDIA_FastForward(pMe->m_pMedia,1000*(pMe->m_nTotalTime - pMe->m_nCurrentTime));
                pMe->m_nCurrentTime = pMe->m_nTotalTime;
            }
            else 
            {
                IMEDIA_FastForward(pMe->m_pMedia,MS_FASTFORWARDREWIND_TIME); 
                pMe->m_nCurrentTime = pMe->m_nCurrentTime + MS_FASTFORWARDREWIND_TIME/1000;
            }
        }
        return TRUE;

    case AVK_STAR:
        if(pMe->m_bPlaying && pMe->m_pMedia)
        {
            MP3_DrawImage(pMe, IDI_REWIND_PRESS, 3,140);
            IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
            ISHELL_SetTimer(pMe->m_pShell,50,(PFNNOTIFY)MP3_DrawRewindImage, pMe);
            if(pMe->m_nCurrentTime < MS_FASTFORWARDREWIND_TIME/1000)
            {
                IMEDIA_Rewind(pMe->m_pMedia,pMe->m_nCurrentTime * 1000);
                pMe->m_nCurrentTime = 0;
            }
            else
            {
                IMEDIA_Rewind(pMe->m_pMedia,MS_FASTFORWARDREWIND_TIME);
                pMe->m_nCurrentTime = pMe->m_nCurrentTime - MS_FASTFORWARDREWIND_TIME/1000;
            }
            
        }
        return TRUE;
        
    case AVK_BGPLAY:
        if(pMe->m_bPlaying)
        {
            //处理背景播放
            SetMp3PlayerStatus(pMe, MP3STATUS_RUNONBACKGROUND);
            ISHELL_CloseApplet(pMe->m_pShell, TRUE);
        }
        else
        {
         //CLOSE_DIALOG(DLGRET_CANCELED);
         ISHELL_CloseApplet(pMe->m_pShell, TRUE);
        }
        return TRUE;
        
   default:
       break;

    }
    return FALSE;
}
 /*==============================================================================
函数：
      MP3_SimplePlayer_HandleEvent
说明：
      IDD_SIMPLEPLAYER 对话框事件处理函数
      
参数：
      pMe [in]：指向CMusicPlayer Applet对象结构的指针。该结构包含小程序的特定信息。
      eCode [in]：事件代码。
      wParam：事件相关数据。
      dwParam：事件相关数据。
      
返回值：
      TRUE：传入事件被处理。
      FALSE：传入事件被忽略。
      
备注：
      
==============================================================================*/  

static boolean MP3_SimplePlayer_HandleEvent(CMusicPlayer *pMe,
                                            AEEEvent eCode,
                                            uint16 wParam,
                                            uint32 dwParam)
 {
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    MSG_FATAL("MP3_SimplePlayer_HandleEvent Start",0,0,0);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:   
        {  
            CMusicPlayer_ReleaseMedia(pMe);
            //CMusicPlayer_InitMusic(pMe);
            //pMe->m_bStartApp = TRUE;
            MP3_InitMusicCB(pMe);
			
            if(pMe->m_pMedia && !pMe->m_bSimPlayFailed)
            {
             pMe->m_bPlaying = TRUE;
             pMe->m_bPaused = FALSE;
             //因为底层使用的是事件传递机制，需要给底层初始化并传递notify的时间，否则会出错，此处等0.2秒钟后播放     
             // for mid or qcp files in sd, it needs to copy file from sd to nor flash for playing, so delay the timer to 1200 for CTA temporarily, huasheng.li2009.03.10
             (void) ISHELL_SetTimer(pMe->m_pShell,1200,(PFNNOTIFY) CMusicPlayer_PlayMusic,pMe);
            }
            return TRUE;
        }
        case EVT_DIALOG_START:
        {			
           (void) ISHELL_PostEvent(pMe->m_pShell, 
                                    AEECLSID_APP_MUSICPLAYER,
                                    EVT_USER_REDRAW,
                                    0,
                                    0);          
            return TRUE;
        }
        case EVT_USER_REDRAW:
        {          
            MP3_DrawSimplePlayerFace(pMe);
            if(pMe->m_bPlaying)
            {
              MP3_DRAW_BOTTOMBAR(BTBAR_PAUSE_BACK);
            }
            else if(pMe->m_bPaused)
            {
                MP3_DRAW_BOTTOMBAR(BTBAR_RESUME_BACK);
            }
            else if(!pMe->m_bPlaying && !pMe->m_bPaused)
            {
                MP3_DRAW_BOTTOMBAR(BTBAR_REPLAY_BACK);
            }
            IDISPLAY_Update(pMe->m_pDisplay);
            return TRUE;
            
        }
        case EVT_DIALOG_END:
        {    			
            return TRUE;            
        }		
		
        case EVT_KEY:
             switch(wParam)
             {
                case AVK_CLR:
				{			
					//Add By zzg 2010_07_29
					if (HS_HEADSET_ON())
					{						
						IMEDIA_SetMediaParm( pMe->m_pMedia, MM_PARM_AUDIO_DEVICE, AEE_SOUND_DEVICE_STEREO_HEADSET, 0);	
					}
					else
					{						
						IMEDIA_SetMediaParm( pMe->m_pMedia, MM_PARM_AUDIO_DEVICE, AEE_SOUND_DEVICE_HANDSET, 0);	
					}
					//Add End
				
					CMusicPlayer_ReleaseMedia(pMe);
                    pMe->m_bPlaying = TRUE;
                    pMe->m_bPaused = FALSE;
                    ISHELL_CancelTimer(pMe->m_pShell,(PFNNOTIFY)MP3_MusicNameAutoScroll,pMe);
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return TRUE;
                }        
               case AVK_UP:
               case AVK_RIGHT:
                    if(pMe->m_MusicPlayerCfg.eMusicVolume < VOLUME_FIVE)
                    {
                        pMe->m_MusicPlayerCfg.eMusicVolume++;
                        pMe->m_nCurrentVolume= pMe->m_MusicPlayerCfg.eMusicVolume * AEE_MAX_VOLUME/5;
                        if(pMe->m_pMedia) 
                        {
                           (void)IMEDIA_SetVolume(pMe->m_pMedia,pMe->m_nCurrentVolume); 
                        } 
                        (void)ISHELL_PostEvent(pMe->m_pShell,
                                               AEECLSID_APP_MUSICPLAYER,
                                               EVT_USER_REDRAW,
                                               0,
                                               0);
                    }
                    return TRUE;
               case AVK_SELECT:
                    if(pMe->m_pMedia && pMe->m_bPlaying)
                    {
                        if(SUCCESS == IMEDIA_Pause(pMe->m_pMedia))
                        {
                            pMe->m_bPaused = TRUE;
                            pMe->m_bPlaying = FALSE;
                            (void)ISHELL_PostEvent(pMe->m_pShell,
                                                   AEECLSID_APP_MUSICPLAYER,
                                                   EVT_USER_REDRAW,
                                                   0,
                                                   0);
                        }
                    }
                    else if(pMe->m_pMedia && pMe->m_bPaused)
                    {
                        if(SUCCESS == IMEDIA_Resume(pMe->m_pMedia))
                        {
                            pMe->m_bPaused = FALSE;
                            pMe->m_bPlaying = TRUE;
                            (void)ISHELL_PostEvent(pMe->m_pShell,
                                                   AEECLSID_APP_MUSICPLAYER,
                                                   EVT_USER_REDRAW,
                                                   0,
                                                   0);
                        }
                    }
                    else if(pMe->m_pMedia && !pMe->m_bPaused && !pMe->m_bPlaying)
                    {
                        if(SUCCESS == IMEDIA_Play(pMe->m_pMedia ))
                        {
                            pMe->m_bPaused = FALSE;
                            pMe->m_bPlaying = TRUE;
                            //pMe->m_nSimPlayCurTime = 0;
                            (void)ISHELL_PostEvent(pMe->m_pShell,
                                                   AEECLSID_APP_MUSICPLAYER,
                                                   EVT_USER_REDRAW,
                                                   0,
                                                   0);
                        }
                    }
                    return TRUE;
                    
               case AVK_DOWN:
               case AVK_LEFT:
                    if(pMe->m_MusicPlayerCfg.eMusicVolume > VOLUME_OFF)
                    {
                        pMe->m_MusicPlayerCfg.eMusicVolume--;
                        pMe->m_nCurrentVolume= pMe->m_MusicPlayerCfg.eMusicVolume * AEE_MAX_VOLUME/5;
                        if(pMe->m_pMedia) 
                        {
                           (void)IMEDIA_SetVolume(pMe->m_pMedia,pMe->m_nCurrentVolume); 
                        } 
                        (void)ISHELL_PostEvent(pMe->m_pShell,
                                               AEECLSID_APP_MUSICPLAYER,
                                               EVT_USER_REDRAW,
                                               0,
                                               0);
                    }
                    return TRUE;
                    
               case AVK_BGPLAY:
			   	{
					//Add By zzg 2010_07_29
					if (HS_HEADSET_ON())
					{						
						IMEDIA_SetMediaParm( pMe->m_pMedia, MM_PARM_AUDIO_DEVICE, AEE_SOUND_DEVICE_STEREO_HEADSET, 0);	
					}
					else
					{						
						IMEDIA_SetMediaParm( pMe->m_pMedia, MM_PARM_AUDIO_DEVICE, AEE_SOUND_DEVICE_HANDSET, 0);	
					}
					//Add End
					
                    CMusicPlayer_ReleaseMedia(pMe);
                    ISHELL_CloseApplet(pMe->m_pShell, TRUE);
                    return TRUE;
               }
               default:
                  break;
             }
#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
	  case EVT_PEN_UP:
		  {
			  int i;
			  AEEDeviceInfo devinfo;
			  int nBarH ;
			  AEERect rc;
			  int16 wXPos = (int16)AEE_GET_X((const char *)dwParam);
			  int16 wYPos = (int16)AEE_GET_Y((const char *)dwParam);



			  //播放
			  SETAEERECT(&rc,PLAY_X,PLAY_Y,PLAY_W, PLAY_H);
			  if(MUSICPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
			  {
				  return  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_INFO,0);
			  }
			  //前一首
			  SETAEERECT(&rc,PREVIOUSPRESS_X,PREVIOUSPRESS_Y,PREVIOUSPRESS_W, PREVIOUSPRESS_H);
			  if(MUSICPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
			  {
				  return  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_INFO,0);
			  }
			  //下一首
			  SETAEERECT(&rc,NEXTPRESS_X,NEXTPRESS_Y,NEXTPRESS_W, NEXTPRESS_H);
			  if(MUSICPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
			  {
				  return  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_INFO,0);
			  }
			  //增加音量
			  SETAEERECT(&rc,ADDVOLUMEPRESS_X,ADDVOLUMEPRESS_Y,ADDVOLUMEPRESS_W, ADDVOLUMEPRESS_H);
			  if(MUSICPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
			  {
				  return  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_INFO,0);
			  }
			  //降低音量
			  SETAEERECT(&rc,DECREASEVOLUMEPRESS_X,DECREASEVOLUMEPRESS_Y,DECREASEVOLUMEPRESS_W, DECREASEVOLUMEPRESS_H);
			  if(MUSICPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
			  {
				  return  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_INFO,0);
			  }
			  //底部操作栏
			  nBarH = GetBottomBarHeight(pMe->m_pDisplay);
			  MEMSET(&devinfo, 0, sizeof(devinfo));
			  ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
			  SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, devinfo.cyScreen);
			  if ((wXPos>0)&&(wXPos<devinfo.cxScreen/3)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
			  {
				  boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_SELECT,1);
				  return rt;
			  }
			  else if ((wXPos>devinfo.cxScreen/3)&&(wXPos<(devinfo.cxScreen/3)*2)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
			  {
				  boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_INFO,0);
				  return rt;
			  }
			  else if ((wXPos>wXPos<(devinfo.cxScreen/3)*2)&&(wXPos<devinfo.cxScreen)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
			  {
				  boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_MUSICPLAYER,EVT_USER,AVK_CLR,0);
				  return rt;
			  }
		  }
		  return TRUE;
#endif//FEATURE_LCD_TOUCH_ENABLE             
        default:
            break;
        }//switch (eCode)

        return FALSE;
 }

/*==============================================================================
函数：
       CMusicPlayer_HandleMsgBoxDlgEvent
说明：
       IDD_MSGBOX 对话框事件处理函数
       
参数：
       pMe [in]：指向ContApp Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。
       
返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。
       
备注：
       
==============================================================================*/
static boolean  CMusicPlayer_HandleMsgBoxDlgEvent( CMusicPlayer  *pMe,
                                               AEEEvent   eCode,
                                               uint16     wParam,
                                               uint32     dwParam)
{
   static IStatic * pStatic = NULL;
   MSG_FATAL("CMusicPlayer_HandleMsgBoxDlgEvent Start",0,0,0);
   if (NULL == pStatic)
   {
        AEERect rect = {0};
        if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                                 AEECLSID_STATIC,
                                                 (void **)&pStatic))
        
        {
            return FALSE;
            ERR("ISHELL_CreateInstance,AEECLSID_STATIC 2",0,0,0);
        }        
        ISTATIC_SetRect(pStatic, &rect);  
   }

    if ((NULL == pStatic) ||(NULL == pMe))
    {
        return FALSE;
    }
    
    switch(eCode)
    {
        case EVT_DIALOG_INIT:
            return TRUE;

        case EVT_DIALOG_START:
            (void) ISHELL_PostEventEx(pMe->m_pShell, 
                                    EVTFLG_ASYNC,
                                    AEECLSID_APP_MUSICPLAYER,
                                    EVT_USER_REDRAW,
                                    0, 
                                    0);

            return TRUE;

        case EVT_USER_REDRAW:
            {
                PromptMsg_Param_type  Msg_Param={0};
                AECHAR  wstrText[MSGBOX_MAXTEXTLEN] = {(AECHAR)'\0'};
                
                // 从资源文件取消息内容
                (void)ISHELL_LoadResString(pMe->m_pShell,
                                MUSICPLAYER_RES_FILE_LANG,                                
                                pMe->m_nMsgResID,
                                wstrText,
                                sizeof(wstrText));
                                
                //Msg_Param.ePMsgType = MESSAGE_INFORMATION;
                Msg_Param.ePMsgType = pMe->m_eMsgType;
                Msg_Param.pwszMsg = wstrText;
                Msg_Param.eBBarType = BTBAR_NONE;
                DrawPromptMessage(pMe->m_pDisplay, pStatic, &Msg_Param);
            }
            // 更新界面
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            
            // 启动发送关闭对话框事件的定时器
            (void) ISHELL_SetTimer(pMe->m_pShell,
                                PROMPTMSG_TIMER,
                                CMusicPlayer_HandleDialogTimer,
                                pMe);
            
            return TRUE;

        case EVT_DIALOG_END:         
            (void) ISHELL_CancelTimer(pMe->m_pShell, CMusicPlayer_HandleDialogTimer, pMe);
  
            // 此对话框返回值仅为 DLGRET_MSGBOX_OK ，为防止挂起 Applet 
            // 关闭对话框回到错误状态，显示给对话框返回值赋值
            pMe->m_eDlgRet = DLGRET_MSGBOX_OK;
            
            ISTATIC_Release(pStatic);
            pStatic = NULL;
            
            return TRUE;

           // 通过按键关闭对话框
        case EVT_KEY:
            if(pMe->m_eMsgType != MESSAGE_WAITING)
            {
              CLOSE_DIALOG(DLGRET_MSGBOX_OK);
            }
            return TRUE;
        
        case EVT_DISPLAYDIALOGTIMEOUT:
            CLOSE_DIALOG(DLGRET_MSGBOX_OK)
            return TRUE;

        default:
            break;
    }

    return FALSE;
}

/*==============================================================================
函数：
       CMusicPlayer_HandleYesNoDlgEvent
说明：
       IDD_YESNO 对话框事件处理函数
       
参数：
       pMe [in]：指向CMusicPlayer Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。
       
返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。
       
备注：
       
==============================================================================*/
static boolean  CMusicPlayer_HandleYesNoDlgEvent( CMusicPlayer  *pMe,
                                              AEEEvent   eCode,
                                              uint16     wParam,
                                              uint32     dwParam)
{
   static IStatic *pStatic = NULL;
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
   MSG_FATAL("CMusicPlayer_HandleYesNoDlgEvent Start",0,0,0);
   if (NULL == pStatic)
   {
        AEERect rect = {0};
        if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                                 AEECLSID_STATIC,
                                                 (void **)&pStatic))
        
        {
            return FALSE;
            ERR("ISHELL_CreateInstance,AEECLSID_STATIC 2",0,0,0);
        }        
        ISTATIC_SetRect(pStatic, &rect);  
   }

    if ((NULL == pStatic) ||(NULL == pMe))
    {
        return FALSE;
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
        {
            return TRUE;
        }
        
        case EVT_DIALOG_START:
            
            // For redraw the dialog
            (void)ISHELL_PostEvent( pMe->m_pShell,
                                    AEECLSID_APP_MUSICPLAYER,
                                    EVT_USER_REDRAW,
                                    0,
                                    0);
                                    
            // Note: Fall through to the EVT_USER_REDRAW
            //lint -fallthrough
            return TRUE;
        case EVT_USER_REDRAW:
            // Draw prompt bar here
            {
                PromptMsg_Param_type  Msg_Param={0};                                
                Msg_Param.ePMsgType = MESSAGE_CONFIRM;
                Msg_Param.pwszMsg = pMe->m_pMsgBox_Msg;
                Msg_Param.eBBarType = BTBAR_OK_CANCEL;
                DrawPromptMessage(pMe->m_pDisplay, pStatic, &Msg_Param);
            }
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            return TRUE;
            
        case EVT_DIALOG_END:
            (void) ISHELL_CancelTimer( pMe->m_pShell,  CMusicPlayer_HandleDialogTimer, pMe );
            ISTATIC_Release(pStatic);
            pStatic = NULL;
            return TRUE;
            
        case EVT_KEY:
            switch (wParam)
            {
                case AVK_INFO:
                case AVK_SELECT:
                    // 如下代码限制在此界面的快速按键
                    (void) ISHELL_SetTimer( pMe->m_pShell, TIMEOUT_MS_QUICK,
                                            CMusicPlayer_HandleDialogTimer, pMe );
                    return TRUE;
                    
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_NO);
                    return TRUE;
                    
                default:
                    break;
                    
            }
            break;
                        
        case EVT_DISPLAYDIALOGTIMEOUT:
            CLOSE_DIALOG(DLGRET_YES);
            return TRUE;
            
        default:
            break;
            
    }
    
    return FALSE;
} 
/*===========================================================================
  在指定位置画图
===========================================================================*/      
static void MP3_DrawImage(CMusicPlayer *pMe, uint16 ResID, int x, int y)
{
   //如果原来内存未释放，释放之
   if (pMe->m_pImage)
   {
      IIMAGE_Release(pMe->m_pImage);
      pMe->m_pImage = NULL;
   }
   //load图片
   pMe->m_pImage = ISHELL_LoadResImage(pMe->m_pShell, 
                                     MUSICPLAYERIMAGE_RES_FILE, 
                                     ResID);
   if(pMe->m_pImage == NULL)
   {
       return;
   }
   
 
   //画图  
   if(pMe->m_pImage)
   {
      IIMAGE_Draw(pMe->m_pImage,x,y);
   } 
    if (pMe->m_pImage)
   {
      IIMAGE_Release(pMe->m_pImage);
      pMe->m_pImage = NULL;
   }
} 
/*==================================================================================
使菜单项成为当前的菜单控件对象选择
===================================================================================*/
void Imenuctl_SetModeSel(CMusicPlayer *pMe)
{ 
   if(pMe==NULL)
   {
    return;
   }
   if(pMe)
   {
    //switch(pMe->m_MusicPlayerCfg.eMusicPlayMode)
    switch(pMe->m_nPlayMode)
    {
       case PLAYMODE_SINGLE:
          IMENUCTL_SetSel(pMe->m_pMode, IDS_PLAYMODE_SINGLE);
          break;

      case PLAYMODE_REPEAT_ONE:
          IMENUCTL_SetSel(pMe->m_pMode, IDS_PLAYMODE_REPEATONE);
          break;
          
      case PLAYMODE_ORDER:
         IMENUCTL_SetSel(pMe->m_pMode, IDS_PLAYMODE_ORDER);
         break;
      /*Random  Mode*/
      #ifdef FEATURE_MUSICPLAYER_RANDOM
      case PLAYMODE_RANDOM:
         IMENUCTL_SetSel(pMe->m_pMode, IDS_PLAYMODE_RANDOM);
         break;
      #endif
      /*End Random Mode*/   
      case PLAYMODE_CYCLE:
         IMENUCTL_SetSel(pMe->m_pMode, IDS_PLAYMODE_CYCLE);
         break;
  }
   }
}
/*==================================================================================
使菜单项成为当前的菜单控件对象选择
===================================================================================*/
void Imenuctl_SetVolumeSel(CMusicPlayer *pMe)
{
 if(pMe==NULL)
   {
    return;
   }
   if(pMe)
   {
    //switch(pMe->m_MusicPlayerCfg.eMusicVolume)
    switch(pMe->m_nVolume)
    {
        case VOLUME_ONE:
            IMENUCTL_SetSel(pMe->m_pVolume, IDS_VOLUME_ONE);
            break;
        case VOLUME_TWO:
            IMENUCTL_SetSel(pMe->m_pVolume,IDS_VOLUME_TWO);
            break;
        case VOLUME_THREE:
            IMENUCTL_SetSel(pMe->m_pVolume,IDS_VOLUME_THREE);
            break;
        case VOLUME_FOUR:
            IMENUCTL_SetSel(pMe->m_pVolume,IDS_VOLUME_FOUR);
            break;
        case VOLUME_FIVE:
            IMENUCTL_SetSel(pMe->m_pVolume,IDS_VOLUME_FIVE);
            break;
        case VOLUME_OFF:
            IMENUCTL_SetSel(pMe->m_pVolume,IDS_VOLUME_OFF);
            break;
          
    }
   }
}
/*==================================================================================
使菜单项成为当前的菜单控件对象选择
===================================================================================*/
void Imenuctl_SetSortSel(CMusicPlayer *pMe)
{
 if(pMe==NULL)
   {
    return;
   }
   if(pMe)
   {
    //switch(pMe->m_MusicPlayerCfg.eMusicSortBy)
    switch(pMe->m_nMusicSort)
    {
        case SORT_NONE:
            IMENUCTL_SetSel(pMe->m_pSort, IDS_SORT_NONE);
            break;
            
       // case SORT_DATE:
         //   IMENUCTL_SetSel(pMe->m_pSort, IDS_SORT_DATE);
          //  break;
        case SORT_NAME:
            IMENUCTL_SetSel(pMe->m_pSort,IDS_SORT_NAME);
            break;
        //case SORT_SIZE:
        //    IMENUCTL_SetSel(pMe->m_pSort,IDS_SORT_SIZE);
          //  break;
      
    }
   }
}
/*==================================================================================
设置LIST控件焦点
===================================================================================*/
static void CMusicPlayer_Set_CTL(CMusicPlayer *pMe)
{
    int currentRectIndex = 0;
    int title_height = 0;
    int bottomheight = GetBottomBarHeight(pMe->m_pDisplay);
    int itemheight = IDISPLAY_GetFontMetrics(pMe->m_pDisplay, AEE_FONT_NORMAL, NULL, NULL);	//AEE_FONT_BOLD
    int lineSpace  = ( pMe->m_rc.dy - title_height - bottomheight - itemheight*3) / 4;	//4/5
    int i = 0;

    AEERect             rects[3];

   	//要和MP3_Draw_SettingsText里的坐标对应好
    for( i = 0; i < 3; i ++)			
    {
     SETAEERECT( &rects[i],
                CONTROL_RECT_START_X + 9,
                title_height + lineSpace  * (i + 1)  + itemheight*i- 1,
                pMe->m_rc.dx - CONTROL_RECT_START_X - 19,
                itemheight + 2); //10);	//高度要比控件高1个像素
    }

    //设置控件焦点
    if (pMe->m_nCtlID == IDC_MUSICPLAY_MODE)
    {
        currentRectIndex = 0;
        IMENUCTL_SetActive(pMe->m_pMode, TRUE);
        IMENUCTL_SetActive(pMe->m_pVolume, FALSE);
        IMENUCTL_SetActive(pMe->m_pSort, FALSE);
    }
    else if(pMe->m_nCtlID==IDC_MUSIC_VOLUME)
    {
        currentRectIndex = 1;
        IMENUCTL_SetActive(pMe->m_pMode, FALSE);
        IMENUCTL_SetActive(pMe->m_pVolume, TRUE);
        IMENUCTL_SetActive(pMe->m_pSort, FALSE);
    }
    else
    {
        currentRectIndex = 2;
        IMENUCTL_SetActive(pMe->m_pMode, FALSE);
        IMENUCTL_SetActive(pMe->m_pVolume, FALSE);
        IMENUCTL_SetActive(pMe->m_pSort, TRUE);
    }
    for( i = 0; i < 3; i ++)
    {
        IDISPLAY_DrawRect( pMe->m_pDisplay, &rects[i], 0, 0, IDF_RECT_FRAME);
    }
    //IDISPLAY_DrawRect( pMe->m_pDisplay, &rects[currentRectIndex], MAKE_RGB(255, 0, 0), 0, IDF_RECT_FRAME);
}
/*画箭头*/
static void CMusicPlayer_Draw_Arrow(CMusicPlayer *pMe,int title_hight, int lineSpace,int itemheight)
{
    IImage      *pR_ResImg = NULL;
    IImage      *pL_ResImg = NULL;
    pL_ResImg  = ISHELL_LoadResImage(pMe->m_pShell,AEE_APPSCOMMONRES_IMAGESFILE,IDB_LEFTARROW);
    pR_ResImg = ISHELL_LoadResImage(pMe->m_pShell,AEE_APPSCOMMONRES_IMAGESFILE,IDB_RIGHTARROW);

    if(pR_ResImg != NULL)
    {
        //在状态和重复模式后面画该ICON,表示上下键改变值
        IIMAGE_Draw( pR_ResImg, pMe->m_rc.dx - 9, title_hight + lineSpace + 4);
        //IIMAGE_Draw( pR_ResImg, pMe->m_rc.dx - 9, title_hight + lineSpace * 2 + itemheight + 4);
        IIMAGE_Draw( pR_ResImg, pMe->m_rc.dx - 9, title_hight + lineSpace * 2 + itemheight + 1);	//4
        IIMAGE_Draw( pR_ResImg, pMe->m_rc.dx - 9, title_hight + lineSpace * 3 + itemheight * 2 + 1);
        IIMAGE_Release(pR_ResImg);
        pR_ResImg = NULL;
    }

    if(pL_ResImg != NULL)
    {
        //在状态和重复模式后面画该ICON,表示上下键改变值
        IIMAGE_Draw( pL_ResImg, CONTROL_RECT_START_X, title_hight + lineSpace + 4);
        IIMAGE_Draw( pL_ResImg, CONTROL_RECT_START_X, title_hight + lineSpace * 2 + itemheight + 1);
        IIMAGE_Draw( pL_ResImg, CONTROL_RECT_START_X, title_hight + lineSpace * 3 + itemheight * 2 + 1);
       // IIMAGE_Draw( pL_ResImg, CONTROL_RECT_START_X, title_hight + lineSpace * 4 + itemheight * 3 + 4);
        IIMAGE_Release(pL_ResImg);
        pL_ResImg = NULL;
    }

}
/*=================================================================================
获取选中的项
==================================================================================*/
static void CMusicPlayer_GetSettings(CMusicPlayer *pMe)
{
 if (pMe == NULL)
    {
      return;
    }
  switch(IMENUCTL_GetSel(pMe->m_pMode))
    {
       
        case IDS_PLAYMODE_SINGLE:   
            //pMe->m_MusicPlayerCfg.eMusicPlayMode= PLAYMODE_SINGLE;
            pMe->m_nPlayMode = PLAYMODE_SINGLE;
            break;

        case IDS_PLAYMODE_REPEATONE:
            //pMe->m_MusicPlayerCfg.eMusicPlayMode = PLAYMODE_REPEAT_ONE;
            pMe->m_nPlayMode = PLAYMODE_REPEAT_ONE;
            break;
            
        case IDS_PLAYMODE_ORDER:
            //pMe->m_MusicPlayerCfg.eMusicPlayMode = PLAYMODE_ORDER;
            pMe->m_nPlayMode = PLAYMODE_ORDER;
            break;
            
            #ifdef FEATURE_MUSICPLAYER_RANDOM
        case IDS_PLAYMODE_RANDOM:
            //pMe->m_MusicPlayerCfg.eMusicPlayMode = PLAYMODE_RANDOM;
            pMe->m_nPlayMode = PLAYMODE_RANDOM;
            break;
            #endif
            
        case IDS_PLAYMODE_CYCLE:
            //pMe->m_MusicPlayerCfg.eMusicPlayMode = PLAYMODE_CYCLE;
            pMe->m_nPlayMode = PLAYMODE_CYCLE;
            break;
        default:
           break;
   }
  switch(IMENUCTL_GetSel(pMe->m_pVolume))
 {
        case IDS_VOLUME_ONE:
            //pMe->m_MusicPlayerCfg.eMusicVolume = VOLUME_ONE;
            pMe->m_nVolume = VOLUME_ONE;
            break;
        case IDS_VOLUME_TWO:
            //pMe->m_MusicPlayerCfg.eMusicVolume = VOLUME_TWO;
            pMe->m_nVolume = VOLUME_TWO;
            break;
        case IDS_VOLUME_THREE:
            //pMe->m_MusicPlayerCfg.eMusicVolume = VOLUME_THREE;
            pMe->m_nVolume = VOLUME_THREE;
            break;
        case IDS_VOLUME_FOUR:
            //pMe->m_MusicPlayerCfg.eMusicVolume = VOLUME_FOUR;
            pMe->m_nVolume = VOLUME_FOUR;
            break;
        case IDS_VOLUME_FIVE:
            //pMe->m_MusicPlayerCfg.eMusicVolume = VOLUME_FIVE;
            pMe->m_nVolume = VOLUME_FIVE;
            break;
        case IDS_VOLUME_OFF:
            //pMe->m_MusicPlayerCfg.eMusicVolume = VOLUME_OFF;
            pMe->m_nVolume = VOLUME_OFF;
            break;
        default:
            break;

 }
  switch(IMENUCTL_GetSel(pMe->m_pSort))
 {
        case IDS_SORT_NONE:
            //pMe->m_MusicPlayerCfg.eMusicSortBy = SORT_NONE;
            pMe->m_nMusicSort = SORT_NONE;
            break;
            
       // case IDS_SORT_DATE:
        //    pMe->m_MusicPlayerCfg.eMusicSortBy = SORT_DATE;
         //   break;
        case IDS_SORT_NAME:
            //pMe->m_MusicPlayerCfg.eMusicSortBy = SORT_NAME;
            pMe->m_nMusicSort = SORT_NAME;
            break;
        //case IDS_SORT_SIZE:
        //    pMe->m_MusicPlayerCfg.eMusicSortBy = SORT_SIZE;
          //  break;
        default:
            break;
  }
}
/*=================================================================================
保存CFG配置项的值
==================================================================================*/
static void CMusicPlayer_SaveCfg(CMusicPlayer *pMe)
{
    if (pMe == NULL)
    {
      return;
    }
    CMusicPlayer_GetSettings(pMe);
    pMe->m_MusicPlayerCfg.eMusicPlayMode = pMe->m_nPlayMode;
    pMe->m_MusicPlayerCfg.eMusicVolume = pMe->m_nVolume;
    pMe->m_MusicPlayerCfg.eMusicSortBy = pMe->m_nMusicSort;
}
/*==============================================================================
函数:
    CMusicPlayer_ShowMsgBox

说明:
    函数由状态处理函数用来唤起消息对话框，显示消息提示用户。

参数:
    pMe [in]: 指向 CMusicPlayer Applet 对象结构的指针。该结构包含小程序的特定信息。
    wTextResId [in]: 消息文本资源 ID。

返回值:
    none

备注:

==============================================================================*/
int CMusicPlayer_ShowMsgBox(CMusicPlayer *pMe, uint16 wTextResId)
{
    pMe->m_nMsgResID = wTextResId;
    
    // 创建消息对话框.
    return CMusicPlayer_ShowDialog(pMe, IDD_MSGBOX);
}

/*==============================================================================
函数:
       CMusicPlayer_ShowYesNoDialog
说明:
       函数由状态处理函数用来创建相应对话框资源ID的对话框。

参数:
       pMe [In]: 指向CMusicPlayer Applet对象结构的指针,该结构包含小程序的特定信息.
       msgResId [in]：对话框字符串资源ID，函数根据该ID显示提示内容。
       msg : 指向需要显示的字符串，优先于msgResId
       msglen: 现实的字符串长度wstrlen而来.-1 Auto caculate the lenght

返回值:
       int : SUCCESS if create dialog

备注:
       

==============================================================================*/
int  CMusicPlayer_ShowYesNoDialog( CMusicPlayer *pMe, 
                               uint16      msgResId ,
                               AECHAR     *msg, 
                               int         msglen )
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
      FREEIF(pMe->m_pMsgBox_Msg);
    if( NULL == msg )
    {
        pMe->m_pMsgBox_Msg = (AECHAR *)MALLOC(sizeof(AECHAR)*MAX_LEN_MSGBOX);
        if( NULL == pMe->m_pMsgBox_Msg )
        {
            return EFAILED;
        }
        
        msglen = ISHELL_LoadResString ( pMe->m_pShell, 
                                        MUSICPLAYER_RES_FILE_LANG, 
                                        msgResId,
                                        pMe->m_pMsgBox_Msg,
                                        sizeof(AECHAR)*(MAX_LEN_MSGBOX -1));
        if( msglen <= 0 )
        {
            return EFAILED;
        }
    }
    else
    {
        if( msglen == -1 )
        {
            msglen = WSTRLEN(msg);
        }
        
        if( msglen <= 0 )
        {
            return EFAILED;
        }
        
        pMe->m_pMsgBox_Msg = (AECHAR *)MALLOC(sizeof(AECHAR)*((uint32)msglen+1));
        (void)MEMCPY(pMe->m_pMsgBox_Msg, msg, sizeof(AECHAR)*(uint32)msglen);
        pMe->m_pMsgBox_Msg[msglen] = (AECHAR)'\0';
    }
    
    return CMusicPlayer_ShowDialog( pMe, IDD_YESNO );
}
/*=============================================================================
FUNCTION: CMusicPlayer_HandleDialogTimer

DESCRIPTION: Generic timer used by several dialogs.  When this callback
             is invoked, it sends a EVT_USER_DIALOG_TIMEOUT event
             to the Idle applet (which will be routed to the active dialog)

PARAMETERS:
   *pUser: CMusicPlayer object pointer

RETURN VALUE:
    None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void CMusicPlayer_HandleDialogTimer(void *pUser)
{
    CMusicPlayer *pMe = (CMusicPlayer *)pUser;
    
    ASSERT(pMe->m_pActiveDlg != NULL);

    (void) ISHELL_PostEventEx(pMe->m_pShell,
                              EVTFLG_ASYNC,
                              AEECLSID_APP_MUSICPLAYER,
                              EVT_DISPLAYDIALOGTIMEOUT, 
                              0, 
                              0);

    
}

/*Build playlist*/
static int CMusicPlayer_BuildPlaylist(CMusicPlayer *pMe,IMenuCtl *pMenuCtl)
{
    AEEFileInfoEx  fi;
    FileName szFileName;
    //AEEFileInfoEx  fiex;
    uint32  fitime[PLAYLIST_MAX_NUM];//用来保存播放列表文件的时间信息
    int i,j;
    uint32  temp;
    char fileName[PLAYLISTNAME_MAX_LEN];
    CtlAddItem ai;
    
    ASSERT(pMe != NULL); 
    if(pMenuCtl == NULL||pMe->m_pFileMgr==NULL)
    {
     return EBADPARM;
    }
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    if (SUCCESS != IFILEMGR_Test(pMe->m_pFileMgr, MUSICPLAYLIST_DIR))
    {
     (void)IFILEMGR_MkDir(pMe->m_pFileMgr, MUSICPLAYLIST_DIR);
    }

    //枚举playlist文件夹    
    (void)IFILEMGR_EnumInit(pMe->m_pFileMgr, MUSICPLAYLIST_DIR, FALSE);
    MEMSET(&ai,0,sizeof(ai));
    if(pMe->m_pBuffer)
    {
     FREEIF(pMe->m_pBuffer);
    }
    pMe->m_pBuffer = (AECHAR*)MALLOC(PLAYLISTNAME_MAX_LEN * sizeof(AECHAR)); 

    if(NULL == pMe->m_pBuffer)
    {
     return ENOMEMORY;
    }

    pMe->m_nPlaylistNum=0;
    for(i=0;i<PLAYLIST_MAX_NUM;i++)
    {
        MEMSET(pMe->m_Playlist[i].pPlaylistName, 0,PLAYLISTNAME_MAX_LEN*sizeof(char));
    }
    // 填充fi结构体
   MEMSET(&fi, 0, sizeof(fi));
   fi.nStructSize = sizeof(fi);
   fi.pszFile = szFileName.m_szFileName;
   fi.nMaxFile = sizeof(szFileName.m_szFileName);   
    //scan playlist文件夹下每一个文件并添加IMENUCTL项  
   while (IFILEMGR_EnumNextEx(pMe->m_pFileMgr, &fi))
   {
        char *            psz = NULL;
        char *            pf = NULL;
        //由于IFILEMGR_EnumNext函数在处理时并没有给dwCreationDate赋值所以要重新获取一遍
        //IFILEMGR_GetInfoEx(pMe->m_pFileMgr, fi.pszFile, &fi);

        fitime[pMe->m_nPlaylistNum]= fi.dwCreationDate;
        psz = STRRCHR(fi.pszFile, '.');

        if (NULL == psz)
        {
          continue; 
        }
            
        pf = STRRCHR(fi.pszFile, '/');
        if (NULL == pf)
        {
          continue;
        }  
        pf++;

        if(psz > pf)
        {
         STRCPY(pMe->m_Playlist[pMe->m_nPlaylistNum].pPlaylistName, pf);
         pMe->m_Playlist[pMe->m_nPlaylistNum].pPlaylistName[psz-pf ] = '\0';
        } 

        pMe->m_nPlaylistNum++; 
        if(pMe->m_nPlaylistNum >= PLAYLIST_MAX_NUM)
        {
            break;
        }
   }
   // if(pMe->m_nPlaylistNum==0)
    //{
    //   IMENUCTL_AddItem(pMenuCtl, MUSICPLAYER_RES_FILE_LANG, IDS_PLAYLIST_NULL, IDS_PLAYLIST_NULL, NULL, 0);
       //IMENUCTL_SetActive(pMenuCtl, FALSE);
    //}
    //else //由于文件枚举的时候是根据文件名来排序枚举的所以在这里根据文件创建时间来排序
    if(pMe->m_nPlaylistNum > 0)
    {
     for(i=0;i<pMe->m_nPlaylistNum;i++)
     { 
        for(j=i+1;j<pMe->m_nPlaylistNum;j++)
        {
         if(fitime[i]>fitime[j])
         {
           temp=fitime[i];
           fitime[i]=fitime[j];
           fitime[j]=temp;
          (void)STRCPY(fileName,pMe->m_Playlist[i].pPlaylistName);
          (void)STRCPY(pMe->m_Playlist[i].pPlaylistName,pMe->m_Playlist[j].pPlaylistName);
          (void)STRCPY(pMe->m_Playlist[j].pPlaylistName,fileName);
         }
        }
     }
     for(i=0;i<pMe->m_nPlaylistNum;i++)
     {    
        ai.pszResImage =MUSICPLAYERIMAGE_RES_FILE;
       if(0==STRCMP(pMe->m_Playlist[i].pPlaylistName,pMe->m_MusicPlayerCfg.lastPlayPlaylist))
          //&& (pMe->m_bPlaying || pMe->m_bPaused))
       {
         ai.wImage = IDI_PLAYINGLIST;
       }
       else
       {
         ai.wImage = IDI_PLAYLIST;
       }
       if(STRCMP(DEFAULT_PLAYLIST,pMe->m_Playlist[i].pPlaylistName)== 0)
       {
          (void)ISHELL_LoadResString(pMe->m_pShell,
                                MUSICPLAYER_RES_FILE_LANG,                                
                                IDS_DEFAULTPLAYLISTNAME,
                                pMe->m_pBuffer,
                                PLAYLISTNAME_MAX_LEN * sizeof(AECHAR));
       }
       else
       {
           (void)UTF8TOWSTR((const byte *)pMe->m_Playlist[i].pPlaylistName,
                             STRLEN(pMe->m_Playlist[i].pPlaylistName),
                             pMe->m_pBuffer,
                             PLAYLISTNAME_MAX_LEN * sizeof(AECHAR));
       }
       ai.pText = pMe->m_pBuffer;
       ai.wItemID = i;
        if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
        {
         return EFAILED;
        }
       MEMSET(pMe->m_pBuffer,0,PLAYLISTNAME_MAX_LEN * sizeof(AECHAR));
    }
   }
    FREEIF(pMe->m_pBuffer);
    
       return SUCCESS;
}

/*Build viewlist*/
static int CMusicPlayer_BuildViewList(CMusicPlayer *pMe,IMenuCtl *pMenuCtl)
{   
    int i;
    CtlAddItem ai;
    char    *pf = NULL;

    ASSERT(pMe != NULL); 
    if(pMenuCtl == NULL)
    {
        return EBADPARM;
    }
    if(pMe->m_pBuffer)
    {
      FREEIF(pMe->m_pBuffer);
    }
     pMe->m_pBuffer = (AECHAR*)MALLOC(MP3NAME_MAX_LEN* sizeof(AECHAR)); 

    (void)IMENUCTL_DeleteAll(pMenuCtl);
    MEMSET(&ai,0,sizeof(ai));
    CMusicPlayer_MusiclistSortBy(pMe);
   // if(pMe->m_nPlaylistMusicNum==0)
   // {
     //   IMENUCTL_AddItem(pMenuCtl, MUSICPLAYER_RES_FILE_LANG, IDS_PLAYLIST_NULL, IDS_PLAYLIST_NULL, NULL, 0);
        //IMENUCTL_SetActive(pMenuCtl, FALSE);
   // }
  //  else
    if(pMe->m_nPlaylistMusicNum > 0)
    {
        for(i=0;i<pMe->m_nPlaylistMusicNum;i++)
        {
            pf = STRRCHR(pMe->m_Musiclist[i].pMusicName, '/');
            if(pf==NULL)
            {
              return EFAILED;
            }
            pf++;
             (void)UTF8TOWSTR((const byte *)pf,
                             STRLEN(pf),
                             pMe->m_pBuffer,
                             MP3NAME_MAX_LEN* sizeof(AECHAR));
             ai.pszResImage =MUSICPLAYERIMAGE_RES_FILE;
             ai.pText       = pMe->m_pBuffer;
             ai.wImage      = IDI_MUSICLOG;
             ai.wItemID     = i;
             if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
            {
             return EFAILED;
            }
            MEMSET(pMe->m_pBuffer,0,MP3NAME_MAX_LEN* sizeof(AECHAR));
        }
    }
    if(pMe->m_pBuffer)
    {
     FREEIF(pMe->m_pBuffer);
    }
   return SUCCESS;
}

/*===========================================================================
  初试化某首音乐
===========================================================================*/      
boolean CMusicPlayer_InitMusic(CMusicPlayer *pMe)
{   
#ifndef WIN32//wlh 临时修改
    char *            pf = NULL;
    boolean           bUse848;
   if(pMe==NULL)
    {
      return FALSE;
    }
   if(pMe->m_MusicPlayerCfg.lastPlayMusicID >= pMe->m_nPlayinglistMusicNum)
   {
      pMe->m_MusicPlayerCfg.lastPlayMusicID = 0;
   }
   MEMSET(pMe->m_pMp3FileToPlay,0,MP3NAME_MAX_LEN*sizeof(AECHAR));
   if(pMe->m_eStartMethod == STARTMETHOD_SIMPLEPLAYER)
   {
      pf = STRRCHR(pMe->m_pSimplePlayMusicName, '/');
   }
   else
   {
      pf = STRRCHR(pMe->m_PlayingMusiclist[pMe->m_MusicPlayerCfg.lastPlayMusicID].pMusicName, '/');
   }
   pf++;
   (void)UTF8TOWSTR((byte *)pf,
                     STRLEN(pf),
                     pMe->m_pMp3FileToPlay,
                     MP3NAME_MAX_LEN*sizeof(AECHAR));
#ifdef FEATURE_SUPPORT_BT_APP
    pf = STRRCHR(pMe->m_PlayingMusiclist[pMe->m_MusicPlayerCfg.lastPlayMusicID].pMusicName, '.');
    pf++;
    if(0 == STRICMP(pf,MP3_TYPE) || 0 == STRICMP(pf,AMR_TYPE) || 0 == STRICMP(pf,AAC_TYPE) || 0 == STRICMP(pf,WAV_TYPE))
    {
       bUse848 = TRUE;
    }
    else
    {
        bUse848 = FALSE;
    }
    if(!((pMe->m_bUse848 && bUse848) || (!pMe->m_bUse848 && !bUse848)))
    {
      pMe->m_bUse848 = bUse848;
      if(pMe->m_bUseBT)
      {
        DBGPRINTF("open sco");
        (void)bcmapp_ag_audio_open(pMe->m_nBTID, 
                                    (PFN_AG_NOTIFY)MP3_PlayMusicByBTCallBack,
                                    pMe, 
                                    pMe->m_MusicPlayerCfg.eMusicVolume,
                                    pMe->m_bUse848); 
      }
    }
#endif
   //填写m_md stucture
   pMe->m_md.clsData = MMD_FILE_NAME;
   if(pMe->m_eStartMethod == STARTMETHOD_SIMPLEPLAYER)
   {
     pMe->m_md.pData=(void *)pMe->m_pSimplePlayMusicName;
   }
   else
   {
     pMe->m_md.pData = (void *)(pMe->m_PlayingMusiclist[pMe->m_MusicPlayerCfg.lastPlayMusicID].pMusicName+1);
   }
   pMe->m_md.dwSize = 0; 
   
   //初始化m_pMedia指针
   if (SUCCESS != AEEMediaUtil_CreateMedia(pMe->m_pShell, &pMe->m_md, &pMe->m_pMedia))
   {
      return FALSE;
   }
  
   //m_pMedia不能为空
   if(NULL == pMe->m_pMedia)
   {
      return FALSE;
   }   
#ifdef FEATURE_SUPPORT_BT_APP
    if(!(pMe->m_bUse848) && pMe->m_bUseBT)
    {
        bcmapp_ag_set_device(pMe->m_nBTID);
    }
#endif
   // 如果是QCP格式的歌曲，必须设置一次声音通道否则声音会自动从RECIVE出来
   if(pMe->m_eStartMethod == STARTMETHOD_SIMPLEPLAYER)
   {
     pf = STRRCHR(pMe->m_pSimplePlayMusicName, '.');
   }
   else
   {
     pf = STRRCHR(pMe->m_PlayingMusiclist[pMe->m_MusicPlayerCfg.lastPlayMusicID].pMusicName, '.');
   }
    pf++;
    if(0 == STRICMP(pf,QCP_TYPE))
    {    	
      //IMEDIA_SetMediaParm( pMe->m_pMedia, MM_PARM_AUDIO_DEVICE, AEE_SOUND_DEVICE_SPEAKER, 0);	

	  	//Add By zzg 2010_07_29
	 	if (HS_HEADSET_ON())
	 	{
	 		IMEDIA_SetMediaParm( pMe->m_pMedia, MM_PARM_AUDIO_DEVICE, AEE_SOUND_DEVICE_STEREO_HEADSET, 0);	
	 	}
	 	else
	 	{
	 		IMEDIA_SetMediaParm( pMe->m_pMedia, MM_PARM_AUDIO_DEVICE, AEE_SOUND_DEVICE_SPEAKER, 0);	
	 	}
	 	//Add End
				
    }
    //注册底层回调
   (void)IMEDIA_RegisterNotify(pMe->m_pMedia, CMusicPlayer_MediaNotify, pMe);
   if(!pMe->m_bInterrupt)
   {
        if(pMe->m_eStartMethod != STARTMETHOD_SIMPLEPLAYER)
        {
            pMe->m_nCurrentTime = 0;
            pMe->m_nTotalTime = 0;
        }
   }
   pMe->m_nSimPlayCurTime = 0;
   pMe->m_bInterrupt = FALSE;
   pMe->m_nAutoScrollIdx = 0;
   (void)IMEDIA_GetTotalTime(pMe->m_pMedia);
   pMe->m_nCurrentVolume= pMe->m_MusicPlayerCfg.eMusicVolume * AEE_MAX_VOLUME/5;
    if(pMe->m_pMedia) 
    {
        (void)IMEDIA_SetVolume(pMe->m_pMedia,pMe->m_nCurrentVolume); 
    }
    if(pMe->m_eCurState == STATE_PLAYMUSIC_WINDOWS)
    {
      
       (void) ISHELL_PostEvent(pMe->m_pShell, 
                                    AEECLSID_APP_MUSICPLAYER,
                                    EVT_USER_REDRAW,
                                    0,
                                    0);       
    }
    // 背景播放时需要通知IDLE界面更改歌曲名
    if(GetMp3PlayerStatus() == MP3STATUS_RUNONBACKGROUND)
    {   
        (void)ISHELL_PostEvent( pMe->m_pShell,
                        AEECLSID_CORE_APP,
                        EVT_DRAWMUSICNAME,
                        0,
                        (uint32)pMe->m_pMp3FileToPlay);
    }
#endif
    return TRUE; 
}

/*===========================================================================
   接受CMX底层回调
===========================================================================*/                       
void CMusicPlayer_MediaNotify(void * pUser, AEEMediaCmdNotify * pCmdNotify)
{
    CMusicPlayer*   pMe = (CMusicPlayer*)pUser;
    switch (pCmdNotify->nStatus)
    {
        case MM_STATUS_PAUSE_FAIL:
        case MM_STATUS_RESUME_FAIL:
        default: 
            break;
    }
      
    if (pCmdNotify->nCmd == MM_CMD_PLAY&&pMe->m_eStartMethod !=STARTMETHOD_SIMPLEPLAYER) //播放音乐
    {
        switch (pCmdNotify->nStatus)
        {
            case MM_STATUS_ABORT:   // playback aborted
              {
                // 非播放器界面播放时不弹播放失败提示直接往下播
                if(GetMp3PlayerStatus() == MP3STATUS_RUNONFOREGROUND && pMe->m_eCurState==STATE_PLAYMUSIC_WINDOWS) 
                {
                    (void)IMEDIA_Stop(pMe->m_pMedia);
                    //pMe->m_bUserStopped = TRUE;
                    pMe->m_bPaused = FALSE;
                    pMe->m_bPlaying = FALSE;
                    pMe->m_nCurrentTime = 0;
                    IBACKLIGHT_Enable(pMe->m_pBackLight);
                    if(SUCCESS != IFILEMGR_Test(pMe->m_pFileMgr,pMe->m_PlayingMusiclist[pMe->m_MusicPlayerCfg.lastPlayMusicID].pMusicName+1))
                    {
                       (void)ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_MUSICPLAYER, EVT_POPMSGDIALOG, IDS_MSG_FILE_NOEXIT, 0);
                    }
                    else
                    {
                      (void)ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_MUSICPLAYER, EVT_POPMSGDIALOG, IDS_PLAYFAILED, 0);
                    }
                    break;
                }
             }
            case MM_STATUS_DONE:    // playback done
            {   
               if(TRUE == pMe->m_bUserStopped)
               {
                 pMe->m_bUserStopped = FALSE;
                 break;
               }
                 //如果是用户停止播放或者设置为播放单首时，不再继续播放下一首
                if(pMe->m_MusicPlayerCfg.eMusicPlayMode!=PLAYMODE_SINGLE && 
                    pMe->m_MusicPlayerCfg.eMusicPlayMode != PLAYMODE_REPEAT_ONE) 
                {   
                    CMusicPlayer_PlayNext(pMe, TRUE);//播放下一首
                }
                else// if(pMe->m_MusicPlayerCfg.eMusicPlayMode == PLAYMODE_SINGLE)
                {
                    if(pMe->m_pMedia)
                    {  
                        IMEDIA_Stop(pMe->m_pMedia);
                        pMe->m_nCurrentTime = 0;
                        if(pMe->m_MusicPlayerCfg.eMusicPlayMode == PLAYMODE_SINGLE)
                        {
                            pMe->m_bPaused = FALSE;
                            pMe->m_bPlaying = FALSE;
                        }
                        else if(pMe->m_MusicPlayerCfg.eMusicPlayMode == PLAYMODE_REPEAT_ONE)
                        {
                          (void)ISHELL_SetTimer(pMe->m_pShell,100,(PFNNOTIFY) CMusicPlayer_PlayMusic,pMe);
                        }
                    }
                    if(pMe->m_eCurState == STATE_PLAYMUSIC_WINDOWS)
                    {   
                        (void)ISHELL_PostEvent( pMe->m_pShell,
                                                AEECLSID_APP_MUSICPLAYER,
                                                EVT_USER_REDRAW,
                                                0,
                                                0); 
                    }
                    //如果是背景播放且设置为播放单首歌曲时，播放完需自动退出模块，并置m_PlayBackground = FALSE;
                    if(GetMp3PlayerStatus()== MP3STATUS_RUNONBACKGROUND 
                        && pMe->m_MusicPlayerCfg.eMusicPlayMode == PLAYMODE_SINGLE)
                    {
                        (void)ISHELL_PostEvent( pMe->m_pShell,
                                                AEECLSID_APP_MUSICPLAYER,
                                                EVT_CLOSEAPP,
                                                0,
                                                0); 
                    }
                }
                break;  
            }          
            //播放的时候每秒会发上来一次
            case MM_STATUS_TICK_UPDATE:
                //if(pMe->m_eStartMethod == STARTMETHOD_NORMAL)
                {
                    pMe->m_nCurrentTime++;//当前播放的时间加1
                    //pMe->m_nCurrentTime = ((uint32)pCmdNotify->pCmdData) / 1000;
                    if(pMe->m_eCurState == STATE_PLAYMUSIC_WINDOWS)
                    {
                       (void) ISHELL_PostEvent(pMe->m_pShell, 
                                        AEECLSID_APP_MUSICPLAYER,
                                        EVT_USER_REDRAW,
                                        0,
                                        0);       
                    }
                    break; 
                } 
            default:
                break;        
        }
    }
                             
    else if (pCmdNotify->nCmd == MM_CMD_GETTOTALTIME)
        //&&pMe->m_eStartMethod==STARTMETHOD_NORMAL) //return from IMEDIA_GetTotalTime
    {
        pMe->m_nTotalTime = ((uint32)pCmdNotify->pCmdData) / 1000;
    }
    else if (pCmdNotify->nCmd == MM_CMD_PLAY
        &&pMe->m_eStartMethod==STARTMETHOD_SIMPLEPLAYER)
    {
        switch (pCmdNotify->nStatus)
        {
            case MM_STATUS_ABORT:   // playback aborted
               pMe->m_bPaused = FALSE;
               pMe->m_bPlaying = FALSE;
               pMe->m_nCurrentTime = 0;
               pMe->m_bSimPlayFailed = TRUE;
               (void)ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_MUSICPLAYER, EVT_POPMSGDIALOG, IDS_PLAYFAILED, 0);
               break;
               
            case MM_STATUS_DONE:    // playback done
            //case MM_STATUS_ABORT:   // playback aborted
            {    
                //IMEDIA_Release(pMe->m_psimplePlayMedia);
                pMe->m_bPlaying = FALSE;
                pMe->m_bPaused = FALSE;
                pMe->m_nSimPlayCurTime = 0;
                (void)IMEDIA_Stop(pMe->m_pMedia);
                //CLOSE_DIALOG(DLGRET_CANCELED);
                (void)ISHELL_PostEvent(pMe->m_pShell,
                                       AEECLSID_APP_MUSICPLAYER,
                                       EVT_USER_REDRAW,
                                       0,
                                       0);
                break;
            }
            //播放的时候每秒会发上来一次
            case MM_STATUS_TICK_UPDATE:
                pMe->m_nSimPlayCurTime++;
                //pMe->m_nSimPlayCurTime = ((uint32)pCmdNotify->pCmdData) / 1000;
                DBGPRINTF("pMe->m_nSimPlayCurTime %d",pMe->m_nSimPlayCurTime);
                (void) ISHELL_PostEvent(pMe->m_pShell, 
                                        AEECLSID_APP_MUSICPLAYER,
                                        EVT_USER_REDRAW,
                                        0,
                                        0);       
                break; 
               
            default:
                break;
        }
    }
}

/*===========================================================================
  播放某首音乐
===========================================================================*/      
void CMusicPlayer_PlayMusic(CMusicPlayer *pMe)
{  
    if(pMe->m_bPaused)
    {
        (void)IMEDIA_SetVolume(pMe->m_pMedia,0);
    }
   if(pMe->m_pMedia)
   {
      (void)IMEDIA_Play(pMe->m_pMedia);//播放
   } 
}
/*===========================================================================
  恢复播放某首音乐
===========================================================================*/      
void CMusicPlayer_ResumeMusic(CMusicPlayer *pMe)
{  
   if(pMe->m_pMedia)
   {
     IMEDIA_Resume(pMe->m_pMedia);//播放
   } 
} 

void CMusicPlayer_SeekMusic(CMusicPlayer *pMe)
{
    if(pMe->m_pMedia)
    {
        int ret;
       /* if(0 == pMe->m_nPauseTime)
        {
          IMEDIA_Seek(pMe->m_pMedia ,MM_SEEK_CURRENT,pMe->m_nCurrentTime * 1000);
        }
        else
        {
          IMEDIA_Seek(pMe->m_pMedia ,MM_SEEK_CURRENT,pMe->m_nPauseTime);
        }*/
        ret=IMEDIA_Seek(pMe->m_pMedia ,MM_SEEK_CURRENT,pMe->m_nCurrentTime * 1000);
       if(ret != SUCCESS)
       {
         pMe->m_nCurrentTime = 0;
       }
    }
}

void CMusicPlayer_PauseMusic(CMusicPlayer *pMe)
{
    if(pMe->m_pMedia)
    {
       IMEDIA_Pause(pMe->m_pMedia);
       (void)IMEDIA_SetVolume(pMe->m_pMedia,pMe->m_nCurrentVolume); 
    }
}
/*===========================================================================
   播放下一个文件, 
   bDirection为TRUE, 下一个文件
   bDirection为FALSE, 前一个文件
===========================================================================*/  
void CMusicPlayer_PlayNext(CMusicPlayer *pMe, boolean bDirection)
{
    #ifdef FEATURE_MUSICPLAYER_RANDOM
    int16 rand_index = 0;
    #endif
   //文件数为0，什么都不做
   if(0 == pMe->m_nPlayinglistMusicNum)
   {
       return;    
   }
  
  if(TRUE == bDirection)//下一首
  {
      if(pMe->m_MusicPlayerCfg.lastPlayMusicID>=pMe->m_nPlayinglistMusicNum-1
        && pMe->m_MusicPlayerCfg.eMusicPlayMode != PLAYMODE_RANDOM)
      { 
        if(pMe->m_bUserPressNext==FALSE&&pMe->m_MusicPlayerCfg.eMusicPlayMode==PLAYMODE_ORDER)
        {
            if(pMe->m_pMedia)
            { 
               if(SUCCESS!=IMEDIA_Stop(pMe->m_pMedia))
               {
                 return;
               }
               //pMe->m_bUserStopped = TRUE;
               pMe->m_bPlaying = FALSE;
               pMe->m_bPaused = FALSE;
               pMe->m_nCurrentTime = 0;
               if(pMe->m_eCurState == STATE_PLAYMUSIC_WINDOWS)
               {
                 (void) ISHELL_PostEvent(pMe->m_pShell, 
                                        AEECLSID_APP_MUSICPLAYER,
                                        EVT_USER_REDRAW,
                                        0,
                                        0);       
               }
                //当在背景播放且播放模式为PLAYMODE_ORDER时，播放完应退出该模块并置m_PlayBackground = FALSE
                if(GetMp3PlayerStatus()== MP3STATUS_RUNONBACKGROUND)
                {
                    (void)ISHELL_PostEvent( pMe->m_pShell,
                                            AEECLSID_APP_MUSICPLAYER,
                                            EVT_CLOSEAPP,
                                            0,
                                            0); 
                }
               return;
            }
        }
        else
        {   
            pMe->m_MusicPlayerCfg.lastPlayMusicID= 0;
        }
        
     }
      else
      {
      #ifdef FEATURE_MUSICPLAYER_RANDOM
        if(pMe->m_MusicPlayerCfg.eMusicPlayMode == PLAYMODE_RANDOM)
        {
            rand_index = (pMe->m_MusicPlayerCfg.lastPlayMusicID + (rand()%pMe->m_nPlayinglistMusicNum)+ 1)%pMe->m_nPlayinglistMusicNum;
            rand_index = (rand_index + pMe->m_nPlayinglistMusicNum)% pMe->m_nPlayinglistMusicNum;
            //pMe->m_MusicPlayerCfg.lastPlayMusicID = rand_index;
            pMe->m_nRandNum ++;
            if(pMe->m_bUserPressNext && pMe->m_nRandNum > pMe->m_nPlayinglistMusicNum)
            {
               MP3_ResetRandonIdentifier(pMe);
               pMe->m_nRandNum ++;
            }
            if(pMe->m_nRandNum > pMe->m_nPlayinglistMusicNum)
            {
                if(pMe->m_pMedia)
                { 
                   if(SUCCESS!=IMEDIA_Stop(pMe->m_pMedia))
                   {
                     return;
                   }
                   //pMe->m_bUserStopped = TRUE;
                   pMe->m_bPlaying = FALSE;
                   pMe->m_bPaused = FALSE;
                   pMe->m_nCurrentTime = 0;
                   if(pMe->m_eCurState == STATE_PLAYMUSIC_WINDOWS)
                   {
                     (void) ISHELL_PostEvent(pMe->m_pShell, 
                                            AEECLSID_APP_MUSICPLAYER,
                                            EVT_USER_REDRAW,
                                            0,
                                            0);       
                   }
                    //当在背景播放且播放模式为PLAYMODE_ORDER时，播放完应退出该模块并置m_PlayBackground = FALSE
                    if(GetMp3PlayerStatus()== MP3STATUS_RUNONBACKGROUND)
                    {
                        (void)ISHELL_PostEvent( pMe->m_pShell,
                                                AEECLSID_APP_MUSICPLAYER,
                                                EVT_CLOSEAPP,
                                                0,
                                                0); 
                    }
                }
                return;
            }
            pMe->m_MusicPlayerCfg.lastPlayMusicID = rand_index;
           // 避免重复播放 所以在这里对标识位进行检查
            while(pMe->m_aIdentifier[pMe->m_MusicPlayerCfg.lastPlayMusicID])
            {
                pMe->m_MusicPlayerCfg.lastPlayMusicID ++;
                if(pMe->m_MusicPlayerCfg.lastPlayMusicID > pMe->m_nPlayinglistMusicNum-1)
                {
                    pMe->m_MusicPlayerCfg.lastPlayMusicID = 0;
                }
            }
            pMe->m_aIdentifier[pMe->m_MusicPlayerCfg.lastPlayMusicID] = 1;
        }
        else
      #endif
        {
           pMe->m_MusicPlayerCfg.lastPlayMusicID++;
        }
        /*random mode */ 
      }
 }
  else//上一首
  {
      if(0 == pMe->m_MusicPlayerCfg.lastPlayMusicID)
      {
          pMe->m_MusicPlayerCfg.lastPlayMusicID= pMe->m_nPlayinglistMusicNum- 1;
      }
      else
      {
          #ifdef FEATURE_MUSICPLAYER_RANDOM
          if(pMe->m_MusicPlayerCfg.eMusicPlayMode == PLAYMODE_RANDOM)
          {
            rand_index = (pMe->m_MusicPlayerCfg.lastPlayMusicID + (rand()%pMe->m_nPlayinglistMusicNum)+ 1)%pMe->m_nPlayinglistMusicNum;
            rand_index = (rand_index + pMe->m_nPlayinglistMusicNum)% pMe->m_nPlayinglistMusicNum;
            pMe->m_MusicPlayerCfg.lastPlayMusicID = rand_index;            
          }
          else
          #endif
          {
            pMe->m_MusicPlayerCfg.lastPlayMusicID--;
          } 

      } 
  }
    CMusicPlayer_ReleaseMedia(pMe);//释放本m_pMedia指针
    //因为底层使用的是事件传递机制，需要给底层stop并释放音乐的时间，否则会出错，此处等0.5秒钟后初始化下一首
    (void) ISHELL_SetTimer(pMe->m_pShell,500,(PFNNOTIFY)MP3_InitMusicCB,pMe);
    if(pMe->m_bPlaying)
    {
        //因为底层使用的是事件传递机制，需要给底层初始化并传递notify的时间，否则会出错，此处等0.5秒钟后播放下一首      
        (void) ISHELL_SetTimer(pMe->m_pShell,1000,(PFNNOTIFY) CMusicPlayer_PlayMusic,pMe);
    }
    else
    {
        pMe->m_bPaused = FALSE;
        pMe->m_bPlaying = FALSE;
       // if(pMe->m_eCurState == STATE_PLAYMUSIC_WINDOWS)
       // {
       //    MP3_DRAW_BOTTOMBAR(BTBAR_OPTION_PLAY_BACK);
       //     IDISPLAY_UpdateEx(pMe->m_pDisplay, TRUE);
        //}
    }
    //非用户手动停止
    pMe->m_bUserStopped= FALSE;
}    

/*==============================================================================
函数：CMusicPlayer_ReleaseMedia

说明：
       释放Media指针

参数：
       pMe [in]：指向CMusicPlayer Applet对象结构的指针。该结构包含小程序的特定信息。
       
返回值：
        无

备注：:

==============================================================================*/
void CMusicPlayer_ReleaseMedia(CMusicPlayer *pMe)
{
   if(pMe->m_pMedia)
   {
      (void)IMEDIA_RegisterNotify(pMe->m_pMedia, NULL, pMe);
      //(void)IMEDIA_Stop(pMe->m_pMedia);//因为IMEDIA_Release会stop
      IMEDIA_Release(pMe->m_pMedia);
      pMe->m_pMedia = NULL;
   }
}  
/*================================================================================
函数:CMusicPlayer_MusiclistSortBy
说明:根据用户设置的排序方法(名称、时间、大小)对当前激活的播放列表进行排序
参数:pMe [in]：指向CMusicPlayer Applet对象结构的指针。该结构包含小程序的特定信息。
返回值:无
备注:

=================================================================================*/
 void CMusicPlayer_MusiclistSortBy(CMusicPlayer * pMe)
{
    //FileInfo          fi;
    //uint32  fitime[MUSIC_MAX_FILESNUM];//用来保存音乐文件的时间信息
    //uint32  fisize[MUSIC_MAX_FILESNUM];//用来保持音乐文件的大小信息
    int i,j;
    //uint32  temp;
    char fileName[MP3NAME_MAX_LEN];
    ASSERT(pMe != NULL);
    pMe->m_nPlaylistMusicNum=0;
    for(i=0;i<MUSIC_MAX_FILESNUM;i++)
    {
        if(pMe->m_Musiclist[i].pMusicName[0]!='\0')
        {  
         //   IFILEMGR_GetInfo(pMe->m_pFileMgr,pMe->m_Musiclist[i].pMusicName +1, &fi);
         //   fitime[pMe->m_nPlaylistMusicNum]=fi.dwCreationDate;
          //  fisize[pMe->m_nPlaylistMusicNum]=fi.dwSize;
            pMe->m_nPlaylistMusicNum++;
        }
    }
    switch(pMe->m_MusicPlayerCfg.eMusicSortBy)
    {
        /*case SORT_SIZE:
             for(i=0;i<pMe->m_nPlaylistMusicNum;i++)
             { 
                
                for(j=i+1;j<pMe->m_nPlaylistMusicNum;j++)
                {
                 if(fisize[i]>fisize[j])
                 {
                   temp=fisize[i];
                   fisize[i]=fisize[j];
                   fisize[j]=temp;
                  (void)STRCPY(fileName,pMe->m_Musiclist[i].pMusicName);
                  (void)STRCPY(pMe->m_Musiclist[i].pMusicName,pMe->m_Musiclist[j].pMusicName);
                  (void)STRCPY(pMe->m_Musiclist[j].pMusicName,fileName);
                 }
                }
             }
            break;*/
        case SORT_NONE:
            for(i=0;i<pMe->m_nPlaylistMusicNum;i++)
             { 
                
                for(j=i+1;j<pMe->m_nPlaylistMusicNum;j++)
                {
                 if(pMe->m_Musiclist[i].pMusicName[0]>pMe->m_Musiclist[j].pMusicName[0])
                 {
                  (void)STRCPY(fileName,pMe->m_Musiclist[i].pMusicName);
                  (void)STRCPY(pMe->m_Musiclist[i].pMusicName,pMe->m_Musiclist[j].pMusicName);
                  (void)STRCPY(pMe->m_Musiclist[j].pMusicName,fileName);
                 }
                }
             }
            break;
            
        /*case SORT_DATE:
            for(i=0;i<pMe->m_nPlaylistMusicNum;i++)
            { 
                
                for(j=i+1;j<pMe->m_nPlaylistMusicNum;j++)
                {
                 if(fitime[i]>fitime[j])
                 {
                   temp=fitime[i];
                   fitime[i]=fitime[j];
                   fitime[j]=temp;
                  (void)STRCPY(fileName,pMe->m_Musiclist[i].pMusicName);
                  (void)STRCPY(pMe->m_Musiclist[i].pMusicName,pMe->m_Musiclist[j].pMusicName);
                  (void)STRCPY(pMe->m_Musiclist[j].pMusicName,fileName);
                 }
                }
             }
            break;
              */
        case SORT_NAME:
          {
            AECHAR musicName1[MP3NAME_MAX_LEN];
                AECHAR musicName2[MP3NAME_MAX_LEN];
                char *pf = NULL;
                
                for(i=0;i<pMe->m_nPlaylistMusicNum;i++)
                {
                    for(j=i+1;j<pMe->m_nPlaylistMusicNum;j++)
                    {
                        pf = STRRCHR(pMe->m_Musiclist[i].pMusicName, '/');
                        if(pf==NULL)
                        {
                          return;
                        }
                        pf++;
                        (void)UTF8TOWSTR((const byte *)pf,
                                         STRLEN(pf),
                                         musicName1,
                                         MP3NAME_MAX_LEN* sizeof(AECHAR));
                        pf = STRRCHR(pMe->m_Musiclist[j].pMusicName, '/');
                        if(pf==NULL)
                        {
                          return;
                        }
                        pf++;
                        (void)UTF8TOWSTR((const byte *)pf,
                                         STRLEN(pf),
                                         musicName2,
                                         MP3NAME_MAX_LEN* sizeof(AECHAR));
                        if(Appscommon_CompareName(musicName1, musicName2)>0)
                         {
                          (void)STRCPY(fileName,pMe->m_Musiclist[i].pMusicName);
                          (void)STRCPY(pMe->m_Musiclist[i].pMusicName,pMe->m_Musiclist[j].pMusicName);
                          (void)STRCPY(pMe->m_Musiclist[j].pMusicName,fileName);
                         } 
                    }
                }
           
            break;
        }
    }
}

/*================================================================================
函数:CMusicPlayer_PlayingMusiclistSortBy
说明:根据用户设置的排序方法(名称、时间、大小)对当前激活的播放列表进行排序
参数:pMe [in]：指向CMusicPlayer Applet对象结构的指针。该结构包含小程序的特定信息。
返回值:无
备注:

=================================================================================*/
void CMusicPlayer_PlayingMusiclistSortBy(CMusicPlayer * pMe)

{
   // FileInfo          fi;
    //uint32  fitime[MUSIC_MAX_FILESNUM];//用来保存音乐文件的时间信息
    //uint32  fisize[MUSIC_MAX_FILESNUM];//用来保持音乐文件的大小信息
    int i,j;
    //uint32  temp;
    char fileName[MP3NAME_MAX_LEN];
    ASSERT(pMe != NULL);
    pMe->m_nPlayinglistMusicNum=0;
    for(i=0;i<MUSIC_MAX_FILESNUM;i++)
    {
        if(pMe->m_PlayingMusiclist[i].pMusicName[0]!='\0')
        {  
            //IFILEMGR_GetInfo(pMe->m_pFileMgr,pMe->m_PlayingMusiclist[i].pMusicName +1, &fi);
           // fitime[pMe->m_nPlayinglistMusicNum]=fi.dwCreationDate;
            //fisize[pMe->m_nPlayinglistMusicNum]=fi.dwSize;
            pMe->m_nPlayinglistMusicNum++;
        }
    }
    switch(pMe->m_MusicPlayerCfg.eMusicSortBy)
    {
       /* case SORT_SIZE:
             for(i=0;i<pMe->m_nPlayinglistMusicNum;i++)
             { 
                
                for(j=i+1;j<pMe->m_nPlayinglistMusicNum;j++)
                {
                 if(fisize[i]>fisize[j])
                 {
                   temp=fisize[i];
                   fisize[i]=fisize[j];
                   fisize[j]=temp;
                  (void)STRCPY(fileName,pMe->m_PlayingMusiclist[i].pMusicName);
                  (void)STRCPY(pMe->m_PlayingMusiclist[i].pMusicName,
                               pMe->m_PlayingMusiclist[j].pMusicName);
                  (void)STRCPY(pMe->m_PlayingMusiclist[j].pMusicName,fileName);
                 }
                }
             }
            break;*/

        case SORT_NONE:
            for(i=0;i<pMe->m_nPlayinglistMusicNum;i++)
             { 
                for(j=i+1;j<pMe->m_nPlayinglistMusicNum;j++)
                {
                 if(pMe->m_PlayingMusiclist[i].pMusicName[0]>pMe->m_PlayingMusiclist[j].pMusicName[0])
                 {
                  (void)STRCPY(fileName,pMe->m_PlayingMusiclist[i].pMusicName);
                  (void)STRCPY(pMe->m_PlayingMusiclist[i].pMusicName,pMe->m_PlayingMusiclist[j].pMusicName);
                  (void)STRCPY(pMe->m_PlayingMusiclist[j].pMusicName,fileName);
                 }
                }
             }
            break;
            
       /* case SORT_DATE:
            for(i=0;i<pMe->m_nPlayinglistMusicNum;i++)
            { 
                
                for(j=i+1;j<pMe->m_nPlayinglistMusicNum;j++)
                {
                 if(fitime[i]>fitime[j])
                 {
                   temp=fitime[i];
                   fitime[i]=fitime[j];
                   fitime[j]=temp;
                  (void)STRCPY(fileName,pMe->m_PlayingMusiclist[i].pMusicName);
                  (void)STRCPY(pMe->m_PlayingMusiclist[i].pMusicName,
                               pMe->m_PlayingMusiclist[j].pMusicName);
                  (void)STRCPY(pMe->m_PlayingMusiclist[j].pMusicName,fileName);
                 }
                }
             }
            break;*/

        case SORT_NAME:
            {
                AECHAR musicName1[MP3NAME_MAX_LEN];
                AECHAR musicName2[MP3NAME_MAX_LEN];
                char *pf = NULL;
                
                for(i=0;i<pMe->m_nPlayinglistMusicNum;i++)
                {
                    for(j=i+1;j<pMe->m_nPlayinglistMusicNum;j++)
                    {
                        pf = STRRCHR(pMe->m_PlayingMusiclist[i].pMusicName, '/');
                        if(pf==NULL)
                        {
                          return;
                        }
                        pf++;
                        (void)UTF8TOWSTR((const byte *)pf,
                                         STRLEN(pf),
                                         musicName1,
                                         MP3NAME_MAX_LEN* sizeof(AECHAR));
                        pf = STRRCHR(pMe->m_PlayingMusiclist[j].pMusicName, '/');
                        if(pf==NULL)
                        {
                          return;
                        }
                        pf++;
                        (void)UTF8TOWSTR((const byte *)pf,
                                         STRLEN(pf),
                                         musicName2,
                                         MP3NAME_MAX_LEN* sizeof(AECHAR));
                        if(Appscommon_CompareName(musicName1, musicName2 )>0)
                         {
                          (void)STRCPY(fileName,pMe->m_PlayingMusiclist[i].pMusicName);
                          (void)STRCPY(pMe->m_PlayingMusiclist[i].pMusicName,pMe->m_PlayingMusiclist[j].pMusicName);
                          (void)STRCPY(pMe->m_PlayingMusiclist[j].pMusicName,fileName);
                         } 
                    }
                }
           
            break;
        }
    }
}

/*刷新音量条*/
static void MP3_RefreshVolBar(CMusicPlayer *pMe)
{
    uint16 ResID;
    if(pMe->m_eStartMethod != STARTMETHOD_SIMPLEPLAYER)
    {
        switch ((int)pMe->m_nCurrentVolume)
        {
            case VOLUME_OFF*AEE_MAX_VOLUME/5:
                ResID = IDI_VOLUME_OFF;
                break;
            case VOLUME_ONE*AEE_MAX_VOLUME/5:
                ResID = IDI_VOLUME_ONE;
                break;
            case VOLUME_TWO*AEE_MAX_VOLUME/5:
                ResID = IDI_VOLUME_TWO;
                break;
            case VOLUME_THREE*AEE_MAX_VOLUME/5:
                ResID = IDI_VOLUME_THREE;
                break;
            case VOLUME_FOUR*AEE_MAX_VOLUME/5:
                ResID = IDI_VOLUME_FOUR;
                break;
            case VOLUME_FIVE*AEE_MAX_VOLUME/5:
                ResID = IDI_VOLUME_FIVE;
                break;
            default :
                ResID = IDI_VOLUME_THREE;            
        }
        //MP3_DrawImage( pMe, ResID, 131, 168);//wlh 20090415 mod
    }
    else
    {
        switch ((int)pMe->m_nCurrentVolume)
        {
            case VOLUME_OFF*AEE_MAX_VOLUME/5:
                ResID = IDI_SIMPLEVOL_OFF;
                break;
            case VOLUME_ONE*AEE_MAX_VOLUME/5:
                ResID = IDI_SIMPLEVOL_ONE;
                break;
            case VOLUME_TWO*AEE_MAX_VOLUME/5:
                ResID = IDI_SIMPLEVOL_TWO;
                break;
            case VOLUME_THREE*AEE_MAX_VOLUME/5:
                ResID = IDI_SIMPLEVOL_THREE;
                break;
            case VOLUME_FOUR*AEE_MAX_VOLUME/5:
                ResID = IDI_SIMPLEVOL_FOUR;
                break;
            case VOLUME_FIVE*AEE_MAX_VOLUME/5:
                ResID = IDI_SIMPLEVOL_FIVE;
                break;
            default :
                ResID = IDI_SIMPLEVOL_THREE;            
        }
    }
	MP3_DrawImage( pMe, ResID, VOLUME_X, VOLUME_Y);

}    
static void MP3_DrawImageWithOffset( CMusicPlayer *pMe)//wlh 20090415 mod 为了区别播放区域，加音量，减音量，加了个参数
{
	AEERect clip;
	if(pMe->m_rtype == TYPE_PLAYER)
		SETAEERECT( &clip, OLDPLAY_X, OLDPLAY_Y,OLDPLAY_W,OLDPLAY_H);
	if(pMe->m_rtype == TYPE_ADDVOLUME)
		SETAEERECT( &clip, ADDVOLUMEPRESS_X, ADDVOLUMEPRESS_Y,ADDVOLUMEPRESS_W,ADDVOLUMEPRESS_H);
	if(pMe->m_rtype == TYPE_DECVOLUME)
		SETAEERECT( &clip, DECREASEVOLUMEPRESS_X, DECREASEVOLUMEPRESS_Y,DECREASEVOLUMEPRESS_W,DECREASEVOLUMEPRESS_H);
	if(pMe->m_rtype == TYPE_PREVIOUS)
		SETAEERECT( &clip, PREVIOUSPRESS_X, PREVIOUSPRESS_Y,PREVIOUSPRESS_W,PREVIOUSPRESS_H);
	if(pMe->m_rtype == TYPE_NEXT)
		SETAEERECT( &clip, NEXTPRESS_X, NEXTPRESS_Y,NEXTPRESS_W,NEXTPRESS_H);

    MP3_drawClipRectWithOffset(pMe,IDI_MUSICPLAYER,&clip);
    if(pMe->m_bPlaying)
    {
        MP3_DrawImage(pMe, IDI_PAUSE, PLAY_X, PLAY_Y);
    }
    IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
}

/*===========================================================================
   刷新播放时间
===========================================================================*/   
static void MP3_RefreshPlayingTick(CMusicPlayer *pMe)
{
	char    tick_time[MAX_STR_LEN];
    AECHAR  Wtick_time[MAX_STR_LEN];
    AEERect clip;
	int x,y;//wlh 20090420 add

	if(pMe->m_eStartMethod != STARTMETHOD_SIMPLEPLAYER)//wlh 20090420 add
    {//正常播放
		x = PLAYINGTICK_X;
		y = PLAYINGTICK_Y;
	}
	else
	{//简单播放
		x = SIMPLAYINGTICK_X;
		y = SIMPLAYINGTICK_Y;
	}
   
    //容错
    if(pMe->m_nTotalTime > 0 && pMe->m_nCurrentTime > pMe->m_nTotalTime)
    {
      pMe->m_nCurrentTime = pMe->m_nTotalTime;
    }
	MEMSET(tick_time,0,MAX_STR_LEN);
	if(pMe->m_eStartMethod != STARTMETHOD_SIMPLEPLAYER)
	{
        //SETAEERECT( &clip, 6,165,50,18);//wlh 20090415 mod
		SETAEERECT( &clip, x,y,50,18);
        MP3_drawClipRectWithOffset(pMe,IDI_MUSICPLAYER,&clip);
        //SPRINTF(tick_time, "%02d:%02d", (pMe->m_nTotalTime - pMe->m_nCurrentTime)/60, (pMe->m_nTotalTime - pMe->m_nCurrentTime)% 60);
        SPRINTF(tick_time,"%02d:%02d",pMe->m_nCurrentTime/60,pMe->m_nCurrentTime%60);
	}
	else
	{
        //SETAEERECT( &clip, 6,165,50,18);//wlh 20090415 mod
		SETAEERECT( &clip, x,y,50,18);
        MP3_drawClipRectWithOffset(pMe,IDI_SIMPLEPLAYER,&clip);
        //SPRINTF(tick_time, "%02d:%02d", (pMe->m_nTotalTime - pMe->m_nSimPlayCurTime )/60, (pMe->m_nTotalTime - pMe->m_nSimPlayCurTime)% 60);
        SPRINTF(tick_time,"%02d:%02d",pMe->m_nSimPlayCurTime /60,pMe->m_nSimPlayCurTime %60);
	}
	STRTOWSTR(tick_time, Wtick_time, sizeof(Wtick_time));
	DrawTextWithProfile(pMe->m_pShell, 
                           pMe->m_pDisplay, 
                           RGB_WHITE_NO_TRANS, 
                           AEE_FONT_BOLD,
                           Wtick_time, 
                           -1, 
							  x,
							  y,
                           &clip, 
                           IDF_ALIGN_CENTER|IDF_ALIGN_MIDDLE|IDF_TEXT_TRANSPARENT);
   
}
/*画正在播放的文件名*/
static void MP3_DrawMusicName(CMusicPlayer *pMe ,int index)
{
    //AEERect clip;
    RGBVAL oldColor;
    //SETAEERECT( &clip, 12, 8,149,22);
    if(pMe->m_eStartMethod != STARTMETHOD_SIMPLEPLAYER)
    {
      oldColor = IDISPLAY_SetColor(pMe->m_pDisplay,CLR_USER_TEXT,MP3NAME_COLOR);//MAKE_RGB(66,156,255));
      MP3_drawClipRectWithOffset(pMe, IDI_MUSICPLAYER,&pMe->m_pMP3FaceRect[1]);
    }
    else
    {
        oldColor = IDISPLAY_SetColor(pMe->m_pDisplay,CLR_USER_TEXT,RGB_WHITE);
        MP3_drawClipRectWithOffset(pMe, IDI_SIMPLEPLAYER,&pMe->m_pMP3FaceRect[1]);
    }
    IDISPLAY_DrawText(pMe->m_pDisplay, 
                       AEE_FONT_BOLD,
                       pMe->m_pMp3FileToPlay+index, 
                       -1, 
                       pMe->m_pMP3FaceRect[1].x +3, 
                       pMe->m_pMP3FaceRect[1].y +3, 
                       &pMe->m_pMP3FaceRect[1], 
                       IDF_ALIGN_CENTER|IDF_ALIGN_MIDDLE|IDF_TEXT_TRANSPARENT);
    //if(pMe->m_eStartMethod == STARTMETHOD_SIMPLEPLAYER)
    //{
       (void)IDISPLAY_SetColor(pMe->m_pDisplay,CLR_USER_TEXT,oldColor);
    //}
}

/*画MP3播放器主窗口*/
static void MP3_DrawPlayerWindows(CMusicPlayer *pMe)
{
    if(pMe==NULL)
    {
        return;
    }
	//IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);//wlh test
    MP3_DrawImage(pMe, IDI_MUSICPLAYER, 0, 0);//画背景
	//IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);//wlh test
    MP3_RefreshscheduleBar(pMe);//画进度点
	//IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);//wlh test
    MP3_RefreshVolBar(pMe);//画音量
    //IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);//wlh test
    
    #ifdef FEATURE_MUSICPLAYER_LIST_INDEX_DISPLAY
	
    MP3_DispListIndex(pMe);//画 当前第几首/一共几首
    #endif
    //IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);//wlh test
    MP3_RefreshPlayingTick(pMe);//画当前歌曲执行时间
	//IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);//wlh test
    MP3_ResetScroll(pMe);
	//IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);//wlh test
     if(pMe->m_bPlaying)
    {
        MP3_DrawImage(pMe, IDI_PAUSE, PLAY_X, PLAY_Y);
    }
	// IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);//wlh test
}

/*画简单播放器界面*/
static void MP3_DrawSimplePlayerFace(CMusicPlayer *pMe)
{
    if(pMe == NULL)
    {
        return;
    }
    MP3_DrawImage(pMe, IDI_SIMPLEPLAYER, 0, 0);
    MP3_ResetScroll(pMe);//ok
    MP3_RefreshVolBar(pMe);//ok
    MP3_RefreshPlayingTick(pMe);//ok
    MP3_RefreshSimpleSchBar(pMe);
}
#ifdef FEATURE_MUSICPLAYER_LIST_INDEX_DISPLAY
/*---------------------------------------------------------------------
Function Name :MP3_DispListIndex
Version :1.0.0
Change Time :
---------------------------------------------------------------------*/
static void MP3_DispListIndex(CMusicPlayer *pMe)
{
    AEEMenuColors color;
    char liststr[] = "0/0";//special mode
    char list_n_str[MAX_STR_LEN];//normal mode 
    AECHAR  wliststr[MAX_STR_LEN];
    AEERect clip;

    /*Set Rect text color and bg color*/
    
    /*Draw list index Rect */
    //SETAEERECT( &clip, 126,142,48,18);//wlh 20090415 mod
	//SETAEERECT( &clip, LISTINDEX_X,LISTINDEX_Y,48,18);
    
    MP3_drawClipRectWithOffset(pMe,IDI_MUSICPLAYER,&clip);
    if((pMe->m_nPlayinglistMusicNum == 0)
        ||(pMe->m_MusicPlayerCfg.lastPlayMusicID >= pMe->m_nPlayinglistMusicNum))
    {  
        STRTOWSTR(liststr,wliststr,sizeof(wliststr));   
    }
    else
    {
        int minutetime =     pMe->m_nTotalTime/60;
		int secondtime =     pMe->m_nTotalTime%60;
		AECHAR  wtotaltimestr[MAX_STR_LEN];
		MEMSET(wtotaltimestr,0,sizeof(wtotaltimestr));
        MEMSET(list_n_str,0,sizeof(list_n_str));
        SNPRINTF(list_n_str,sizeof(list_n_str),"%d/%d",pMe->m_MusicPlayerCfg.lastPlayMusicID+1
            ,pMe->m_nPlayinglistMusicNum);
        STRTOWSTR(list_n_str,wliststr,sizeof(wliststr));
		MEMSET(list_n_str,0,sizeof(list_n_str));
		SNPRINTF(list_n_str,sizeof(list_n_str),"               %02d:%02d",minutetime,secondtime);
		STRTOWSTR(list_n_str,wtotaltimestr,sizeof(wtotaltimestr));
		WSTRCAT(wliststr,wtotaltimestr);
		
    }
    /*Draw Text With Profile */
    /*need changed skin and global var def 090118*/
	
	IANNUNCIATOR_SetHasTitleText(pMe->m_pIAnn,TRUE);
	IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,wliststr);
	IANNUNCIATOR_SetHasTitleText(pMe->m_pIAnn,FALSE);
	//IANNUNCIATOR_Redraw(pMe->m_pIAnn);
	/*
    DrawTextWithProfile(pMe->m_pShell, 
               pMe->m_pDisplay, 
               RGB_WHITE_NO_TRANS, 
               AEE_FONT_BOLD,
               wliststr, 
               -1, 
			   LISTINDEX_X, //wlh 20090415 mod
               LISTINDEX_Y,
               &clip, 
               IDF_ALIGN_CENTER|IDF_ALIGN_MIDDLE|IDF_TEXT_TRANSPARENT
               );*/
}
#endif
/*画进度条*/
static void MP3_RefreshscheduleBar(CMusicPlayer *pMe)
{
	/*
    AEERect clip;
    
    SETAEERECT( &clip, 2, 129,58,10);
    MP3_drawClipRectWithOffset(pMe,IDI_MUSICPLAYER,&clip);
    if(pMe->m_nCurrentTime >= pMe->m_nTotalTime)
    {
      pMe->m_nCurrentTime = pMe->m_nTotalTime;
    }
     if(pMe->m_nTotalTime!=0)
    {
      MP3_DrawImage(pMe,IDI_GLIDE, 46*pMe->m_nCurrentTime/pMe->m_nTotalTime+4, 134);
    }
    else
    {
        MP3_DrawImage(pMe,IDI_GLIDE, 4, 134);
    }
	*/

	//wlh 20090415 mod new ui start
	AEERect clip;
	AEERect clip1;
	AEERect oldClip;
    
    SETAEERECT( &clip, SCHEDULEBAR_X, SCHEDULEBAR_Y,SCHEDULEBAR_W,SCHEDULEBAR_H);
	
    MP3_drawClipRectWithOffset(pMe,IDI_MUSICPLAYER,&clip);
    if(pMe->m_nCurrentTime >= pMe->m_nTotalTime)
    {
      pMe->m_nCurrentTime = pMe->m_nTotalTime;
    }
	if(pMe->m_nCurrentTime != 0)
	{
		SETAEERECT( &clip1, SCHEDULEBAR_X, SCHEDULEBAR_Y,SCHEDULEBAR_W*pMe->m_nCurrentTime/pMe->m_nTotalTime,SCHEDULEBAR_H);
	}
	else
	{
		SETAEERECT( &clip1, SCHEDULEBAR_X, SCHEDULEBAR_Y,1,SCHEDULEBAR_H);
	}
     if(pMe->m_nTotalTime!=0)
    {
		IImage *image = ISHELL_LoadResImage( pMe->m_pShell,
                                         MUSICPLAYERIMAGE_RES_FILE,
                                         IDI_SCHEDULEBAR_FULL);
		if( image != NULL)
        {
			IDISPLAY_GetClipRect( pMe->m_pDisplay, &oldClip);
			IDISPLAY_SetClipRect( pMe->m_pDisplay, &clip1);
			IIMAGE_Draw( image, SCHEDULEBAR_X, SCHEDULEBAR_Y);
			IDISPLAY_SetClipRect( pMe->m_pDisplay, &oldClip);
			IIMAGE_Release( image);
			image = NULL;
		}
		MP3_DrawImage(pMe,IDI_GLIDE, (SCHEDULEBAR_W*pMe->m_nCurrentTime/pMe->m_nTotalTime)+SCHEDULEBAR_X, SCHEDULEBAR_Y);
    }
    else
    {
        MP3_DrawImage(pMe,IDI_GLIDE, SCHEDULEBAR_X, SCHEDULEBAR_Y);
    }
	 //wlh 20090415 mod new ui end
}
static void MP3_RefreshSimpleSchBar(CMusicPlayer *pMe)
{
    AEERect clip;
    AEERect oldClip;
	int x,y;//wlh 20090420 add

	if(pMe->m_eStartMethod != STARTMETHOD_SIMPLEPLAYER)//wlh 20090420 add
    {//正常播放
		x = SCHEDULEBAR_X;
		y = SCHEDULEBAR_Y;
	}
	else
	{//简单播放
		x = SIMSCHEDULEBAR_X;
		y = SIMSCHEDULEBAR_Y;
	}

    if(pMe->m_nSimPlayCurTime >= pMe->m_nTotalTime)
    {
		pMe->m_nSimPlayCurTime = pMe->m_nTotalTime;
    }
    MP3_DrawImage(pMe, IDI_SCHEDULEBAR_BLANK, x, y);
	if(pMe->m_nTotalTime!=0)
    {
		IImage *image = ISHELL_LoadResImage( pMe->m_pShell,
                                         MUSICPLAYERIMAGE_RES_FILE,
                                         IDI_SCHEDULEBAR_FULL);
        if( image != NULL)
        {
			DBGPRINTF("pMe->m_nSimPlayCurTime is %dpMe->m_nTotalTime is %d",pMe->m_nSimPlayCurTime,pMe->m_nTotalTime);
            //SETAEERECT( &clip, x,y, 72*pMe->m_nSimPlayCurTime/pMe->m_nTotalTime,1);//wlh mod 
			SETAEERECT( &clip, x,y, SIMSCHEDULEBAR_W*pMe->m_nSimPlayCurTime/pMe->m_nTotalTime,SIMSCHEDULEBAR_H);
            IDISPLAY_GetClipRect( pMe->m_pDisplay, &oldClip);
            IDISPLAY_SetClipRect( pMe->m_pDisplay, &clip);

            //IIMAGE_SetOffset( image, clip.x, clip.y);
            //IIMAGE_SetDrawSize( image, clip.dx, clip.dy);
            IIMAGE_Draw( image, x, y);
            IDISPLAY_SetClipRect( pMe->m_pDisplay, &oldClip);
            IIMAGE_Release( image);
            image = NULL;
        }   
    }
}
/*根据正在播放的歌曲名字找到其在播放列表中所处的ID号*/
void MP3_SearchIdByName(CMusicPlayer *pMe,char *musicName)
{
    int i;

    for(i=0;i<MUSIC_MAX_FILESNUM;i++)
    {
        if(0==STRCMP(musicName,pMe->m_PlayingMusiclist[i].pMusicName))
        {
            break;
        }
    }
    if(i < MUSIC_MAX_FILESNUM)
    {
      pMe->m_MusicPlayerCfg.lastPlayMusicID=i;
    }
}

/*滚动显示当前正在播放的歌曲名称*/
void MP3_MusicNameAutoScroll(CMusicPlayer *pMe)
{
    int nIdx,nIdxNew,n;
     nIdx  = nIdxNew = pMe->m_nAutoScrollIdx;
     n = WSTRLEN(pMe->m_pMp3FileToPlay);

    if(nIdx < n)
    {
        // Measure the text starting from the auto-scroll offset
        if(IsRequiredScroll(pMe, nIdx))
        {
            nIdxNew++;
        }
        else
        {
            nIdxNew = 0;
        }
		
        MP3_DrawMusicName(pMe,nIdx);
		
        IDISPLAY_Update(pMe->m_pDisplay);
    }
    else
    {
        nIdxNew = 0;
    }

    pMe->m_nAutoScrollIdx = nIdxNew;

    ISHELL_SetTimer( pMe->m_pShell,
                     MUSICNAME_AUTO_SCROLL_TIME,
                    (PFNNOTIFY)MP3_MusicNameAutoScroll, 
                     pMe);
}
/*判断是否需要滚动显示*/
static boolean IsRequiredScroll(CMusicPlayer *pMe, int nIdx)
{
    int   cx,cxMax;

    cx = IDISPLAY_MeasureText(pMe->m_pDisplay, AEE_FONT_BOLD, pMe->m_pMp3FileToPlay+ nIdx);
   
    cxMax = 149; //显示名字矩形筐宽度   
    
    return(cx > cxMax);
}
/*设置是否需要滚动*/
static void MP3_ResetScroll(CMusicPlayer *pMe)
{
    if(pMe==NULL)
    {
        return;
    }
    //pMe->m_nAutoScrollIdx = 0;

    if(IsRequiredScroll(pMe,0))
    {
       MP3_MusicNameAutoScroll(pMe); 
    }
    else
    {
        ISHELL_CancelTimer(pMe->m_pShell,(PFNNOTIFY)MP3_MusicNameAutoScroll,pMe);
		
        MP3_DrawMusicName(pMe,0);  
        
    }
}

/*画后退的图标*/
static void MP3_DrawRewindImage(CMusicPlayer *pMe)
{
   MP3_DrawImage(pMe, IDI_REWIND, 3,140);
   IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
}
/*画快进的图标*/
static void MP3_DrawForwardImage(CMusicPlayer *pMe)
{
   MP3_DrawImage(pMe, IDI_FORWARD, 40,140);
   IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
}
/*回调*/
int MP3_AddMusicCB(void* pv, FileNamesBuf pBuf, uint32 nBufSize)
{
    CMusicPlayer *pMe = (CMusicPlayer*)pv;
    char* pSrc = NULL;
    int spareNum,i;
    
    if(!pMe)
    {
        return EBADPARM;
    }
    spareNum = MUSIC_MAX_FILESNUM - pMe->m_nPlaylistMusicNum;
    for(i=0;spareNum>0 && nBufSize > 0;spareNum--,nBufSize--)
    {   
        //为了保存添加播放列表中歌曲的顺序，这里用第一位来存储该歌曲的添加顺序，这里用'0'开始的30个字符表示
         // PS:选择的该段字符最好不能含有'/''.'这两个字符
        pMe->m_Musiclist[pMe->m_nPlaylistMusicNum].pMusicName[0]=(pMe->m_nPlaylistMusicNum+48);
        pSrc = (char *)(pBuf + i);
        STRLCPY(pMe->m_Musiclist[pMe->m_nPlaylistMusicNum].pMusicName+1,
              pSrc,MP3_MAX_FILE_NAME * sizeof(char));
        //STRLCPY(pMe->m_Musiclist[pMe->m_nPlaylistMusicNum].pMusicName+1,
              // pSrc+i,MAX_FILE_NAME * sizeof(char));
        i++;
        pMe->m_nPlaylistMusicNum++;
    }
    CMusicPlayer_WriteMusiclist(pMe ,pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName);
    if(STRCMP(pMe->m_MusicPlayerCfg.lastPlayPlaylist,
      pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName)==0)
    {
        char *musicName;

        musicName=(char *)MALLOC(MP3NAME_MAX_LEN*sizeof(char));
        MEMSET(musicName,0,MP3NAME_MAX_LEN*sizeof(char));
       (void)STRCPY(musicName,
                    pMe->m_PlayingMusiclist[pMe->m_MusicPlayerCfg.lastPlayMusicID].pMusicName);
        CMusicPlayer_ReadMusiclist(pMe,
                                   pMe->m_MusicPlayerCfg.lastPlayPlaylist,
                                   TRUE);
        MP3_SearchIdByName(pMe, musicName);
        FREEIF(musicName);
    }
    return SUCCESS;
}

/*播放选中的播放列表的歌曲*/
static boolean MP3_PlayPlaylist(CMusicPlayer *pMe)
{
    if(!pMe)
    {
        return FALSE;
    }
    if(pMe->m_nPlaylistMusicNum == 0)
    {
        CLOSE_DIALOG(DLGRET_NO_RECORD);
        return FALSE;
    }
   if(STRCMP(pMe->m_MusicPlayerCfg.lastPlayPlaylist,
     pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName)!=0)
   {
        if(SUCCESS!=CMusicPlayer_ReadMusiclist(pMe,
                                               pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName,
                                               TRUE))
        {
            int i;

            for(i=0;i<MUSIC_MAX_FILESNUM;i++)
            {
             MEMSET(pMe->m_PlayingMusiclist[i].pMusicName,0,MP3NAME_MAX_LEN*sizeof(char));
            }
           // CLOSE_DIALOG(DLGRET_NO_RECORD);
           // return FALSE;
        }
        (void)STRCPY(pMe->m_MusicPlayerCfg.lastPlayPlaylist,
                     pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName);
   }
    CMusicPlayer_ReleaseMedia(pMe);
    pMe->m_bPlaying = FALSE;
    pMe->m_bPaused = FALSE;
   if(pMe->m_eCurState == STATE_PLAYLIST_OPTS)
   {
      pMe->m_MusicPlayerCfg.lastPlayMusicID=0;
   }
   else
   {
     pMe->m_MusicPlayerCfg.lastPlayMusicID = pMe->m_nCurMusiclistID;
   }
    if(pMe->m_MusicPlayerCfg.eMusicPlayMode == PLAYMODE_RANDOM)
    {
        MP3_ResetRandonIdentifier(pMe);
    }
    // 给一定时间给底层RELEASE
    (void) ISHELL_SetTimer(pMe->m_pShell,500,(PFNNOTIFY) CMusicPlayer_PlayPlaylistCB,pMe);
    return TRUE;
}
/*添加OPT主菜单*/
static void MP3_Build_MainOpts_Menu(CMusicPlayer *pMe,IMenuCtl *pMenuCtl)
{
    if(!pMenuCtl||!pMe)
    {
     return;
    }
    if(pMe->m_bPlaying==FALSE)
    {
      MP3MENU_ADDITEM(pMenuCtl,IDS_PLAY);
    }
    else
    {
      MP3MENU_ADDITEM(pMenuCtl,IDS_PAUSE);
    }
    MP3MENU_ADDITEM(pMenuCtl,IDS_PLAYLIST);
    MP3MENU_ADDITEM(pMenuCtl,IDS_SET_AS_RINGTONE);
    MP3MENU_ADDITEM(pMenuCtl,IDS_SETTINGS);
    if(pMe->m_bPlaying==TRUE)
    {
#ifdef FEATURE_SUPPORT_BT_APP
      //if(FALSE == GetIsPlayMP3ByBt())
      if(!pMe->m_bUseBT)
      {
         MP3MENU_ADDITEM(pMenuCtl, IDS_USEBT_HEADSET);
      }
      else
      {
         MP3MENU_ADDITEM(pMenuCtl, IDS_UNUSE_BT_HEADSET);
      }
#endif //FEATURE_SUPPORT_BT_APP
      MP3MENU_ADDITEM(pMenuCtl,IDS_MINIMIZE);
    }
         
}

/*build setting menu and set properties*/
static void MP3_Build_Settings_Menu(CMusicPlayer *pMe)
{
    if(!pMe)
    {
        return;
    }
    MP3MENU_ADDITEM(pMe->m_pMode,IDS_PLAYMODE_SINGLE);
    MP3MENU_ADDITEM(pMe->m_pMode,IDS_PLAYMODE_REPEATONE);
    MP3MENU_ADDITEM(pMe->m_pMode,IDS_PLAYMODE_ORDER);
    #ifdef FEATURE_MUSICPLAYER_RANDOM
    MP3MENU_ADDITEM(pMe->m_pMode,IDS_PLAYMODE_RANDOM);
    #endif
    MP3MENU_ADDITEM(pMe->m_pMode,IDS_PLAYMODE_CYCLE);
    MP3MENU_ADDITEM(pMe->m_pVolume,IDS_VOLUME_ONE);
    MP3MENU_ADDITEM(pMe->m_pVolume,IDS_VOLUME_TWO);
    MP3MENU_ADDITEM(pMe->m_pVolume,IDS_VOLUME_THREE);
    MP3MENU_ADDITEM(pMe->m_pVolume,IDS_VOLUME_FOUR);
    MP3MENU_ADDITEM(pMe->m_pVolume,IDS_VOLUME_FIVE);
    MP3MENU_ADDITEM(pMe->m_pVolume,IDS_VOLUME_OFF);
    MP3MENU_ADDITEM(pMe->m_pSort,IDS_SORT_NONE);
    //MP3MENU_ADDITEM(pMe->m_pSort,IDS_SORT_DATE);
    MP3MENU_ADDITEM(pMe->m_pSort,IDS_SORT_NAME);
    //MP3MENU_ADDITEM(pMe->m_pSort,IDS_SORT_SIZE);
    //set properties
    IMENUCTL_SetOemProperties(pMe->m_pMode, OEMMP_SWITCHNAVIGATIONKEY | OEMMP_IDF_ALIGN_CENTER);  
    IMENUCTL_SetOemProperties(pMe->m_pVolume, OEMMP_SWITCHNAVIGATIONKEY | OEMMP_IDF_ALIGN_CENTER);
    IMENUCTL_SetOemProperties(pMe->m_pSort, OEMMP_SWITCHNAVIGATIONKEY | OEMMP_IDF_ALIGN_CENTER);
}
/*set menuctlrect and set menu propoties*/
static void MP3_SetMenuCtlRect_Prop(CMusicPlayer * pMe, IMenuCtl* pMenuCtl,uint32 dwProps)
{
    AEERect        rc;
    AEEDeviceInfo devinfo;

    if(!pMe ||!pMenuCtl)
    {
        return;
    }
    ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
    rc = pMe->m_rc;
    rc.dy = devinfo.cyScreen;
    rc.dy -= GetBottomBarHeight(pMe->m_pDisplay); 
    IMENUCTL_SetRect(pMenuCtl,&rc);
    IMENUCTL_SetProperties(pMenuCtl, dwProps);
    //IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, 0));
}

/*draw settings text*/
static void MP3_Draw_SettingsText(CMusicPlayer *pMe)
{
    
    AEEMenuColors color;        //List Menu 的文本及背景颜色
    AECHAR      wszMode[8];    //播放模式
    // AECHAR      wszStyle[8];     //风格
    AECHAR      wszVolume[8];     //音量
    AECHAR      wszSort[8];   //排序方法
    AEERect     rc;
    AEEDeviceInfo devinfo;
    int16       title_hight ;
    int16       bottomheight ;
    int16       itemheight ;
    int16       lineSpace;
    
    if(!pMe)
    {
        return;
    }
    ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
    pMe->m_rc.dy = devinfo.cyScreen;
    title_hight = 0;
    bottomheight = GetBottomBarHeight(pMe->m_pDisplay);
    itemheight = IDISPLAY_GetFontMetrics(pMe->m_pDisplay, AEE_FONT_NORMAL, NULL, NULL);	//AEE_FONT_BOLD
    lineSpace = ( pMe->m_rc.dy - bottomheight - itemheight*3) / 4;	//4/5
    MEMSET(wszMode,0,sizeof(wszMode));	
    //MEMSET(wszStyle,0,sizeof(wszStyle));
    MEMSET(wszVolume,0,sizeof(wszVolume));
    MEMSET(wszSort,0,sizeof(wszSort));
     //以后把这四个函数合并为一个 
    Imenuctl_SetModeSel(pMe);
    //Imenuctl_SetStyleSel(pMe);
    Imenuctl_SetVolumeSel(pMe);
    Imenuctl_SetSortSel(pMe);
    //初始化List Menu的颜色
    color.wMask = MC_SEL_TEXT | MC_SEL_BACK | MC_TEXT | MC_BACK;
    color.cBack = RGB_WHITE;
    color.cText =  0x84848400;
    color.cSelBack = RGB_WHITE;
    color.cSelText = RGB_BLACK;

    //填充背景颜色
    //IDISPLAY_FillRect(pMe->m_pDisplay, &pMe->m_rc, 0);
    Appscommon_ResetBackgroundEx(pMe->m_pDisplay, &pMe->m_rc, TRUE);

    //play mode LIST控件的矩形
    SETAEERECT( &rc,
                CONTROL_RECT_START_X+10,
                lineSpace,
                pMe->m_rc.dx - CONTROL_RECT_START_X - 20,
                itemheight + 1//8
            );
    IDISPLAY_EraseRect(pMe->m_pDisplay,&rc);
    IMENUCTL_SetRect(pMe->m_pMode, &rc);
    //volume list控件的矩形
    SETAEERECT( &rc,
                CONTROL_RECT_START_X+10,
                lineSpace*2+itemheight,
                pMe->m_rc.dx - CONTROL_RECT_START_X - 20,
                itemheight + 1//8
            );
    IDISPLAY_EraseRect( pMe->m_pDisplay,&rc);
    IMENUCTL_SetRect(pMe->m_pVolume, &rc);
    // sort by list控件矩形
    SETAEERECT( &rc,
                CONTROL_RECT_START_X+10,
                lineSpace*3+itemheight*2,
                pMe->m_rc.dx - CONTROL_RECT_START_X - 20,
                itemheight + 1//8
            );
    IDISPLAY_EraseRect( pMe->m_pDisplay,&rc);
    IMENUCTL_SetRect(pMe->m_pSort, &rc);
   //模式标题
    (void) ISHELL_LoadResString(pMe->m_pShell,
                                MUSICPLAYER_RES_FILE_LANG,
                                IDS_PLAYMODE,
                                wszMode,
                                sizeof(wszMode));

    //风格标题
    //(void) ISHELL_LoadResString(pMe->m_pShell,
    //                            MUSICPLAYER_RES_FILE_LANG,
     //                           IDS_MUSICSTYLE,
     //                           wszStyle,
     //                           sizeof(wszStyle));

    //音量标题
    (void) ISHELL_LoadResString(pMe->m_pShell,
                                MUSICPLAYER_RES_FILE_LANG,
                                IDS_VOLUME,
                                wszVolume,
                                sizeof(wszVolume));
   //排序标题
    (void) ISHELL_LoadResString(pMe->m_pShell,
                                MUSICPLAYER_RES_FILE_LANG,
                                IDS_MUSICSORT,
                                wszSort,
                                sizeof(wszSort));

    IDISPLAY_SetColor( pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
   
    //模式
    (void)IDISPLAY_DrawText(pMe->m_pDisplay, AEE_FONT_NORMAL,
                          wszMode,-1,1,2 + lineSpace,NULL,
                          IDF_ALIGN_LEFT | IDF_TEXT_TRANSPARENT);
    //风格
    //(void)IDISPLAY_DrawText(pMe->m_pDisplay, AEE_FONT_NORMAL,
    //                      wszStyle,-1,1,2 + title_hight + lineSpace*2+itemheight,NULL,
    //                      IDF_ALIGN_LEFT | IDF_TEXT_TRANSPARENT);
    //音量
    (void)IDISPLAY_DrawText(pMe->m_pDisplay, AEE_FONT_NORMAL,
                          wszVolume,-1,1,2  + lineSpace*2+itemheight,NULL,
                          IDF_ALIGN_LEFT | IDF_TEXT_TRANSPARENT);
     //排序
    (void)IDISPLAY_DrawText(pMe->m_pDisplay, AEE_FONT_NORMAL,
                          wszSort,-1,1,2  + lineSpace*3+itemheight*2,NULL,
                          IDF_ALIGN_LEFT | IDF_TEXT_TRANSPARENT);
 
    IDISPLAY_SetColor( pMe->m_pDisplay, CLR_USER_TEXT, RGB_BLACK);



    //设置LIST menu控件的颜色
    IMENUCTL_SetColors(pMe->m_pMode, &color);
    IMENUCTL_SetColors(pMe->m_pVolume, &color);
    IMENUCTL_SetColors(pMe->m_pSort, &color);
    CMusicPlayer_Draw_Arrow(pMe, 0, lineSpace,itemheight);
    MP3_Draw_Settings_TitleBar(pMe);
    
     CMusicPlayer_Set_CTL(pMe);
    (void)IMENUCTL_Redraw(pMe->m_pMode);
    (void)IMENUCTL_Redraw(pMe->m_pVolume);
    (void)IMENUCTL_Redraw(pMe->m_pSort);
}
/*draw settings titlebar*/
static void MP3_Draw_Settings_TitleBar(CMusicPlayer *pMe)
{ 
    TitleBar_Param_type title;
    AECHAR wszTitle[16] = {0};

    if(!pMe)
    {
        return;
    }
    ISHELL_LoadResString(pMe->m_pShell,
                         MUSICPLAYER_RES_FILE_LANG,
                         IDS_SETTINGS,
                         wszTitle,
                         sizeof(wszTitle));
    MEMSET(&title,0,sizeof(TitleBar_Param_type));
    title.dwAlignFlags = IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER;
    title.pwszTitle = wszTitle;
	#if 0
    DrawTitleBar(pMe->m_pDisplay,&title);
	#else
    IANNUNCIATOR_SetHasTitleText(pMe->m_pIAnn, TRUE);
	IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,wszTitle);
	#endif
    MP3_DRAW_BOTTOMBAR( BTBAR_SAVE_BACK);
}
/*draw cliprect*/
static void MP3_drawClipRectWithOffset(CMusicPlayer *pMe,uint32 imageId,AEERect *rect)
{
    IImage *image = ISHELL_LoadResImage( pMe->m_pShell,
                                         MUSICPLAYERIMAGE_RES_FILE,
                                         imageId);
    if( image != NULL)
    {

        AEERect oldClip;
        //AEERect clip;

        //SETAEERECT( &clip, 60, 129,52,50);
        IDISPLAY_GetClipRect( pMe->m_pDisplay, &oldClip);
        IDISPLAY_SetClipRect( pMe->m_pDisplay, rect);

        IIMAGE_SetOffset( image, rect->x, rect->y);
        IIMAGE_SetDrawSize( image, rect->dx, rect->dy);
        IIMAGE_Draw( image, rect->x, rect->y);
        IIMAGE_Release( image);
        image = NULL;

        IDISPLAY_SetClipRect( pMe->m_pDisplay, &oldClip);
    }
}

static void MP3_DrawNoRecord(CMusicPlayer *pMe)
{
    AECHAR wStrText[16] = {(AECHAR)'\0'};
    RGBVAL  oldColor; 
      
    (void)ISHELL_LoadResString(pMe->m_pShell,
                                MUSICPLAYER_RES_FILE_LANG,
                                IDS_PLAYLIST_NULL,
                                wStrText,
                                sizeof(wStrText)); 
    oldColor = IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
    (void)IDISPLAY_DrawText( pMe->m_pDisplay,
                            AEE_FONT_NORMAL,
                            wStrText,
                            -1,
                            6,
                            6 + TITLEBAR_HEIGHT,
                            NULL,
                            IDF_TEXT_TRANSPARENT);
    IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, oldColor);
}

static void CMusicPlayer_PlayPlaylistCB(CMusicPlayer *pMe)
{
    if(FALSE == CMusicPlayer_InitMusic(pMe))//该列表还没有存储歌曲
    {
         if(pMe->m_PlayingMusiclist[0].pMusicName[0]==0)
        {
            (void)ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_MUSICPLAYER, EVT_POPMSGDIALOG, IDS_MSG_NORECORD, 0);
        }
        else
        {
           (void)ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_MUSICPLAYER, EVT_POPMSGDIALOG, IDS_MSG_ERR_FORMAT, 0); 
        }
        pMe->m_MusicPlayerCfg.lastPlayPlaylist[0]='\0';
        (void)STRCPY(pMe->m_PlayingMusiclist[0].pMusicName,"1fs:/hsmm/music/Away.mp3");
        pMe->m_MusicPlayerCfg.lastPlayMusicID=0;
        pMe->m_nPlayinglistMusicNum = 1;
        (void) ISHELL_SetTimer(pMe->m_pShell,300,(PFNNOTIFY)CMusicPlayer_InitMusic,pMe);
        return;
    }
    else
    {
        //因为底层使用的是事件传递机制，需要给底层初始化并传递notify的时间，否则会出错，此处等0.6秒钟后播放下一首      
        (void) ISHELL_SetTimer(pMe->m_pShell,500,(PFNNOTIFY) CMusicPlayer_PlayMusic,pMe);
        pMe->m_bPlaying=TRUE;
        pMe->m_bPaused = FALSE;
    }
}
#ifdef FEATURE_SUPPORT_BT_APP
void MP3_PlayMusicByBTCallBack(void *pCxt, int nErr)
{
    CMusicPlayer *pMe = (CMusicPlayer *)pCxt;
#ifndef WIN32
    if(nErr == BCMAPP_AG_SUCCESS)
    {
    }
    else
    {
        pMe->m_bUseBT = FALSE;
        SetIsPlayMP3ByBt(FALSE);
        (void)ISHELL_PostEvent(pMe->m_pShell,
                               AEECLSID_APP_MUSICPLAYER,
                               EVT_POPMSGDIALOG,
                               IDS_MSG_OPEN_FAILED,
                               0);
    }
#endif
}
#endif
