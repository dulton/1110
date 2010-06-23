#ifndef MUSICPLAYER_PRIV_H
#define MUSICPLAYER_PRIV_H

/*==============================================================================
// 文件：
//        MusicPlayer.h
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
#ifndef AEE_SIMULATOR
#include "OEMFeatures.h"   //Handset Feature Definitions
#else
#include "BREWSimFeatures.h"   //Simulator Feature Definitions
#endif

#include "AEEShell.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
#include "OEMClassIDs.h"        
#include "AEEMenu.h"          
#include "AEEFile.h" 
#include "AEEText.h"
#include "AEE.h"
#include "AEEMedia.h"         
#include "AEEMediaFormats.h"
#include "AEEImageCtl.h"      
#include "AEEMIMETypes.h"  
#include "AEEDisp.h"
//#include "err.h" 
#include "AEEMediaUtil.h"     
#include "OEMFS.h"
//#include "OEMAppFuncs.h"  
#include "AEEConfig.h"    
#include "AEEBacklight.h" 
#include "Appscommon.h"
#include "AppComFunc.h"
#if defined(FEATURE_DISP_160X128)
#include "Appscommon_160x128.brh"
#elif defined(FEATURE_DISP_128X128)
#include "Appscommon_color.brh"
#else
#include "Appscommon_color.brh"
#endif
#include "AEESound.h"
#include "OEMCFGI.h"
#include "AEECM.h"
#if defined(AEE_STATIC)
#include "AEE_static.h"
#endif
#ifdef FEATRUE_SUPPORT_G_SENSOR
#include "g_sensor.h"
#endif
#ifndef WIN32
#include "ui.h"
#else
#include "oemhelperfunctype.h"
#endif//WIN32
#include "MusicPlayer.h"
#include "musicplayer.brh"
#include "musicplayerimage.brh"

//#ifdef FEATURE_SUPPORT_BT_APP
//#undef FEATURE_SUPPORT_BT_APP
//#endif

#ifndef WIN32
#ifdef FEATURE_SUPPORT_BT_APP
#include "bcmapp_ag.h"
#endif
#endif//WIN32

#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
//发送keypress消息用的宏
#define KEY_PRESS_DWPARM   10000
#endif//FEATURE_LCD_TOUCH_ENABLE
/*==============================================================================
                                 宏定义和常数
==============================================================================*/
//For BREW3.0 support the muti-language re-define the RES maco
#define MUSICPLAYER_RES_FILE_LANG  AEE_RES_LANGDIR MUSICPLAYER_RES_FILE
//音乐文件夹
#define MUSICPLAYLIST_DIR      "fs:/shared/playlist"
#define MP3_SD_MUSIC_DIR       "fs:/card0/music"
#define MP3_PHONE_MUSIC_DIR    "fs:/hsmm/music"
#define DEFAULT_PLAYLIST       "Default playlist"
#define  MUSICPLAYERCFG_VERSION     (0x0010)
//可支持的音乐文件类型
#define MP3_MUSIC_TYPE     "midi,mp3,wav"

#define MIDI_TYPE    "mid"
#define QCP_TYPE     "qcp"
#define MP3_TYPE     "mp3"
#define AMR_TYPE     "amr"
#define AAC_TYPE     "aac"
#define WAV_TYPE     "wav"
//定义的字符串数组最大长度
#define MAX_STR_LEN            64


//每个播放列表最多可以添加的文件数目
#define MUSIC_MAX_FILESNUM     50

//最大可以创建的播放列表的数
#define PLAYLIST_MAX_NUM  10

//列表名称最大长度
#define PLAYLISTNAME_MAX_LEN   MAX_STR_LEN  //因为可以最大输入16个中文字，所以转化为UTF8至少需要48个字符

#define MP3_MAX_FILE_NAME      128
//音乐全名(包括路径)的最大长度
#define MP3NAME_MAX_LEN   MP3_MAX_FILE_NAME+1



#define CONTROL_RECT_START_X    55  //设置项的控件位置

#define MAX_INPUT_NAME   16
#define STARTARGS_SIMPLEPLAYER ((char)'S')
#define STARTARGS_PEDOMETERCALLPLAYER ((char)'P')
//LIST控件
#define IDC_MUSICPLAY_MODE       1
#define IDC_MUSIC_VOLUME         2
#define IDC_MUSIC_SORT           3

#define MAX_LEN_MSGBOX                 (256)

#define TIMEOUT_MS_QUICK               (500)
#define MP3APPISREADY_TIME             (200)
#define MS_FASTFORWARDREWIND_TIME      (5000)  // 毫秒

#define MP3NAME_COLOR                  MAKE_RGB(66,156,255)


#ifndef ERR
#define ERR DBGPRINTF
#endif

#ifndef ASSERT
#define ASSERT(x)\
if(!(x))         \
{                \
   return FALSE;    \
}
#endif

#define FARF_STATE   TRUE
#define FARF_EVENT   TRUE
#define FARF_ADDR    TRUE

#define MP3_FARF(x, p) if(TRUE == FARF_##x)  DBGPRINTF p

#if defined (FEATURE_DISP_160X128)
//wlh 20090415 add 设置ICON 坐标 
//开机动画
#define ANI_X 0
#define ANI_Y 0//27
#define ANI_W 176
#define ANI_H 82
//动画帧数
#define ANI_NUM 5

//进度条
#define SCHEDULEBAR_X 25
#define SCHEDULEBAR_Y 40
#define SCHEDULEBAR_W 94
#define SCHEDULEBAR_H 7

//进度条(简单播放)
#define SIMSCHEDULEBAR_X 17
#define SIMSCHEDULEBAR_Y 147
#define SIMSCHEDULEBAR_W 94
#define SIMSCHEDULEBAR_H 7

//播放按钮
#define PLAY_X 65
#define PLAY_Y 56
#define PLAY_W 45
#define PLAY_H 44

//前一个曲目
#define PREVIOUSPRESS_X 37
#define PREVIOUSPRESS_Y 60
#define PREVIOUSPRESS_W 31
#define PREVIOUSPRESS_H 32

//后一个曲目
#define NEXTPRESS_X 107
#define NEXTPRESS_Y 60
#define NEXTPRESS_W 31
#define NEXTPRESS_H 32

//降低音量
#define DECREASEVOLUMEPRESS_X 2
#define DECREASEVOLUMEPRESS_Y 79
#define DECREASEVOLUMEPRESS_W 21
#define DECREASEVOLUMEPRESS_H 17

//增加音量
#define ADDVOLUMEPRESS_X 2
#define ADDVOLUMEPRESS_Y 43
#define ADDVOLUMEPRESS_W 18
#define ADDVOLUMEPRESS_H 18

//音量
#define VOLUME_X 5
#define VOLUME_Y 60
#define VOLUME_W 12
#define VOLUME_H 18

//当前序号/总数量
#define LISTINDEX_X 5
#define LISTINDEX_Y 95

//当前播放时间
#define PLAYINGTICK_X 117
#define PLAYINGTICK_Y 47
//当前播放时间（简单播放）
#define SIMPLAYINGTICK_X 60
#define SIMPLAYINGTICK_Y 155

//播放区域，包含前一首，播放，下一首
#define OLDPLAY_X 30
#define OLDPLAY_Y 68
#define OLDPLAY_W 114
#define OLDPLAY_H 45

#elif defined (FEATURE_DISP_128X128)
//开机动画
#define ANI_X 0
#define ANI_Y 0//27
#define ANI_W 176
#define ANI_H 82
//动画帧数
#define ANI_NUM 5

//进度条
#define SCHEDULEBAR_X 25
#define SCHEDULEBAR_Y 40
#define SCHEDULEBAR_W 94
#define SCHEDULEBAR_H 7

//进度条(简单播放)
#define SIMSCHEDULEBAR_X 17
#define SIMSCHEDULEBAR_Y 147
#define SIMSCHEDULEBAR_W 94
#define SIMSCHEDULEBAR_H 7

//播放按钮
#define PLAY_X 51
#define PLAY_Y 58
#define PLAY_W 45
#define PLAY_H 44

//前一个曲目
#define PREVIOUSPRESS_X 23
#define PREVIOUSPRESS_Y 62
#define PREVIOUSPRESS_W 31
#define PREVIOUSPRESS_H 32

//后一个曲目
#define NEXTPRESS_X 93
#define NEXTPRESS_Y 62
#define NEXTPRESS_W 31
#define NEXTPRESS_H 32

//降低音量
#define DECREASEVOLUMEPRESS_X 2
#define DECREASEVOLUMEPRESS_Y 79
#define DECREASEVOLUMEPRESS_W 21
#define DECREASEVOLUMEPRESS_H 17

//增加音量
#define ADDVOLUMEPRESS_X 2
#define ADDVOLUMEPRESS_Y 43
#define ADDVOLUMEPRESS_W 18
#define ADDVOLUMEPRESS_H 18

//音量
#define VOLUME_X 5
#define VOLUME_Y 60
#define VOLUME_W 12
#define VOLUME_H 18

//当前序号/总数量
#define LISTINDEX_X 5
#define LISTINDEX_Y 95

//当前播放时间
#define PLAYINGTICK_X 75
#define PLAYINGTICK_Y 47
//当前播放时间（简单播放）
#define SIMPLAYINGTICK_X 60
#define SIMPLAYINGTICK_Y 155

//播放区域，包含前一首，播放，下一首
#define OLDPLAY_X 30
#define OLDPLAY_Y 68
#define OLDPLAY_W 96
#define OLDPLAY_H 45

#endif
typedef enum MP3Recttype
{
	TYPE_PLAYER,//pause一样
	TYPE_DECVOLUME,
	TYPE_ADDVOLUME,
	TYPE_PREVIOUS,
	TYPE_NEXT
}MP3Recttype;

#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
#define MUSICPLAYER_PT_IN_RECT(a,b,rct)      (boolean)( ((a) >= (rct).x && (a) <= ((rct).x + (rct).dx)) && ((b) >= (rct).y && (b) <= ((rct).y + (rct).dy)) )
#endif//FEATURE_LCD_TOUCH_ENABLE
/*==============================================================================
                                 全局数据
==============================================================================*/
/*==============================================================================
                                 本地（静态）数据
==============================================================================*/

/*==============================================================================
                                 类型定义
==============================================================================*/
/*----------------------对话框相关数据类型声明---------------------*/
// 对话框关闭时返回值列表
typedef enum DLGRetValue
{
   DLGRET_CREATE,
   
   DLGRET_PLAY,
   DLGRET_PAUSE,
   DLGRET_PLAYLIST,
   DLGRET_SETASRINGTONE,
   DLGRET_SETTINGS,
   DLGRET_MINIMIZE,
   
   DLGRET_CREATEORRENAMELIST,
   DLGRET_PLAYLISTPLAY,
   DLGRET_PLAYLISTVIEW,
   DLGRET_ADDMUSIC,
   DLGRET_PLAYLISTDELETE,
   
   DLGRET_VIEWPLAY,
   DLGRET_VIEWDELETE,
   DLGRET_VIEWDELETEALL,
   
   DLGRET_INPUT_NULL,
   DLGRET_CREATELIST_FAILED,
   DLGRET_RENAMELIST_FAILED,
   DLGRET_REPEAT_PLAYLISTNAME,
   DLGRET_DELPLAYLISTWARN,
   //DLGRET_INITMUSICFAILED,
   DLGRET_NO_RECORD,
   DLGRET_ERR_FORMAT,
   DLGRET_INVALID_PLAYLISTNAME,
   DLGRET_ADDMUSIC_FAILED,
   DLGRET_SELECT_NONE,
   DLGRET_UNCONNECT_BT,
   DLGRET_OPENNING_SCO,
   DLGRET_CLOSED_BT,
   // 对话框关闭时可返回的通用值
   DLGRET_OK,
   DLGRET_NO,
   DLGRET_YES,
   DLGRET_MSGBOX_OK,
   DLGRET_CANCELED
} DLGRetValue;
//音量等级
typedef enum CVolumeLevel {
    
    VOLUME_OFF,
    VOLUME_ONE,
    VOLUME_TWO,
    VOLUME_THREE,
    VOLUME_FOUR,
    VOLUME_FIVE

} CVolumeLevel;
//播放模式
typedef enum CPlayMode{
     PLAYMODE_SINGLE,
     PLAYMODE_REPEAT_ONE,
     PLAYMODE_ORDER,
     PLAYMODE_RANDOM,
     PLAYMODE_CYCLE
     
}CPlayMode;
//排序方式
typedef enum CSort{
     SORT_NONE,
     //SORT_DATE,
     SORT_NAME
     //SORT_SIZE
}CSort;

typedef enum CPlayerSkin{
            PLAYERSKIN1,
            PLAYERSKIN2
            /*and more*/
}CPlayerSkin;
typedef struct _CMusiclist
{
  char pMusicName[MP3NAME_MAX_LEN];//用来存储包括路径名和扩展名的音乐文件名
}CMusiclist;
typedef struct _CPlaylist
{
  char  pPlaylistName[PLAYLISTNAME_MAX_LEN];//用来存储不包括路径名和扩展名的播放列表文件名
}CPlaylist;
typedef struct _CMusicPlayerCfg
{
   CPlayMode eMusicPlayMode;
   CVolumeLevel eMusicVolume; 
   CSort eMusicSortBy;
#ifdef FEATURE_MP3PLAYER_SWITCHSKIN
   CPlayerSkin eMp3Skin;
#endif
   int lastPlayMusicID;//退出前最后一次播放的音乐ID也用来表示当前正在播放的音乐ID
   char lastPlayPlaylist[PLAYLISTNAME_MAX_LEN];//退出前最后一次播放的列表名也用于临时存储正在播放的列表名
}CMusicPlayerCfg;
typedef enum _AppletStartMethod{
    STARTMETHOD_NORMAL,
    STARTMETHOD_SIMPLEPLAYER,
    STARTMETHOD_PEDOMETERCALLPLAYER
}AppletStartMethod;

typedef struct _FileName{
   char m_szFileName[MP3NAME_MAX_LEN];
}FileName;
/*----------------------状态机相关数据类型声明---------------------*/
// MusicPlayer Applet 状态机状态：
typedef enum MP3State
{
   STATE_NONE = 0,               // No state
   STATE_INIT,                   // Initial state.
   STATE_PLAYMUSIC_WINDOWS,
   STATE_MAINOPTSMENU,
   STATE_PLAYLIST,
   STATE_SET_AS_RINGTONE,
   STATE_SETTING,
   STATE_MSG,
   STATE_PLAYLIST_OPTS,
   STATE_CREATEORRENAMELIST,
   STATE_PLAYLIST_VIEW,
   STATE_VIEW_OPTS,
   STATE_VIEW_DELETEALL,
   STATE_SIMPLEPLAYER,
   STATE_PEDOMETERCALLPLAYER,
   STATE_MSGFMBGPLAYPROMPT,
   STATE_EXIT                    // State where we close the applet.
} MP3State;

// 状态处理函数返回给状态处理主函数的值类型
typedef enum NextFSMAction
{
   NFSMACTION_WAIT,
   NFSMACTION_CONTINUE
} NextFSMAction;

// 保存模块信息和模块引用计数的结构
typedef struct CMusicPlayerMod
{
    DECLARE_VTBL(IModule)
    uint32   m_nRefs;
} CMusicPlayerMod;
// MusicPlayer Applet对象结构体：
typedef struct _CMusicPlayer
{
    DECLARE_VTBL(IMusicPlayer)
    uint32          m_nRefs;          // 对象引用计数器
    IModule        *m_pModule;
    IDisplay       *m_pDisplay;
    IShell         *m_pShell;
    IConfig        *m_pConfig;

    AEERect       m_rc;

    // 当前活动对话框IDialog接口指针
    IDialog             *m_pActiveDlg;

    // 当前活动对话框ID
    uint16        m_pActiveDlgID;

    // 活动对话框关闭时的返回结果
    DLGRetValue   m_eDlgRet;

    // 是否不改写对话框返回结果，一般情况需要改写(FALSE)。
    boolean m_bNotOverwriteDlgRet;

    // Applet 前一状态
    MP3State m_ePreState;

    // Applet 当前状态
    MP3State m_eCurState;

    // Applet是否处于挂起状态
    boolean m_bSuspending;
    // Applet是否处于活动状态
    boolean             m_bActive;
    // Applet start type
    AppletStartMethod   m_eStartMethod;
    IImage     *m_pImage;
    IImage     *m_paniImage;
    //IMedia指针
    IMedia     *m_pMedia;
    //IMedia指针简单播放时用
    //IMedia     *m_psimplePlayMedia;
    CMusicPlayerCfg m_MusicPlayerCfg;
    //用来保存当前激活的播放列表的所有音乐名称
    CMusiclist  m_Musiclist[MUSIC_MAX_FILESNUM];
    //用来保存当前正播放的播放列表的所有音乐名称
    CMusiclist  m_PlayingMusiclist[MUSIC_MAX_FILESNUM];
    //用于保存播放列表
    CPlaylist        m_Playlist[PLAYLIST_MAX_NUM];
    //用来保存GALLERY传过来的文件名
    char        *m_pSimplePlayMusicName;
    //播放列表数量
    uint16  m_nPlaylistNum;
    //当前播放de列表的music数量
    uint16  m_nPlayinglistMusicNum;
    //当前激活控件播放列表的music数量
    uint16  m_nPlaylistMusicNum;
    AEEMediaData   m_md;
    //当前激活的播放列表ID
    uint16    m_nCurPlaylistID;
    //当前激活的音乐列表ID
    uint16    m_nCurMusiclistID;
    //settings
    IMenuCtl      *m_pMode; // IMenuCtl指针播放模式
    IMenuCtl      *m_pVolume;//IMenuCtl指针音量
    IMenuCtl      *m_pSort;//IMenuCtl指针 排序方式
    //当前活动的LIST控件ID
    uint16          m_nCtlID;
    AECHAR   *m_pInputListName;
    AECHAR   *m_pBuffer;
    AECHAR   *m_pMp3FileToPlay; // 用来保存当前正在播放的歌曲名
    // message resource ID
    uint16              m_nMsgResID;
    // 指向MessageBox信息的指针，动态分配需释放
    AECHAR             *m_pMsgBox_Msg;
    // Applet 返回状态
    MP3State        m_eMsgRetState;
    IFileMgr           *m_pFileMgr;       // IFileMgr interface  
    //当前正播放到的时间
    uint32    m_nCurrentTime;
    //正播放的音乐的总的时间
    uint32    m_nTotalTime;
    uint32    m_nSimPlayCurTime;
    //当前声音大小
    uint16    m_nCurrentVolume;
    //是否正在播放
    boolean   m_bPlaying;
    //是否暂停
    boolean   m_bPaused;
    //是否是用户停止播放
    boolean   m_bUserStopped;
    //是否是用户按键播放下一首
    boolean   m_bUserPressNext;
    //是否在播WAVE动画
    boolean   m_bStartAni;
    //App是否准备好
    boolean     m_bAppReady;
    //是否是重命名操作，主要是为了代码复用把创建播放列表和重命名合二为一
    boolean     m_bRenameOpt;
    //滚动显示的起始IDX
    int        m_nAutoScrollIdx;
    //是否是创建播放列表或者重命名播放列表弹出的提示框
    //boolean    m_bDifferentMsg;
    //在播放返回状态为MM_STATUS_ABORT时是否不继续播
    boolean    m_bNotContinue;
    boolean    m_bStartApp;
    boolean    m_bSimPlayFailed;
    byte       m_keyBeepVol;
    boolean    m_bInterrupt;
    Mp3Player_Status m_ePreMp3Status;
    PromptMsg_e_Type m_eMsgType; 
    IBacklight *m_pBackLight;
    /*用于当APP SUSPEND时用来保存 设置项*/
    uint16     m_nPlayMode;
    uint16     m_nVolume;
    uint16     m_nMusicSort;
    boolean    m_aIdentifier[MUSIC_MAX_FILESNUM];
    //随机播放时用来记录随机的次数
    uint8      m_nRandNum;
    AEERect    *m_pMP3FaceRect;
    uint16     m_nSkinImageId;
#ifdef FEATURE_SUPPORT_BT_APP
    //play music by BT or not
    boolean   m_bUseBT;
    // play music by 848 chip or qsc
    boolean   m_bUse848;
    boolean   m_bInterruptByBT;
    // bt ID
    uint8     m_nBTID;
#endif //FEATURE_SUPPORT_BT_APP
    MP3Recttype m_rtype;////wlh 20090415 mod 为了区别播放区域，加音量，减音量的刷新，加了个参数
} CMusicPlayer;

/*==============================================================================
                                 宏定义和常数
==============================================================================*/

// 状态移动宏：先将前一状态用当前状态更新，再将当前状态设为nextState
#define MOVE_TO_STATE(nextState) {            \
            MP3State tpState;            \
            tpState = nextState;          \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;        \
        }
// 关闭对话框宏
#define CLOSE_DIALOG(DlgRet)  {    \
                                     pMe->m_eDlgRet = DlgRet; \
                                     (void) ISHELL_EndDialog(pMe->m_pShell);  \
                               }


/*==============================================================================
                                 函数声明
==============================================================================*/
/*----------------------对话框相关函数声明---------------------*/
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
int CMusicPlayer_ShowDialog(CMusicPlayer *pMe, uint16  dlgResId);

int CMusicPlayer_ShowMsgBox(CMusicPlayer *pMe, uint16 wTextResId);

int  CMusicPlayer_ShowYesNoDialog( CMusicPlayer *pMe, 
                               uint16      msgResId ,
                               AECHAR     *msg, 
                               int         msglen );

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
                                      AEEEvent      eCode,
                                      uint16        wParam,
                                      uint32        dwParam
                                     );

/*----------------------状态机相关函数声明---------------------*/
/*==============================================================================
函数:
       CMusicPlayer_ProcessState
说明:
       MusicPlayer Applet状态处理主函数。函数根据pMe->m_currState将调用路由至
       相应的处理函数。

参数:
       pMe [In]: 指向MusicPlayer Applet对象结构的指针,该结构包含小程序的特定信息.

返回值:
       NFSMACTION_CONTINUE：指示后有子状态，状态机不能停止。
       NFSMACTION_WAIT：指示因要显示对话框界面给用户，应挂起状态机。

备注:

==============================================================================*/
NextFSMAction CMusicPlayer_ProcessState(CMusicPlayer *pMe);

/*==============================================================================
函数： CMusicPlayer_ReadMusiclist

说明：
      用来读取指定播放列表里面的所有音乐名称

参数：
      pMe:CMusicPlayer *类型指针
      curfileName:不包含路径和后缀的文件名
      isPlaying:TRUE:把从文件中读取的数据放到pMe->m_PlayingMusiclist中
                FALSE:把从文件中读取的数据存放到pMe->m_Musiclist中

返回值：
    SUCCESS:读取成功
    EFAILED:读取失败

备注：:
        无
        
==============================================================================*/
int CMusicPlayer_ReadMusiclist(CMusicPlayer *pMe , char *curFileName,boolean isPlaying);

/*==============================================================================
函数： CMusicPlayer_WriteMusiclist

说明：
       用来写入指定播放列表里面的所有音乐名称

参数：
      pMe:CMusicPlayer *类型指针
      curfileName:不包含路径和后缀的文件名

返回值：
    SUCCESS:读取成功
    EFAILED:读取失败

备注：:
        无
        
==============================================================================*/
int CMusicPlayer_WriteMusiclist(CMusicPlayer *pMe , char *curFileName);
/*==============================================================================
函数：MP_ReleaseMedia

说明：
       停止并释放Media指针

参数：
       pMe [in]：指向MusicPlayer Applet对象结构的指针。该结构包含小程序的特定信息。
       
返回值：
        无

备注：:

==============================================================================*/
void CMusicPlayer_ReleaseMedia(CMusicPlayer *pMe);

//初试化音乐
boolean CMusicPlayer_InitMusic(CMusicPlayer *pMe);

/*===========================================================================
  播放某首音乐
===========================================================================*/      
void CMusicPlayer_PlayMusic(CMusicPlayer *pMe);

/*===========================================================================
  恢复播放某首音乐
===========================================================================*/      
void CMusicPlayer_ResumeMusic(CMusicPlayer *pMe);

void CMusicPlayer_SeekMusic(CMusicPlayer *pMe);
void CMusicPlayer_PauseMusic(CMusicPlayer *pMe);

/*根据时间，名称，大小对正在播放的列表里面的音乐进行排序*/
void CMusicPlayer_PlayingMusiclistSortBy(CMusicPlayer * pMe);

/*根据时间，名称，大小对当前VIEW的列表里面的音乐进行排序*/
void CMusicPlayer_MusiclistSortBy(CMusicPlayer * pMe);

/*根据正在播放的歌曲名字找到其在播放列表中所处的ID号*/
void MP3_SearchIdByName(CMusicPlayer *pMe,char *musicName);

/*滚动显示当前正在播放的歌曲名称*/
void MP3_MusicNameAutoScroll(CMusicPlayer *pMe);

void MP3_DrawWaveAni(CMusicPlayer *pMe);

void MP3_StopAni(CMusicPlayer *pMe);

/*===========================================================================
   接受CMX底层回调
===========================================================================*/                       
void CMusicPlayer_MediaNotify(void * pUser, AEEMediaCmdNotify * pCmdNotify);

/*===========================================================================
   播放下一个文件, 
   bDirection为TRUE, 下一个文件
   bDirection为FALSE, 前一个文件
===========================================================================*/  
void CMusicPlayer_PlayNext(CMusicPlayer *pMe, boolean bDirection);

void SetMp3PlayerStatus(CMusicPlayer *pMe,Mp3Player_Status eSt);
void MP3_InitMusicCB(CMusicPlayer *pMe);
AEERect *GetPlayerSkinRect(CPlayerSkin ePlayerSkin);
//用来重置随机播放的相关参数
void MP3_ResetRandonIdentifier(CMusicPlayer *pMe);
#ifdef FEATURE_SUPPORT_BT_APP
void MP3_PlayMusicByBTCallBack(void *pCxt, int nErr);
void SetIsPlayMP3ByBt(boolean bPlayMp3ByBt);
#endif

#endif             
//MUSICPLAYER_PRIV_H

