#ifndef __INCLUDE_DEMO_RESOURCE_UTF8_H__
#define __INCLUDE_DEMO_RESOURCE_UTF8_H__

/* Screen 1: Multiple lines text. */
/* Start screen scroll text. */
#define     DEMO_START_NOTICE           (\
"  欢迎来到SimpleGUI演示工程，本工程用于演示SimpleGUI各API的显示效果、\
使用方法以及运作机理，在演示过程中，您可以通过键盘输入与SimpleGUI演示工\
程进行交互。\n\
  按“空格”键开始演示。"\
)

/* Screen 2: List*/
/* Demo list title. */
#define     DEMO_LIST_TITLE             ("测试列表")
/* Enumed list item choice type. */
#define     DEMO_LIST_ITEM_NOTICE_TEXT  ("文字消息")
#define     DEMO_LIST_ITEM_NOTICE_TIME  ("系统时间")
#define     DEMO_LIST_ITEM_ENUM_VALUE1  ("枚举项1")
#define     DEMO_LIST_ITEM_ENUM_VALUE2  ("枚举项2")
#define     DEMO_LIST_ITEM_ENUM_VALUE3  ("枚举项3")


/* List items data.                      Sign   ListText                            List item type           List value                 Decimal point           Enumed text list    */
/*                                                                                                           init.  min.   max.         init.  min.   max.                          */
#define     DEMO_LIST_ITEM_0            {0,     "简单列表项",                       LIST_ITEM_NORMAL,		{0,     0,     0     },    {0,     0,     0     },  NULL}
#define     DEMO_LIST_ITEM_1			{1,     "枚举类型列表项",		            LIST_ITEM_ENUM,			{0,     0,     1     },    {0,     0,     0     },  s_arrszNoticeType}
#define     DEMO_LIST_ITEM_2            {2,     "数字列表项",			            LIST_ITEM_DIGIT,		{0,     -50,   50    },    {0,     0,     3     },  NULL}
#define     DEMO_LIST_ITEM_3			{3,     "带小数的数字列表项",               LIST_ITEM_DIGIT,		{1,     -50,   50    },    {2,     0,     5     },  NULL}
#define     DEMO_LIST_ITEM_4            {4,     "超长文字的简单列表项",             LIST_ITEM_NORMAL,		{0,     0,     0     },    {0,     0,     0     },  NULL}
#define     DEMO_LIST_ITEM_5            {5,     "编辑框",				            LIST_ITEM_NORMAL,		{0,     0,     0     },    {0,     0,     0     },  NULL}
#define     DEMO_LIST_ITEM_6			{6,     "实时曲线",				            LIST_ITEM_NORMAL,		{0,     0,     0     },    {0,     0,     0     },  NULL}
#define     DEMO_LIST_ITEM_7            {7,     "测试项目1",				        LIST_ITEM_NORMAL,		{0,     0,     0     },    {0,     0,     0     },  NULL}
#define     DEMO_LIST_ITEM_8            {8,     "测试项目2",				        LIST_ITEM_ENUM,			{0,     0,     2     },    {0,     0,     0     },  s_arrszEnumedValue}
#define     DEMO_LIST_ITEM_9			{9,     "测试项目3",				        LIST_ITEM_DIGIT,		{0,     -50,   50    },    {0,     0,     3     },  NULL}

/* Screen 3: Text notice box. */
/* List notice text format. */
#define     DEMO_LIST_NOTICE_TEXT_FMT   ("选择列表项%u.")

/* Screen 4: RTC notice box. */
#define     DEMO_RTC_NOTICE_TEXT_FMT    ("系统时间\n%04u-%02u-%02u\n%02u:%02u:%02u")

/* Screen 5: Variable box. */
#define     DEMO_VARIABLE_BOX_TITLE     ("数值/文本编辑演示")
#define     DEMO_VARIABLE_BOX_HELPER    ("TAB键切换焦点编辑框。\n上下箭头调整数值。\n左右箭头调整焦点字符。\n按空格键继续。")

#endif // __INCLUDE_DEMO_RESOURCE_UTF8_H__
