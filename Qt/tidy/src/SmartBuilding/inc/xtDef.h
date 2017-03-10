#pragma once

#define NULL    0
// 数据类型定义
typedef unsigned char           INT8U;
typedef INT8U                   UCHAR;
typedef unsigned short          INT16U;
typedef signed short            INT16S;
typedef unsigned int            INT32U;
typedef unsigned long long      INT64U;
typedef signed long long        INT64S;
typedef float                   FP32;
typedef double                  FP64;
typedef unsigned char           byte;

typedef void                    S_VOID;
typedef char                    S_CHAR;
typedef short                   S_SHORT;
typedef long                    S_LONG;
typedef unsigned char           S_BYTE;
typedef unsigned short          S_WORD;
typedef unsigned int            S_DWORD;

#define DEFAULT_HEIGHT          30
#define DEFAULT_WIDTH           110
#define DEFAULT_GAP             10
#define BOTTOM_BTN_W            150
#define BOTTOM_BTN_H            46

// 尺寸定义
#define WIDGET_WIDTH            800
#define WIDGET_HEIGHT           480

// 头
#define TOP_BAR_LEFT            0
#define TOP_BAR_TOP             0
#define TOP_BAR_WIDTH           WIDGET_WIDTH
#define TOP_BAR_HEIGHT          38

// 内容
#define CONTENT_L               0
#define CONTENT_T               39
#define CONTENT_W               WIDGET_WIDTH
#define CONTENT_H               WIDGET_HEIGHT - TOP_BAR_HEIGHT

// 菜单按钮
#define MENU_BTN_WIDTH          64
#define MENU_BTN_HEIGHT         60

// 左侧菜单栏
#define MENU_BAR_LEFT           0
#define MENU_BAR_TOP            10
#define MENU_BAR_WIDTH          MENU_BTN_WIDTH
#define MENU_BAR_HEIGHT         MENU_BTN_HEIGHT * 7

// 右侧功能栏
#define FUN_WIDGET_LEFT         MENU_BAR_WIDTH
#define FUN_WIDGET_TOP          10
#define FUN_WIDGET_WIDTH        WIDGET_WIDTH - MENU_BAR_WIDTH
#define FUN_WIDGET_HEIGHT       WIDGET_HEIGHT - TOP_BAR_HEIGHT

// 运行功能界面
#define CABINET_REPORT_L        0
#define CABINET_REPORT_T        0
#define CABINET_REPORT_W        600
#define CABINET_REPORT_H        30

#define DATA_REPORT_L           0
#define DATA_REPORT_T           30
#define DATA_REPORT_W           600
#define DATA_REPORT_H           330

#define MANUAL_CHECK_L          CABINET_REPORT_W + DEFAULT_GAP
#define MANUAL_CHECK_T          0
#define MANUAL_CHECK_W          DEFAULT_WIDTH
#define MANUAL_CHECK_H          DEFAULT_HEIGHT

#define RIGHT_FRAME_L           CABINET_REPORT_W + DEFAULT_GAP
#define RIGHT_FRAME_T           50
#define RIGHT_FRAME_W           116
#define RIGHT_FRAME_H           346

#define TIME_LABEL_L            0
#define TIME_LABEL_T            0
#define TIME_LABEL_W            DEFAULT_WIDTH
#define TIME_LABEL_H            DEFAULT_HEIGHT

#define TIME_LINEEDIT_L         0
#define TIME_LINEEDIT_T         30
#define TIME_LINEEDIT_W         DEFAULT_WIDTH
#define TIME_LINEEDIT_H         DEFAULT_HEIGHT

#define DIM_LABEL_L             0
#define DIM_LABEL_T             90
#define DIM_LABEL_W             DEFAULT_WIDTH
#define DIM_LABEL_H             DEFAULT_HEIGHT

#define DIM_COMBOX_L            0
#define DIM_COMBOX_T            120
#define DIM_COMBOX_W            DEFAULT_WIDTH
#define DIM_COMBOX_H            DEFAULT_HEIGHT

#define DIM_LINEEDIT_L          0
#define DIM_LINEEDIT_T          160
#define DIM_LINEEDIT_W          DEFAULT_WIDTH
#define DIM_LINEEDIT_H          DEFAULT_HEIGHT

#define TEN_ADD_BTN_L           0
#define TEN_ADD_BTN_T           200
#define TEN_ADD_BTN_W           55
#define TEN_ADD_BTN_H           DEFAULT_HEIGHT + 2

#define TEN_SUB_BTN_L           55
#define TEN_SUB_BTN_T           TEN_ADD_BTN_T
#define TEN_SUB_BTN_W           TEN_ADD_BTN_W
#define TEN_SUB_BTN_H           DEFAULT_HEIGHT + 2

#define SWITCH_BTN_L            0
#define SWITCH_BTN_T            262
#define SWITCH_BTN_W            DEFAULT_WIDTH
#define SWITCH_BTN_H            DEFAULT_HEIGHT + 3

#define TOTAL_LABEL_L           10
#define TOTAL_LABEL_T           380
#define TOTAL_LABEL_W           DEFAULT_WIDTH + 20
#define TOTAL_LABEL_H           DEFAULT_HEIGHT

#define TOTAL_LINEEDIT_L        TOTAL_LABEL_W + 10
#define TOTAL_LINEEDIT_T        TOTAL_LABEL_T
#define TOTAL_LINEEDIT_W        DEFAULT_WIDTH
#define TOTAL_LINEEDIT_H        DEFAULT_HEIGHT

#define DATA_BACK_BTN_L         586
#define DATA_BACK_BTN_T         386
#define DATA_BACK_BTN_W         BOTTOM_BTN_W
#define DATA_BACK_BTN_H         BOTTOM_BTN_H

// 时控和光控任务列表界面
#define TASK_CTRL_REPORT_L      0
#define TASK_CTRL_REPORT_T      0
#define TASK_CTRL_REPORT_W      FUN_WIDGET_WIDTH
#define TASK_CTRL_REPORT_H      385

#define TASK_CTRL_ADD_BTN_L     286
#define TASK_CTRL_ADD_BTN_T     385
#define TASK_CTRL_ADD_BTN_W     BOTTOM_BTN_W
#define TASK_CTRL_ADD_BTN_H     BOTTOM_BTN_H

#define TASK_CTRL_UPDATE_BTN_L  TASK_CTRL_ADD_BTN_L + 150
#define TASK_CTRL_UPDATE_BTN_T  TASK_CTRL_ADD_BTN_T
#define TASK_CTRL_UPDATE_BTN_W  TASK_CTRL_ADD_BTN_W
#define TASK_CTRL_UPDATE_BTN_H  TASK_CTRL_ADD_BTN_H

#define TASK_CTRL_DEL_BTN_L     TASK_CTRL_UPDATE_BTN_L + 150
#define TASK_CTRL_DEL_BTN_T     TASK_CTRL_ADD_BTN_T
#define TASK_CTRL_DEL_BTN_W     TASK_CTRL_ADD_BTN_W
#define TASK_CTRL_DEL_BTN_H     TASK_CTRL_ADD_BTN_H

// 日志和黑匣子列表
#define LOG_REPORT_L            0
#define LOG_REPORT_T            0
#define LOG_REPORT_W            FUN_WIDGET_WIDTH
#define LOG_REPORT_H            340

#define LOG_REPORT_DATA_W       223
#define LOG_REPORT_TYPE_W       160
#define LOG_REPORT_CONNECT_W    353

#define LOG_PAGE_LABEL_L        10
#define LOG_PAGE_LABEL_T        340
#define LOG_PAGE_LABEL_W        100
#define LOG_PAGE_LABEL_H        BOTTOM_BTN_H

#define LOG_FLUSH_BTN_L         586
#define LOG_FLUSH_BTN_T         LOG_PAGE_LABEL_T
#define LOG_FLUSH_BTN_W         BOTTOM_BTN_W
#define LOG_FLUSH_BTN_H         BOTTOM_BTN_H

// 系统设置参数
#define SYSTEM_FRAME_L          0
#define SYSTEM_FRAME_T          0
#define SYSTEM_FRAME_W          FUN_WIDGET_WIDTH
#define SYSTEM_FRAME_H          340

#define SYSTEM_READ_BTN_L       436
#define SYSTEM_READ_BTN_T       SYSTEM_FRAME_H
#define SYSTEM_READ_BTN_W       BOTTOM_BTN_W
#define SYSTEM_READ_BTN_H       BOTTOM_BTN_H

#define SYSTEM_SAVE_BTN_L       586
#define SYSTEM_SAVE_BTN_T       SYSTEM_READ_BTN_T
#define SYSTEM_SAVE_BTN_W       BOTTOM_BTN_W
#define SYSTEM_SAVE_BTN_H       BOTTOM_BTN_H

#define SYSTEM_CLEAR_BTN_L      256
#define SYSTEM_CLEAR_BTN_T      SYSTEM_READ_BTN_T
#define SYSTEM_CLEAR_BTN_W      180
#define SYSTEM_CLEAR_BTN_H      BOTTOM_BTN_H

#define DEFEND_REPORT_L         0
#define DEFEND_REPORT_T         0
#define DEFEND_REPORT_W         FUN_WIDGET_WIDTH
#define DEFEND_REPORT_H         340

#define FRAME_DISTANCE          20
#define FRAME_EDIT_W            220
#define FRAME_DEFAULT_W         140

#define TIMING_BTN_L            FRAME_DISTANCE
#define TIMING_BTN_T            FRAME_DISTANCE
#define TIMING_BTN_W            FRAME_DEFAULT_W
#define TIMING_BTN_H            DEFAULT_HEIGHT

#define TIMING_EDIT_L           160 + FRAME_DISTANCE
#define TIMING_EDIT_T           TIMING_BTN_T
#define TIMING_EDIT_W           FRAME_EDIT_W
#define TIMING_EDIT_H           DEFAULT_HEIGHT

#define SOFT_VER_LABEL_L        440
#define SOFT_VER_LABEL_T        TIMING_BTN_T
#define SOFT_VER_LABEL_W        FRAME_EDIT_W
#define SOFT_VER_LABEL_H        DEFAULT_HEIGHT

#define TEL_NUM_BTN_L           FRAME_DISTANCE
#define TEL_NUM_BTN_T           TIMING_BTN_T + 40
#define TEL_NUM_BTN_W           FRAME_DEFAULT_W
#define TEL_NUM_BTN_H           DEFAULT_HEIGHT

#define TEL_NUM_EDIT_L          160 + FRAME_DISTANCE
#define TEL_NUM_EDIT_T          TEL_NUM_BTN_T
#define TEL_NUM_EDIT_W          FRAME_EDIT_W
#define TEL_NUM_EDIT_H          DEFAULT_HEIGHT

#define SET_TEL_BTN_L           440
#define SET_TEL_BTN_T           TEL_NUM_BTN_T
#define SET_TEL_BTN_W           FRAME_DEFAULT_W / 2
#define SET_TEL_BTN_H           DEFAULT_HEIGHT

#define IP_ADD_LABEL_L          FRAME_DISTANCE
#define IP_ADD_LABEL_T          TEL_NUM_BTN_T + 40
#define IP_ADD_LABEL_W          FRAME_DEFAULT_W
#define IP_ADD_LABEL_H          DEFAULT_HEIGHT

#define IP_ADD_EDIT_L           160 + FRAME_DISTANCE
#define IP_ADD_EDIT_T           IP_ADD_LABEL_T
#define IP_ADD_EDIT_W           FRAME_EDIT_W
#define IP_ADD_EDIT_H           DEFAULT_HEIGHT

#define MASK_LABEL_L            FRAME_DISTANCE
#define MASK_LABEL_T            IP_ADD_LABEL_T + 40
#define MASK_LABEL_W            FRAME_DEFAULT_W
#define MASK_LABEL_H            DEFAULT_HEIGHT

#define MASK_EDIT_L             160 + FRAME_DISTANCE
#define MASK_EDIT_T             MASK_LABEL_T
#define MASK_EDIT_W             FRAME_EDIT_W
#define MASK_EDIT_H             DEFAULT_HEIGHT

#define GATEWAY_LABEL_L         FRAME_DISTANCE
#define GATEWAY_LABEL_T         MASK_LABEL_T + 40
#define GATEWAY_LABEL_W         FRAME_DEFAULT_W
#define GATEWAY_LABEL_H         DEFAULT_HEIGHT

#define GATEWAY_EDIT_L          160 + FRAME_DISTANCE
#define GATEWAY_EDIT_T          GATEWAY_LABEL_T
#define GATEWAY_EDIT_W          FRAME_EDIT_W
#define GATEWAY_EDIT_H          DEFAULT_HEIGHT

#define SERVER_LABEL_L          FRAME_DISTANCE
#define SERVER_LABEL_T          GATEWAY_LABEL_T + 40
#define SERVER_LABEL_W          FRAME_DEFAULT_W
#define SERVER_LABEL_H          DEFAULT_HEIGHT

#define SERVER_EDIT_L           160 + FRAME_DISTANCE
#define SERVER_EDIT_T           SERVER_LABEL_T
#define SERVER_EDIT_W           FRAME_EDIT_W
#define SERVER_EDIT_H           DEFAULT_HEIGHT

#define PORT_LABEL_L            440
#define PORT_LABEL_T            SERVER_EDIT_T
#define PORT_LABEL_W            60
#define PORT_LABEL_H            DEFAULT_HEIGHT

#define PORT_EDIT_L             510
#define PORT_EDIT_T             PORT_LABEL_T
#define PORT_EDIT_W             100
#define PORT_EDIT_H             DEFAULT_HEIGHT

#define LOGIC_LABEL_L           FRAME_DISTANCE
#define LOGIC_LABEL_T           SERVER_LABEL_T + 40
#define LOGIC_LABEL_W           FRAME_DEFAULT_W
#define LOGIC_LABEL_H           DEFAULT_HEIGHT

#define LOGIC_L                 160 + FRAME_DISTANCE
#define LOGIC_T                 LOGIC_LABEL_T
#define LOGIC_W                 FRAME_EDIT_W
#define LOGIC_H                 DEFAULT_HEIGHT

#define OLD_PWD_LABEL_L         FRAME_DISTANCE
#define OLD_PWD_LABEL_T         FRAME_DISTANCE
#define OLD_PWD_LABEL_W         FRAME_DEFAULT_W + 60
#define OLD_PWD_LABEL_H         DEFAULT_HEIGHT

#define OLD_PWD_EDIT_L          240
#define OLD_PWD_EDIT_T          OLD_PWD_LABEL_T
#define OLD_PWD_EDIT_W          FRAME_EDIT_W
#define OLD_PWD_EDIT_H          DEFAULT_HEIGHT

#define NEW_PWD_LABEL_L         FRAME_DISTANCE
#define NEW_PWD_LABEL_T         OLD_PWD_LABEL_T + 50
#define NEW_PWD_LABEL_W         FRAME_DEFAULT_W + 60
#define NEW_PWD_LABEL_H         DEFAULT_HEIGHT

#define NEW_PWD_EDIT_L          240
#define NEW_PWD_EDIT_T          NEW_PWD_LABEL_T
#define NEW_PWD_EDIT_W          FRAME_EDIT_W
#define NEW_PWD_EDIT_H          DEFAULT_HEIGHT

#define ENSURE_PWD_LABEL_L      FRAME_DISTANCE
#define ENSURE_PWD_LABEL_T      NEW_PWD_LABEL_T + 50
#define ENSURE_PWD_LABEL_W      FRAME_DEFAULT_W + 60
#define ENSURE_PWD_LABEL_H      DEFAULT_HEIGHT

#define ENSURE_PWD_EDIT_L       240
#define ENSURE_PWD_EDIT_T       ENSURE_PWD_LABEL_T
#define ENSURE_PWD_EDIT_W       FRAME_EDIT_W
#define ENSURE_PWD_EDIT_H       DEFAULT_HEIGHT

// 配置界面相关
#define DIM_CIRCUIT_W           69
#define DIM_PRECENT_W           59

#define DIM_REPORT_L            0
#define DIM_REPORT_T            0
#define DIM_REPORT_W            FUN_WIDGET_WIDTH
#define DIM_REPORT_H            340

#define CONFIG_FRAME_L          0
#define CONFIG_FRAME_T          0
#define CONFIG_FRAME_W          FUN_WIDGET_WIDTH
#define CONFIG_FRAME_H          340

#define CONFIG_SAVE_BTN_L       586
#define CONFIG_SAVE_BTN_T       340
#define CONFIG_SAVE_BTN_W       BOTTOM_BTN_W
#define CONFIG_SAVE_BTN_H       BOTTOM_BTN_H

#define CONFIG_READ_BTN_L       436
#define CONFIG_READ_BTN_T       CONFIG_SAVE_BTN_T
#define CONFIG_READ_BTN_W       BOTTOM_BTN_W
#define CONFIG_READ_BTN_H       BOTTOM_BTN_H

#define CONFIG_FLUSH_BTN_L      586
#define CONFIG_FLUSH_BTN_T      340
#define CONFIG_FLUSH_BTN_W      BOTTOM_BTN_W
#define CONFIG_FLUSH_BTN_H      BOTTOM_BTN_H

// 恒照、光控、时控信息界面
#define DEFAULT_LABEL_W         75
#define DEFAULT_WIDGET_L        100
#define DEFAULT_WIDGET_W        FRAME_EDIT_W

#define TASK_FRAME_L            0
#define TASK_FRAME_T            0
#define TASK_FRAME_W            FUN_WIDGET_WIDTH
#define TASK_FRAME_H            385

#define TASK_INFO_L             0
#define TASK_INFO_T             0
#define TASK_INFO_W             FUN_WIDGET_WIDTH
#define TASK_INFO_H             432

#define TASK_FLUSH_BTN_L        436
#define TASK_FLUSH_BTN_T        386
#define TASK_FLUSH_BTN_W        BOTTOM_BTN_W
#define TASK_FLUSH_BTN_H        BOTTOM_BTN_H

#define TASK_SAVE_BTN_L         586
#define TASK_SAVE_BTN_T         TASK_FLUSH_BTN_T
#define TASK_SAVE_BTN_W         BOTTOM_BTN_W
#define TASK_SAVE_BTN_H         BOTTOM_BTN_H

#define TASK_CANCEL_BTN_L       436
#define TASK_CANCEL_BTN_T       TASK_SAVE_BTN_T
#define TASK_CANCEL_BTN_W       BOTTOM_BTN_W
#define TASK_CANCEL_BTN_H       BOTTOM_BTN_H

#define TASK_ADDACTION_BTN_L    286
#define TASK_ADDACTION_BTN_T    TASK_SAVE_BTN_T
#define TASK_ADDACTION_BTN_W    BOTTOM_BTN_W
#define TASK_ADDACTION_BTN_H    BOTTOM_BTN_H

#define ENABLE_LABEL_L          FRAME_DISTANCE
#define ENABLE_LABEL_T          FRAME_DISTANCE
#define ENABLE_LABEL_W          DEFAULT_LABEL_W
#define ENABLE_LABEL_H          DEFAULT_HEIGHT

#define ENABLE_CHECK_L          DEFAULT_WIDGET_L
#define ENABLE_CHECK_T          ENABLE_LABEL_T
#define ENABLE_CHECK_W          DEFAULT_WIDGET_W
#define ENABLE_CHECK_H          DEFAULT_HEIGHT

#define ID_LABEL_L              FRAME_DISTANCE
#define ID_LABEL_T              ENABLE_LABEL_T + 45
#define ID_LABEL_W              DEFAULT_LABEL_W
#define ID_LABEL_H              DEFAULT_HEIGHT

#define ID_COMBOX_L             DEFAULT_WIDGET_L
#define ID_COMBOX_T             ID_LABEL_T
#define ID_COMBOX_W             DEFAULT_WIDGET_W
#define ID_COMBOX_H             DEFAULT_HEIGHT

// 恒照
#define CON_LUX_LABEL_L         FRAME_DISTANCE
#define CON_LUX_LABEL_T         ID_LABEL_T + 45
#define CON_LUX_LABEL_W         DEFAULT_LABEL_W
#define CON_LUX_LABEL_H         DEFAULT_HEIGHT

#define CON_LUX_EDIT_L          DEFAULT_WIDGET_L
#define CON_LUX_EDIT_T          CON_LUX_LABEL_T
#define CON_LUX_EDIT_W          DEFAULT_WIDGET_W
#define CON_LUX_EDIT_H          DEFAULT_HEIGHT

#define CON_STARTL_L            FRAME_DISTANCE
#define CON_STARTL_T            CON_LUX_LABEL_T + 45
#define CON_STARTL_W            DEFAULT_LABEL_W
#define CON_STARTL_H            DEFAULT_HEIGHT

#define CON_STARTE_L            DEFAULT_WIDGET_L
#define CON_STARTE_T            CON_STARTL_T
#define CON_STARTE_W            DEFAULT_WIDGET_W
#define CON_STARTE_H            DEFAULT_HEIGHT

#define CON_END_LABEL_L         FRAME_DISTANCE
#define CON_END_LABEL_T         CON_STARTL_T + 45
#define CON_END_LABEL_W         DEFAULT_LABEL_W
#define CON_END_LABEL_H         DEFAULT_HEIGHT

#define CON_END_EDIT_L          DEFAULT_WIDGET_L
#define CON_END_EDIT_T          CON_END_LABEL_T
#define CON_END_EDIT_W          DEFAULT_WIDGET_W
#define CON_END_EDIT_H          DEFAULT_HEIGHT

#define CON_EXECTIMEL_L         FRAME_DISTANCE
#define CON_EXECTIMEL_T         CON_END_LABEL_T + 45
#define CON_EXECTIMEL_W         DEFAULT_LABEL_W
#define CON_EXECTIMEL_H         DEFAULT_HEIGHT

#define CON_EXECTIMEE_L         DEFAULT_WIDGET_L - 3
#define CON_EXECTIMEE_T         CON_EXECTIMEL_T - 3
#define CON_EXECTIMEE_W         240
#define CON_EXECTIMEE_H         DEFAULT_HEIGHT

#define CON_CIRCUITL_L          FRAME_DISTANCE
#define CON_CIRCUITL_T          CON_EXECTIMEL_T + 45
#define CON_CIRCUITL_W          DEFAULT_LABEL_W
#define CON_CIRCUITL_H          DEFAULT_HEIGHT

#define CON_CIRCUITC_L          DEFAULT_WIDGET_L
#define CON_CIRCUITC_T          CON_CIRCUITL_T
#define CON_CIRCUITC_W          DEFAULT_WIDGET_W
#define CON_CIRCUITC_H          DEFAULT_HEIGHT

#define CON_LOOP_LABEL_L        FRAME_DISTANCE
#define CON_LOOP_LABEL_T        CON_CIRCUITL_T + 50
#define CON_LOOP_LABEL_W        DEFAULT_LABEL_W
#define CON_LOOP_LABEL_H        DEFAULT_HEIGHT

#define CON_LOOP_WAY_L          DEFAULT_WIDGET_L
#define CON_LOOP_WAY_T          CON_CIRCUITL_T + 35
#define CON_LOOP_WAY_W          240
#define CON_LOOP_WAY_H          DEFAULT_HEIGHT + 20

#define CON_WEIGHTL_L           343
#define CON_WEIGHTL_T           FRAME_DISTANCE
#define CON_WEIGHTL_W           DEFAULT_LABEL_W
#define CON_WEIGHTL_H           DEFAULT_HEIGHT

#define CON_WEIGHT_L            336
#define CON_WEIGHT_T            CON_WEIGHTL_T + 30
#define CON_WEIGHT_W            400
#define CON_WEIGHT_H            331

// 光控
#define START_LUX_LABEL_L       FRAME_DISTANCE
#define START_LUX_LABEL_T       ID_LABEL_T + 45
#define START_LUX_LABEL_W       DEFAULT_LABEL_W
#define START_LUX_LABEL_H       DEFAULT_HEIGHT

#define START_LUX_EDIT_L        DEFAULT_WIDGET_L
#define START_LUX_EDIT_T        START_LUX_LABEL_T
#define START_LUX_EDIT_W        DEFAULT_WIDGET_W
#define START_LUX_EDIT_H        DEFAULT_HEIGHT

#define END_LUX_LABEL_L         FRAME_DISTANCE
#define END_LUX_LABEL_T         START_LUX_LABEL_T + 45
#define END_LUX_LABEL_W         DEFAULT_LABEL_W
#define END_LUX_LABEL_H         DEFAULT_HEIGHT

#define END_LUX_EDIT_L          DEFAULT_WIDGET_L
#define END_LUX_EDIT_T          END_LUX_LABEL_T
#define END_LUX_EDIT_W          DEFAULT_WIDGET_W
#define END_LUX_EDIT_H          DEFAULT_HEIGHT

#define LIGHT_START_TIMEL_L     FRAME_DISTANCE
#define LIGHT_START_TIMEL_T     END_LUX_LABEL_T + 45
#define LIGHT_START_TIMEL_W     DEFAULT_LABEL_W
#define LIGHT_START_TIMEL_H     DEFAULT_HEIGHT

#define LIGHT_START_TIME_L      DEFAULT_WIDGET_L
#define LIGHT_START_TIME_T      LIGHT_START_TIMEL_T
#define LIGHT_START_TIME_W      DEFAULT_WIDGET_W
#define LIGHT_START_TIME_H      DEFAULT_HEIGHT

#define LIGHT_END_TIMEL_L       FRAME_DISTANCE
#define LIGHT_END_TIMEL_T       LIGHT_START_TIMEL_T + 45
#define LIGHT_END_TIMEL_W       DEFAULT_LABEL_W
#define LIGHT_END_TIMEL_H       DEFAULT_HEIGHT

#define LIGHT_END_TIME_L        DEFAULT_WIDGET_L
#define LIGHT_END_TIME_T        LIGHT_END_TIMEL_T
#define LIGHT_END_TIME_W        DEFAULT_WIDGET_W
#define LIGHT_END_TIME_H        DEFAULT_HEIGHT

#define LIGHT_CIRCUITL_L        FRAME_DISTANCE
#define LIGHT_CIRCUITL_T        LIGHT_END_TIMEL_T + 45
#define LIGHT_CIRCUITL_W        DEFAULT_LABEL_W
#define LIGHT_CIRCUITL_H        DEFAULT_HEIGHT

#define LIGHT_CIRCUIT_L         DEFAULT_WIDGET_L
#define LIGHT_CIRCUIT_T         LIGHT_CIRCUITL_T
#define LIGHT_CIRCUIT_W         DEFAULT_WIDGET_W
#define LIGHT_CIRCUIT_H         DEFAULT_HEIGHT

#define LIGHT_LOOP_LABEL_L      FRAME_DISTANCE
#define LIGHT_LOOP_LABEL_T      LIGHT_CIRCUITL_T + 50
#define LIGHT_LOOP_LABEL_W      DEFAULT_LABEL_W
#define LIGHT_LOOP_LABEL_H      DEFAULT_HEIGHT

#define LIGHT_LOOP_WAY_L        DEFAULT_WIDGET_L
#define LIGHT_LOOP_WAY_T        LIGHT_CIRCUITL_T + 35
#define LIGHT_LOOP_WAY_W        240
#define LIGHT_LOOP_WAY_H        DEFAULT_HEIGHT + 20

#define ACTION_REPORT_L         353
#define ACTION_REPORT_T         FRAME_DISTANCE
#define ACTION_REPORT_W         363
#define ACTION_REPORT_H         200

#define SENSOR_SELECTION_L      ACTION_REPORT_L
#define SENSOR_SELECTION_T      ACTION_REPORT_T + 214
#define SENSOR_SELECTION_W      ACTION_REPORT_W
#define SENSOR_SELECTION_H      153

// 时控
#define TIME_ID_LABEL_L         FRAME_DISTANCE
#define TIME_ID_LABEL_T         ENABLE_LABEL_T + 55
#define TIME_ID_LABEL_W         DEFAULT_LABEL_W
#define TIME_ID_LABEL_H         DEFAULT_HEIGHT

#define TIME_ID_COM_L           DEFAULT_WIDGET_L
#define TIME_ID_COM_T           TIME_ID_LABEL_T
#define TIME_ID_COM_W           DEFAULT_WIDGET_W
#define TIME_ID_COM_H           DEFAULT_HEIGHT

#define TIME_PRIORITYL_L        FRAME_DISTANCE
#define TIME_PRIORITYL_T        TIME_ID_LABEL_T + 55
#define TIME_PRIORITYL_W        DEFAULT_LABEL_W
#define TIME_PRIORITYL_H        DEFAULT_HEIGHT

#define TIME_PRIORITY_L         DEFAULT_WIDGET_L
#define TIME_PRIORITY_T         TIME_PRIORITYL_T
#define TIME_PRIORITY_W         DEFAULT_WIDGET_W
#define TIME_PRIORITY_H         DEFAULT_HEIGHT

#define TIME_START_TIMEL_L      FRAME_DISTANCE
#define TIME_START_TIMEL_T      TIME_PRIORITYL_T + 55
#define TIME_START_TIMEL_W      DEFAULT_LABEL_W
#define TIME_START_TIMEL_H      DEFAULT_HEIGHT

#define TIME_START_TIME_L       DEFAULT_WIDGET_L
#define TIME_START_TIME_T       TIME_START_TIMEL_T
#define TIME_START_TIME_W       DEFAULT_WIDGET_W
#define TIME_START_TIME_H       DEFAULT_HEIGHT

#define TIME_END_TIMEL_L        FRAME_DISTANCE
#define TIME_END_TIMEL_T        TIME_START_TIMEL_T + 55
#define TIME_END_TIMEL_W        DEFAULT_LABEL_W
#define TIME_END_TIMEL_H        DEFAULT_HEIGHT

#define TIME_END_TIME_L         DEFAULT_WIDGET_L
#define TIME_END_TIME_T         TIME_END_TIMEL_T
#define TIME_END_TIME_W         DEFAULT_WIDGET_W
#define TIME_END_TIME_H         DEFAULT_HEIGHT

#define TIME_LOOP_LABEL_L       FRAME_DISTANCE
#define TIME_LOOP_LABEL_T       TIME_END_TIMEL_T + 60
#define TIME_LOOP_LABEL_W       DEFAULT_LABEL_W
#define TIME_LOOP_LABEL_H       DEFAULT_HEIGHT

#define TIME_LOOP_WAY_L         DEFAULT_WIDGET_L
#define TIME_LOOP_WAY_T         TIME_END_TIMEL_T + 50
#define TIME_LOOP_WAY_W         240
#define TIME_LOOP_WAY_H         DEFAULT_HEIGHT + 20

#define CIRCUIT_SELECTION_L     LIGHT_REPORT_L
#define CIRCUIT_SELECTION_T     LIGHT_REPORT_T + 214
#define CIRCUIT_SELECTION_W     LIGHT_REPORT_W
#define CIRCUIT_SELECTION_H     153

// 表格行高
#define REPORT_ROW_HEIGHT       30

// 类型定义
enum FUN_TYPE
{
    FUN_MONITOR = 0,
    FUN_TIME_CTRL,
    FUN_LIGHT_CTRL,
    FUN_CONSTANCE_AS,
    FUN_LOG,
    FUN_SYSTEM,
    FUN_CONFIG,

    FUN_COUNT,
};
