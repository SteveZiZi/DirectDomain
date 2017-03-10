using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Reflection;
using Excel = Microsoft.Office.Interop.Excel;
using System.Runtime.InteropServices;

using System.Threading;
using System.Data.OleDb;
using System.Runtime.InteropServices;
using System.Collections;
using System.IO.Ports;
using System.Text.RegularExpressions;
using System.Net.Sockets;
using System.Net;

namespace CJQTest
{
    public struct Frame_Stu
    {
        public byte ctrl;
        public byte afn;
        public byte[] addr;
        public int datalen;
        public ArrayList dataArray;
    }

    public partial class MainFrame : Form
    {

        public string configFilePath = Application.StartupPath + "\\config.xls";
        public string testerFilePath = (Application.StartupPath).Remove(Application.StartupPath.Length - 4) + "\\tester.xls";
        public string telnetRebootPath = Application.StartupPath + "\\tool\\";

        public FrmConfig schemeConfigFrm;
        public FrmKeyboard paramConfig;
        public FrmSet frmSet;
        public PowkeySceneSet powkeySceneSet;
        public FrmRcuState rcuStateQuq;
        public DoorcardSceneSet doorcardSceneSet;
        public DoorSceneSet doorSceneSet;
        public IrdtSceneSet irdtSceneSet;
        public SceneCompose sceneSet;
        public Information information;
        public int testTotalNum = 0;
        public IPAddress HostIP;
        public IPEndPoint point;
        public Socket socket;
        public AREA m_areaId;
        public CGwRule GwRule;//������Լ����
        public CGdRule GdRule;//�㶫��Լ����
        public CRule Rule;

        Thread thread; //���Խ���
        byte[] spaddr = { 0x88, 0x88, 0x88, 0x88, 0x88, 0x88 };
        public int UpRecDataLen;
        public const int GW_PW_LEN = 16;
        public const int GX_PW_LEN = 2;


        public enum REC_RESULT
        {

            OK = 0x00, // ȷ��֡
            TYPE2_OK = 0x95,// ȷ��֡
            ERROR = 0xc0, // ����֡
            TIME_OUT = 0xd0//��ʱ֡
        }

        public enum TEST_RESULT
        {
            PASS = 0x01,
            FAIL,
            COM_FAIL
        }

        public enum GW_AFN_CODE
        {
            OK = 0x00,
            SetClock = 0x05,
            GetClock = 0x0C,
            GetClockOK = 0x05,
            GetRelData = 0x0C,
            Down = 0x20,
            Up = 0x30,
            Beat = 0x02,
            SetDeviceNo = 0xaa,
            GetDeviceAddr
        }

        public enum GD_AFN_CODE
        {
            GetRealtimeParam = 0x01,
            GetRealParamOK = 0x81,
            SetRealtimeParam = 0x08,
            AddCJQ = 0x08,
            AddMeter = 0x08,
            AddCJQOK = 0x88,
            SetRealParamOK = 0x88,
            ReadRealTimeData = 0x11,
            ReadRealTimeDataOK = 0x91
        }

        public enum GX_AFN_CODE
        {
            OK = 0x00,
            SetClock = 0x04,
            GetClock = 0x0A,
            GetRelData = 0x8C,
            Down = 0x20,
            Up = 0x30,
            Beat = 0x02,
            SetDeviceNo = 0xaa,
            GetDeviceAddr
        }

        /// <summary>
        /// �������
        /// </summary>
        public enum ErrorCode
        {
            OK = 0,
            PW_ERROR = 1,
            NO_VALID_DATA = 2,
            VERSION_CHANGE = 3,
            METER_NO_DUPLICATE = 4,
            METER_ADDR_DUPLICATE = 5,
            TERM_NO_DUPLICATE = 6,
        }
        public enum TESTID
        {
            TS_CLOCK = 1,
            TS_BAT,
            TS_4851,
            TS_4852,
            TS_4853,
            TS_DOWN,
            TS_GPRS,
            TS_WATCHDOG,
            TS_REDWIRE,
            TS_NONE = 0XFF
        }

        public enum AREA
        {
            AREA_CQ = 0,
            AREA_GD,
            AREA_GX,
            AREA_MAX
        }

        public struct IPPORT_SET
        {
            public string ipaddr;
            public string port;
        }
        public struct PORT_SET
        {
            public int portno;
            public int bps;
            public int stopbit;
            public int checkbit;
            public void GetCheckbit(string str)
            {
                if (str == "��")
                    checkbit = 0;
                else if (str == "��")
                    checkbit = 1;
                else if (str == "ż")
                    checkbit = 2;
            }

        }



        public struct testItem
        {
            public string testName;
            public int testID;
            public PORT_SET serialPort;
            public IPPORT_SET netPort;
            public string meterAddr;
            public int ModuleId;

            public int GetTestId(string str)
            {
                if (str == "ϵͳʱ��")
                    testID = (int)TESTID.TS_CLOCK;
                else if (str == "���")
                    testID = (int)TESTID.TS_BAT;
                else if (str == "485-1")
                    testID = (int)TESTID.TS_4851;
                else if (str == "485-2")
                    testID = (int)TESTID.TS_4852;
                else if (str == "485-3")
                    testID = (int)TESTID.TS_4853;
                else if (str == "����ͨѶģ��")
                    testID = (int)TESTID.TS_DOWN;
                else if (str == "Զ��ģ��")
                    testID = (int)TESTID.TS_GPRS;
                else if (str == "���Ź�")
                    testID = (int)TESTID.TS_WATCHDOG;
                else if (str == "Զ����")
                    testID = (int)TESTID.TS_REDWIRE;
                else
                    testID = (int)TESTID.TS_NONE;
                return testID;
            }
        }

        bool bserialPortOpen = false;

        public testItem[] TestMember;
        public MainFrame()
        {
            InitializeComponent();
            Control.CheckForIllegalCrossThreadCalls = false; // ����һ���߳̿��Ե��ø��̴߳����Ŀؼ�


            //*******************88Ȩ�޼��
            string authorFilePath = Application.StartupPath + "\\run.txt";
            if (File.Exists(authorFilePath) == false)
            {
                //                return;
            }

            string year = DateTime.Now.Year.ToString();

            byte[] date = new byte[7];
            while (true)
            {
                if (year.Length > 2)
                {
                    year = year.Remove(0, 1);
                }
                else
                {
                    break;
                }
            }

            date[5] = Convert.ToByte(year, 16);
            date[4] = Convert.ToByte(DateTime.Now.Month.ToString(), 16);
            //byte month = Convert.ToByte(GetWeekDay(DateTime.Now.DayOfWeek.ToString()));
            //month = (byte)(month << 5);
            //date[4] |= month;
            //month = date[4];
            date[3] = Convert.ToByte(DateTime.Now.Day.ToString(), 16);
            date[2] = Convert.ToByte(DateTime.Now.Hour.ToString(), 16);
            date[1] = Convert.ToByte(DateTime.Now.Minute.ToString(), 16);
            date[0] = Convert.ToByte(DateTime.Now.Second.ToString(), 16);
            //2018��4��1�չ���
            if (date[5] >= 0x18 && date[4] >= 0x4 && date[3] >= 0x01)
            {
                File.Delete(authorFilePath);
                return;
            }
            //***************Ȩ�޼�����
            schemeConfigFrm = new FrmConfig(); // �������ô�������

            paramConfig = new FrmKeyboard();//�����������ô���
            frmSet = new FrmSet();//�������ò�������
            powkeySceneSet = new PowkeySceneSet();//�ܵ�Դ����
            doorcardSceneSet = new DoorcardSceneSet();//�ſ�����
            doorSceneSet = new DoorSceneSet();//���ų�������
            irdtSceneSet = new IrdtSceneSet();//���ⳡ������
            sceneSet = new SceneCompose();//��ͨ����
            rcuStateQuq = new FrmRcuState();//RCU�豸״̬��ѯ

            /* lvRunInfo�ؼ���ʼ��*/

            { ListViewItem item = new ListViewItem("������ť״̬"); item.SubItems.Add(""); lvRunInfo.Items.Add(item); }
            { ListViewItem item = new ListViewItem("�����ſ�����״̬"); item.SubItems.Add(""); lvRunInfo.Items.Add(item); }
            { ListViewItem item = new ListViewItem("������Ŵż��"); item.SubItems.Add(""); lvRunInfo.Items.Add(item); }
            { ListViewItem item = new ListViewItem("�ܵ�Դ����״̬"); item.SubItems.Add(""); lvRunInfo.Items.Add(item); }
            { ListViewItem item = new ListViewItem("�ȵƿ���״̬"); item.SubItems.Add(""); lvRunInfo.Items.Add(item); }
            { ListViewItem item = new ListViewItem("ҹ�ƿ���״̬"); item.SubItems.Add(""); lvRunInfo.Items.Add(item); }
            { ListViewItem item = new ListViewItem("�յ�����״̬"); item.SubItems.Add(""); lvRunInfo.Items.Add(item); }
            { ListViewItem item = new ListViewItem("�����䰴ť״̬"); item.SubItems.Add(""); lvRunInfo.Items.Add(item); }
            { ListViewItem item = new ListViewItem("�������״̬"); item.SubItems.Add(""); lvRunInfo.Items.Add(item); }
            { ListViewItem item = new ListViewItem("�����·�����"); item.SubItems.Add(""); lvRunInfo.Items.Add(item); }
            { ListViewItem item = new ListViewItem("�����¶�"); item.SubItems.Add(""); lvRunInfo.Items.Add(item); }
            { ListViewItem item = new ListViewItem("�յ������¶�"); item.SubItems.Add(""); lvRunInfo.Items.Add(item); }
            { ListViewItem item = new ListViewItem("�յ����ת�ٵ�"); item.SubItems.Add(""); lvRunInfo.Items.Add(item); }

            frmSet.cmbDoorCardType.SelectedIndex = 0;
            frmSet.cmbAirSeason.SelectedIndex = 0;
            frmSet.cmbDoorDisplaytype.SelectedIndex = 0;


            m_areaId = AREA.AREA_GD;

            information = new Information(); // ����ͨѶ��¼��ӡ����
            GwRule = new CGwRule(); //����������Լ����
            GdRule = new CGdRule(); //�����㶫��Լ����
            //�������������Լ����
            Rule = (CRule)GdRule;

        }


        private void config_Click(object sender, EventArgs e)
        {
            schemeConfigFrm.Visible = false;
            schemeConfigFrm.Focus();
            schemeConfigFrm.ShowDialog();
        }

        private void Start_Click(object sender, EventArgs e)
        {
            LoaderConfigFile();
            // �������ݴ����߳�
            //��ʼ��ϵͳӲ��ʱ��
            // SetSysInitTime();
            thread = new Thread(new ThreadStart(RunTest));
            thread.Start();
        }

        /// �������ݻ�����
        /// </summary>
        private ArrayList ReceivedDataBuff = new ArrayList();

        public ArrayList FrameBuff = new ArrayList();

        private void OnSerialReceivedData(object sender, System.EventArgs e)
        {
            System.Threading.Thread.Sleep(100);
            if (serialPort.BytesToRead > 0)
            {
                try
                {
                    byte[] data = new byte[serialPort.BytesToRead];
                    serialPort.Read(data, 0, data.Length);

                    string stemp = "\r\n���գ�";
                    for (int i = 0; i < data.Length; i++)
                    {
                        stemp += String.Format("{0:X2}", (byte)data[i]);
                        stemp += " ";
                    }
                    information.textBox1.Text += stemp;

                    if (bTestWatchDog == true)
                    {
                        return;
                    }
                    ReceivedDataBuff.AddRange(data);
                }
                catch (SystemException er)
                {
                    MessageBox.Show(er.Message);
                }

                int startindex = 0, endindex = 0;
                Frame_Stu RuleInfo = new Frame_Stu();
                while (true)
                {
                    switch (Rule.ParsePackage(ref ReceivedDataBuff, ref RuleInfo, ref startindex, ref endindex))
                    {
                        case ParseResult.OK:
                            ReceivedDataBuff.RemoveRange(0, endindex + 1);
                            //OnReceivedPackage(RuleInfo);
                            if (RuleInfo.afn != (byte)GW_AFN_CODE.Beat)
                            {
                                FrameBuff.Add(RuleInfo);    // ��ӽ��ܵ�����֡��������
                            }
                            break;

                        case ParseResult.Waitting:
                            return;

                        case ParseResult.Error:
                            if (ReceivedDataBuff.Count > 0)
                            {
                                try
                                {
                                    ReceivedDataBuff.RemoveRange(0, endindex + 1);
                                }
                                catch (Exception ex)
                                {
                                    //MessageBox.Show(this,ex.Message.ToString());
                                }
                            }
                            return;
                        case ParseResult.Unintegrated:
                            return;
                        default:
                            return;
                    }
                }
            }

        }
        #region//����
        private void OnNetReceivedData()
        {
            if (socket.Connected)
            {
                byte[] data = new byte[200];
                int recLen;
                while (true)
                {
                    try
                    {
                        recLen = socket.Receive(data, data.Length, 0);
                        if (recLen == 0)
                        {
                            continue;
                        }
                        byte[] recdata = new byte[recLen];
                        string stemp = "\r\n���գ�";
                        for (int i = 0; i < recLen; i++)
                        {
                            recdata[i] = data[i];
                            stemp += String.Format("{0:X2}", (byte)data[i]);
                            stemp += " ";
                        }
                        information.textBox1.Text += stemp;
                        ReceivedDataBuff.AddRange(recdata);
                    }
                    catch (SystemException er)
                    {
                        MessageBox.Show(er.Message);
                    }

                    int startindex = 0, endindex = 0;
                    Frame_Stu RuleInfo = new Frame_Stu();
                    // while (true)
                    {
                        switch (Rule.ParsePackage(ref ReceivedDataBuff, ref RuleInfo, ref startindex, ref endindex))
                        {
                            case ParseResult.OK:
                                ReceivedDataBuff.RemoveRange(0, endindex + 1);
                                //OnReceivedPackage(RuleInfo);
                                if (RuleInfo.afn != (byte)GW_AFN_CODE.Beat)
                                {
                                    FrameBuff.Add(RuleInfo);    // ��ӽ��ܵ�����֡��������
                                }
                                break;
                            case ParseResult.Waitting:
                                return;
                            case ParseResult.Error:
                                if (ReceivedDataBuff.Count > 0)
                                {
                                    try
                                    {
                                        ReceivedDataBuff.RemoveRange(0, endindex + 1);
                                    }
                                    catch (Exception ex)
                                    {
                                        //MessageBox.Show(this,ex.Message.ToString());
                                    }
                                }
                                return;
                            case ParseResult.Unintegrated:
                                return;
                            default:
                                return;
                        }
                    }
                }
            }


        }
        #endregion


        private REC_RESULT SendFrame(byte[] addr, byte ctl, byte[] data, ref Frame_Stu frame, int timeOutS, int retryTimes)
        {
            ArrayList sendframe = new ArrayList();
            int i;
            Rule.Makeframes(ref sendframe, addr, ctl, data);
            for (i = 0; i < retryTimes; i++)
            {
                sendDatas(sendframe);
                if (GetFrame(ref frame, timeOutS) == true)
                {
                    break;
                }
            }

            if (i == retryTimes)
            {
                return REC_RESULT.TIME_OUT;
            }
            if (m_areaId == AREA.AREA_GD)
            {
                if (frame.ctrl == (byte)GD_AFN_CODE.SetRealParamOK
                    || frame.ctrl == (byte)GD_AFN_CODE.GetRealParamOK
                    || frame.ctrl == (byte)GD_AFN_CODE.ReadRealTimeDataOK
                    )
                {
                    return REC_RESULT.OK;
                }
                else if (frame.ctrl == (byte)REC_RESULT.ERROR)
                {
                    return REC_RESULT.ERROR;
                }

            }
            else
            {
                if (frame.ctrl == (byte)GW_AFN_CODE.OK || frame.ctrl == (byte)GW_AFN_CODE.GetClock)
                {
                    return REC_RESULT.OK;
                }
                else if (frame.ctrl == (byte)REC_RESULT.ERROR)
                {
                    return REC_RESULT.ERROR;
                }
            }

            return REC_RESULT.TIME_OUT;
        }
        private REC_RESULT SendFrame(byte[] addr, byte ctl, byte[] data, int length, ref Frame_Stu frame, int timeOutS, int retryTimes)
        {
            ArrayList sendframe = new ArrayList();
            int i;
            Rule.Makeframes(ref sendframe, addr, ctl, data, length);
            for (i = 0; i < retryTimes; i++)
            {
                sendDatas(sendframe);
                
                if (GetFrame(ref frame, timeOutS) == true)
                {
                    break;
                }
            }

            if (i == retryTimes)
            {
                return REC_RESULT.TIME_OUT;
            }
            if (m_areaId == AREA.AREA_GD)
            {
                if (frame.ctrl == (byte)GD_AFN_CODE.SetRealParamOK
                    || frame.ctrl == (byte)GD_AFN_CODE.GetRealParamOK
                    || frame.ctrl == (byte)GD_AFN_CODE.ReadRealTimeDataOK
                    )
                {
                    return REC_RESULT.OK;
                }
                else if (frame.ctrl == (byte)REC_RESULT.ERROR)
                {
                    return REC_RESULT.ERROR;
                }

            }
            else
            {
                if (frame.ctrl == (byte)GW_AFN_CODE.OK || frame.ctrl == (byte)GW_AFN_CODE.GetClock)
                {
                    return REC_RESULT.OK;
                }
                else if (frame.ctrl == (byte)REC_RESULT.ERROR)
                {
                    return REC_RESULT.ERROR;
                }
            }

            return REC_RESULT.TIME_OUT;
        }

        private REC_RESULT SendFrame(byte[] addr, byte ctl, bool pw, int pwLen, byte[] pn, byte[] fn, byte[] data, ref Frame_Stu frame, int timeOutS, int retryTimes)
        {
            int i;
            ArrayList sendframe = new ArrayList();
            Rule.Makeframes(ref sendframe, addr, ctl, pw, pwLen, pn, fn, data);
            for (i = 0; i < retryTimes; i++)
            {
                sendDatas(sendframe);
                if (GetFrame(ref frame, timeOutS) == true)
                {
                    break;
                }
            }

            if (i == retryTimes)
            {
                return REC_RESULT.TIME_OUT;
            }

            if (frame.afn == (byte)REC_RESULT.OK
               || frame.afn == (byte)GW_AFN_CODE.GetClock
               || frame.afn == (byte)GW_AFN_CODE.GetRelData
               || frame.afn == (byte)GW_AFN_CODE.SetDeviceNo
               || frame.afn == (byte)GX_AFN_CODE.GetClock
               || frame.afn == (byte)GX_AFN_CODE.GetRelData
               )
            {
                return REC_RESULT.OK;
            }
            else if (frame.ctrl == (byte)REC_RESULT.ERROR)
            {
                return REC_RESULT.ERROR;
            }

            return REC_RESULT.TIME_OUT;
        }

        private REC_RESULT SendFrame(byte[] addr, byte ctl, bool pw, int pwLen, byte[] pn, byte[] fn, ref Frame_Stu frame, int timeOutS, int retryTimes)
        {
            int i;
            ArrayList sendframe = new ArrayList();
            Rule.Makeframes(ref sendframe, addr, ctl, pw, pwLen, pn, fn);

            for (i = 0; i < retryTimes; i++)
            {
                sendDatas(sendframe);
                if (GetFrame(ref frame, timeOutS) == true)
                {
                    break;
                }
            }

            if (i == retryTimes)
            {
                return REC_RESULT.TIME_OUT;
            }

            if (frame.afn == (byte)REC_RESULT.OK
                || frame.afn == (byte)GW_AFN_CODE.GetClock
                || frame.afn == (byte)GW_AFN_CODE.GetRelData
                || frame.afn == (byte)GW_AFN_CODE.SetDeviceNo
                || frame.afn == (byte)GX_AFN_CODE.GetClock
                || frame.afn == (byte)GX_AFN_CODE.GetRelData
                || frame.afn == (byte)GX_AFN_CODE.GetRelData
                )
            {
                return REC_RESULT.OK;
            }
            else if (frame.ctrl == (byte)REC_RESULT.ERROR)
            {
                return REC_RESULT.ERROR;
            }

            return REC_RESULT.TIME_OUT;
        }
        private bool GetFrame(ref Frame_Stu frame, int timeOutS)
        {
            int n = 0;
            while (FrameBuff.Count == 0)
            {
                System.Threading.Thread.Sleep(100);
                n++;
                if (n > timeOutS * 10)
                {
                    return false;
                }
            }
            frame = (Frame_Stu)FrameBuff[0];
            FrameBuff.Clear();
            return true;

        }

        public bool bNetOpenFlag = false;
        public Thread threadNet;
        private bool OpenNetPort(IPPORT_SET portSet)
        {
            HostIP = IPAddress.Parse(portSet.ipaddr);
            try
            {
                point = new IPEndPoint(HostIP, Int32.Parse(portSet.port));
                socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                socket.Connect(point);
                //threadNet = new Thread(new ThreadStart(OnNetReceivedData));

                // �������ݴ����߳�
                threadNet = new Thread(new ThreadStart(OnNetReceivedData));
                threadNet.Start();
                bNetOpenFlag = true;
                return true;
            }
            catch (Exception ey)
            {
                Console.WriteLine("������û�п���\r\n");
                return false;
            }
        }

        private void GetNetPort(ref IPPORT_SET portSet)
        {
            portSet.ipaddr = schemeConfigFrm.txtIP.Text.ToString();
            portSet.port = schemeConfigFrm.txtPort.Text.ToString();
        }

        private void GetSerialPort(ref PORT_SET portSet)
        {
            portSet.portno = Convert.ToInt16(schemeConfigFrm.comRedWire_no.Text.ToString());
            portSet.bps = Convert.ToInt32(schemeConfigFrm.comRedWire_bps.Text.ToString());
            portSet.GetCheckbit(schemeConfigFrm.comRedWire_checkbit.Text.ToString());
            portSet.stopbit = Convert.ToInt16(schemeConfigFrm.comRedWire_stopbit.Text.ToString());
        }

        private bool OpenSerialPort(PORT_SET portSet)
        {
            if (bserialPortOpen == true)
                return true;
            bserialPortOpen = true;

            serialPort.Close();
            serialPort.PortName = "com" + portSet.portno.ToString();
            serialPort.BaudRate = portSet.bps;
            serialPort.DataBits = 8;
            if (portSet.stopbit == 1)
            {
                serialPort.StopBits = StopBits.One;
            }
            else if (portSet.stopbit == 2)
            {
                serialPort.StopBits = StopBits.Two;
            }

            if (portSet.checkbit == 0)
            {
                serialPort.Parity = Parity.None;
            }
            else if (portSet.checkbit == 1)
            {
                serialPort.Parity = Parity.Odd;
            }
            else if (portSet.checkbit == 2)
            {
                serialPort.Parity = Parity.Even;
            }

            try
            {
                serialPort.Open();
                if (serialPort.IsOpen)
                {
                    //this.Hide();
                    serialPort.DataReceived += new SerialDataReceivedEventHandler(OnSerialReceivedData);
                    return true;
                }
                else
                {
                    string str = "����" + serialPort.PortName + "��ʧ�ܣ�";
                    MessageBox.Show(str);

                    return false;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message.ToString());
                return false;
            }
        }

        private void ClosePort()
        {
            ///*if (serialPort.IsOpen == true /*&& schemeConfigFrm.chkRS485.Checked == true*/)
            //{
            //    serialPort.Close();
            //}*/

            if (bNetOpenFlag == true && schemeConfigFrm.chkNet.Checked == true)
            {
                socket.Close();
                bNetOpenFlag = false;
            }
        }

        public void sendDatas(ArrayList frame)
        {
            byte[] btp = new byte[frame.Count];
            frame.CopyTo(btp);
            if (serialPort.IsOpen == true)
            {
                for (int i = 0; i < frame.Count; i++)
                {
                    System.Threading.Thread.Sleep(10);
                    serialPort.Write(btp, i, 1);
                }
            }
            else if (bNetOpenFlag == true)
            {
                socket.Send(btp, frame.Count, 0);
            }

            string stemp = "\r\n���ͣ�";
            for (int i = 0; i < btp.Length; i++)
            {
                stemp += String.Format("{0:X2}", (byte)btp[i]);
                stemp += " ";
            }
            information.textBox1.Text += stemp;
        }

        public void sendDatas(byte[] data, int len)
        {

            if (serialPort.IsOpen == true)
            {
                for (int i = 0; i < len; i++)
                {
                    System.Threading.Thread.Sleep(10);
                    serialPort.Write(data, i, 1);
                }
            }
            else
            {
                socket.Send(data, len, 0);
            }
            string stemp = "\r\n���ͣ�";
            for (int i = 0; i < data.Length; i++)
            {
                stemp += String.Format("{0:X2}", (byte)data[i]);
                stemp += " ";
            }
            information.textBox1.Text += stemp;
        }


        [DllImport("User32.dll", CharSet = CharSet.Auto)]
        public static extern int GetWindowThreadProcessId(IntPtr hwnd, out   int ID);
        private void LoaderConfigFile()
        {

        }

        private void GetConfig(ref testItem item, string st)
        {
            string[] str = st.Split(',');

            if (str.Length == 2)
            {
                item.netPort.ipaddr = str[0].ToString();
                item.netPort.port = str[1].ToString();
            }
            else if (str.Length == 3)
            {
                item.netPort.ipaddr = str[0].ToString();
                item.netPort.port = str[1].ToString();
                item.meterAddr = str[2].ToString();
            }
            else if (str.Length == 4)
            {
                item.serialPort.portno = Convert.ToInt16(str[0].ToString());
                item.serialPort.bps = Convert.ToInt32(str[1].ToString());
                item.serialPort.GetCheckbit(str[2].ToString());
                item.serialPort.stopbit = Convert.ToInt16(str[3].ToString());
            }

        }

        /******************************************  ���� ************************************/

        /// <summary>
        /// ���ڻ�ȡ�������
        /// </summary>
        /// <param name="MeterAddr">����ַ�����ȱ���Ϊ12�ֽڣ�������а�����BCD�ַ�����������ҿ�ʼ��ʽ
        /// ����ȡ��һ��BCD���ַ�����β��Ϊ����ͨѶ��ַ��������ģ������</param>
        /// 
        /// <returns></returns>
        private AREA GetArea(string str)
        {
            if (str == "����")
            {
                return AREA.AREA_CQ;
            }
            else if (str == "�㶫")
            {
                return AREA.AREA_GD;
            }
            else if (str == "����")
            {
                return AREA.AREA_GX;
            }
            else
            {
                return AREA.AREA_MAX;
            }
        }
        private bool GetAddress(ref Byte[] data, string addr)
        {
            if (addr.Length != 8)
            {
                MessageBox.Show("�豸��ַ������Ϊ8λ���֣�");
                return false;
            }

            data[0] = Convert.ToByte(addr.Substring(2, 2), 16);
            data[1] = Convert.ToByte(addr.Substring(0, 2), 16);
            data[2] = Convert.ToByte(addr.Substring(6, 2), 16);
            data[3] = Convert.ToByte(addr.Substring(4, 2), 16);
            return true;
        }

        private bool GetGDAddress(ref Byte[] data, string addr)
        {
            if (addr.Length != 8)
            {
                MessageBox.Show("�豸��ַ������Ϊ8λ���֣�");
                return false;
            }

            data[0] = Convert.ToByte(addr.Substring(0, 2), 16);
            data[1] = Convert.ToByte(addr.Substring(2, 2), 16);
            data[2] = Convert.ToByte(addr.Substring(6, 2), 16);
            data[3] = Convert.ToByte(addr.Substring(4, 2), 16);
            return true;
        }

        private bool GetGdMeterAddr(string addr, ref Byte[] data)
        {
            if (addr.Length != 12)
            {
                MessageBox.Show("����ַ������Ϊ12λ���֣�");
                return false;
            }

            data[5] = Convert.ToByte(addr.Substring(0, 2), 16);
            data[4] = Convert.ToByte(addr.Substring(2, 2), 16);
            data[3] = Convert.ToByte(addr.Substring(4, 2), 16);
            data[2] = Convert.ToByte(addr.Substring(6, 2), 16);
            data[1] = Convert.ToByte(addr.Substring(8, 2), 16);
            data[0] = Convert.ToByte(addr.Substring(10, 2), 16);
            return true;
        }


        void RecordTestResult(int row, int col, TEST_RESULT res)
        {

        }

        void RecordTestResult(int row, int col, string res)
        {

        }

        private int GetWeekDay(string str)
        {
            if (str == "Monday")
                return 1;
            else if (str == "Tuesday")
                return 2;
            else if (str == "Wednesday")
                return 3;
            else if (str == "Thursday")
                return 4;
            else if (str == "Friday")
                return 5;
            else if (str == "Saturday")
                return 6;
            else if (str == "Sunday")
                return 7;
            return 0;
        }

        private bool CompareArry(byte[] arry1, byte[] arry2, int len)
        {
            int i;
            for (i = 0; i < len; i++)
            {
                if (arry1[i] != arry2[i])
                {
                    break;
                }
            }

            if (i == len)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        private bool GetWatchDogPortset(ref PORT_SET portset)
        {
            portset.portno = Convert.ToInt16(schemeConfigFrm.com_no.Text.ToString());
            portset.bps = Convert.ToInt32(schemeConfigFrm.com_bps.Text.ToString());
            portset.GetCheckbit(schemeConfigFrm.com_checkbit.Text.ToString());
            portset.stopbit = Convert.ToInt16(schemeConfigFrm.com_stopbit.Text.ToString());
            return true;
        }

        private void SetSysInitTime()
        {
            string resetCmd1 = "root\r";
            string resetCmd2 = "ptu@keli\r";
            string wclock = "hwclock -w\r";
            string month = "";
            string day = "";
            string hour = "";
            string minute = "";
            string second = "";
            PORT_SET portSet = new PORT_SET();

            GetWatchDogPortset(ref portSet);
            if (OpenSerialPort(portSet) == false)
            {
                MessageBox.Show("���Ź����ڴ�ʧ�ܣ�ϵͳʱ���ʼ��ʧ�ܣ�");
                return;
            }

            char[] cmd1 = new char[resetCmd1.Length];
            byte[] dcmd1 = new byte[resetCmd1.Length];
            cmd1 = resetCmd1.ToCharArray();
            for (int i = 0; i < cmd1.Length; i++)
            {
                dcmd1[i] = Convert.ToByte(cmd1[i]);
            }
            sendDatas(dcmd1, dcmd1.Length);
            System.Threading.Thread.Sleep(1000);

            char[] cmd2 = new char[resetCmd2.Length];
            byte[] dcmd2 = new byte[resetCmd2.Length];
            cmd2 = resetCmd2.ToCharArray();
            for (int i = 0; i < cmd2.Length; i++)
            {
                dcmd2[i] = Convert.ToByte(cmd2[i]);
            }
            sendDatas(dcmd2, dcmd2.Length);
            System.Threading.Thread.Sleep(1000);

            string date = "date " + DateTime.Now.Month.ToString() + DateTime.Now.Day.ToString() + DateTime.Now.Hour.ToString() + DateTime.Now.Minute.ToString() + DateTime.Now.Year.ToString() + "." + DateTime.Now.Second.ToString() + "\r";

            char[] cmd3 = new char[date.Length];
            byte[] dcmd3 = new byte[date.Length];
            cmd3 = date.ToCharArray();
            for (int i = 0; i < cmd3.Length; i++)
            {
                dcmd3[i] = Convert.ToByte(cmd3[i]);
            }
            sendDatas(dcmd3, dcmd3.Length);
            System.Threading.Thread.Sleep(1000);

            char[] cmd4 = new char[wclock.Length];
            byte[] dcmd4 = new byte[wclock.Length];
            cmd4 = wclock.ToCharArray();
            for (int i = 0; i < cmd4.Length; i++)
            {
                dcmd4[i] = Convert.ToByte(cmd4[i]);
            }
            sendDatas(dcmd4, dcmd4.Length);
            System.Threading.Thread.Sleep(1000);




        }
        private void telnetReboot()
        {
            //����һ��������Ϣ�� 
            System.Diagnostics.ProcessStartInfo Info = new System.Diagnostics.ProcessStartInfo();
            //�����ⲿ������ 
            Info.FileName = "telnetreboot.exe";
            //�����ⲿ��������������������в�����Ϊtest.txt 
            Info.Arguments = "";
            //�����ⲿ������Ŀ¼ΪC:\\ 
            Info.WorkingDirectory = telnetRebootPath;
            //����һ�������� 
            System.Diagnostics.Process Proc;
            try
            {
                Proc = System.Diagnostics.Process.Start(Info);
            }
            catch (SystemException er)
            {
                MessageBox.Show(er.Message);
            }

        }
        private void DelUserDir()
        {
            //����һ��������Ϣ�� 
            System.Diagnostics.ProcessStartInfo Info = new System.Diagnostics.ProcessStartInfo();
            //�����ⲿ������ 
            Info.FileName = "delGdUserDir.exe";
            //�����ⲿ��������������������в�����Ϊtest.txt 
            Info.Arguments = "";
            //�����ⲿ������Ŀ¼ΪC:\\ 
            Info.WorkingDirectory = telnetRebootPath;
            //����һ�������� 
            System.Diagnostics.Process Proc;
            try
            {
                Proc = System.Diagnostics.Process.Start(Info);
            }
            catch (SystemException er)
            {
                MessageBox.Show(er.Message);
            }

        }

        /******************************************  ������ ************************************/

        private void RunTest()
        {
            bool bRes;
            string getData = "";
            while (true)
            {
                int i = 0;
                foreach (testItem dt in TestMember)
                {

                    if (dt.testID == (int)TESTID.TS_WATCHDOG || dt.testID == (int)TESTID.TS_REDWIRE)
                    {
                        switch (dt.testID)
                        {
                            case (int)TESTID.TS_WATCHDOG:
                                if (OpenSerialPort(dt.serialPort) == true)
                                {
                                    if (test_watchdog() == true)
                                    {
                                        RecordTestResult(i, 2, TEST_RESULT.PASS);
                                    }
                                    else
                                    {
                                        RecordTestResult(i, 2, TEST_RESULT.FAIL);
                                    }

                                    ClosePort();
                                }
                                break;

                            case (int)TESTID.TS_REDWIRE:
                                if (OpenSerialPort(dt.serialPort) == true)
                                {
                                    string time = "";
                                    if (test_redwire(ref time) == true)
                                    {
                                        RecordTestResult(i, 2, TEST_RESULT.PASS);
                                        RecordTestResult(i, 3, time);
                                    }
                                    else
                                    {
                                        RecordTestResult(i, 2, TEST_RESULT.FAIL);
                                    }
                                    break;
                                    ClosePort();
                                }
                                break;

                            default:
                                break;

                        }

                    }
                    else
                    {
                        if (bNetOpenFlag == false)
                        {
                            if (OpenNetPort(dt.netPort) == false)
                            {
                                MessageBox.Show("��������ʧ�ܣ��˳�����");
                                thread.Abort();
                                return;
                            }

                        }

                        switch (dt.testID)
                        {
                            case (int)TESTID.TS_CLOCK:
                                if (test_clock() == true)
                                {
                                    RecordTestResult(i, 2, TEST_RESULT.PASS);
                                }
                                else
                                {
                                    RecordTestResult(i, 2, TEST_RESULT.FAIL);
                                }
                                break;

                            case (int)TESTID.TS_BAT:
                                if (test_bat(dt.netPort) == true)
                                {
                                    RecordTestResult(i, 2, TEST_RESULT.PASS);
                                }
                                else
                                {
                                    RecordTestResult(i, 2, TEST_RESULT.FAIL);
                                }
                                break;

                            case (int)TESTID.TS_4851:
                                if (test_4851(dt.meterAddr, ref getData) == true)
                                {
                                    RecordTestResult(i, 2, TEST_RESULT.PASS);
                                    RecordTestResult(i, 3, getData);
                                }
                                else
                                {
                                    RecordTestResult(i, 2, TEST_RESULT.FAIL);
                                }
                                break;

                            case (int)TESTID.TS_4852:
                                if (test_4852(dt.meterAddr, ref getData) == true)
                                {
                                    RecordTestResult(i, 2, TEST_RESULT.PASS);
                                    RecordTestResult(i, 3, getData);
                                }
                                else
                                {
                                    RecordTestResult(i, 2, TEST_RESULT.FAIL);
                                }
                                break;

                            case (int)TESTID.TS_4853:
                                if (test_4853(dt.meterAddr, ref getData) == true)
                                {
                                    RecordTestResult(i, 2, TEST_RESULT.PASS);
                                    RecordTestResult(i, 3, getData);
                                }
                                else
                                {
                                    RecordTestResult(i, 2, TEST_RESULT.FAIL);
                                }
                                break;

                            case (int)TESTID.TS_DOWN:
                                if (test_down(dt.meterAddr, ref getData) == true)
                                {
                                    RecordTestResult(i, 2, TEST_RESULT.PASS);
                                    RecordTestResult(i, 3, getData);
                                }
                                else
                                {
                                    RecordTestResult(i, 2, TEST_RESULT.FAIL);
                                }
                                break;

                            default:
                                break;
                        }

                    }
                    i++;
                }
                MessageBox.Show("���Խ�����");
                ClosePort();
                thread.Abort();
            }

        }
        private bool Get_guowang_clock(ref string strDate)
        {
            Frame_Stu frame = new Frame_Stu();
            byte[] date = new byte[6];
            byte[] addr = new byte[4];

            if (GetAddress(ref addr, txtAddr.Text) == false)
            {
                return false;
            }

            string year = DateTime.Now.Year.ToString();
            while (true)
            {
                if (year.Length > 2)
                {
                    year = year.Remove(0, 1);
                }
                else
                {
                    break;
                }
            }

            date[5] = Convert.ToByte(year, 16);
            date[4] = Convert.ToByte(DateTime.Now.Month.ToString(), 16);
            byte month = Convert.ToByte(GetWeekDay(DateTime.Now.DayOfWeek.ToString()));
            month = (byte)(month << 5);
            date[4] |= month;
            month = date[4];
            date[3] = Convert.ToByte(DateTime.Now.Day.ToString(), 16);
            date[2] = Convert.ToByte(DateTime.Now.Hour.ToString(), 16);
            date[1] = Convert.ToByte(DateTime.Now.Minute.ToString(), 16);
            date[0] = Convert.ToByte(DateTime.Now.Second.ToString(), 16);

            byte[] pn = { 0x00, 0x00 };
            byte[] fn = { 0x40, 0x03 };
            if (m_areaId == AREA.AREA_CQ)
            {

                pn[0] = 0x00;
                pn[1] = 0x00;
                fn[0] = 0x02;
                fn[1] = 0x00;

                FrameBuff.Clear();
                if (SendFrame(addr, (byte)GW_AFN_CODE.GetClock, true, GW_PW_LEN, pn, fn, ref frame, 20, 3) == REC_RESULT.OK)
                {
                    byte[] data = new byte[frame.dataArray.Count];
                    frame.dataArray.CopyTo(data);
                    strDate = String.Format("{0:X2}", (byte)data[5]) + "��" + String.Format("{0:X2}", (byte)(data[4] & 0x1f)) + "��" + String.Format("{0:X2}", (byte)data[3]) + "��";
                    return true;
                }
                else
                {
                    return false;
                }

            }
            else if (m_areaId == AREA.AREA_GX)
            {

                pn[0] = 0x00;
                pn[1] = 0x00;
                fn[0] = 0x40;
                fn[1] = 0x03;

                FrameBuff.Clear();
                if (SendFrame(addr, (byte)GX_AFN_CODE.GetClock, false, GX_PW_LEN, pn, fn, ref frame, 20, 3) == REC_RESULT.OK)
                {
                    byte[] data = new byte[frame.dataArray.Count];
                    frame.dataArray.CopyTo(data);
                    strDate = String.Format("{0:X2}", (byte)data[5]) + "��" + String.Format("{0:X2}", (byte)(data[4] & 0x1f)) + "��" + String.Format("{0:X2}", (byte)data[3]) + "��";
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }

        }


        private bool test_guowang_clock()
        {
            Frame_Stu frame = new Frame_Stu();
            byte[] date = new byte[6];
            byte[] addr = new byte[4];

            if (GetAddress(ref addr, txtAddr.Text) == false)
            {
                return false;
            }

            string year = DateTime.Now.Year.ToString();
            while (true)
            {
                if (year.Length > 2)
                {
                    year = year.Remove(0, 1);
                }
                else
                {
                    break;
                }
            }

            date[5] = Convert.ToByte(year, 16);
            date[4] = Convert.ToByte(DateTime.Now.Month.ToString(), 16);
            byte month = Convert.ToByte(GetWeekDay(DateTime.Now.DayOfWeek.ToString()));
            month = (byte)(month << 5);
            date[4] |= month;
            month = date[4];
            date[3] = Convert.ToByte(DateTime.Now.Day.ToString(), 16);
            date[2] = Convert.ToByte(DateTime.Now.Hour.ToString(), 16);
            date[1] = Convert.ToByte(DateTime.Now.Minute.ToString(), 16);
            date[0] = Convert.ToByte(DateTime.Now.Second.ToString(), 16);

            byte[] pn = { 0x00, 0x00 };
            byte[] fn = { 0x40, 0x03 };
            if (m_areaId == AREA.AREA_CQ)
            {
                if (SendFrame(addr, (byte)GW_AFN_CODE.SetClock, true, GW_PW_LEN, pn, fn, date, ref frame, 15, 3) == REC_RESULT.OK)
                {
                    pn[0] = 0x00;
                    pn[1] = 0x00;
                    fn[0] = 0x02;
                    fn[1] = 0x00;
                    System.Threading.Thread.Sleep(1000);
                    FrameBuff.Clear();
                    if (SendFrame(addr, (byte)GW_AFN_CODE.GetClock, true, GW_PW_LEN, pn, fn, ref frame, 20, 3) == REC_RESULT.OK)
                    {
                        year = DateTime.Now.Year.ToString();
                        while (true)
                        {
                            if (year.Length > 2)
                            {
                                year = year.Remove(0, 1);
                            }
                            else
                            {
                                break;
                            }
                        }

                        if (/*((Byte)frame.dataArray[0] == Convert.ToByte(DateTime.Now.Second.ToString(), 16))
                            &&((Byte)frame.dataArray[1] == Convert.ToByte(DateTime.Now.Minute.ToString(), 16))
                            &&*/
                                ((Byte)frame.dataArray[2] == Convert.ToByte(DateTime.Now.Hour.ToString(), 16))
                            && ((Byte)frame.dataArray[3] == Convert.ToByte(DateTime.Now.Day.ToString(), 16))
                            && ((Byte)frame.dataArray[4] == month)
                            && ((Byte)frame.dataArray[5] == Convert.ToByte(year, 16))
                            )
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        return false;
                    }


                }
                else
                {
                    return false;
                }
            }
            else if (m_areaId == AREA.AREA_GX)
            {
                if (SendFrame(addr, (byte)GX_AFN_CODE.SetClock, true, GX_PW_LEN, pn, fn, date, ref frame, 15, 3) == REC_RESULT.OK)
                {
                    pn[0] = 0x00;
                    pn[1] = 0x00;
                    fn[0] = 0x40;
                    fn[1] = 0x03;
                    System.Threading.Thread.Sleep(1000);
                    FrameBuff.Clear();
                    if (SendFrame(addr, (byte)GX_AFN_CODE.GetClock, false, GX_PW_LEN, pn, fn, ref frame, 20, 3) == REC_RESULT.OK)
                    {
                        year = DateTime.Now.Year.ToString();
                        while (true)
                        {
                            if (year.Length > 2)
                            {
                                year = year.Remove(0, 1);
                            }
                            else
                            {
                                break;
                            }
                        }

                        if (/*((Byte)frame.dataArray[0] == Convert.ToByte(DateTime.Now.Second.ToString(), 16))
                            &&((Byte)frame.dataArray[1] == Convert.ToByte(DateTime.Now.Minute.ToString(), 16))
                            &&*/
                                ((Byte)frame.dataArray[2] == Convert.ToByte(DateTime.Now.Hour.ToString(), 16))
                            && ((Byte)frame.dataArray[3] == Convert.ToByte(DateTime.Now.Day.ToString(), 16))
                            && ((Byte)frame.dataArray[4] == month)
                            && ((Byte)frame.dataArray[5] == Convert.ToByte(year, 16))
                            )
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        return false;
                    }


                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }

        }

        private bool test_guangdong_clock()
        {
            Frame_Stu frame = new Frame_Stu();

            byte[] data = new byte[14];
            byte[] addr = new byte[4];

            if (GetGDAddress(ref addr, txtAddr.Text) == false)
            {
                return false;
            }

            string year = DateTime.Now.Year.ToString();
            while (true)
            {
                if (year.Length > 2)
                {
                    year = year.Remove(0, 1);
                }
                else
                {
                    break;
                }
            }

            //������
            data[0] = 0x00;
            data[1] = 0x00;
            //Ȩ������
            data[2] = 0x11;
            data[3] = 0x11;
            data[4] = 0x11;
            data[5] = 0x11;
            //������8030
            data[6] = 0x30;
            data[7] = 0x80;
            //���ʱ������
            data[8] = Convert.ToByte(DateTime.Now.Second.ToString(), 16);
            data[9] = Convert.ToByte(DateTime.Now.Minute.ToString(), 16);
            data[10] = Convert.ToByte(DateTime.Now.Hour.ToString(), 16);
            data[11] = Convert.ToByte(DateTime.Now.Day.ToString(), 16);

            data[12] = Convert.ToByte(DateTime.Now.Month.ToString(), 16);
            byte month = data[12];

            data[13] = Convert.ToByte(year, 16);

            if (SendFrame(addr, (byte)GD_AFN_CODE.SetRealtimeParam, data, ref frame, 15, 3) == REC_RESULT.OK)
            {

                System.Threading.Thread.Sleep(1000);
                FrameBuff.Clear();
                byte[] dataArry = new byte[10];
                dataArry.Initialize();
                // ������
                dataArry[0] = 0x01;
                // ������
                dataArry[8] = 0x30;
                dataArry[9] = 0x80;

                if (SendFrame(addr, (byte)GD_AFN_CODE.GetRealtimeParam, dataArry, ref frame, 20, 3) == REC_RESULT.OK)
                {
                    year = DateTime.Now.Year.ToString();
                    while (true)
                    {
                        if (year.Length > 2)
                        {
                            year = year.Remove(0, 1);
                        }
                        else
                        {
                            break;
                        }
                    }

                    if (/*((Byte)frame.dataArray[0] == Convert.ToByte(DateTime.Now.Second.ToString(), 16))
                        &&((Byte)frame.dataArray[1] == Convert.ToByte(DateTime.Now.Minute.ToString(), 16))
                        &&*/
                            ((Byte)frame.dataArray[12] == Convert.ToByte(DateTime.Now.Hour.ToString(), 16))
                        && ((Byte)frame.dataArray[13] == Convert.ToByte(DateTime.Now.Day.ToString(), 16))
                        && ((Byte)frame.dataArray[14] == month)
                        && ((Byte)frame.dataArray[15] == Convert.ToByte(year, 16))
                        )
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        private bool test_clock()
        {
            if (m_areaId == AREA.AREA_GD)
            {
                return test_guangdong_clock();
            }
            else
            {
                return test_guowang_clock();
            }
        }

        private bool test_GW_bat(IPPORT_SET portset)
        {
            Frame_Stu frame = new Frame_Stu();
            byte[] date = new byte[6];
            byte[] addr = new byte[4];

            if (GetAddress(ref addr, txtAddr.Text) == false)
            {
                return false;
            }

            string year = DateTime.Now.Year.ToString();
            while (true)
            {
                if (year.Length > 2)
                {
                    year = year.Remove(0, 1);
                }
                else
                {
                    break;
                }
            }

            date[5] = Convert.ToByte(year, 16);
            date[4] = Convert.ToByte(DateTime.Now.Month.ToString(), 16);
            byte month = Convert.ToByte(GetWeekDay(DateTime.Now.DayOfWeek.ToString()));
            month = (byte)(month << 5);
            date[4] |= month;
            month = date[4];
            date[3] = Convert.ToByte(DateTime.Now.Day.ToString(), 16);
            date[2] = Convert.ToByte(DateTime.Now.Hour.ToString(), 16);
            date[1] = Convert.ToByte(DateTime.Now.Minute.ToString(), 16);
            date[0] = Convert.ToByte(DateTime.Now.Second.ToString(), 16);

            byte[] pn = { 0x00, 0x00 };
            byte[] fn = { 0x40, 0x03 };

            if (m_areaId == AREA.AREA_CQ)
            {
                if (SendFrame(addr, (byte)GW_AFN_CODE.SetClock, true, GW_PW_LEN, pn, fn, date, ref frame, 5, 3) == REC_RESULT.OK)
                {
                    pn[0] = 0x00;
                    pn[1] = 0x00;
                    fn[0] = 0x02;
                    fn[1] = 0x00;
                    MessageBox.Show("�뽫�������ϵ磬������������ϵ磬���������������к󣬵����ʾ��ȷ�ϡ���ť�������ԣ�");
                    System.Threading.Thread.Sleep(15000);
                    if (OpenNetPort(portset) == false)
                    {
                        MessageBox.Show("��������ʧ�ܣ��˳�����");
                        thread.Abort();
                        return false;
                    }

                    FrameBuff.Clear();
                    if (SendFrame(addr, (byte)GW_AFN_CODE.GetClock, true, GW_PW_LEN, pn, fn, ref frame, 20, 3) == REC_RESULT.OK)
                    {
                        year = DateTime.Now.Year.ToString();
                        while (true)
                        {
                            if (year.Length > 2)
                            {
                                year = year.Remove(0, 1);
                            }
                            else
                            {
                                break;
                            }
                        }

                        if (/*((Byte)frame.dataArray[0] == Convert.ToByte(DateTime.Now.Second.ToString(), 16))
                            &&((Byte)frame.dataArray[1] == Convert.ToByte(DateTime.Now.Minute.ToString(), 16))
                            &&*/
                                ((Byte)frame.dataArray[2] == Convert.ToByte(DateTime.Now.Hour.ToString(), 16))
                            && ((Byte)frame.dataArray[3] == Convert.ToByte(DateTime.Now.Day.ToString(), 16))
                            && ((Byte)frame.dataArray[4] == month)
                            && ((Byte)frame.dataArray[5] == Convert.ToByte(year, 16))
                            )
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        return false;
                    }
                    return true;

                }
                else
                {
                    return false;
                }
            }
            else if (m_areaId == AREA.AREA_GX)
            {
                if (SendFrame(addr, (byte)GX_AFN_CODE.SetClock, true, GX_PW_LEN, pn, fn, date, ref frame, 5, 3) == REC_RESULT.OK)
                {
                    pn[0] = 0x00;
                    pn[1] = 0x00;
                    fn[0] = 0x40;
                    fn[1] = 0x03;
                    MessageBox.Show("�뽫�������ϵ磬������������ϵ磬���������������к󣬵����ʾ��ȷ�ϡ���ť�������ԣ�");
                    System.Threading.Thread.Sleep(15000);
                    if (OpenNetPort(portset) == false)
                    {
                        MessageBox.Show("��������ʧ�ܣ��˳�����");
                        thread.Abort();
                        return false;
                    }

                    FrameBuff.Clear();
                    if (SendFrame(addr, (byte)GX_AFN_CODE.GetClock, true, GX_PW_LEN, pn, fn, ref frame, 20, 3) == REC_RESULT.OK)
                    {
                        year = DateTime.Now.Year.ToString();
                        while (true)
                        {
                            if (year.Length > 2)
                            {
                                year = year.Remove(0, 1);
                            }
                            else
                            {
                                break;
                            }
                        }

                        if (/*((Byte)frame.dataArray[0] == Convert.ToByte(DateTime.Now.Second.ToString(), 16))
                            &&((Byte)frame.dataArray[1] == Convert.ToByte(DateTime.Now.Minute.ToString(), 16))
                            &&*/
                                ((Byte)frame.dataArray[2] == Convert.ToByte(DateTime.Now.Hour.ToString(), 16))
                            && ((Byte)frame.dataArray[3] == Convert.ToByte(DateTime.Now.Day.ToString(), 16))
                            && ((Byte)frame.dataArray[4] == month)
                            && ((Byte)frame.dataArray[5] == Convert.ToByte(year, 16))
                            )
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        return false;
                    }
                    return true;

                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        private bool test_GD_bat(IPPORT_SET portset)
        {
            Frame_Stu frame = new Frame_Stu();

            byte[] data = new byte[14];
            byte[] addr = new byte[4];

            if (GetGDAddress(ref addr, txtAddr.Text) == false)
            {
                return false;
            }

            string year = DateTime.Now.Year.ToString();
            while (true)
            {
                if (year.Length > 2)
                {
                    year = year.Remove(0, 1);
                }
                else
                {
                    break;
                }
            }

            //������
            data[0] = 0x00;
            data[1] = 0x00;
            //Ȩ������
            data[2] = 0x11;
            data[3] = 0x11;
            data[4] = 0x11;
            data[5] = 0x11;
            //������8030
            data[6] = 0x30;
            data[7] = 0x80;
            //���ʱ������
            data[8] = Convert.ToByte(DateTime.Now.Second.ToString(), 16);
            data[9] = Convert.ToByte(DateTime.Now.Minute.ToString(), 16);
            data[10] = Convert.ToByte(DateTime.Now.Hour.ToString(), 16);
            data[11] = Convert.ToByte(DateTime.Now.Day.ToString(), 16);

            data[12] = Convert.ToByte(DateTime.Now.Month.ToString(), 16);
            byte month = data[12];

            data[13] = Convert.ToByte(year, 16);

            if (SendFrame(addr, (byte)GD_AFN_CODE.SetRealtimeParam, data, ref frame, 5, 3) == REC_RESULT.OK)
            {
                MessageBox.Show("�뽫�������ϵ磬������������ϵ磬���������������к󣬵����ʾ��ȷ�ϡ���ť�������ԣ�");
                System.Threading.Thread.Sleep(15000);
                if (OpenNetPort(portset) == false)
                {
                    MessageBox.Show("��������ʧ�ܣ��˳�����");
                    thread.Abort();
                    return false;
                }
                System.Threading.Thread.Sleep(1000);
                FrameBuff.Clear();
                byte[] dataArry = new byte[10];
                dataArry.Initialize();
                // ������
                dataArry[0] = 0x01;
                // ������
                dataArry[8] = 0x30;
                dataArry[9] = 0x80;

                if (SendFrame(addr, (byte)GD_AFN_CODE.GetRealtimeParam, dataArry, ref frame, 20, 3) == REC_RESULT.OK)
                {
                    year = DateTime.Now.Year.ToString();
                    while (true)
                    {
                        if (year.Length > 2)
                        {
                            year = year.Remove(0, 1);
                        }
                        else
                        {
                            break;
                        }
                    }

                    if (/*((Byte)frame.dataArray[0] == Convert.ToByte(DateTime.Now.Second.ToString(), 16))
                        &&((Byte)frame.dataArray[1] == Convert.ToByte(DateTime.Now.Minute.ToString(), 16))
                        &&*/
                            ((Byte)frame.dataArray[12] == Convert.ToByte(DateTime.Now.Hour.ToString(), 16))
                        && ((Byte)frame.dataArray[13] == Convert.ToByte(DateTime.Now.Day.ToString(), 16))
                        && ((Byte)frame.dataArray[14] == month)
                        && ((Byte)frame.dataArray[15] == Convert.ToByte(year, 16))
                        )
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        private bool test_bat(IPPORT_SET portset)
        {
            if (m_areaId == AREA.AREA_GD)
            {
                return test_GD_bat(portset);
            }
            else
            {
                return test_GW_bat(portset);
            }
        }

        private bool test_GW_4851(string meterAddr, ref string getData)
        {
            Frame_Stu frame = new Frame_Stu();
            byte[] date = new byte[6];
            byte[] addr = new byte[4];

            if (GetAddress(ref addr, txtAddr.Text) == false)
            {
                return false;
            }

            if (m_areaId == AREA.AREA_CQ)
            {
                byte[] pn = { 0x01, 0x01 };
                byte[] fn = { 0x01, 0x10 };

                if (SendFrame(addr, (byte)GW_AFN_CODE.GetRelData, true, GW_PW_LEN, pn, fn, ref frame, 15, 3) == REC_RESULT.OK)
                {
                    byte[] data = new byte[frame.dataArray.Count];
                    frame.dataArray.CopyTo(data);
                    if (data[6] == 0xee && data[7] == 0xee && data[8] == 0xee && data[9] == 0xee && data[10] == 0xee)
                    {
                        return false;
                    }
                    getData = String.Format("{0:X2}", (byte)data[4]) + "��" + String.Format("{0:X2}", (byte)data[3]) + "��" + String.Format("{0:X2}", (byte)data[2]) + "��" + String.Format("{0:X2}", (byte)data[1]) + "ʱ" + String.Format("{0:X2}", (byte)data[1]) + "��";
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if (m_areaId == AREA.AREA_GX)
            {
                byte[] pn = { 0x00, 0x00 };
                byte[] fn = { 0x01, 0x07 };
                byte[] sdata = new byte[5];
                sdata[0] = 0x00;//������ʽ

                sdata[1] = 0x01;//������
                sdata[2] = 0x00;

                sdata[3] = 0x01;//�������
                sdata[4] = 0x00;
                if (SendFrame(addr, (byte)GX_AFN_CODE.GetRelData, true, GX_PW_LEN, pn, fn, sdata, ref frame, 15, 3) == REC_RESULT.OK)
                {
                    byte[] data = new byte[frame.dataArray.Count];
                    frame.dataArray.CopyTo(data);
                    if (data[0] == 0x00 && data[1] == 0x00)
                    {
                        return false;
                    }
                    getData = String.Format("{0:X2}", (byte)data[7]) + String.Format("{0:X2}", (byte)data[6]) + String.Format("{0:X2}", (byte)data[5]) + "." + String.Format("{0:X2}", (byte)data[4]) + "KW";
                    return true;
                }
                else
                {
                    return false;
                }

            }
            else
            {
                return false;
            }
        }
        private bool test_GD_485(string meterAddr, int tn, ref string getData)
        {
            Frame_Stu frame = new Frame_Stu();
            byte[] data = new byte[35];
            byte[] addr = new byte[4];

            if (GetGDAddress(ref addr, txtAddr.Text) == false)
            {
                return false;
            }

            //���ַ
            GetGdMeterAddr(meterAddr, ref data);
            //TN��
            data[6] = (byte)(tn & 0x0ff);
            data[7] = (byte)0x00; //data[7] = (byte)((tn >> 8) & 0xff);
            // �Զ��м�
            data[8] = 0xff;
            // �м̱��
            for (int i = 0; i < 24; i++)
            {
                data[9 + i] = 0x99;
            }
            data[33] = 0x10;
            data[34] = 0x90;

            if (SendFrame(addr, (byte)GD_AFN_CODE.ReadRealTimeData, data, ref frame, 30, 3) == REC_RESULT.OK)
            {
                byte[] rcdata = new byte[frame.dataArray.Count];
                frame.dataArray.CopyTo(rcdata);
                if (rcdata[6] == 0xee && rcdata[7] == 0xee && rcdata[8] == 0xee && rcdata[9] == 0xee && rcdata[10] == 0xee)
                {
                    return false;
                }
                getData = String.Format("{0:X2}", (byte)rcdata[9]) + "." + String.Format("{0:X2}", (byte)rcdata[8]) + "KW";
                return true;
            }
            else
            {
                return false;
            }
        }
        private bool test_4851(string meterAddr, ref string getData)
        {
            if (m_areaId == AREA.AREA_CQ || m_areaId == AREA.AREA_GX)
            {
                return test_GW_4851(meterAddr, ref getData);
            }
            else if (m_areaId == AREA.AREA_GD)
            {
                return test_GD_485(meterAddr, 1, ref getData);
            }
            else
            {
                return false;
            }

        }

        private bool test_GW_4852(string meterAddr, ref string getData)
        {
            Frame_Stu frame = new Frame_Stu();
            byte[] date = new byte[6];
            byte[] addr = new byte[4];

            if (GetAddress(ref addr, txtAddr.Text) == false)
            {
                return false;
            }

            if (m_areaId == AREA.AREA_CQ)
            {
                byte[] pn = { 0x02, 0x01 };
                byte[] fn = { 0x01, 0x10 };

                if (SendFrame(addr, (byte)GW_AFN_CODE.GetRelData, true, GW_PW_LEN, pn, fn, ref frame, 15, 3) == REC_RESULT.OK)
                {
                    byte[] data = new byte[frame.dataArray.Count];
                    frame.dataArray.CopyTo(data);
                    if (data[0] == 0x00 && data[1] == 0x00)
                    {
                        return false;
                    }
                    getData = String.Format("{0:X2}", (byte)data[7]) + String.Format("{0:X2}", (byte)data[6]) + String.Format("{0:X2}", (byte)data[5]) + "." + String.Format("{0:X2}", (byte)data[4]) + "KW";
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if (m_areaId == AREA.AREA_GX)
            {
                byte[] pn = { 0x00, 0x00 };
                byte[] fn = { 0x01, 0x07 };
                byte[] sdata = new byte[5];
                sdata[0] = 0x00;//������ʽ

                sdata[1] = 0x01;//������
                sdata[2] = 0x00;

                sdata[3] = 0x02;//�������
                sdata[4] = 0x00;
                if (SendFrame(addr, (byte)GX_AFN_CODE.GetRelData, true, GX_PW_LEN, pn, fn, sdata, ref frame, 15, 3) == REC_RESULT.OK)
                {
                    byte[] data = new byte[frame.dataArray.Count];
                    frame.dataArray.CopyTo(data);
                    if (data[0] == 0x00 && data[1] == 0x00)
                    {
                        return false;
                    }
                    getData = String.Format("{0:X2}", (byte)data[7]) + String.Format("{0:X2}", (byte)data[6]) + String.Format("{0:X2}", (byte)data[5]) + "." + String.Format("{0:X2}", (byte)data[4]) + "KW";
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        private bool test_GW_4853(string meterAddr, ref string getData)
        {
            Frame_Stu frame = new Frame_Stu();
            byte[] date = new byte[6];
            byte[] addr = new byte[4];

            if (GetAddress(ref addr, txtAddr.Text) == false)
            {
                return false;
            }

            if (m_areaId == AREA.AREA_CQ)
            {
                byte[] pn = { 0x08, 0x01 };
                byte[] fn = { 0x01, 0x10 };

                if (SendFrame(addr, (byte)GW_AFN_CODE.GetRelData, true, GW_PW_LEN, pn, fn, ref frame, 15, 3) == REC_RESULT.OK)
                {
                    byte[] data = new byte[frame.dataArray.Count];
                    frame.dataArray.CopyTo(data);
                    if (data[0] == 0x00 && data[1] == 0x00)
                    {
                        return false;
                    }
                    getData = String.Format("{0:X2}", (byte)data[7]) + String.Format("{0:X2}", (byte)data[6]) + String.Format("{0:X2}", (byte)data[5]) + "." + String.Format("{0:X2}", (byte)data[4]) + "KW";
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if (m_areaId == AREA.AREA_GX)
            {
                byte[] pn = { 0x00, 0x00 };
                byte[] fn = { 0x01, 0x07 };
                byte[] sdata = new byte[5];
                sdata[0] = 0x00;//������ʽ

                sdata[1] = 0x01;//������
                sdata[2] = 0x00;

                sdata[3] = 0x02;//�������
                sdata[4] = 0x00;
                if (SendFrame(addr, (byte)GX_AFN_CODE.GetRelData, true, GX_PW_LEN, pn, fn, sdata, ref frame, 15, 3) == REC_RESULT.OK)
                {
                    byte[] data = new byte[frame.dataArray.Count];
                    frame.dataArray.CopyTo(data);
                    if (data[0] == 0x00 && data[1] == 0x00)
                    {
                        return false;
                    }
                    getData = String.Format("{0:X2}", (byte)data[7]) + String.Format("{0:X2}", (byte)data[6]) + String.Format("{0:X2}", (byte)data[5]) + "." + String.Format("{0:X2}", (byte)data[4]) + "KW";
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        private bool test_4852(string meterAddr, ref string getData)
        {
            if (m_areaId == AREA.AREA_CQ || m_areaId == AREA.AREA_GX)
            {
                return test_GW_4852(meterAddr, ref getData);
            }
            else if (m_areaId == AREA.AREA_GD)
            {
                return test_GD_485(meterAddr, 2, ref getData);
            }
            else
            {
                return false;
            }

        }

        private bool test_4853(string meterAddr, ref string getData)
        {
            if (m_areaId == AREA.AREA_CQ || m_areaId == AREA.AREA_GX)
            {
                return test_GW_4853(meterAddr, ref getData);
            }
            else if (m_areaId == AREA.AREA_GD)
            {
                return test_GD_485(meterAddr, 4, ref getData);
            }
            else
            {
                return false;
            }
        }

        private bool test_GW_down(string meterAddr, ref string getData)
        {
            Frame_Stu frame = new Frame_Stu();
            byte[] date = new byte[6];
            byte[] addr = new byte[4];

            if (GetAddress(ref addr, txtAddr.Text) == false)
            {
                return false;
            }

            if (m_areaId == AREA.AREA_CQ)
            {
                byte[] pn = { 0x04, 0x01 };
                byte[] fn = { 0x01, 0x10 };
                if (SendFrame(addr, (byte)GW_AFN_CODE.GetRelData, true, GW_PW_LEN, pn, fn, ref frame, 15, 3) == REC_RESULT.OK)
                {
                    byte[] data = new byte[frame.dataArray.Count];
                    frame.dataArray.CopyTo(data);
                    if (data[6] == 0xee && data[7] == 0xee && data[8] == 0xee && data[9] == 0xee && data[10] == 0xee)
                    {
                        return false;
                    }
                    getData = String.Format("{0:X2}", (byte)data[4]) + "��" + String.Format("{0:X2}", (byte)data[3]) + "��" + String.Format("{0:X2}", (byte)data[2]) + "��" + String.Format("{0:X2}", (byte)data[1]) + "ʱ" + String.Format("{0:X2}", (byte)data[1]) + "��";
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if (m_areaId == AREA.AREA_GX)
            {
                byte[] pn = { 0x00, 0x00 };
                byte[] fn = { 0x01, 0x07 };
                byte[] sdata = new byte[5];
                sdata[0] = 0x00;//������ʽ

                sdata[1] = 0x01;//������
                sdata[2] = 0x00;

                sdata[3] = 0x03;//�������
                sdata[4] = 0x00;

                if (SendFrame(addr, (byte)GX_AFN_CODE.GetRelData, true, GX_PW_LEN, pn, fn, sdata, ref frame, 15, 3) == REC_RESULT.OK)
                {
                    byte[] data = new byte[frame.dataArray.Count];
                    frame.dataArray.CopyTo(data);
                    if (data[0] == 0x00 && data[1] == 0x00)
                    {
                        return false;
                    }
                    getData = String.Format("{0:X2}", (byte)data[7]) + String.Format("{0:X2}", (byte)data[6]) + String.Format("{0:X2}", (byte)data[5]) + "." + String.Format("{0:X2}", (byte)data[4]) + "KW";
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        private bool test_down(string meterAddr, ref string getData)
        {
            if (m_areaId == AREA.AREA_CQ || m_areaId == AREA.AREA_GX)
            {
                return test_GW_down(meterAddr, ref getData);
            }
            else if (m_areaId == AREA.AREA_GD)
            {
                return test_GD_485(meterAddr, 3, ref getData);
            }
            else
            {
                return false;
            }
        }

        private bool test_redwire(ref string strDate)
        {
            /*            Frame_Stu frame = new Frame_Stu();
                        byte[] date = new byte[6];
                        byte[] addr = new byte[4];
                        byte[] pn = { 0x00, 0x00 };
                        byte[] fn = { 0x02, 0x00 };

                        if (GetAddress(ref addr, txtAddr.Text) == false)
                        {
                            return false;
                        }

                        FrameBuff.Clear();
                        if (SendFrame(addr, (byte)GW_AFN_CODE.GetClock, true, GW_PW_LEN, pn, fn, ref frame, 20, 3) == REC_RESULT.OK)
                        {

                            byte[] data = new byte[frame.dataArray.Count];
                            frame.dataArray.CopyTo(data);
                            strDate = String.Format("{0:X2}", (byte)data[2]) + "ʱ" + String.Format("{0:X2}", (byte)data[1]) + "��" + String.Format("{0:X2}", (byte)data[0]) + "��";
                            return true;
                        }
                        else
                        {
                            return false;
                        }*/
            if (m_areaId == AREA.AREA_CQ || m_areaId == AREA.AREA_GX)
            {
                return Get_guowang_clock(ref strDate);
            }
            else
            {
                // �㶫������� Ŀǰȱʡ
                return false;
            }
        }

        private bool bTestWatchDog = false;
        private bool test_watchdog()
        {
            string resetCmd1 = "root\r";
            string resetCmd2 = "ptu@keli\r";
            string resetCmd3 = "killall dyjc dyjcd\r";
            string returnKeyword = "52 6F 6D 42 4F 4F 54";//RomBOOT

            //char[] cmd1 = new char[resetCmd1.Length];
            //byte[] dcmd1 = new byte[resetCmd1.Length];
            //cmd1 = resetCmd1.ToCharArray();
            //for (int i = 0; i < cmd1.Length; i++)
            //{
            //    dcmd1[i] = Convert.ToByte(cmd1[i]);
            //}
            //sendDatas(dcmd1, dcmd1.Length);
            //System.Threading.Thread.Sleep(1000);

            //char[] cmd2 = new char[resetCmd2.Length];
            //byte[] dcmd2 = new byte[resetCmd2.Length];
            //cmd2 = resetCmd2.ToCharArray();
            //for (int i = 0; i < cmd2.Length; i++)
            //{
            //    dcmd2[i] = Convert.ToByte(cmd2[i]);
            //}
            //sendDatas(dcmd2, dcmd2.Length);
            //System.Threading.Thread.Sleep(1000);

            //char[] cmd3 = new char[resetCmd3.Length];
            //byte[] dcmd3 = new byte[resetCmd3.Length];
            //cmd3 = resetCmd3.ToCharArray();
            //for (int i = 0; i < cmd3.Length; i++)
            //{
            //    dcmd3[i] = Convert.ToByte(cmd3[i]);
            //}
            //sendDatas(dcmd3, dcmd3.Length);
            telnetReboot();
            bTestWatchDog = true; //��ʼ���������ϱ�����
            information.textBox1.Text = "";
            System.Threading.Thread.Sleep(15000);


            for (int j = 0; j < 300; j++)
            {
                if (information.textBox1.Text.ToString().Contains(returnKeyword) == true)
                {

                    bTestWatchDog = false; //ֹͣ���������ϱ�����
                    return true;
                }
                System.Threading.Thread.Sleep(100);
            }
            bTestWatchDog = false; //ֹͣ���������ϱ�����
            return false;
        }

        private void Add33(ref byte[] data)
        {
            for (int i = 0; i < data.Length; i++)
            {
                data[i] += 0x33;
            }
        }
        private void Sub33(ref byte[] data)
        {
            for (int i = 0; i < data.Length; i++)
            {
                data[i] -= 0x33;
            }
        }


        private void Stop_Click(object sender, EventArgs e)
        {
            MessageBox.Show("����ֹͣ��");
            thread.Abort();
        }

        private string GetTester()
        {
            // ����Application����
            Missing Miss = Missing.Value;
            int i = 2, j = 0;

            // ����Application����
            Excel.Application xlsApp = new Excel.Application();
            if (xlsApp == null)
            {
                return "";
            }

            if (xlsApp == null)
            {
                return "";
            }

            /* ���ļ� */
            Excel.Workbook xlsBook = xlsApp.Workbooks.Open(testerFilePath, Miss, Miss, Miss, Miss, Miss, Miss, Miss, Miss, Miss, Miss, Miss, Miss, Miss, Miss);
            Excel.Worksheet xlsSheet = (Excel.Worksheet)xlsBook.Sheets[1];

            string name = (string)(((Excel.Range)xlsSheet.Cells[2, 1]).Value2);
            // �ر�XLS�ļ�
            xlsBook.Close(false, Type.Missing, Type.Missing);
            xlsApp.Quit();

            // ����������ر�excel.exe
            IntPtr t = new IntPtr(xlsApp.Hwnd);
            int k = 0;
            GetWindowThreadProcessId(t, out   k);
            System.Diagnostics.Process p = System.Diagnostics.Process.GetProcessById(k);
            p.Kill();
            return name;
        }

        private void saveResult_Click(object sender, EventArgs e)
        {

        }

        private void btnInfo_Click(object sender, EventArgs e)
        {
            information.Visible = false;
            information.Focus();
            information.ShowDialog();
        }

        private void button2_Click(object sender, EventArgs e)
        {

        }


        Int16 GetDT(Int16 fn)
        {
            Int16 dt = 0;
            if (fn != 0)
            {
                fn -= 1;
                dt = (Int16)((((fn / 8) << 8) & 0xFF00) | ((1 << (fn % 8)) & 0x00FF));
            }
            return dt;
        }

        private bool GetSoftwareVersion(ref string version)
        {
            Frame_Stu frame = new Frame_Stu();
            byte[] addr = new byte[4];

            Int16 fcn = GetDT(13);

            byte[] fn = new byte[2];
            fn[0] = (byte)(fcn & 0xff);
            fn[1] = (byte)((fcn >> 8) & 0xff);
            byte[] pn = { 0x00, 0x00 };

            IPPORT_SET portSet = new IPPORT_SET();
            PORT_SET serialPortSet = new PORT_SET();

            if (schemeConfigFrm.chkNet.Checked == true)
            {
                GetNetPort(ref portSet);
                if (bNetOpenFlag == false)
                {
                    if (OpenNetPort(portSet) == false)
                    {
                        MessageBox.Show("��������ʧ�ܣ��˳�����");
                        return false;
                    }

                }
            }
            else if (schemeConfigFrm.chkRS485.Checked == true)
            {
                GetSerialPort(ref serialPortSet);
                OpenSerialPort(serialPortSet);
            }
            else
            {
                return false;
            }

            if (GetAddress(ref addr, txtAddr.Text) == false)
            {
                return false;
            }
            if (SendFrame(addr, (byte)GW_AFN_CODE.SetDeviceNo, false, GW_PW_LEN, pn, fn, ref frame, 5, 3) == REC_RESULT.OK)
            {
                byte[] data = new byte[frame.dataArray.Count];
                frame.dataArray.CopyTo(data);
                version += 'V';
                for (int j = 0; j < data.Length; j++)
                {
                    if (data[j] == 0x00)
                    {
                        break;
                    }
                    version += (char)data[j];
                }

                //   getData = String.Format("{0:X2}", (byte)data[4]) + "��" + String.Format("{0:X2}", (byte)data[3]) + "��" + String.Format("{0:X2}", (byte)data[2]) + "��" + String.Format("{0:X2}", (byte)data[1]) + "ʱ" + String.Format("{0:X2}", (byte)data[1]) + "��";
                return true;
            }
            else
            {
                return false;
            }
        }

        private bool SetDevNo(string str)
        {
            Frame_Stu frame = new Frame_Stu();
            //IPPORT_SET portSet = new IPPORT_SET();
            byte[] date = new byte[6];
            byte[] addr = new byte[4];


            long value = Convert.ToInt32(str);
            if (value < 20100001 || value > 20109999)
            {
                MessageBox.Show("�������豸���Ϊ20100001 ~ 20109999");
                return false;
            }

            char[] cmd1 = new char[str.Length];
            byte[] dcmd1 = new byte[str.Length];
            cmd1 = str.ToCharArray();
            for (int i = 0; i < cmd1.Length; i++)
            {
                dcmd1[i] = Convert.ToByte(cmd1[i]);
            }

            if (GetAddress(ref addr, txtAddr.Text) == false)
            {
                return false;
            }

            Int16 fcn = GetDT(16);

            byte[] fn = new byte[2];
            fn[0] = (byte)(fcn & 0xff);
            fn[1] = (byte)((fcn >> 8) & 0xff);
            byte[] pn = { 0x00, 0x00 };

            IPPORT_SET portSet = new IPPORT_SET();
            PORT_SET serialPortSet = new PORT_SET();

            if (schemeConfigFrm.chkNet.Checked == true)
            {
                GetNetPort(ref portSet);
                if (bNetOpenFlag == false)
                {
                    if (OpenNetPort(portSet) == false)
                    {
                        MessageBox.Show("��������ʧ�ܣ��˳�����");
                        return false;
                    }

                }
            }
            else if (schemeConfigFrm.chkRS485.Checked == true)
            {
                GetSerialPort(ref serialPortSet); ;
                OpenSerialPort(serialPortSet);
            }
            else
            {
                return false;
            }

            if (SendFrame(addr, (byte)GW_AFN_CODE.SetDeviceNo, false, GW_PW_LEN, pn, fn, dcmd1, ref frame, 5, 3) == REC_RESULT.OK)
            {
                byte[] data = new byte[frame.dataArray.Count];
                frame.dataArray.CopyTo(data);

                //   getData = String.Format("{0:X2}", (byte)data[4]) + "��" + String.Format("{0:X2}", (byte)data[3]) + "��" + String.Format("{0:X2}", (byte)data[2]) + "��" + String.Format("{0:X2}", (byte)data[1]) + "ʱ" + String.Format("{0:X2}", (byte)data[1]) + "��";
                return true;
            }
            else
            {
                return false;
            }

        }

        private void btnRdSoftwareVersion_Click(object sender, EventArgs e)
        {

        }

        private void ABout_Click(object sender, EventArgs e)
        {
            CAbout aboutFrame = new CAbout();
            aboutFrame.Text = "���˵��";
            aboutFrame.labelProductName.Text = "������ƣ�DJS6000ϵ�м����������������";
            aboutFrame.labelVersion.Text = String.Format("�汾{0} ", " V5.0-2012-0925");
            aboutFrame.labelCopyright.Text = "�汾�鼯����Ʒ������";
            aboutFrame.labelCompanyName.Text = "�����Ϸ��������ſƼ���չ���޹�˾";
            aboutFrame.About.Text = "ʹ��˵�������DJS6000ϵ�������������ʹ��˵����.doc";
            aboutFrame.ShowDialog();
        }

        private void comArea_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            CTool toolfrm = new CTool();
            toolfrm.ShowDialog();
        }

        private void btnAddCJQ_Click(object sender, EventArgs e)
        {

        }

        private void btnClearUserDir_Click(object sender, EventArgs e)
        {

            return;
        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            paramConfig.Visible = false;
            paramConfig.Focus();
            paramConfig.ShowDialog();
        }
        public enum DEVICE_KIND
        {
            KIND_CONTROL = 1,
            KIND_DISPLAY
        }

        public enum DEVICE_TYPE
        {
            MD_RCU = 0,
            MD_AIRCONDITION = 1,
            MD_RELAY_8 = 2,
            MD_LIGHT_4 = 3,
            MD_RELAY_2 = 4,
            MD_LEDV12_3 = 5,
            MD_DOORDISPLAY = 6,
            MD_RFID_CARD = 7,
            MD_KEYBOARD_20 = 8,
            MD_RTF = 9,
            MD_LUXSENSOR = 10,
            MD_TYPE_MAX,
        }

        public enum T_emFuncType
        {
            FUNC_NORMAL = 0,
            FUNC_LIGHT_UP = 1,//����
            FUNC_LIGHT_DOWN = 2,//����
            FUNC_LIGHT_AUTO = 3,
            FUNC_LIGHT_AUTO_UP = 4,//�򿪼̵�������
            FUNC_LIGHT_AUTO_DOWN = 5,//�������ص��̵���
            FUNC_POWER_KEY = 6,
            FUNC_KEYBACKLIGHT = 7,
            FUNC_NIGHT_LIGHT = 8,
            FUNC_RELAY_ON = 9,//�̵�����
            FUNC_RELAY_OFF = 10,//�̵�����
            FUNC_WINDOW_OPEN = 11,//�����������
            FUNC_WINDOW_CLOSE = 12,//�����ص����
            FUNC_SINGLE_BRAKE = 13,//�յ����ܷ�
            FUNC_AIRCDI_DOUBLE_COLD_BRAKE = 14,//�յ�˫���䷧
            FUNC_AIRCDI_DOUBLE_HOT_BRAKE = 15,//�յ�˫���ȷ�
            FUNC_AIRCDI_WIND_SPEED = 16,//�յ�����ٶȿ���
            FUNC_AIRCDI_DIGTIAL_BACKLIGHT = 17,//�յ�����ܱ������
            FUNC_URGENT_KEY = 18,//������ť
            FUNC_DOOR_CHECK = 19,//�����Ŵż��
            FUNC_GALLERY_LIGHT = 20,//�ȵ�
            FUNC_KEY_CLEANROOM = 21,//������
            FUNC_KEY_DONOT_DISTURB = 22,//�������
            FUNC_KEY_BELL = 23,//����
            FUNC_INPUT_PORT_TRIGLE = 24,//����˵�ƽ���ϴ���
            FUNC_INPUT_PORT_HIGH_LEVEL = 25,//����˸ߵ�ƽ����
            FUNC_INPUT_PORT_LOW_LEVEL = 26,//����˵͵�ƽ����
            FUNC_LIGHT_LEVEL = 27,//����������
            FUNC_KEY_WASH_CLOSE = 28,//ϴ�·�
            FUNC_SCENE_COMPOSE1 = 29,//���1�ƹ�����
            FUNC_SCENE_COMPOSE2 = 30,//���2�ƹ�����
            FUNC_SCENE_COMPOSE3 = 31,//���3�ƹ�����
            FUNC_SCENE_COMPOSE4 = 32,//���4�ƹ�����
            FUNC_SCENE_COMPOSE5 = 33,//���5�ƹ�����
            FUNC_SCENE_COMPOSE6 = 34,//���6�ƹ�����
            FUNC_SCENE_COMPOSE7 = 35,//���7�ƹ�����
            FUNC_LEFT_BED_LIGHT = 36,//��ͷ��
            FUNC_RIGHT_BED_LIGHT = 37,//�Ҵ�ͷ��
            FUNC_MAX
        }
        private UInt32 GetSubMapValue(string strText)
        {
            UInt32 mapdbs = 0;
            string[] str = strText.Split(',');
            for (int i = 0; i < str.Length; i++)
            {
                mapdbs |= (UInt32)(0x01 << (Convert.ToByte(str[i], 10) - 1));
            }
            return mapdbs;
        }
        private void btnParamterLoad_Click(object sender, EventArgs e)
        {
            Frame_Stu frame = new Frame_Stu();

            byte[] data = new byte[1024];
            byte[] addr = new byte[4];
            byte[] deviceAddr = new byte[6];
            IPPORT_SET portSet = new IPPORT_SET();
            PORT_SET serialPortSet = new PORT_SET();

            if (schemeConfigFrm.chkNet.Checked == true)
            {
                GetNetPort(ref portSet);
                if (bNetOpenFlag == false)
                {
                    if (OpenNetPort(portSet) == false)
                    {
                        MessageBox.Show("��������ʧ�ܣ��˳�����");
                        return;
                    }

                }
            }
            else if (schemeConfigFrm.chkRS485.Checked == true)
            {
                GetSerialPort(ref serialPortSet); ;
                OpenSerialPort(serialPortSet);
            }
            else
            {
                return;
            }

            if (GetGDAddress(ref addr, txtAddr.Text) == false)
            {
                return;
            }


            //������
            int pos = 0;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            //Ȩ������
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            //������895D
            data[pos++] = 0x5D;
            data[pos++] = 0x89;

            data[pos++] = 0x00;	//����

            int uncheckedmodulenum = 0;
            if (paramConfig.chkIORelaya.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr1a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr2a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr3a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr4a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr5a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr6a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr7a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr8a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr9a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr10a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr11a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr12a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr13a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkcpr14a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkcpr15a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkcpr16a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkcpr17a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkcpr18a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkcpr19a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkcpr20a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr21a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr22a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr23a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr24a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr25a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr26a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr27a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr28a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr29a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr30a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr31a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr32a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkcpr33a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkcpr34a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkcpr35a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkcpr36a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr37a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr38a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr39a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr40a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr41a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr42a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr43a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr44a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr45a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr46a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr47a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr48a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkcpr49a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkcpr50a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkcpr51a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkcpr52a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr53a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr54a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr55a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr56a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr57a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr58a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr59a.Checked == false)
            {
                uncheckedmodulenum++;
            }

            if (paramConfig.chkcpr60a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkcpr61a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkcpr62a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkcpr63a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkcpr64a.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkLuxSnr1.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkLuxSnr2.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkLuxSnr3.Checked == false)
            {
                uncheckedmodulenum++;
            }
            if (paramConfig.chkLuxSnr4.Checked == false)
            {
                uncheckedmodulenum++;
            }


            data[pos++] = (byte)(69 - uncheckedmodulenum);//ģ����
            UInt32 subno;

            /* CPR1ģ��*/
            if (paramConfig.chkcpr1a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn1a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo1a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x01;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR2ģ��*/
            if (paramConfig.chkcpr2a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn2a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo2a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x01;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR3ģ��*/
            if (paramConfig.chkcpr3a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn3a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo3a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x01;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR4ģ��*/
            if (paramConfig.chkcpr4a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn4a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo4a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x01;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR5ģ��*/
            if (paramConfig.chkcpr5a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn5a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo5a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x02;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR6ģ��*/
            if (paramConfig.chkcpr6a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn6a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo6a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x02;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR7ģ��*/
            if (paramConfig.chkcpr7a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn7a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo7a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x02;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR8ģ��*/
            if (paramConfig.chkcpr8a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn8a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo8a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x02;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR9ģ��*/
            if (paramConfig.chkcpr9a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn9a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo9a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x03;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR10ģ��*/
            if (paramConfig.chkcpr10a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn10a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo10a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x03;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR11ģ��*/
            if (paramConfig.chkcpr11a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn11a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo11a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x03;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR12ģ��*/
            if (paramConfig.chkcpr12a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn12a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo12a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x03;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR13ģ��*/
            if (paramConfig.chkcpr13a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn13a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo13a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x04;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR14ģ��*/
            if (paramConfig.chkcpr14a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn14a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo14a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x04;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR15ģ��*/
            if (paramConfig.chkcpr15a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn15a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo15a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x04;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR16ģ��*/
            if (paramConfig.chkcpr16a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn16a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo16a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x04;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }
            /* CPR17ģ��*/
            if (paramConfig.chkcpr17a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn17a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo17a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x05;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR18ģ��*/
            if (paramConfig.chkcpr18a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn18a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo18a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x05;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR19ģ��*/
            if (paramConfig.chkcpr19a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn19a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo19a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x05;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR20ģ��*/
            if (paramConfig.chkcpr20a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn20a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo20a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x05;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR21ģ��*/
            if (paramConfig.chkcpr21a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn21a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo21a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x06;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR22ģ��*/
            if (paramConfig.chkcpr22a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn22a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo22a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x06;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR23ģ��*/
            if (paramConfig.chkcpr23a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn23a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo23a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x06;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR24ģ��*/
            if (paramConfig.chkcpr24a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn24a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo24a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x06;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR25ģ��*/
            if (paramConfig.chkcpr25a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn25a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo25a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x07;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR26ģ��*/
            if (paramConfig.chkcpr26a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn26a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo26a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x07;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR27ģ��*/
            if (paramConfig.chkcpr27a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn27a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo27a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x07;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR28ģ��*/
            if (paramConfig.chkcpr28a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn28a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo28a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x07;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR29ģ��*/
            if (paramConfig.chkcpr29a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn29a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo29a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x08;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR30ģ��*/
            if (paramConfig.chkcpr30a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn30a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo30a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x08;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR31ģ��*/
            if (paramConfig.chkcpr31a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn31a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo31a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x08;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR32ģ��*/
            if (paramConfig.chkcpr32a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn32a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo32a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x08;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }
            /* CPR33ģ��*/
            if (paramConfig.chkcpr33a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn33a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo33a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x09;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR34ģ��*/
            if (paramConfig.chkcpr34a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn34a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo34a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x09;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR35ģ��*/
            if (paramConfig.chkcpr35a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn35a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo35a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x09;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR36ģ��*/
            if (paramConfig.chkcpr36a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn36a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo36a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x09;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR37ģ��*/
            if (paramConfig.chkcpr37a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn37a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo37a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x10;//��·��,BCD
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR38ģ��*/
            if (paramConfig.chkcpr38a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn38a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo38a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x10;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR39ģ��*/
            if (paramConfig.chkcpr39a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn39a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo39a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x10;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR40ģ��*/
            if (paramConfig.chkcpr40a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn40a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo40a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x10;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR41ģ��*/
            if (paramConfig.chkcpr41a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn41a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo41a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x11;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR42ģ��*/
            if (paramConfig.chkcpr42a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn42a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo42a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x11;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR43ģ��*/
            if (paramConfig.chkcpr43a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn43a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo43a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x11;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR44ģ��*/
            if (paramConfig.chkcpr44a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn44a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo44a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x11;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR45ģ��*/
            if (paramConfig.chkcpr45a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn45a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo45a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x12;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR46ģ��*/
            if (paramConfig.chkcpr46a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn46a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo46a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x12;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR47ģ��*/
            if (paramConfig.chkcpr47a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn47a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo47a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x12;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR48ģ��*/
            if (paramConfig.chkcpr48a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn48a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo48a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x12;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }
            /* CPR49ģ��*/
            if (paramConfig.chkcpr49a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn49a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo49a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x13;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR50ģ��*/
            if (paramConfig.chkcpr50a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn50a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo50a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x13;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR51ģ��*/
            if (paramConfig.chkcpr51a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn51a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo51a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x13;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR52ģ��*/
            if (paramConfig.chkcpr52a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn52a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo52a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x13;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR53ģ��*/
            if (paramConfig.chkcpr53a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn53a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo53a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x14;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR54ģ��*/
            if (paramConfig.chkcpr54a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn54a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo54a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x14;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR55ģ��*/
            if (paramConfig.chkcpr55a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn55a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo55a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x14;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR56ģ��*/
            if (paramConfig.chkcpr56a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn56a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo56a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x14;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR57ģ��*/
            if (paramConfig.chkcpr57a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn57a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo57a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x15;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR58ģ��*/
            if (paramConfig.chkcpr58a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn58a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo58a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x15;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR59ģ��*/
            if (paramConfig.chkcpr59a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn59a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo59a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x15;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR60ģ��*/
            if (paramConfig.chkcpr60a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn60a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo60a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x15;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR61ģ��*/
            if (paramConfig.chkcpr61a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn61a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo61a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x16;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR62ģ��*/
            if (paramConfig.chkcpr62a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn62a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo62a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x16;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR63ģ��*/
            if (paramConfig.chkcpr63a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn63a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo63a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x16;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }

            /* CPR64ģ��*/
            if (paramConfig.chkcpr64a.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtTn64a.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtDevNo64a.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x16;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RTF;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }
            /* 12·����8·���ģ��*/
            if (paramConfig.chkIORelaya.Checked == true)
            {
                byte deviceno = Convert.ToByte(paramConfig.txt8RNoa.Text.ToString(), 10);
                data[pos++] = deviceno;	//�豸���	
                GetGdMeterAddr(paramConfig.txt8RelayAddra.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] =  Convert.ToByte(paramConfig.txtRelayClNoa.Text.ToString(), 16);//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_RELAY_8;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x0;//�����豸��
            }

            /* �����1*/
            if (paramConfig.chkLuxSnr1.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtLuxSnr1.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtLuxSnrAdr1.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x00;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_LUXSENSOR;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }
            /* �����2*/
            if (paramConfig.chkLuxSnr2.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtLuxSnr2.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtLuxSnrAdr2.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x00;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_LUXSENSOR;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }
            /* �����3*/
            if (paramConfig.chkLuxSnr3.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtLuxSnr3.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtLuxSnrAdr3.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x00;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_LUXSENSOR;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }
            /* �����4*/
            if (paramConfig.chkLuxSnr4.Checked != false)
            {
                data[pos++] = Convert.ToByte(paramConfig.txtLuxSnr4.Text.ToString(), 10);	//�豸���	
                GetGdMeterAddr(paramConfig.txtLuxSnrAdr4.Text.ToString(), ref deviceAddr);
                for (int i = 0; i < 6; i++)
                {
                    data[pos++] = deviceAddr[i];
                }
                data[pos++] = 0x00;//��·��
                data[pos++] = (byte)DEVICE_KIND.KIND_CONTROL;//��������
                data[pos++] = (byte)DEVICE_TYPE.MD_LUXSENSOR;	//�豸����
                data[pos++] = 0x04;//������		
                data[pos++] = 0x00;//�����豸��
            }
            if (SendFrame(addr, (byte)GD_AFN_CODE.SetRealtimeParam, data, pos, ref frame, 15, 3) == REC_RESULT.OK)
            {
                byte[] rcdata = new byte[frame.dataArray.Count];
                frame.dataArray.CopyTo(rcdata);
                if (rcdata[2] == 0x5D && rcdata[3] == 0x89 && rcdata[4] == 0x00)
                {
                    MessageBox.Show("�����ɹ�");
                }
                else
                {
                    MessageBox.Show("����ʧ��");
                }
            }
            else
            {
                MessageBox.Show("����ʧ��");
            }
            ClosePort();
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            Frame_Stu frame = new Frame_Stu();

            byte[] data = new byte[400];
            byte[] addr = new byte[4];
            byte[] deviceAddr = new byte[6];
            IPPORT_SET portSet = new IPPORT_SET();
            PORT_SET serialPortSet = new PORT_SET();

            if (schemeConfigFrm.chkNet.Checked == true)
            {
                GetNetPort(ref portSet);
                if (bNetOpenFlag == false)
                {
                    if (OpenNetPort(portSet) == false)
                    {
                        MessageBox.Show("��������ʧ�ܣ��˳�����");
                        return;
                    }

                }
            }
            else if (schemeConfigFrm.chkRS485.Checked == true)
            {
                GetSerialPort(ref serialPortSet); ;
                OpenSerialPort(serialPortSet);
            }
            else
            {
                return;
            }


            if (GetGDAddress(ref addr, txtAddr.Text) == false)
            {
                return;
            }


            //������
            int pos = 0;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            //Ȩ������
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            //������895D
            data[pos++] = 0x5D;
            data[pos++] = 0x89;

            data[pos++] = 0x02;	//ɾ��
            data[pos++] = 0;//ȫ��ɾ��

            if (SendFrame(addr, (byte)GD_AFN_CODE.SetRealtimeParam, data, pos, ref frame, 30, 3) == REC_RESULT.OK)
            {
                byte[] rcdata = new byte[frame.dataArray.Count];
                frame.dataArray.CopyTo(rcdata);
                if (rcdata[2] == 0x5D && rcdata[3] == 0x89 && rcdata[4] == 0x00)
                {
                    MessageBox.Show("�����ɹ�");
                }
                else
                {
                    MessageBox.Show("�����ɹ�");
                }
            }
            else
            {
                MessageBox.Show("����ʧ��");
            }
            ClosePort();
        }

        private void btnSet_Click(object sender, EventArgs e)
        {
            frmSet.Visible = false;
            frmSet.Focus();
            frmSet.ShowDialog();
        }

        private void btnSetAction_Click(object sender, EventArgs e)
        {
            Frame_Stu frame = new Frame_Stu();

            byte[] data = new byte[400];
            byte[] addr = new byte[4];
            byte[] deviceAddr = new byte[6];
            IPPORT_SET portSet = new IPPORT_SET();
            PORT_SET serialPortSet = new PORT_SET();

            if (schemeConfigFrm.chkNet.Checked == true)
            {
                GetNetPort(ref portSet);
                if (bNetOpenFlag == false)
                {
                    if (OpenNetPort(portSet) == false)
                    {
                        MessageBox.Show("��������ʧ�ܣ��˳�����");
                        return;
                    }

                }
            }
            else if (schemeConfigFrm.chkRS485.Checked == true)
            {
                GetSerialPort(ref serialPortSet); ;
                OpenSerialPort(serialPortSet);
            }
            else
            {
                return;
            }

            if (GetGDAddress(ref addr, txtAddr.Text) == false)
            {
                return;
            }


            //������
            int pos = 0;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            //Ȩ������
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            //������0101
            data[pos++] = 0x01;
            data[pos++] = 0x01;
            data[pos++] = Convert.ToByte(frmSet.txtNightEndMin.Text.ToString(), 16);
            data[pos++] = Convert.ToByte(frmSet.txtNightEndHour.Text.ToString(), 16);
            data[pos++] = Convert.ToByte(frmSet.txtNightStartMin.Text.ToString(), 16);
            data[pos++] = Convert.ToByte(frmSet.txtNightStartHour.Text.ToString(), 16);


            //������0102
            data[pos++] = 0x02;
            data[pos++] = 0x01;
            data[pos++] = Convert.ToByte(frmSet.txtLightLeve.Text.ToString(), 16);

            //������0103
            data[pos++] = 0x03;
            data[pos++] = 0x01;
            data[pos++] = Convert.ToByte(frmSet.txtMotorRunTime.Text.ToString(), 16);

            //������0104
            data[pos++] = 0x04;
            data[pos++] = 0x01;
            data[pos++] = (byte)(frmSet.cmbAirSeason.SelectedIndex + 1);


            //������0105
            data[pos++] = 0x05;
            data[pos++] = 0x01;
            data[pos++] = Convert.ToByte(frmSet.txtSumerDegree.Text.ToString(), 16);
            data[pos++] = Convert.ToByte(frmSet.txtWinterDegree.Text.ToString(), 16);

            //������0108
            data[pos++] = 0x08;
            data[pos++] = 0x01;
            if (frmSet.cmbDoorCardType.SelectedIndex == 1)
            {
                data[pos] |= (byte)0x01;
            }

            if (frmSet.cmbDoorDisplaytype.SelectedIndex == 1)
            {
                data[pos] |= (byte)0x02;
            }

            if (frmSet.comBusRule.SelectedIndex == 1)
            {
                data[pos] |= (byte)0x04;
            }
            else if (frmSet.comBusRule.SelectedIndex == 2)
            {
                data[pos] |= (byte)0x08;
            }
            pos++;

            //������0109
            data[pos++] = 0x09;
            data[pos++] = 0x01;
            data[pos++] = Convert.ToByte(frmSet.txtIrdaTime.Text.ToString(), 10);

            if (SendFrame(addr, (byte)GD_AFN_CODE.SetRealtimeParam, data, pos, ref frame, 15, 3) == REC_RESULT.OK)
            {
                byte[] rcdata = new byte[frame.dataArray.Count];
                frame.dataArray.CopyTo(rcdata);
                MessageBox.Show("�����ɹ�");
                /*if (rcdata[2] == 0x5D && rcdata[3] == 0x89 && rcdata[4] == 0x00)
                {
                    MessageBox.Show("�����ɹ�");    
                }
                else
                {
                    MessageBox.Show("����ʧ��");
                }*/
            }
            else
            {
                MessageBox.Show("����ʧ��");
            }
            ClosePort();
        }
        private bool Quereflag = false;
        private void btnQuere_Click(object sender, EventArgs e)
        {
            if (Quereflag == false)
            {
                Quereflag = true;
                thread = new Thread(new ThreadStart(RunInfoInquiry));
                thread.Start();
                btnQuere.Text = "ֹͣ��ѯ";
                btnClear.Enabled = false;
                btnParamterLoad.Enabled = false;
                btnSetAction.Enabled = false;
                btnReadSet.Enabled = false;
                btnLoad.Enabled = false;
                btnSet.Enabled = false;
                btnRcuState.Enabled = false;
                btnIrdtSet.Enabled = false;
                btnDoorSet.Enabled = false;
                btnSceneSet.Enabled = false;
                btnPowKeyScene.Enabled = false;
                btnDrSet.Enabled = false;
                btnCtlCommand.Enabled = false;
            }
            else
            {
                Quereflag = false;
                ClosePort();
                thread.Abort();
                btnQuere.Text = "������ѯ";
                btnClear.Enabled = true;
                btnParamterLoad.Enabled = true;
                btnSetAction.Enabled = true;
                btnReadSet.Enabled = true;
                btnLoad.Enabled = true;
                btnSet.Enabled = true;
                btnRcuState.Enabled = true;
                btnIrdtSet.Enabled = true;
                btnDoorSet.Enabled = true;
                btnSceneSet.Enabled = true;
                btnPowKeyScene.Enabled = true;
                btnDrSet.Enabled = true;
                btnCtlCommand.Enabled = true;
            }

        }


        private bool SetSystemclock()
        {
            Frame_Stu frame = new Frame_Stu();

            byte[] data = new byte[14];
            byte[] addr = new byte[4];

            if (GetGDAddress(ref addr, txtAddr.Text) == false)
            {
                return false;
            }

            string year = DateTime.Now.Year.ToString();
            while (true)
            {
                if (year.Length > 2)
                {
                    year = year.Remove(0, 1);
                }
                else
                {
                    break;
                }
            }

            //������
            data[0] = 0x00;
            data[1] = 0x00;
            //Ȩ������
            data[2] = 0x11;
            data[3] = 0x11;
            data[4] = 0x11;
            data[5] = 0x11;
            //������8030
            data[6] = 0x30;
            data[7] = 0x80;
            //���ʱ������
            data[8] = Convert.ToByte(DateTime.Now.Second.ToString(), 16);
            data[9] = Convert.ToByte(DateTime.Now.Minute.ToString(), 16);
            data[10] = Convert.ToByte(DateTime.Now.Hour.ToString(), 16);
            data[11] = Convert.ToByte(DateTime.Now.Day.ToString(), 16);

            data[12] = Convert.ToByte(DateTime.Now.Month.ToString(), 16);
            byte month = data[12];

            data[13] = Convert.ToByte(year, 16);

            SendFrame(addr, (byte)GD_AFN_CODE.SetRealtimeParam, data, ref frame, 15, 3);
            return true;
        }

        private void RunInfoInquiry()
        {
            Frame_Stu frame = new Frame_Stu();

            byte[] data = new byte[400];
            byte[] addr = new byte[4];
            byte[] deviceAddr = new byte[6];
            IPPORT_SET portSet = new IPPORT_SET();
            PORT_SET serialPortSet = new PORT_SET();

            if (schemeConfigFrm.chkNet.Checked == true)
            {
                GetNetPort(ref portSet);
                if (bNetOpenFlag == false)
                {
                    if (OpenNetPort(portSet) == false)
                    {
                        MessageBox.Show("��������ʧ�ܣ��˳�����");
                        return;
                    }

                }
            }
            else if (schemeConfigFrm.chkRS485.Checked == true)
            {
                GetSerialPort(ref serialPortSet); ;
                OpenSerialPort(serialPortSet);
            }
            else
            {
                return;
            }

            SetSystemclock();

            if (GetGDAddress(ref addr, txtAddr.Text) == false)
            {
                return;
            }


            //������
            int pos = 0;
            data[pos++] = 0x01;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            data[pos++] = 0x00;

            //������0106
            data[pos++] = 0x06;
            data[pos++] = 0x01;

            //������0107
            data[pos++] = 0x07;
            data[pos++] = 0x01;

            while (true)
            {
                if (SendFrame(addr, (byte)GD_AFN_CODE.GetRealtimeParam, data, pos, ref frame, 15, 3) == REC_RESULT.OK)
                {
                    byte[] rcdata = new byte[frame.dataArray.Count];
                    frame.dataArray.CopyTo(rcdata);
                    /*if (frame.dataArray.Count > 9 && rcdata[8] == 0x06 && rcdata[9] == 0x01)
                    {
                        /*
                            bit0������ť״̬��0��δ���£�1���Ѱ���
                            Bit1�����ſ�����״̬��0��δ�忨��1�Ѳ忨
                            Bit2 ������Ŵż�⣺0�����Źأ�1���ſ�
                            Bit3 �ܵ�Դ����״̬��0���أ�1������
                            Bit4 �ȵƿ���״̬ 0���أ�1������
                            Bit5 ҹ�ƿ���״̬ 0���أ�1������
                            Bit6 �յ�����״̬ 0���ļ���1��������
                            Bit7 �����䰴ť״̬0���أ�1������
                            Bit8 ϴ�·���ť״̬0���أ�1������
                            Bit9 �������״̬0���أ�1������

                            Bit8~31 ������
                         *
                        if ((byte)(rcdata[10] & 0x01) == (byte)0x01)
                        {
                            lvRunInfo.Items[0].SubItems[1].Text = "�Ѱ���";
                        }
                        else
                        {
                            lvRunInfo.Items[0].SubItems[1].Text = "δ����";
                        }

                        if ((byte)(rcdata[10] & 0x02) == (byte)0x02)
                        {
                            lvRunInfo.Items[1].SubItems[1].Text = "�Ѳ忨";
                        }
                        else
                        {
                            lvRunInfo.Items[1].SubItems[1].Text = "δ�忨";
                        }

                        if ((byte)(rcdata[10] & 0x04) == (byte)0x04)
                        {
                            lvRunInfo.Items[2].SubItems[1].Text = "���ſ�";
                        }
                        else
                        {
                            lvRunInfo.Items[2].SubItems[1].Text = "���Ź�";
                        }
                        if ((byte)(rcdata[10] & 0x08) == (byte)0x08)
                        {
                            lvRunInfo.Items[3].SubItems[1].Text = "��";
                        }
                        else
                        {
                            lvRunInfo.Items[3].SubItems[1].Text = "��";
                        }
                        if ((byte)(rcdata[10] & 0x10) == (byte)0x10)
                        {
                            lvRunInfo.Items[4].SubItems[1].Text = "��";
                        }
                        else
                        {
                            lvRunInfo.Items[4].SubItems[1].Text = "��";
                        }
                        if ((byte)(rcdata[10] & 0x20) == (byte)0x20)
                        {
                            lvRunInfo.Items[5].SubItems[1].Text = "��";
                        }
                        else
                        {
                            lvRunInfo.Items[5].SubItems[1].Text = "��";
                        }
                        if ((byte)(rcdata[10] & 0x40) == (byte)0x40)
                        {
                            lvRunInfo.Items[6].SubItems[1].Text = "����";
                        }
                        else
                        {
                            lvRunInfo.Items[6].SubItems[1].Text = "�ļ�";
                        }
                        if ((byte)(rcdata[10] & 0x80) == (byte)0x80)
                        {
                            lvRunInfo.Items[7].SubItems[1].Text = "��";
                        }
                        else
                        {
                            lvRunInfo.Items[7].SubItems[1].Text = "��";
                        }
                        if ((byte)(rcdata[11] & 0x01) == (byte)0x01)
                        {
                            lvRunInfo.Items[8].SubItems[1].Text = "��";
                        }
                        else
                        {
                            lvRunInfo.Items[8].SubItems[1].Text = "��";
                        }
                        if ((byte)(rcdata[11] & 0x02) == (byte)0x02)
                        {
                            lvRunInfo.Items[9].SubItems[1].Text = "��";
                        }
                        else
                        {
                            lvRunInfo.Items[9].SubItems[1].Text = "��";
                        }

                        lvRunInfo.Items[10].SubItems[1].Text = String.Format("{0:D2}", (byte)rcdata[16]) + " ��";
                        lvRunInfo.Items[11].SubItems[1].Text = String.Format("{0:D2}", (byte)rcdata[17]) + " ��";
                        lvRunInfo.Items[12].SubItems[1].Text = String.Format("{0:D2}", (byte)rcdata[18]) + " ��";
                    }
                    else
                    {

                    }*/
                }
                System.Threading.Thread.Sleep(10);
            }


            ClosePort();
            thread.Abort();
        }

        private void btnReadSet_Click(object sender, EventArgs e)
        {
            Frame_Stu frame = new Frame_Stu();

            byte[] data = new byte[400];
            byte[] addr = new byte[4];
            byte[] deviceAddr = new byte[6];
            IPPORT_SET portSet = new IPPORT_SET();
            PORT_SET serialPortSet = new PORT_SET();

            if (schemeConfigFrm.chkNet.Checked == true)
            {
                GetNetPort(ref portSet);
                if (bNetOpenFlag == false)
                {
                    if (OpenNetPort(portSet) == false)
                    {
                        MessageBox.Show("��������ʧ�ܣ��˳�����");
                        return;
                    }

                }
            }
            else if (schemeConfigFrm.chkRS485.Checked == true)
            {
                GetSerialPort(ref serialPortSet); ;
                OpenSerialPort(serialPortSet);
            }
            else
            {
                return;
            }

            if (GetGDAddress(ref addr, txtAddr.Text) == false)
            {
                return;
            }


            //������
            int pos = 0;
            data[pos++] = 0x01;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            data[pos++] = 0x00;

            //������0101
            data[pos++] = 0x01;
            data[pos++] = 0x01;

            //������0102
            data[pos++] = 0x02;
            data[pos++] = 0x01;

            //������0103
            data[pos++] = 0x03;
            data[pos++] = 0x01;

            //������0104
            data[pos++] = 0x04;
            data[pos++] = 0x01;

            //������0105
            data[pos++] = 0x05;
            data[pos++] = 0x01;

            //������0108
            data[pos++] = 0x08;
            data[pos++] = 0x01;
            //������0109
            data[pos++] = 0x09;
            data[pos++] = 0x01;


            if (SendFrame(addr, (byte)GD_AFN_CODE.GetRealtimeParam, data, pos, ref frame, 15, 3) == REC_RESULT.OK)
            {
                byte[] rcdata = new byte[frame.dataArray.Count];
                frame.dataArray.CopyTo(rcdata);
                if (rcdata[8] == 0x01 && rcdata[9] == 0x01)
                {
                    frmSet.txtNightEndMin.Text = String.Format("{0:X2}", (byte)rcdata[10]);
                    frmSet.txtNightEndHour.Text = String.Format("{0:X2}", (byte)rcdata[11]);
                    frmSet.txtNightStartMin.Text = String.Format("{0:X2}", (byte)rcdata[12]);
                    frmSet.txtNightStartHour.Text = String.Format("{0:X2}", (byte)rcdata[13]);
                }
                if (rcdata[14] == 0x02 && rcdata[15] == 0x01)
                {
                    frmSet.txtLightLeve.Text = String.Format("{0:X2}", (byte)rcdata[16]);
                }
                if (rcdata[17] == 0x03 && rcdata[18] == 0x01)
                {
                    frmSet.txtMotorRunTime.Text = String.Format("{0:X2}", (byte)rcdata[19]);
                }

                if (rcdata[20] == 0x04 && rcdata[21] == 0x01)
                {
                    frmSet.cmbAirSeason.SelectedIndex = (byte)(rcdata[22] - 1);
                }
                if (rcdata[23] == 0x05 && rcdata[24] == 0x01)
                {
                    frmSet.txtSumerDegree.Text = String.Format("{0:X2}", (byte)rcdata[25]);
                    frmSet.txtWinterDegree.Text = String.Format("{0:X2}", (byte)rcdata[26]);
                }
                if (rcdata[27] == 0x08 && rcdata[28] == 0x01)
                {
                    frmSet.cmbDoorCardType.SelectedIndex = (byte)(rcdata[29] & 0x01);
                    frmSet.cmbDoorDisplaytype.SelectedIndex = (byte)((rcdata[29] >> 1) & 0x01);
                    frmSet.comBusRule.SelectedIndex = (byte)((rcdata[29] >> 2) & 0x01);
                }
                if (rcdata[30] == 0x09 && rcdata[31] == 0x01)
                {
                    frmSet.txtIrdaTime.Text = String.Format("{0:X2}", (byte)rcdata[32]);
                }
            }
            ClosePort();
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void button5_Click(object sender, EventArgs e)
        {
            powkeySceneSet.Visible = false;
            powkeySceneSet.Focus();
            powkeySceneSet.ShowDialog();
        }

        private void btnPowKeyScene_Click(object sender, EventArgs e)
        {
            Frame_Stu frame = new Frame_Stu();

            byte[] data = new byte[400];
            byte[] addr = new byte[4];
            byte[] deviceAddr = new byte[6];
            IPPORT_SET portSet = new IPPORT_SET();
            PORT_SET serialPortSet = new PORT_SET();

            if (schemeConfigFrm.chkNet.Checked == true)
            {
                GetNetPort(ref portSet);
                if (bNetOpenFlag == false)
                {
                    if (OpenNetPort(portSet) == false)
                    {
                        MessageBox.Show("��������ʧ�ܣ��˳�����");
                        return;
                    }

                }
            }
            else if (schemeConfigFrm.chkRS485.Checked == true)
            {
                GetSerialPort(ref serialPortSet); ;
                OpenSerialPort(serialPortSet);
            }
            else
            {
                return;
            }

            if (GetGDAddress(ref addr, txtAddr.Text) == false)
            {
                return;
            }


            //������
            int pos = 0;
            byte memberNum = 0;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            //Ȩ������
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            //������0140
            data[pos++] = 0x40;
            data[pos++] = 0x01;

            data[pos++] = memberNum;

            if (powkeySceneSet.PowChk1.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo1.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO1.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK1cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam1.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (powkeySceneSet.PowChk2.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo2.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO2.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK2cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam2.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (powkeySceneSet.PowChk3.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo3.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO3.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK3cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam3.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (powkeySceneSet.PowChk4.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo4.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO4.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK4cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam4.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (powkeySceneSet.PowChk5.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo5.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO5.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK5cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam5.Text.ToString(), 10);
                data[pos++] = (byte)((byte)(dataArea & 0xff));
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (powkeySceneSet.PowChk6.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo6.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO6.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK6cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam6.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (powkeySceneSet.PowChk7.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo7.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO7.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK7cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam7.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (powkeySceneSet.PowChk8.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo8.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO8.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK8cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam8.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (powkeySceneSet.PowChk9.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo9.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO9.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK9cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam9.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (powkeySceneSet.PowChk10.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo10.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO10.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK10cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam10.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (powkeySceneSet.PowChk11.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo11.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO11.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK11cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam11.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (powkeySceneSet.PowChk12.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo12.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO12.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK12cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam12.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }
            if (powkeySceneSet.PowChk13.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo13.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO13.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK13cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam13.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (powkeySceneSet.PowChk14.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo14.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO14.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK14cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam14.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }
            if (powkeySceneSet.PowChk15.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo15.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO15.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK15cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam15.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (powkeySceneSet.PowChk16.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo16.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO16.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK16cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam16.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (powkeySceneSet.PowChk17.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo17.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO17.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK17cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam17.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (powkeySceneSet.PowChk18.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo18.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO18.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK18cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam18.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (powkeySceneSet.PowChk19.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo19.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO19.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK19cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam19.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (powkeySceneSet.PowChk20.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeyCtlDNo20.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(powkeySceneSet.powkeySNO20.Text.ToString(), 10);
                data[pos++] = (byte)(powkeySceneSet.powK20cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(powkeySceneSet.powkeyParam20.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            data[8] = memberNum;
            if (SendFrame(addr, (byte)GD_AFN_CODE.SetRealtimeParam, data, pos, ref frame, 15, 3) == REC_RESULT.OK)
            {
                byte[] rcdata = new byte[frame.dataArray.Count];
                frame.dataArray.CopyTo(rcdata);
                if (rcdata[2] == 0x40 && rcdata[3] == 0x01 && rcdata[4] == 0x00)
                {
                    MessageBox.Show("�����ɹ�");
                }
                else
                {
                    MessageBox.Show("����ʧ��");
                }
            }
            else
            {
                MessageBox.Show("����ʧ��");
            }
            ClosePort();

        }

        private void btnDoorcardScene_Click(object sender, EventArgs e)
        {
            doorcardSceneSet.Visible = false;
            doorcardSceneSet.Focus();
            doorcardSceneSet.ShowDialog();
        }

        private void btnDrSet_Click(object sender, EventArgs e)
        {
            Frame_Stu frame = new Frame_Stu();

            byte[] data = new byte[400];
            byte[] addr = new byte[4];
            byte[] deviceAddr = new byte[6];
            IPPORT_SET portSet = new IPPORT_SET();
            PORT_SET serialPortSet = new PORT_SET();

            if (schemeConfigFrm.chkNet.Checked == true)
            {
                GetNetPort(ref portSet);
                if (bNetOpenFlag == false)
                {
                    if (OpenNetPort(portSet) == false)
                    {
                        MessageBox.Show("��������ʧ�ܣ��˳�����");
                        return;
                    }

                }
            }
            else if (schemeConfigFrm.chkRS485.Checked == true)
            {
                GetSerialPort(ref serialPortSet); ;
                OpenSerialPort(serialPortSet);
            }
            else
            {
                return;
            }

            if (GetGDAddress(ref addr, txtAddr.Text) == false)
            {
                return;
            }


            //������
            int pos = 0;
            byte memberNum = 0;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            //Ȩ������
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            //������0142
            data[pos++] = 0x42;
            data[pos++] = 0x01;

            data[pos++] = memberNum;

            if (doorcardSceneSet.DrChk1.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo1.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO1.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK1cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardParam1.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrChk2.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo2.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO2.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK2cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardParam2.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrChk3.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo3.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO3.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK3cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardParam3.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (doorcardSceneSet.DrChk4.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo4.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO4.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK4cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardParam4.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (doorcardSceneSet.DrChk5.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo5.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO5.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK5cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardParam5.Text.ToString(), 10);
                data[pos++] = (byte)((byte)(dataArea & 0xff));
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (doorcardSceneSet.DrChk6.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo6.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO6.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK6cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardParam6.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (doorcardSceneSet.DrChk7.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo7.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO7.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK7cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardParam7.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (doorcardSceneSet.DrChk8.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo8.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO8.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK8cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardParam8.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (doorcardSceneSet.DrChk9.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo9.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO9.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK9cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardParam9.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (doorcardSceneSet.DrChk10.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo10.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO10.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK10cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardyParam10.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrChk11.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo11.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO11.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK11cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardyParam11.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrChk12.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo12.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO12.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK12cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardyParam12.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }
            if (doorcardSceneSet.DrChk13.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo13.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO13.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK13cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardyParam13.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrChk14.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo14.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO14.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK14cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardyParam14.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrChk15.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo15.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO15.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK15cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardyParam15.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrChk16.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo16.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO16.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK16cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardyParam16.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrChk17.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo17.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO17.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK17cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardyParam17.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrChk18.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo18.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO18.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK18cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardyParam18.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrChk19.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo19.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO19.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK19cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardyParam19.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrChk20.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardCtlDNo20.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardSNO20.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardK20cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardyParam20.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }
            data[8] = memberNum;

            //������0143
            data[pos++] = 0x43;
            data[pos++] = 0x01;
            int tmppos = pos;
            memberNum = 0;
            data[pos++] = memberNum;

            if (doorcardSceneSet.DrOutChk1.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo1.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO1.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK1cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardOutParam1.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrOutChk2.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo2.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO2.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK2cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardOutParam2.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrOutChk3.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo3.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO3.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK3cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardParam3.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (doorcardSceneSet.DrOutChk4.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo4.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO4.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK4cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardOutParam4.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (doorcardSceneSet.DrOutChk5.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo5.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO5.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK5cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardOutParam5.Text.ToString(), 10);
                data[pos++] = (byte)((byte)(dataArea & 0xff));
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (doorcardSceneSet.DrOutChk6.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo6.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO6.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK6cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardOutParam6.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (doorcardSceneSet.DrOutChk7.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo7.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO7.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK7cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardOutParam7.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (doorcardSceneSet.DrOutChk8.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo8.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO8.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK8cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardOutParam8.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (doorcardSceneSet.DrOutChk9.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo9.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO9.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK9cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardOutParam9.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (doorcardSceneSet.DrOutChk10.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo10.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO10.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK10cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardOutParam10.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrOutChk11.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo11.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO11.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK11cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardOutParam11.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrOutChk12.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo12.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO12.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK12cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardOutParam12.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrOutChk13.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo13.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO13.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK13cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardOutParam13.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrOutChk14.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo14.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO14.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK14cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardOutParam14.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrOutChk15.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo15.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO15.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK15cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardOutParam15.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrOutChk16.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo16.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO16.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK16cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardOutParam16.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrOutChk17.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo17.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO17.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK17cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardOutParam17.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrOutChk18.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo18.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO18.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK18cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardOutParam18.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrOutChk19.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo19.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO19.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK19cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardOutParam19.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorcardSceneSet.DrOutChk20.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutCtlDNo20.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorcardSceneSet.doorcardOutSNO20.Text.ToString(), 10);
                data[pos++] = (byte)(doorcardSceneSet.doorcardOutK20cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorcardSceneSet.doorcardOutParam20.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }
            data[tmppos] = memberNum;

            if (SendFrame(addr, (byte)GD_AFN_CODE.SetRealtimeParam, data, pos, ref frame, 15, 3) == REC_RESULT.OK)
            {
                byte[] rcdata = new byte[frame.dataArray.Count];
                frame.dataArray.CopyTo(rcdata);
                if (rcdata[2] == 0x42 && rcdata[3] == 0x01 && rcdata[4] == 0x00)
                {
                    MessageBox.Show("�����ɹ�");
                }
                else
                {
                    MessageBox.Show("����ʧ��");
                }
            }
            else
            {
                MessageBox.Show("����ʧ��");
            }
            ClosePort();
        }

        private void btnScene_Click(object sender, EventArgs e)
        {
            sceneSet.Visible = false;
            sceneSet.Focus();
            sceneSet.ShowDialog();
        }

        private void btnSceneSet_Click(object sender, EventArgs e)
        {
            Frame_Stu frame = new Frame_Stu();

            byte[] data = new byte[400];
            byte[] addr = new byte[4];
            byte[] deviceAddr = new byte[6];
            IPPORT_SET portSet = new IPPORT_SET();
            PORT_SET serialPortSet = new PORT_SET();

            if (schemeConfigFrm.chkNet.Checked == true)
            {
                GetNetPort(ref portSet);
                if (bNetOpenFlag == false)
                {
                    if (OpenNetPort(portSet) == false)
                    {
                        MessageBox.Show("��������ʧ�ܣ��˳�����");
                        return;
                    }

                }
            }
            else if (schemeConfigFrm.chkRS485.Checked == true)
            {
                GetSerialPort(ref serialPortSet); ;
                OpenSerialPort(serialPortSet);
            }
            else
            {
                return;
            }

            if (GetGDAddress(ref addr, txtAddr.Text) == false)
            {
                return;
            }


            //������
            int pos = 0;
            int memberNumPos = 0;
            byte memberNum = 0;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            //Ȩ������
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            //������0111
            data[pos++] = 0x11;
            data[pos++] = 0x01;
            memberNumPos = pos;
            data[pos++] = memberNum;

            if (sceneSet.Scene1Chk1.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(sceneSet.scene1CtlDNo1.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(sceneSet.scene1SNO1.Text.ToString(), 10);
                data[pos++] = (byte)(sceneSet.scene1K1cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(sceneSet.scene1Param1.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (sceneSet.Scene1Chk2.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(sceneSet.scene1CtlDNo2.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(sceneSet.scene1SNO2.Text.ToString(), 10);
                data[pos++] = (byte)(sceneSet.scene1K2cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(sceneSet.scene1Param2.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (sceneSet.Scene1Chk3.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(sceneSet.scene1CtlDNo3.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(sceneSet.scene1SNO3.Text.ToString(), 10);
                data[pos++] = (byte)(sceneSet.scene1K3cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(sceneSet.scene1Param3.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (sceneSet.Scene1Chk4.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(sceneSet.scene1CtlDNo4.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(sceneSet.scene1SNO4.Text.ToString(), 10);
                data[pos++] = (byte)(sceneSet.scene1K4cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(sceneSet.scene1Param4.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (sceneSet.Scene1Chk5.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(sceneSet.scene1CtlDNo5.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(sceneSet.scene1SNO5.Text.ToString(), 10);
                data[pos++] = (byte)(sceneSet.scene1K5cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(sceneSet.scene1Param5.Text.ToString(), 10);
                data[pos++] = (byte)((byte)(dataArea & 0xff));
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (sceneSet.Scene1Chk6.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(sceneSet.scene1CtlDNo6.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(sceneSet.scene1SNO6.Text.ToString(), 10);
                data[pos++] = (byte)(sceneSet.scene1K6cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(sceneSet.scene1Param6.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }



            data[memberNumPos] = memberNum;


            /* ��������������*/
            data[pos++] = 0x12;
            data[pos++] = 0x01;
            memberNumPos = pos;
            memberNum = 0;
            data[pos++] = memberNum;

            if (sceneSet.Scene2Chk1.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(sceneSet.scene2CtlDNo1.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(sceneSet.scene2SNO1.Text.ToString(), 10);
                data[pos++] = (byte)(sceneSet.scene2K1cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(sceneSet.scene2Param1.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (sceneSet.Scene2Chk2.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(sceneSet.scene2CtlDNo2.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(sceneSet.scene2SNO2.Text.ToString(), 10);
                data[pos++] = (byte)(sceneSet.scene2K2cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(sceneSet.scene2Param2.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (sceneSet.Scene2Chk3.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(sceneSet.scene2CtlDNo3.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(sceneSet.scene2SNO3.Text.ToString(), 10);
                data[pos++] = (byte)(sceneSet.scene2K3cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(sceneSet.scene2Param3.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (sceneSet.Scene2Chk4.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(sceneSet.scene2CtlDNo4.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(sceneSet.scene2SNO4.Text.ToString(), 10);
                data[pos++] = (byte)(sceneSet.scene2K4cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(sceneSet.scene2Param4.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (sceneSet.Scene2Chk5.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(sceneSet.scene2CtlDNo5.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(sceneSet.scene2SNO5.Text.ToString(), 10);
                data[pos++] = (byte)(sceneSet.scene2K5cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(sceneSet.scene2Param5.Text.ToString(), 10);
                data[pos++] = (byte)((byte)(dataArea & 0xff));
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (sceneSet.Scene2Chk6.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(sceneSet.scene2CtlDNo6.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(sceneSet.scene2SNO6.Text.ToString(), 10);
                data[pos++] = (byte)(sceneSet.scene2K6cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(sceneSet.scene2Param6.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            data[memberNumPos] = memberNum;

            /* ��������������*/
            data[pos++] = 0x13;
            data[pos++] = 0x01;
            memberNumPos = pos;
            memberNum = 0;
            data[pos++] = memberNum;

            if (sceneSet.Scene3Chk1.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(sceneSet.scene3CtlDNo1.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(sceneSet.scene3SNO1.Text.ToString(), 10);
                data[pos++] = (byte)(sceneSet.scene3K1cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(sceneSet.scene3Param1.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (sceneSet.Scene3Chk2.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(sceneSet.scene3CtlDNo2.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(sceneSet.scene3SNO2.Text.ToString(), 10);
                data[pos++] = (byte)(sceneSet.scene3K2cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(sceneSet.scene3Param2.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (sceneSet.Scene3Chk3.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(sceneSet.scene3CtlDNo3.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(sceneSet.scene3SNO3.Text.ToString(), 10);
                data[pos++] = (byte)(sceneSet.scene3K3cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(sceneSet.scene3Param3.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (sceneSet.Scene3Chk4.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(sceneSet.scene3CtlDNo4.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(sceneSet.scene3SNO4.Text.ToString(), 10);
                data[pos++] = (byte)(sceneSet.scene3K4cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(sceneSet.scene3Param4.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (sceneSet.Scene3Chk5.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(sceneSet.scene3CtlDNo5.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(sceneSet.scene3SNO5.Text.ToString(), 10);
                data[pos++] = (byte)(sceneSet.scene3K5cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(sceneSet.scene3Param5.Text.ToString(), 10);
                data[pos++] = (byte)((byte)(dataArea & 0xff));
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (sceneSet.Scene3Chk6.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(sceneSet.scene3CtlDNo6.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(sceneSet.scene3SNO6.Text.ToString(), 10);
                data[pos++] = (byte)(sceneSet.scene3K6cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(sceneSet.scene3Param6.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            data[memberNumPos] = memberNum;

            if (SendFrame(addr, (byte)GD_AFN_CODE.SetRealtimeParam, data, pos, ref frame, 15, 3) == REC_RESULT.OK)
            {
                byte[] rcdata = new byte[frame.dataArray.Count];
                frame.dataArray.CopyTo(rcdata);
                if (rcdata[2] == 0x11 && rcdata[3] == 0x01 && rcdata[4] == 0x00)
                {
                    MessageBox.Show("�����ɹ�");
                }
                else
                {
                    MessageBox.Show("����ʧ��");
                }
            }
            else
            {
                MessageBox.Show("����ʧ��");
            }
            ClosePort();
        }

        private void btnCtlCommand_Click(object sender, EventArgs e)
        {
            Frame_Stu frame = new Frame_Stu();

            byte[] data = new byte[400];
            byte[] addr = new byte[4];
            byte[] deviceAddr = new byte[6];
            IPPORT_SET portSet = new IPPORT_SET();
            PORT_SET serialPortSet = new PORT_SET();

            if (schemeConfigFrm.chkNet.Checked == true)
            {
                GetNetPort(ref portSet);
                if (bNetOpenFlag == false)
                {
                    if (OpenNetPort(portSet) == false)
                    {
                        MessageBox.Show("��������ʧ�ܣ��˳�����");
                        return;
                    }

                }
            }
            else if (schemeConfigFrm.chkRS485.Checked == true)
            {
                GetSerialPort(ref serialPortSet); ;
                OpenSerialPort(serialPortSet);
            }
            else
            {
                return;
            }

            if (GetGDAddress(ref addr, txtAddr.Text) == false)
            {
                return;
            }


            //������
            int pos = 0;
            byte memberNum = 0;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            //Ȩ������
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            //������0140
            data[pos++] = 0x01;
            data[pos++] = 0x02;

            data[pos++] = memberNum;

            memberNum++;
            data[pos++] = Convert.ToByte(txtCtlDNo.Text.ToString(), 10);
            data[pos++] = Convert.ToByte(txtSubno.Text.ToString(), 10);
            data[pos++] = (byte)(cmbFunc.SelectedIndex); //���⹦������ 

            UInt32 dataArea = Convert.ToUInt32(txtParam.Text.ToString(), 10);
            //data[pos++] = (byte)(dataArea & 0xff);
            //data[pos++] = (byte)((dataArea >> 8) & 0xff);
            //data[pos++] = (byte)((dataArea >> 16) & 0xff);
            //data[pos++] = (byte)((dataArea >> 24) & 0xff);
            data[pos++] = (byte)((dataArea >> 24) & 0xff);
            data[pos++] = (byte)((dataArea >> 16) & 0xff);
            data[pos++] = (byte)((dataArea >> 8) & 0xff);
            data[pos++] = (byte)(dataArea & 0xff);

            data[8] = memberNum;
            if (SendFrame(addr, (byte)GD_AFN_CODE.SetRealtimeParam, data, pos, ref frame, 15, 3) == REC_RESULT.OK)
            {
                byte[] rcdata = new byte[frame.dataArray.Count];
                frame.dataArray.CopyTo(rcdata);
                if (rcdata[2] == 0x01 && rcdata[3] == 0x02 && rcdata[4] == 0x00)
                {
                    MessageBox.Show("�����ɹ�");
                }
                else
                {
                    MessageBox.Show("����ʧ��");
                }
            }
            else
            {
                MessageBox.Show("����ʧ��");
            }
            ClosePort();

        }

        private void MainFrame_Load(object sender, EventArgs e)
        {

        }

        private void DSScenebtn_Click(object sender, EventArgs e)
        {
            doorSceneSet.Visible = false;
            doorSceneSet.Focus();
            doorSceneSet.ShowDialog();
        }

        private void btnDoorSet_Click(object sender, EventArgs e)
        {
            Frame_Stu frame = new Frame_Stu();

            byte[] data = new byte[400];
            byte[] addr = new byte[4];
            byte[] deviceAddr = new byte[6];
            IPPORT_SET portSet = new IPPORT_SET();
            PORT_SET serialPortSet = new PORT_SET();

            if (schemeConfigFrm.chkNet.Checked == true)
            {
                GetNetPort(ref portSet);
                if (bNetOpenFlag == false)
                {
                    if (OpenNetPort(portSet) == false)
                    {
                        MessageBox.Show("��������ʧ�ܣ��˳�����");
                        return;
                    }

                }
            }
            else if (schemeConfigFrm.chkRS485.Checked == true)
            {
                GetSerialPort(ref serialPortSet); ;
                OpenSerialPort(serialPortSet);
            }
            else
            {
                return;
            }

            if (GetGDAddress(ref addr, txtAddr.Text) == false)
            {
                return;
            }


            //������
            int pos = 0;
            byte memberNum = 0;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            //Ȩ������
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            //������0144
            data[pos++] = 0x44;
            data[pos++] = 0x01;

            data[pos++] = memberNum;

            if (doorSceneSet.DSChk1.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorSceneSet.DSCtlDNo1.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorSceneSet.DSSNO1.Text.ToString(), 10);
                data[pos++] = (byte)(doorSceneSet.DSK1cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorSceneSet.DSParam1.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorSceneSet.DSChk2.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorSceneSet.DSCtlDNo2.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorSceneSet.DSSNO2.Text.ToString(), 10);
                data[pos++] = (byte)(doorSceneSet.DSK2cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorSceneSet.DSParam2.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (doorSceneSet.DSChk3.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorSceneSet.DSCtlDNo3.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorSceneSet.DSSNO3.Text.ToString(), 10);
                data[pos++] = (byte)(doorSceneSet.DSK3cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorSceneSet.DSParam3.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (doorSceneSet.DSChk4.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(doorSceneSet.DSCtlDNo4.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(doorSceneSet.DSSNO4.Text.ToString(), 10);
                data[pos++] = (byte)(doorSceneSet.DSK4cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(doorSceneSet.DSParam4.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            data[8] = memberNum;
            if (SendFrame(addr, (byte)GD_AFN_CODE.SetRealtimeParam, data, pos, ref frame, 15, 3) == REC_RESULT.OK)
            {
                byte[] rcdata = new byte[frame.dataArray.Count];
                frame.dataArray.CopyTo(rcdata);
                if (rcdata[2] == 0x44 && rcdata[3] == 0x01 && rcdata[4] == 0x00)
                {
                    MessageBox.Show("�����ɹ�");
                }
                else
                {
                    MessageBox.Show("����ʧ��");
                }
            }
            else
            {
                MessageBox.Show("����ʧ��");
            }
            ClosePort();
        }

        private void btnIrdtEdit_Click(object sender, EventArgs e)
        {
            irdtSceneSet.Visible = false;
            irdtSceneSet.Focus();
            irdtSceneSet.ShowDialog();
        }

        private void btnIrdtSet_Click(object sender, EventArgs e)
        {
            Frame_Stu frame = new Frame_Stu();

            byte[] data = new byte[400];
            byte[] addr = new byte[4];
            byte[] deviceAddr = new byte[6];
            IPPORT_SET portSet = new IPPORT_SET();
            PORT_SET serialPortSet = new PORT_SET();

            if (schemeConfigFrm.chkNet.Checked == true)
            {
                GetNetPort(ref portSet);
                if (bNetOpenFlag == false)
                {
                    if (OpenNetPort(portSet) == false)
                    {
                        MessageBox.Show("��������ʧ�ܣ��˳�����");
                        return;
                    }

                }
            }
            else if (schemeConfigFrm.chkRS485.Checked == true)
            {
                GetSerialPort(ref serialPortSet); ;
                OpenSerialPort(serialPortSet);
            }
            else
            {
                return;
            }

            if (GetGDAddress(ref addr, txtAddr.Text) == false)
            {
                return;
            }


            //������
            int pos = 0;
            byte memberNum = 0;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            //Ȩ������
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            data[pos++] = 0x11;
            //������0145
            data[pos++] = 0x45;
            data[pos++] = 0x01;

            data[pos++] = memberNum;

            if (irdtSceneSet.IrdtInChk1.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInCtlDNo1.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInSNO1.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtInK1cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtInParam1.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (irdtSceneSet.IrdtInChk2.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInCtlDNo2.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInSNO2.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtInK2cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtInParam2.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (irdtSceneSet.IrdtInChk3.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInCtlDNo3.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInSNO3.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtInK3cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtInParam3.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (irdtSceneSet.IrdtInChk4.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInCtlDNo4.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInSNO4.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtInK4cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtInParam4.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (irdtSceneSet.IrdtInChk5.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInCtlDNo5.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInSNO5.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtInK5cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtInParam5.Text.ToString(), 10);
                data[pos++] = (byte)((byte)(dataArea & 0xff));
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (irdtSceneSet.IrdtInChk6.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInCtlDNo6.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInSNO6.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtInK6cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtInParam6.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (irdtSceneSet.IrdtInChk7.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInCtlDNo7.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInSNO7.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtInK7cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtInParam7.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (irdtSceneSet.IrdtInChk8.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInCtlDNo8.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInSNO8.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtInK8cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtInParam8.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (irdtSceneSet.IrdtInChk9.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInCtlDNo9.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInSNO9.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtInK9cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtInParam9.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (irdtSceneSet.IrdtInChk10.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInCtlDNo10.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtInSNO10.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtInK10cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtInParam10.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            data[8] = memberNum;

            //������0146
            data[pos++] = 0x46;
            data[pos++] = 0x01;
            int tmppos = pos;
            memberNum = 0;
            data[pos++] = memberNum;

            if (irdtSceneSet.IrdtOutChk1.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutCtlDNo1.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutSNO1.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtOutK1cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtOutParam1.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (irdtSceneSet.IrdtOutChk2.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutCtlDNo2.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutSNO2.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtOutK2cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtOutParam2.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }

            if (irdtSceneSet.IrdtOutChk3.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutCtlDNo3.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutSNO3.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtOutK3cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtOutParam3.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (irdtSceneSet.IrdtOutChk4.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutCtlDNo4.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutSNO4.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtOutK4cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtOutParam4.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (irdtSceneSet.IrdtOutChk5.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutCtlDNo5.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutSNO5.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtOutK5cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtOutParam5.Text.ToString(), 10);
                data[pos++] = (byte)((byte)(dataArea & 0xff));
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (irdtSceneSet.IrdtOutChk6.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutCtlDNo6.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutSNO6.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtOutK6cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtOutParam6.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (irdtSceneSet.IrdtOutChk7.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutCtlDNo7.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutSNO7.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtOutK7cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtOutParam7.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (irdtSceneSet.IrdtOutChk8.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutCtlDNo8.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutSNO8.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtOutK8cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtOutParam8.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (irdtSceneSet.IrdtOutChk9.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutCtlDNo9.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutSNO9.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtOutK9cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtOutParam9.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }


            if (irdtSceneSet.IrdtOutChk10.Checked == true)
            {
                memberNum++;
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutCtlDNo10.Text.ToString(), 10);
                data[pos++] = Convert.ToByte(irdtSceneSet.IrdtOutSNO10.Text.ToString(), 10);
                data[pos++] = (byte)(irdtSceneSet.IrdtOutK10cmbFunc.SelectedIndex); //���⹦������ 

                UInt32 dataArea = Convert.ToUInt32(irdtSceneSet.IrdtOutParam10.Text.ToString(), 10);
                data[pos++] = (byte)(dataArea & 0xff);
                data[pos++] = (byte)((dataArea >> 8) & 0xff);
                data[pos++] = (byte)((dataArea >> 16) & 0xff);
                data[pos++] = (byte)((dataArea >> 24) & 0xff);
            }
            data[tmppos] = memberNum;

            if (SendFrame(addr, (byte)GD_AFN_CODE.SetRealtimeParam, data, pos, ref frame, 15, 3) == REC_RESULT.OK)
            {
                byte[] rcdata = new byte[frame.dataArray.Count];
                frame.dataArray.CopyTo(rcdata);
                if (rcdata[2] == 0x45 && rcdata[3] == 0x01 && rcdata[4] == 0x00 && rcdata[5] == 0x46 && rcdata[6] == 0x01 && rcdata[7] == 0x00)
                {
                    MessageBox.Show("�����ɹ�");
                }
                else
                {
                    MessageBox.Show("����ʧ��");
                }
            }
            else
            {
                MessageBox.Show("����ʧ��");
            }
            ClosePort();
        }

        byte lvLinePos;
        void PraseRcuState(ref byte[] rcdata)
        {
            /*
                ��һ�ֽڣ�
                bit0������ť״̬��0��δ���£�1���Ѱ���
                Bit1�����ſ�����״̬��0��δ�忨��1�Ѳ忨
                Bit2 ������Ŵż�⣺0�����Źأ�1���ſ�
                Bit3 �ܵ�Դ����״̬��0���أ�1������
                Bit4 �����䰴ť״̬0���أ�1������
                Bit5 ϴ�·���ť״̬0���أ�1������
                Bit6 �������״̬0���أ�1������
                Bit7 ���Ժ�״̬0���أ�1������
            */
            /* lvRcuStateInfo�ؼ���ʼ��*/
            { ListViewItem item = new ListViewItem("������ť״̬"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            if ((byte)(rcdata[0] & 0x01) == (byte)0x01)
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "�Ѱ���";
            }
            else
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "δ����";
            }

            { ListViewItem item = new ListViewItem("�����ſ�����״̬"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            if ((byte)(rcdata[0] & 0x02) == (byte)0x02)
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "�Ѳ忨";
            }
            else
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "δ�忨";
            }

            { ListViewItem item = new ListViewItem("������Ŵż��"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            if ((byte)(rcdata[0] & 0x04) == (byte)0x04)
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }
            else
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }

            { ListViewItem item = new ListViewItem("�ܵ�Դ����״̬"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            if ((byte)(rcdata[0] & 0x08) == (byte)0x08)
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }
            else
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }

            { ListViewItem item = new ListViewItem("�����䰴ť״̬"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            if ((byte)(rcdata[0] & 0x10) == (byte)0x10)
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }
            else
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }

            { ListViewItem item = new ListViewItem("ϴ�·���ť״̬"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            if ((byte)(rcdata[0] & 0x20) == (byte)0x20)
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }
            else
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }

            { ListViewItem item = new ListViewItem("�������״̬"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            if ((byte)(rcdata[0] & 0x40) == (byte)0x40)
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }
            else
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }

            { ListViewItem item = new ListViewItem("���Ժ�״̬"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            if ((byte)(rcdata[0] & 0x80) == (byte)0x80)
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }
            else
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }
            /*
             * �ڶ��ֽڣ�
            bit0�����⣺0�����ˣ�1������
            Bit1�������0��������1�������
            Bit2 ��̨�Ŵţ�0����̨�Źأ�1��̨�ſ�
            Bit3 ҹ����0���أ�1������
            Bit4 ������0���أ�1������
            Bit5 �����˷�0���أ�1������
            Bit6 ���� 0���أ�1������
            Bit7 ά����0���أ�1������
            */
            { ListViewItem item = new ListViewItem("������"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            if ((byte)(rcdata[1] & 0x01) == (byte)0x01)
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "����";
            }
            else
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "����";
            }

            { ListViewItem item = new ListViewItem("�������"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            if ((byte)(rcdata[1] & 0x02) == (byte)0x02)
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "�������";
            }
            else
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "������";
            }

            { ListViewItem item = new ListViewItem("��̨�Ŵ�"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            if ((byte)(rcdata[1] & 0x04) == (byte)0x04)
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��̨�ſ�";
            }
            else
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��̨�Ź�";
            }

            { ListViewItem item = new ListViewItem("ҹ��"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            if ((byte)(rcdata[1] & 0x08) == (byte)0x08)
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }
            else
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }

            { ListViewItem item = new ListViewItem("������"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            if ((byte)(rcdata[1] & 0x10) == (byte)0x10)
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }
            else
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }

            { ListViewItem item = new ListViewItem("�����˷�"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            if ((byte)(rcdata[1] & 0x20) == (byte)0x20)
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }
            else
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }

            { ListViewItem item = new ListViewItem("����"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            if ((byte)(rcdata[1] & 0x40) == (byte)0x40)
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }
            else
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }

            { ListViewItem item = new ListViewItem("ά����"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            if ((byte)(rcdata[1] & 0x80) == (byte)0x80)
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }
            else
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
            }
        }

        void PraseAirconditionState(ref byte[] rcdata)
        {
            { ListViewItem item = new ListViewItem("�յ�����ģʽ"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            if ((byte)(rcdata[0]) == (byte)0x01)
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "����";
            }
            else
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "�ļ�";
            }

            { ListViewItem item = new ListViewItem("�յ�����ģʽ"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            if ((byte)(rcdata[1]) == (byte)0x01)
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "����";
            }
            else
            {
                rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "����";
            }

            { ListViewItem item = new ListViewItem("�յ������¶�"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = String.Format("{0:D2}", (byte)rcdata[2]) + " ��";

            { ListViewItem item = new ListViewItem("�����¶�"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = String.Format("{0:D2}", (byte)rcdata[3]) + " ��";

            { ListViewItem item = new ListViewItem("�յ�����"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
            rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = String.Format("{0:D2}", (byte)rcdata[4]) + " ��";

        }
        void PraseRelay8State(ref byte[] rcdata)
        {
            BitArray bitarray = new BitArray(rcdata);

            for (int i = 13; i <= 20; i++)
            {
                { ListViewItem item = new ListViewItem("��" + i.ToString() + "·�̵���"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
                if (bitarray[i] == false)
                {
                    rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
                }
                else
                {
                    rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
                }
            }
        }

        void PraseLight4State(ref byte[] rcdata)
        {

        }

        void PraseLed3State(ref byte[] rcdata)
        {

        }

        void PraseRelay2State(ref byte[] rcdata)
        {
            BitArray bitarray = new BitArray(rcdata);

            for (int i = 5; i <= 6; i++)
            {
                { ListViewItem item = new ListViewItem("��" + i.ToString() + "·�̵���"); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
                if (bitarray[i] == false)
                {
                    rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
                }
                else
                {
                    rcuStateQuq.lvRcuStateInfo.Items[lvLinePos++].SubItems[1].Text = "��";
                }
            }
        }






        private void btnReadRcuState()
        {
            Frame_Stu frame = new Frame_Stu();

            byte[] data = new byte[400];
            byte[] addr = new byte[4];
            byte[] deviceAddr = new byte[6];
            IPPORT_SET portSet = new IPPORT_SET();
            PORT_SET serialPortSet = new PORT_SET();

            if (schemeConfigFrm.chkNet.Checked == true)
            {
                GetNetPort(ref portSet);
                if (bNetOpenFlag == false)
                {
                    if (OpenNetPort(portSet) == false)
                    {
                        MessageBox.Show("��������ʧ�ܣ��˳�����");
                        return;
                    }

                }
            }
            else if (schemeConfigFrm.chkRS485.Checked == true)
            {
                GetSerialPort(ref serialPortSet); ;
                OpenSerialPort(serialPortSet);
            }
            else
            {
                return;
            }

            if (GetGDAddress(ref addr, txtAddr.Text) == false)
            {
                return;
            }

            /* ��ʼ���б�*/
            lvLinePos = 0;
            rcuStateQuq.lvRcuStateInfo.Items.Clear();

            //������
            int pos = 0;
            data[pos++] = 0x01;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            data[pos++] = 0x00;
            data[pos++] = 0x00;

            //������0202
            data[pos++] = 0x02;
            data[pos++] = 0x02;

            if (SendFrame(addr, (byte)GD_AFN_CODE.GetRealtimeParam, data, pos, ref frame, 15, 3) == REC_RESULT.OK)
            {
                byte[] rcdata = new byte[frame.dataArray.Count];
                frame.dataArray.CopyTo(rcdata);
                if (frame.dataArray.Count > 9 && rcdata[8] == 0x02 && rcdata[9] == 0x02)
                {
                    byte deviceNum;
                    deviceNum = rcdata[10];//�豸��
                    byte[] tempdata = new byte[8];

                    /* �ֽڳ��ȼ��� */

                    /* ģ�����ݽ���*/
                    for (int i = 0; i < deviceNum; i++)
                    {
                        switch (rcdata[12 + i * 10])
                        {
                            case (byte)DEVICE_TYPE.MD_RCU:
                                { ListViewItem item = new ListViewItem("<RCU����ģ��״̬>" + "�豸���:" + String.Format("{0:D2}", (byte)rcdata[11 + i * 10])); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
                                lvLinePos++;
                                System.Array.Copy(rcdata, 12 + i * 10 + 1, tempdata, 0, 8);
                                PraseRcuState(ref tempdata);
                                break;

                            case (byte)DEVICE_TYPE.MD_AIRCONDITION:
                                { ListViewItem item = new ListViewItem("<�յ�ģ��״̬>" + "�豸���:" + String.Format("{0:D2}", (byte)rcdata[11 + i * 10])); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
                                lvLinePos++;
                                System.Array.Copy(rcdata, 12 + i * 10 + 1, tempdata, 0, 8);
                                PraseAirconditionState(ref tempdata);
                                break;

                            case (byte)DEVICE_TYPE.MD_RELAY_8:
                                { ListViewItem item = new ListViewItem("<8·�̵���ģ��״̬>" + "�豸���:" + String.Format("{0:D2}", (byte)rcdata[11 + i * 10])); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
                                lvLinePos++;
                                System.Array.Copy(rcdata, 12 + i * 10 + 1, tempdata, 0, 8);
                                PraseRelay8State(ref tempdata);
                                break;

                            case (byte)DEVICE_TYPE.MD_LIGHT_4:
                                { ListViewItem item = new ListViewItem("<4·����ģ��״̬>" + "�豸���:" + String.Format("{0:D2}", (byte)rcdata[11 + i * 10])); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
                                lvLinePos++;
                                System.Array.Copy(rcdata, 12 + i * 10 + 1, tempdata, 0, 8);
                                PraseLight4State(ref tempdata);
                                break;

                            case (byte)DEVICE_TYPE.MD_RELAY_2:
                                { ListViewItem item = new ListViewItem("<2·�̵���ģ��״̬>" + "�豸���:" + String.Format("{0:D2}", (byte)rcdata[11 + i * 10])); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
                                lvLinePos++;
                                System.Array.Copy(rcdata, 12 + i * 10 + 1, tempdata, 0, 8);
                                PraseRelay2State(ref tempdata);
                                break;

                            case (byte)DEVICE_TYPE.MD_LEDV12_3:
                                { ListViewItem item = new ListViewItem("<12VLED����ģ��״̬>" + "�豸���:" + String.Format("{0:D2}", (byte)rcdata[11 + i * 10])); item.SubItems.Add(""); rcuStateQuq.lvRcuStateInfo.Items.Add(item); }
                                lvLinePos++;
                                System.Array.Copy(rcdata, 12 + i * 10 + 1, tempdata, 0, 8);
                                PraseLed3State(ref tempdata);
                                break;

                            default:
                                break;
                        }
                    }
                }

            }
            ClosePort();
        }

        private void btnRcuState_Click(object sender, EventArgs e)
        {
            btnReadRcuState();
            rcuStateQuq.Visible = false;
            rcuStateQuq.Focus();
            rcuStateQuq.ShowDialog();
        }
    }

}
