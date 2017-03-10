#include "deviceCfg.h"
#include "../share/ipcSharemem.h"

cfg_opt_t OPT_MAP_SETTINGS[] =
{
	CFG_BOOL("valid", cfg_false,CFGF_NOCASE),
	CFG_INT("subno", 1, CFGF_NOCASE),
	CFG_INT("functype", 0, CFGF_NOCASE),
	CFG_INT("maptn", 0, CFGF_NOCASE),
	CFG_STR_LIST("no", "{1,2,3,4}", CFGF_NONE),
	CFG_END()
};

cfg_opt_t OPT_DEVICE_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_false,CFGF_NOCASE),
	CFG_STR("MeasureNo","0", CFGF_NOCASE),
	CFG_STR("Type", "", CFGF_NOCASE),
	CFG_STR("Address", "000000000000",CFGF_NOCASE),
	CFG_INT("ClRelayNo", 0,CFGF_NOCASE),
	CFG_INT("PortSet.Baud", 1200, CFGF_NOCASE),
	CFG_INT("PortSet.DataBit", 8, CFGF_NOCASE),
	CFG_INT("PortSet.StopBit", 1, CFGF_NOCASE),
	CFG_INT("PortSet.Parity", 2, CFGF_NOCASE),
	
	CFG_SEC("Map",OPT_MAP_SETTINGS , CFGF_MULTI |  CFGF_NOCASE),

	CFG_END()
};
cfg_opt_t OPT_DEVICES_SETTINGS[] =
{
	CFG_SEC("Device", OPT_DEVICE_SETTINGS, CFGF_MULTI |  CFGF_NOCASE),
	CFG_END()
};

INT8U MAKE_DEVICE_TYPE_VAL(const char* key)
{
	const char* keys[MD_TYPE_MAX]={"","Aircondition","Relay_8","Light_4","Relay_2","Led_12V_3","Doordisplay","RfidCard","Keyboard_20","Rectifier","LuxSensor"};//顺序需与T_emDeviceType对应	

	for(size_t i=0;i<sizeof(keys)/sizeof(keys[0]);i++)
	{
		if (strcasecmp(key,keys[i])==0)
		{
			return (INT8U)i;
		}
	}
	INT8U val=(INT8U)atoi(key);
	if (val>=sizeof(keys)/sizeof(keys[0]))
	{
		val=0;
	}
	return val;
}

void GET_DEVICE_TYPE_STRING(INT8U val, char* key)
{
	const char* keys[MD_TYPE_MAX]={"","Aircondition","Relay_8","Light_4","Relay_2","Led_12V_3","Doordisplay","RfidCard","Keyboard_20","Rectifier","LuxSensor"};//顺序需与T_emDeviceType对应	

	if (val>=sizeof(keys)/sizeof(keys[0]))
	{
		val=0;

	}
	strcpy(key,keys[val]);
}
int DeviceCfg::LoadDevices(Context& oContext,const char* name)
{
	
	cfg_t* cfg;
	cfg = cfg_init(OPT_DEVICES_SETTINGS, CFGF_NONE);
	if (CfgBase::ParseSetFile(cfg,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey())!=0)
	{
		cfg_free(cfg);
		return -1;
	}
	cfg_t* cfgDevice;
	for(unsigned int i = 0; i < cfg_size(cfg, "Device"); i++)
	{		
		cfgDevice = cfg_getnsec(cfg, "Device", i);	
		if (cfg_getbool(cfgDevice, "Enable")!=cfg_true) 
		{
			continue;
		}
		
		const char* MeasureNoStr=cfg_getstr(cfgDevice, "MeasureNo");
		DevicesBitSet dbs;
		dbs.Parse(MeasureNoStr);
		for(INT16U k=0;k<(INT16U)dbs.size();k++)
		{
			if (dbs.test(k)==false)
			{
				continue;
			}
			LoadDevice(oContext,cfgDevice,k);
		}
		
	}
	cfg_free(cfg);
	oContext.Devices.m_DBS.Stat();
	oContext.Devices.m_RtfBS.Stat();
	oContext.Devices.m_RelayBS.Stat();
	oContext.Devices.m_LuxSensorBS.Stat();
	return 0;
}

int DeviceCfg::LoadDevice(Context& oContext,cfg_t* cfgDevice,INT16U measureNo)
{
	cfg_t* cfgMap;
	cfg_t* cfgMapItem;
	if (Device::IsValidNo(measureNo)==false) 
	{
		return -1;
	}
	Device& device = oContext.Devices.m_Item[measureNo];
	device.m_Enable = (cfg_getbool(cfgDevice, "Enable")==cfg_true)?1:0;
	device.m_Tn = measureNo;

	device.m_Type = MAKE_DEVICE_TYPE_VAL(cfg_getstr(cfgDevice, "Type"));

	memset(device.m_Addr,0,sizeof(device.m_Addr));

	string strs;
	strs.reserve(CONS_METER_ADDR_MAX_LEN+1);
	strs.append(cfg_getstr(cfgDevice, "Address"));
	if(strs.length()>CONS_METER_ADDR_MAX_LEN)
	{
		KL_ERROR("%d号表的电表地址长度超过%d个字符\n",device.m_Tn,CONS_METER_ADDR_MAX_LEN);
		ASSERT(false);
	}
	int zc=0;
	for(int i=0;i<CONS_METER_ADDR_MAX_LEN;i++)
	{
		if((strs[i]<='0')||(strs[i]>'9'))
			zc+=1;
		else			
			break;
	}
	strs.erase(0,zc);
	strcpy(device.m_Addr,strs.c_str());
	device.m_ClRelayNo = (INT8U)cfg_getint(cfgDevice, "ClRelayNo");
	IPCSHAREMEM.m_IpcDevicesRegData[device.m_Tn].m_ClNo = device.m_ClRelayNo;
	IPCSHAREMEM.m_IpcDevicesRegData[device.m_Tn].m_DeviceNo = device.m_Tn;
	IPCSHAREMEM.m_IpcDevicesRegData[device.m_Tn].m_DeviceType = device.m_Type;
	IPCSHAREMEM.Update();
	device.m_PortSet.Baud=cfg_getint(cfgDevice, "PortSet.Baud");
	device.m_PortSet.DataBit=(INT8S)cfg_getint(cfgDevice, "PortSet.DataBit");
	device.m_PortSet.StopBit=(INT8S)cfg_getint(cfgDevice, "PortSet.StopBit");
	device.m_PortSet.Parity=(INT8S)cfg_getint(cfgDevice, "PortSet.Parity");

	for(unsigned t = 0; t < cfg_size(cfgDevice, "Map"); t++)
	{
		cfgMapItem = cfg_getnsec(cfgDevice, "Map", t);	
		device.m_MapItems[t].m_bValid = (cfg_getbool(cfgMapItem, "valid")==cfg_true)?1:0;
		device.m_MapItems[t].m_subno = (INT8S)cfg_getint(cfgMapItem, "subno");
		device.m_MapItems[t].m_funcType = (INT8S)cfg_getint(cfgMapItem, "functype");
		device.m_MapItems[t].m_mapTn = (INT8S)cfg_getint(cfgMapItem, "maptn");
		device.m_MapItemCount++;
		for(unsigned k = 0; k < cfg_size(cfgMapItem, "no"); k++)
		{
			device.m_MapItems[t].m_mapNbs.Parse(cfg_getnstr(cfgMapItem, "no", k),0,CONS_MAP_DEVICE_SUBNO_MAX_COUNT);
		}
		
	}
	if (device.m_Type == MD_RTF)
	{
		oContext.Devices.m_RtfBS.set(measureNo);
	}
	if (device.m_Type == MD_RELAY_8)
	{
		oContext.Devices.m_RelayBS.set(measureNo);
	}
	if (device.m_Type == MD_LUX_SENSOR)
	{
		oContext.Devices.m_LuxSensorBS.set(measureNo);
	}
	oContext.Devices.m_DBS.set(measureNo);
	return 0;
}


int DeviceCfg::SaveDevices(Context& oContext,const char* name)
{

	Devices& devices=oContext.Devices;
	string buff;
	buff.reserve(CONS_DEVICE_MAX_COUNT*sizeof(Device));
	size_t deviceSize=devices.m_DBS.size();
	
	string strs;
	strs.reserve(CONS_METER_ADDR_MAX_LEN);
	for(size_t i=0;i<deviceSize;i++)
	{
		Device &device = devices.m_Item[i];
		if (device.m_Enable==0x01)
		{
			char cTemp[32];
			sprintf(cTemp,"%s",device.m_Enable==1?"true":"false");
			buff.append("Device\n{\n\tEnable=\"").append(cTemp).append("\"");
			
			sprintf(cTemp,"%d",devices.m_Item[i].m_Tn);
			buff.append("\n\tMeasureNo=\"").append(cTemp).append("\"");

			GET_DEVICE_TYPE_STRING(device.m_Type,cTemp);
			buff.append("\n\tType=").append(cTemp);

			IPCSHAREMEM.m_IpcDevicesRegData[device.m_Tn].m_ClNo = device.m_ClRelayNo;
			IPCSHAREMEM.m_IpcDevicesRegData[device.m_Tn].m_DeviceNo = device.m_Tn;
			IPCSHAREMEM.m_IpcDevicesRegData[device.m_Tn].m_DeviceType = device.m_Type;
			IPCSHAREMEM.Update();
			strs.erase(strs.begin(),strs.end());
			strs.append(device.m_Addr);
			while(strs.length()<CONS_METER_ADDR_MAX_LEN)
				strs.insert(0,1,'0');
			buff.append("\n\tAddress=\"").append(strs).append("\"");


			sprintf(cTemp,"%d",device.m_PortSet.Baud);
			buff.append("\n\tPortSet.Baud=").append(cTemp);
			
			sprintf(cTemp,"%d",device.m_PortSet.DataBit);
			buff.append("\n\tPortSet.DataBit=").append(cTemp);
			
			sprintf(cTemp,"%d",device.m_PortSet.StopBit);
			buff.append("\n\tPortSet.StopBit=").append(cTemp);
			
			sprintf(cTemp,"%d",device.m_PortSet.Parity);
			buff.append("\n\tPortSet.Parity=").append(cTemp);

			string sStr;
			sStr.reserve(500);
			for (int j=0; j<CONS_MAP_DEVICE_SUBNO_MAX_COUNT; j++)
			{
				MapItem& mapItem = device.m_MapItems[j];
				if (mapItem.m_bValid == true)
				{
					sprintf(cTemp,"%s",mapItem.m_bValid==1?"true":"false");
					buff.append("\n\tMap\n\t{\n\t\tvalid=\"").append(cTemp).append("\"");
			
					sprintf(cTemp,"%d",mapItem.m_subno);
					buff.append("\n\t\tsubno=\"").append(cTemp).append("\"");

					sprintf(cTemp,"%d",mapItem.m_funcType);
					buff.append("\n\t\tfunctype=\"").append(cTemp).append("\"");


					sprintf(cTemp,"%d",mapItem.m_mapTn);
					buff.append("\n\t\tmaptn=\"").append(cTemp).append("\"");

					sStr.erase();
					mapItem.m_mapNbs.ToString(sStr,0,CONS_MAP_DEVICE_SUBNO_MAX_COUNT);
					buff.append("\n\t\tno={").append(sStr).append("}");
					buff.append("\n\t}\n");
				}
			}
			buff.append("\n}\n");
		}
	}

	int iFlag=CfgBase::SaveSetFile(buff,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());
	return iFlag;
}



