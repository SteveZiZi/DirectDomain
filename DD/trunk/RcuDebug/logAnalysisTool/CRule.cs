using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Collections;
using System.IO.Ports;
using System.Text.RegularExpressions;


namespace CJQTest
{
    /// <summary>
    /// ���������ָ����֡ͷ��֡β��
    /// </summary>
    public enum ParseResult
    {
        /// <summary>
        /// ��ȷ�ı���
        /// </summary>
        OK = 0,
        /// <summary>
        /// ������ʱδ֪��;
        /// </summary>
        Waitting = 1,
        /// <summary>
        /// �������ı���
        /// </summary>
        Unintegrated = 2,
        /// <summary>
        /// ����ı��� û��68 ��CS����
        /// </summary>
        Error = -1,
    }

      public class CRule
    {
        virtual public bool Makeframes(ref ArrayList frame, byte[] addr, byte ctl, byte[] data){return true;}
	 virtual public bool Makeframes(ref ArrayList frame, byte[] addr, byte ctl, byte[] data,int length){return true;}
        virtual public bool Makeframes(ref ArrayList frame, byte[] addr, byte AFN, bool pw, int pwLen, byte[] pn, byte[] fn, byte[] data){return true;}
        virtual public bool Makeframes(ref ArrayList frame, byte[] addr, byte AFN, bool pw, int pwLen, byte[] pn, byte[] fn) { return true; }

        virtual public ParseResult ParsePackage(ref ArrayList irReceived, ref Frame_Stu ruleinfo, ref int startindex, ref int endindex) { return ParseResult.OK; }

    }
    public class CGwRule : CRule
    {
        public const int GW_PW_LEN = 16;
        public const int GX_PW_LEN = 2;
        /// <summary>
        /// ����У���
        /// </summary>
        /// <returns></returns>
        public byte getCS(ArrayList frame)
        {
            byte temp = 0;
            foreach (byte item in frame)
                temp += item;
            return temp;
        }

        public byte getCS(Array frame)
        {
            byte temp = 0;
            foreach (byte item in frame)
                temp += item;
            return temp;
        }
        public override ParseResult ParsePackage(ref ArrayList irReceived, ref Frame_Stu ruleinfo, ref int startindex, ref int endindex)
        {
            ruleinfo.dataArray = new ArrayList();
            ArrayList temp = new ArrayList();
            temp.AddRange(irReceived);
            //�����в������κ����ݶ�ʱ����̳���Ϊ15�ֽ�
            if (irReceived.Count <= 15)
            {
                startindex = 0;
                endindex = 0;
                return ParseResult.Unintegrated;
            }
            int indexof68 = 0;

            while (startindex < irReceived.Count)
            {
                //������ʼ0x68
                startindex = irReceived.IndexOf((byte)0x68, indexof68);
                if (startindex < 0)
                    return ParseResult.Error;

                endindex = startindex;
                indexof68 = startindex;

                if ((byte)irReceived[startindex + 5] != 0x68)
                {
                    irReceived.RemoveRange(0, 1);
                    continue;
                }
                else
                    endindex = startindex + 5;//endindexָ���˵ڶ���0x68

                int length = 0;
                length = Convert.ToInt32((byte)irReceived[startindex + 2]);
                length <<= 6;
                length |= Convert.ToInt32(((byte)irReceived[startindex + 1]) >> 2);

                //�����յ��Ĳ�����      ֡��ʼ����  ͷ  ���ݳ���
                if (irReceived.Count < startindex + 5 + length)
                    return ParseResult.Unintegrated;
                else
                    endindex = startindex + 5 + length;

                ruleinfo.ctrl = (byte)irReceived[startindex + 6];

                endindex = startindex + 5 + length;//endindexָ�����û����ݶε�ĩβ
                endindex = endindex + 2;//endindexָ���˱���ĩβ��16
                if (irReceived.Count < endindex)
                    return ParseResult.Unintegrated;

                if ((byte)irReceived[endindex] != 0x16)
                    return ParseResult.Error;

                byte[] addr = new byte[5];
                addr[0] = (byte)irReceived[startindex + 7];
                addr[1] = (byte)irReceived[startindex + 8];
                addr[2] = (byte)irReceived[startindex + 9];
                addr[3] = (byte)irReceived[startindex + 10];
                addr[4] = (byte)irReceived[startindex + 11];
                ruleinfo.afn = (byte)irReceived[startindex + 12];

                Array altemp = irReceived.GetRange(startindex + 6, endindex - startindex - 6 - 1).ToArray(typeof(byte));
                if ((byte)irReceived[endindex - 1] != getCS(altemp))
                    return ParseResult.Error;

                ruleinfo.dataArray.Clear();
                ruleinfo.dataArray.AddRange(irReceived.GetRange(startindex + 18, (endindex - 2) - (startindex + 18) + 1).ToArray(typeof(byte)));
                return ParseResult.OK;
            }
            return ParseResult.Unintegrated;
        }
        /// <summary>
        /// ������֯����
        /// </summary>
        /// <param name="frame">�������������ڼ�¼��֯�õı���</param>
        /// <param name="shm">�����ڴ�����</param>
        /// <returns>trueΪ�ɹ� falseΪʧ��</returns>
        public override bool Makeframes(ref ArrayList frame, byte[] addr, byte ctl, byte[] data)
        {
            ArrayList ftp = new ArrayList();
            frame.Clear();
            frame.Add((byte)0x68);
            for (int i = 0; i < 6; i++)
                frame.Add((byte)addr[i]);
            frame.Add((byte)0x68);

            frame.Add(ctl);

            int framelength = data.Length;
            frame.Add((byte)(framelength));

            for (int j = 0; j < data.Length; j++)
            {
                frame.Add((byte)(data[j]));
            }
            //frame.AddRange(framedatas.GetRange(0, framedatas.Count));
            //foreach (byte bt in framedatas)
            //    frame.Add(bt);

            //frame.Add(getCS(framedatas));
            frame.Add(getCS(frame));
            frame.Add((byte)0x16);

            return true;
        }
	 public override bool Makeframes(ref ArrayList frame, byte[] addr, byte ctl, byte[] data,int length)
        {
            ArrayList ftp = new ArrayList();
            frame.Clear();
            frame.Add((byte)0x68);
            for (int i = 0; i < 6; i++)
                frame.Add((byte)addr[i]);
            frame.Add((byte)0x68);

            frame.Add(ctl);

            int framelength = length;
            frame.Add((byte)(framelength));

            for (int j = 0; j < length; j++)
            {
                frame.Add((byte)(data[j]));
            }
            //frame.AddRange(framedatas.GetRange(0, framedatas.Count));
            //foreach (byte bt in framedatas)
            //    frame.Add(bt);

            //frame.Add(getCS(framedatas));
            frame.Add(getCS(frame));
            frame.Add((byte)0x16);

            return true;
        }

        /*
        68
        E2 00 E2 00 ���ĳ���
        68 
        4B ������
        00 51 02 00 �������߼���ַBCD��
        02 		��վ��ţ�������Ŀǰ����֤���ַ���
        04 	AFN ������ 04Ϊ���ò���
        70 SEQ
        00 00 04 00	������PN=0 FN=3
        */
        public override bool Makeframes(ref ArrayList frame, byte[] addr, byte AFN, bool pw, int pwLen, byte[] pn, byte[] fn, byte[] data)
        {
            ArrayList ftp = new ArrayList();
            frame.Clear();
            frame.Add((byte)0x68);
            frame.Add((byte)0x68);

            frame.Add((byte)0x4B);  // ������
            /* ��������ַ */
            frame.AddRange(addr);
            /* ��վ��� */
            frame.Add((byte)0x02);
            /* AFN */
            frame.Add(AFN);
            /* MSTA&SEQ */
            frame.Add((byte)0x70);
            /* PN */
            frame.AddRange(pn);
            /* FN */
            frame.AddRange(fn);
            /* data */
            frame.AddRange(data);
            /* PW */
            if (pw == true)
            {
                for (int n = 0; n < pwLen; n++)
                {
                    frame.Add((byte)0x00);
                }
            }

            int len = frame.Count;
            if (pwLen == GW_PW_LEN)
            {
                len = ((len - 2) << 2) | 0x02;
            }
            else
            {
                len = ((len - 2) << 2) | 0x01;
            }
            frame.Insert(1, (byte)(len & 0xff));
            frame.Insert(2, (byte)((len >> 8) & 0xff));
            frame.Insert(3, (byte)(len & 0xff));
            frame.Insert(4, (byte)((len >> 8) & 0xff));

            frame.Add(getCS(frame.GetRange(6, frame.Count - 6)));
            frame.Add((byte)0x16);

            return true;
        }


        public override bool Makeframes(ref ArrayList frame, byte[] addr, byte AFN, bool pw, int pwLen, byte[] pn, byte[] fn)
        {
            ArrayList ftp = new ArrayList();
            frame.Clear();
            frame.Add((byte)0x68);
            frame.Add((byte)0x68);

            frame.Add((byte)0x4B);  // ������
            /* ��������ַ */
            frame.AddRange(addr);
            /* ��վ��� */
            frame.Add((byte)0x02);
            /* AFN */
            frame.Add(AFN);
            /* MSTA&SEQ */
            frame.Add((byte)0x70);
            /* PN */
            frame.AddRange(pn);
            /* FN */
            frame.AddRange(fn);
            ///* PW */
            if (pw == true)
            {
                for (int n = 0; n < pwLen; n++)
                {
                    frame.Add((byte)0x00);
                }
            }
            int len = frame.Count;
            if (pwLen == GW_PW_LEN)
            {
                len = ((len - 2) << 2) | 0x02;
            }
            else
            {
                len = ((len - 2) << 2) | 0x01;
            }
            frame.Insert(1, (byte)(len & 0xff));
            frame.Insert(2, (byte)((len >> 8) & 0xff));
            frame.Insert(3, (byte)(len & 0xff));
            frame.Insert(4, (byte)((len >> 8) & 0xff));

            frame.Add(getCS(frame.GetRange(6, frame.Count - 6)));
            frame.Add((byte)0x16);

            return true;
        }

    }

    public class CGdRule : CRule
    {
      
        public byte getCS(ArrayList frame)
        {
            byte temp = 0;
            foreach (byte item in frame)
                temp += item;
            return temp;
        }

        public byte getCS(Array frame)
        {
            byte temp = 0;
            foreach (byte item in frame)
                temp += item;
            return temp;
        }
        public override ParseResult ParsePackage(ref ArrayList irReceived, ref Frame_Stu ruleinfo, ref int startindex, ref int endindex)
        {
            ruleinfo.dataArray = new ArrayList();
            ArrayList temp = new ArrayList();
            temp.AddRange(irReceived);
            //�����в������κ����ݶ�ʱ����̳���Ϊ13�ֽ�
            if (irReceived.Count <= 13)
            {
                startindex = 0;
                endindex = 0;
                return ParseResult.Error;
            }
            int indexof68 = 0;

            while (startindex < irReceived.Count)
            {
                //������ʼ0x68
                startindex = irReceived.IndexOf((byte)0x68, indexof68);
                if (startindex < 0)
                    return ParseResult.Error;

                endindex = startindex;
                indexof68 = startindex;

                if ((byte)irReceived[startindex + 7] != 0x68)
                {
                    irReceived.RemoveRange(0, 1);
                    continue;
                }
                else
                    endindex = startindex + 7;//endindexָ���˵ڶ���0x68

                int length = 0;
                length = Convert.ToInt32((byte)irReceived[endindex + 2]);
                length += Convert.ToInt32((byte)irReceived[endindex + 3]); ;

                //�����յ��Ĳ�����      ֡��ʼ����  ͷ  ���ݳ���
                if (irReceived.Count < startindex + 13 + length)
                    return ParseResult.Unintegrated;
                else
                    endindex = startindex + 13 + length-1;

                ruleinfo.ctrl = (byte)irReceived[startindex + 8];

                //endindex = startindex + 5 + length;//endindexָ�����û����ݶε�ĩβ
                //endindex = endindex + 2;//endindexָ���˱���ĩβ��16
                //if (irReceived.Count < endindex)
                //    return ParseResult.Unintegrated;

                if ((byte)irReceived[endindex] != 0x16)
                    return ParseResult.Error;

                //byte[] addr = new byte[5];
                //addr[0] = (byte)irReceived[startindex + 7];
                //addr[1] = (byte)irReceived[startindex + 8];
                //addr[2] = (byte)irReceived[startindex + 9];
                //addr[3] = (byte)irReceived[startindex + 10];
                //addr[4] = (byte)irReceived[startindex + 11];
                //ruleinfo.afn = (byte)irReceived[startindex + 12];

                Array altemp = irReceived.GetRange(startindex, endindex-startindex-1).ToArray(typeof(byte));
                if ((byte)irReceived[endindex - 1] != getCS(altemp))
                    return ParseResult.Error;

                ruleinfo.dataArray.Clear();
                ruleinfo.dataArray.AddRange(irReceived.GetRange(startindex + 11, length).ToArray(typeof(byte)));
                return ParseResult.OK;
            }
            return ParseResult.Unintegrated;
        }
        /// <summary>
        /// ������֯����
        /// </summary>
        /// <param name="frame">�������������ڼ�¼��֯�õı���</param>
        /// <param name="shm">�����ڴ�����</param>
        /// <returns>trueΪ�ɹ� falseΪʧ��</returns>
        /// 68 81 41 09 00 9E 00 68 01 0A 00 01 00 00 00 00 00 00 00 30 80 F5 16 
        public override bool Makeframes(ref ArrayList frame, byte[] addr, byte ctl, byte[] data)
        {
            ArrayList ftp = new ArrayList();
            int FSEQ = 1;

            frame.Clear();
            frame.Add((byte)0x68);
            for (int i = 0; i < 4; i++)
            {
                frame.Add((byte)addr[i]);
            }

            int nMSTA = 30 + (FSEQ << 6);	//��д��վ��ַ���������
            if (FSEQ == 0X7F) //ѭ���������
            {
                FSEQ = 0X01;
            }
            else
            {
                FSEQ++;
            }
            frame.Add((byte)(nMSTA & 0XFF));
            frame.Add((byte)((nMSTA >> 8) & 0XFF));

            frame.Add((byte)0x68);

            frame.Add(ctl);

            int framelength = data.Length;
            frame.Add((byte)(framelength&0xff));
            frame.Add((byte)((framelength>>8)&0xff));

            for (int j = 0; j < data.Length; j++)
            {
                frame.Add((byte)(data[j]));
            }
            //frame.AddRange(framedatas.GetRange(0, framedatas.Count));
            //foreach (byte bt in framedatas)
            //    frame.Add(bt);

            //frame.Add(getCS(framedatas));
            frame.Add(getCS(frame));
            frame.Add((byte)0x16);

            return true;
        }
        public override bool Makeframes(ref ArrayList frame, byte[] addr, byte ctl, byte[] data, int Length)
        {
            ArrayList ftp = new ArrayList();
            int FSEQ = 1;

            frame.Clear();
            frame.Add((byte)0x68);
            for (int i = 0; i < 4; i++)
            {
                frame.Add((byte)addr[i]);
            }

            int nMSTA = 30 + (FSEQ << 6);	//��д��վ��ַ���������
            if (FSEQ == 0X7F) //ѭ���������
            {
                FSEQ = 0X01;
            }
            else
            {
                FSEQ++;
            }
            frame.Add((byte)(nMSTA & 0XFF));
            frame.Add((byte)((nMSTA >> 8) & 0XFF));

            frame.Add((byte)0x68);

            frame.Add(ctl);

            int framelength = Length;
            frame.Add((byte)(framelength & 0xff));
            frame.Add((byte)((framelength >> 8) & 0xff));

            for (int j = 0; j < Length; j++)
            {
                frame.Add((byte)(data[j]));
            }
            //frame.AddRange(framedatas.GetRange(0, framedatas.Count));
            //foreach (byte bt in framedatas)
            //    frame.Add(bt);

            //frame.Add(getCS(framedatas));
            frame.Add(getCS(frame));
            frame.Add((byte)0x16);

            return true;
        }
    }
}
