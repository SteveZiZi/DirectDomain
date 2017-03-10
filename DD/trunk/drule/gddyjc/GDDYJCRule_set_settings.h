#ifndef INCLUDED_GDDYJC_SET_SETTINGS
#define INCLUDED_GDDYJC_SET_SETTINGS

#ifndef INCLUDED_STRING
#include "../../base/String.h"
#endif

#ifndef DATA_CONVERT
#include "data_convert.h"
#endif

#ifndef	INCLUDED_GDDYJC_DATA
#include "GDDYJCData.h"
#endif
void SetSetting_80(int &i,bstring::iterator &operatorNow,bstring &orAns,INT16U &DataLen,Connection& irConnection,GDDYJCData &m_WriteSettings,INT8U limit);
void SetSetting_87(int &i,bstring::iterator &operatorNow,bstring &orAns,INT16U &DataLen,Connection& irConnection,GDDYJCData &m_WriteSettings,INT8U limit);
void SetSetting_88(int &i,bstring::iterator &operatorNow,bstring &orAns,INT16U &DataLen,GDDYJCData &m_WriteSettings,INT8U limit);
INT8U SetSetting_89(int &i,bstring::iterator &operatorNow,bstring &orAns,INT16U &DataLen,Connection& irConnection,GDDYJCData &m_WriteSettings,INT8U limit);
//suxufeng:[2010-3-24]此函数与AlterMeter几乎完全一样，改用AlterMeter实现。
//void SetSetting_89(int &i,bstring::iterator &operatorNow,bstring &orAns,INT16U &DataLen,GDDYJCData &m_WriteSettings,INT8U limit);
void SetSetting_C1(int &i,bstring::iterator &operatorNow,bstring &orAns,INT16U &DataLen,GDDYJCData &m_WriteSettings,INT8U limit);
void SetSetting_8A(int &i,bstring::iterator &operatorNow,bstring &orAns,INT16U &DataLen,GDDYJCData &m_WriteSettings,INT8U limit);
void AlterMeter(int &i,bstring::iterator &operatorNow,bstring::iterator operatorEnd,bstring &orAns,int &meterNo,INT16U &DataLen,GDDYJCData &m_WriteSettings,INT8U limit);
void SetSetting_01(int &i,bstring::iterator &operatorNow,bstring &orAns,INT16U &DataLen,Connection& irConnection,GDDYJCData &m_WriteSettings,INT8U limit);
void SetSetting_02(int &i,bstring::iterator &operatorNow,bstring &orAns,INT16U &DataLen,Connection& irConnection,GDDYJCData &m_WriteSettings,INT8U limit);
void SetSetting_05(int &i,bstring::iterator &operatorNow,bstring &orAns,INT16U &DataLen,Connection& irConnection,GDDYJCData &m_WriteSettings,INT8U limit);

#endif
