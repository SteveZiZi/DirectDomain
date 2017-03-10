namespace CJQTest
{
    partial class FrmConfig
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.panel1 = new System.Windows.Forms.Panel();
            this.Information = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.chkRS485 = new System.Windows.Forms.CheckBox();
            this.comRedWire_stopbit = new System.Windows.Forms.ComboBox();
            this.label9 = new System.Windows.Forms.Label();
            this.comRedWire_checkbit = new System.Windows.Forms.ComboBox();
            this.label10 = new System.Windows.Forms.Label();
            this.comRedWire_bps = new System.Windows.Forms.ComboBox();
            this.label11 = new System.Windows.Forms.Label();
            this.comRedWire_no = new System.Windows.Forms.ComboBox();
            this.label12 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.txtmeter4 = new System.Windows.Forms.TextBox();
            this.chk4853 = new System.Windows.Forms.CheckBox();
            this.allchecked = new System.Windows.Forms.CheckBox();
            this.label13 = new System.Windows.Forms.Label();
            this.chkRedWire = new System.Windows.Forms.CheckBox();
            this.txtmeter3 = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.txtmeter2 = new System.Windows.Forms.TextBox();
            this.lable = new System.Windows.Forms.Label();
            this.txtmeter1 = new System.Windows.Forms.TextBox();
            this.chk4852 = new System.Windows.Forms.CheckBox();
            this.chk4851 = new System.Windows.Forms.CheckBox();
            this.chkWatchDog = new System.Windows.Forms.CheckBox();
            this.chkGPRS = new System.Windows.Forms.CheckBox();
            this.chkDown = new System.Windows.Forms.CheckBox();
            this.chkBattery = new System.Windows.Forms.CheckBox();
            this.chkSysTime = new System.Windows.Forms.CheckBox();
            this.save = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.com_stopbit = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.com_checkbit = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.com_bps = new System.Windows.Forms.ComboBox();
            this.label7 = new System.Windows.Forms.Label();
            this.com_no = new System.Windows.Forms.ComboBox();
            this.label8 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.chkNet = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.txtPort = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.txtIP = new System.Windows.Forms.TextBox();
            this.panel1.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "config.xls";
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.panel1.Controls.Add(this.Information);
            this.panel1.Controls.Add(this.groupBox4);
            this.panel1.Controls.Add(this.groupBox1);
            this.panel1.Controls.Add(this.save);
            this.panel1.Controls.Add(this.groupBox3);
            this.panel1.Controls.Add(this.groupBox2);
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(684, 591);
            this.panel1.TabIndex = 0;
            // 
            // Information
            // 
            this.Information.AutoSize = true;
            this.Information.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Information.ForeColor = System.Drawing.Color.Black;
            this.Information.Location = new System.Drawing.Point(450, 523);
            this.Information.Name = "Information";
            this.Information.Size = new System.Drawing.Size(0, 14);
            this.Information.TabIndex = 14;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.chkRS485);
            this.groupBox4.Controls.Add(this.comRedWire_stopbit);
            this.groupBox4.Controls.Add(this.label9);
            this.groupBox4.Controls.Add(this.comRedWire_checkbit);
            this.groupBox4.Controls.Add(this.label10);
            this.groupBox4.Controls.Add(this.comRedWire_bps);
            this.groupBox4.Controls.Add(this.label11);
            this.groupBox4.Controls.Add(this.comRedWire_no);
            this.groupBox4.Controls.Add(this.label12);
            this.groupBox4.Location = new System.Drawing.Point(34, 123);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(611, 59);
            this.groupBox4.TabIndex = 13;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "RS485端口：";
            // 
            // chkRS485
            // 
            this.chkRS485.AutoSize = true;
            this.chkRS485.Location = new System.Drawing.Point(6, 31);
            this.chkRS485.Name = "chkRS485";
            this.chkRS485.Size = new System.Drawing.Size(48, 16);
            this.chkRS485.TabIndex = 16;
            this.chkRS485.Text = "有效";
            this.chkRS485.UseVisualStyleBackColor = true;
            this.chkRS485.CheckedChanged += new System.EventHandler(this.chkRS485_CheckedChanged);
            // 
            // comRedWire_stopbit
            // 
            this.comRedWire_stopbit.FormattingEnabled = true;
            this.comRedWire_stopbit.Items.AddRange(new object[] {
            "1",
            "2"});
            this.comRedWire_stopbit.Location = new System.Drawing.Point(564, 29);
            this.comRedWire_stopbit.Name = "comRedWire_stopbit";
            this.comRedWire_stopbit.Size = new System.Drawing.Size(44, 20);
            this.comRedWire_stopbit.TabIndex = 7;
            this.comRedWire_stopbit.Text = "1";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(517, 32);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(53, 12);
            this.label9.TabIndex = 6;
            this.label9.Text = "停止位：";
            // 
            // comRedWire_checkbit
            // 
            this.comRedWire_checkbit.FormattingEnabled = true;
            this.comRedWire_checkbit.Items.AddRange(new object[] {
            "无",
            "奇",
            "偶"});
            this.comRedWire_checkbit.Location = new System.Drawing.Point(433, 29);
            this.comRedWire_checkbit.Name = "comRedWire_checkbit";
            this.comRedWire_checkbit.Size = new System.Drawing.Size(43, 20);
            this.comRedWire_checkbit.TabIndex = 5;
            this.comRedWire_checkbit.Text = "无";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(385, 32);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(53, 12);
            this.label10.TabIndex = 4;
            this.label10.Text = "校验位：";
            // 
            // comRedWire_bps
            // 
            this.comRedWire_bps.FormattingEnabled = true;
            this.comRedWire_bps.Items.AddRange(new object[] {
            "1200",
            "2400",
            "4800",
            "9600",
            "19200",
            "38400",
            "43000",
            "56000",
            "57600",
            "115200"});
            this.comRedWire_bps.Location = new System.Drawing.Point(288, 29);
            this.comRedWire_bps.Name = "comRedWire_bps";
            this.comRedWire_bps.Size = new System.Drawing.Size(65, 20);
            this.comRedWire_bps.TabIndex = 3;
            this.comRedWire_bps.Text = "1200";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(229, 32);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(53, 12);
            this.label11.TabIndex = 2;
            this.label11.Text = "波特率：";
            // 
            // comRedWire_no
            // 
            this.comRedWire_no.FormattingEnabled = true;
            this.comRedWire_no.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10",
            "11",
            "12",
            "13",
            "14",
            "15",
            "16",
            "17",
            "18",
            "19",
            "20",
            "21",
            "22",
            "23",
            "24",
            "25",
            "26",
            "27",
            "28",
            "29",
            "30"});
            this.comRedWire_no.Location = new System.Drawing.Point(153, 29);
            this.comRedWire_no.Name = "comRedWire_no";
            this.comRedWire_no.Size = new System.Drawing.Size(47, 20);
            this.comRedWire_no.TabIndex = 1;
            this.comRedWire_no.Text = "1";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(105, 32);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(53, 12);
            this.label12.TabIndex = 0;
            this.label12.Text = "端口号：";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.txtmeter4);
            this.groupBox1.Controls.Add(this.chk4853);
            this.groupBox1.Controls.Add(this.allchecked);
            this.groupBox1.Controls.Add(this.label13);
            this.groupBox1.Controls.Add(this.chkRedWire);
            this.groupBox1.Controls.Add(this.txtmeter3);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.txtmeter2);
            this.groupBox1.Controls.Add(this.lable);
            this.groupBox1.Controls.Add(this.txtmeter1);
            this.groupBox1.Controls.Add(this.chk4852);
            this.groupBox1.Controls.Add(this.chk4851);
            this.groupBox1.Controls.Add(this.chkWatchDog);
            this.groupBox1.Controls.Add(this.chkGPRS);
            this.groupBox1.Controls.Add(this.chkDown);
            this.groupBox1.Controls.Add(this.chkBattery);
            this.groupBox1.Controls.Add(this.chkSysTime);
            this.groupBox1.Location = new System.Drawing.Point(124, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(530, 38);
            this.groupBox1.TabIndex = 10;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "测试项";
            this.groupBox1.Visible = false;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(101, 165);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(65, 12);
            this.label4.TabIndex = 26;
            this.label4.Text = "采表地址：";
            // 
            // txtmeter4
            // 
            this.txtmeter4.Location = new System.Drawing.Point(172, 162);
            this.txtmeter4.MaxLength = 12;
            this.txtmeter4.Name = "txtmeter4";
            this.txtmeter4.Size = new System.Drawing.Size(100, 21);
            this.txtmeter4.TabIndex = 25;
            this.txtmeter4.Text = "000000000004";
            // 
            // chk4853
            // 
            this.chk4853.AutoSize = true;
            this.chk4853.Location = new System.Drawing.Point(17, 164);
            this.chk4853.Name = "chk4853";
            this.chk4853.Size = new System.Drawing.Size(54, 16);
            this.chk4853.TabIndex = 24;
            this.chk4853.Text = "485-3";
            this.chk4853.UseVisualStyleBackColor = true;
            // 
            // allchecked
            // 
            this.allchecked.AutoSize = true;
            this.allchecked.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.allchecked.Location = new System.Drawing.Point(453, 202);
            this.allchecked.Name = "allchecked";
            this.allchecked.Size = new System.Drawing.Size(54, 18);
            this.allchecked.TabIndex = 23;
            this.allchecked.Text = "全选";
            this.allchecked.UseVisualStyleBackColor = true;
            this.allchecked.CheckedChanged += new System.EventHandler(this.allchecked_CheckedChanged);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(142, 203);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(65, 12);
            this.label13.TabIndex = 22;
            this.label13.Text = "采表地址：";
            // 
            // chkRedWire
            // 
            this.chkRedWire.AutoSize = true;
            this.chkRedWire.Location = new System.Drawing.Point(376, 111);
            this.chkRedWire.Name = "chkRedWire";
            this.chkRedWire.Size = new System.Drawing.Size(60, 16);
            this.chkRedWire.TabIndex = 20;
            this.chkRedWire.Text = "远红外";
            this.chkRedWire.UseVisualStyleBackColor = true;
            // 
            // txtmeter3
            // 
            this.txtmeter3.Location = new System.Drawing.Point(213, 200);
            this.txtmeter3.MaxLength = 12;
            this.txtmeter3.Name = "txtmeter3";
            this.txtmeter3.Size = new System.Drawing.Size(100, 21);
            this.txtmeter3.TabIndex = 18;
            this.txtmeter3.Text = "000000000003";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(101, 125);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(65, 12);
            this.label3.TabIndex = 17;
            this.label3.Text = "采表地址：";
            // 
            // txtmeter2
            // 
            this.txtmeter2.Location = new System.Drawing.Point(172, 122);
            this.txtmeter2.MaxLength = 12;
            this.txtmeter2.Name = "txtmeter2";
            this.txtmeter2.Size = new System.Drawing.Size(100, 21);
            this.txtmeter2.TabIndex = 16;
            this.txtmeter2.Text = "000000000002";
            // 
            // lable
            // 
            this.lable.AutoSize = true;
            this.lable.Location = new System.Drawing.Point(101, 79);
            this.lable.Name = "lable";
            this.lable.Size = new System.Drawing.Size(65, 12);
            this.lable.TabIndex = 15;
            this.lable.Text = "采表地址：";
            // 
            // txtmeter1
            // 
            this.txtmeter1.Location = new System.Drawing.Point(172, 77);
            this.txtmeter1.MaxLength = 12;
            this.txtmeter1.Name = "txtmeter1";
            this.txtmeter1.Size = new System.Drawing.Size(100, 21);
            this.txtmeter1.TabIndex = 14;
            this.txtmeter1.Text = "000000000001";
            // 
            // chk4852
            // 
            this.chk4852.AutoSize = true;
            this.chk4852.Location = new System.Drawing.Point(17, 124);
            this.chk4852.Name = "chk4852";
            this.chk4852.Size = new System.Drawing.Size(54, 16);
            this.chk4852.TabIndex = 10;
            this.chk4852.Text = "485-2";
            this.chk4852.UseVisualStyleBackColor = true;
            // 
            // chk4851
            // 
            this.chk4851.AutoSize = true;
            this.chk4851.Location = new System.Drawing.Point(20, 79);
            this.chk4851.Name = "chk4851";
            this.chk4851.Size = new System.Drawing.Size(54, 16);
            this.chk4851.TabIndex = 9;
            this.chk4851.Text = "485-1";
            this.chk4851.UseVisualStyleBackColor = true;
            // 
            // chkWatchDog
            // 
            this.chkWatchDog.AutoSize = true;
            this.chkWatchDog.Location = new System.Drawing.Point(329, 21);
            this.chkWatchDog.Name = "chkWatchDog";
            this.chkWatchDog.Size = new System.Drawing.Size(60, 16);
            this.chkWatchDog.TabIndex = 8;
            this.chkWatchDog.Text = "看门狗";
            this.chkWatchDog.UseVisualStyleBackColor = true;
            this.chkWatchDog.CheckedChanged += new System.EventHandler(this.chkWatchDog_CheckedChanged);
            // 
            // chkGPRS
            // 
            this.chkGPRS.AutoSize = true;
            this.chkGPRS.Enabled = false;
            this.chkGPRS.Location = new System.Drawing.Point(376, 75);
            this.chkGPRS.Name = "chkGPRS";
            this.chkGPRS.Size = new System.Drawing.Size(72, 16);
            this.chkGPRS.TabIndex = 6;
            this.chkGPRS.Text = "远程模块";
            this.chkGPRS.UseVisualStyleBackColor = true;
            this.chkGPRS.Visible = false;
            // 
            // chkDown
            // 
            this.chkDown.AutoSize = true;
            this.chkDown.Location = new System.Drawing.Point(17, 202);
            this.chkDown.Name = "chkDown";
            this.chkDown.Size = new System.Drawing.Size(96, 16);
            this.chkDown.TabIndex = 5;
            this.chkDown.Text = "本地通讯模块";
            this.chkDown.UseVisualStyleBackColor = true;
            // 
            // chkBattery
            // 
            this.chkBattery.AutoSize = true;
            this.chkBattery.Location = new System.Drawing.Point(169, 20);
            this.chkBattery.Name = "chkBattery";
            this.chkBattery.Size = new System.Drawing.Size(48, 16);
            this.chkBattery.TabIndex = 1;
            this.chkBattery.Text = "电池";
            this.chkBattery.UseVisualStyleBackColor = true;
            // 
            // chkSysTime
            // 
            this.chkSysTime.AutoSize = true;
            this.chkSysTime.Location = new System.Drawing.Point(20, 20);
            this.chkSysTime.Name = "chkSysTime";
            this.chkSysTime.Size = new System.Drawing.Size(72, 16);
            this.chkSysTime.TabIndex = 0;
            this.chkSysTime.Text = "系统时钟";
            this.chkSysTime.UseVisualStyleBackColor = true;
            // 
            // save
            // 
            this.save.Location = new System.Drawing.Point(325, 520);
            this.save.Name = "save";
            this.save.Size = new System.Drawing.Size(75, 26);
            this.save.TabIndex = 13;
            this.save.Text = "保存";
            this.save.UseVisualStyleBackColor = true;
            this.save.Click += new System.EventHandler(this.save_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.com_stopbit);
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Controls.Add(this.com_checkbit);
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Controls.Add(this.com_bps);
            this.groupBox3.Controls.Add(this.label7);
            this.groupBox3.Controls.Add(this.com_no);
            this.groupBox3.Controls.Add(this.label8);
            this.groupBox3.Location = new System.Drawing.Point(124, 342);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(530, 59);
            this.groupBox3.TabIndex = 12;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "看门狗检测端口：";
            // 
            // com_stopbit
            // 
            this.com_stopbit.FormattingEnabled = true;
            this.com_stopbit.Items.AddRange(new object[] {
            "1",
            "2"});
            this.com_stopbit.Location = new System.Drawing.Point(477, 26);
            this.com_stopbit.Name = "com_stopbit";
            this.com_stopbit.Size = new System.Drawing.Size(44, 20);
            this.com_stopbit.TabIndex = 7;
            this.com_stopbit.Text = "1";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(430, 29);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(53, 12);
            this.label5.TabIndex = 6;
            this.label5.Text = "停止位：";
            // 
            // com_checkbit
            // 
            this.com_checkbit.FormattingEnabled = true;
            this.com_checkbit.Items.AddRange(new object[] {
            "无",
            "奇",
            "偶"});
            this.com_checkbit.Location = new System.Drawing.Point(346, 26);
            this.com_checkbit.Name = "com_checkbit";
            this.com_checkbit.Size = new System.Drawing.Size(43, 20);
            this.com_checkbit.TabIndex = 5;
            this.com_checkbit.Text = "无";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(298, 29);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(53, 12);
            this.label6.TabIndex = 4;
            this.label6.Text = "校验位：";
            // 
            // com_bps
            // 
            this.com_bps.FormattingEnabled = true;
            this.com_bps.Items.AddRange(new object[] {
            "1200",
            "2400",
            "4800",
            "9600",
            "19200",
            "38400",
            "43000",
            "56000",
            "57600",
            "115200"});
            this.com_bps.Location = new System.Drawing.Point(201, 26);
            this.com_bps.Name = "com_bps";
            this.com_bps.Size = new System.Drawing.Size(65, 20);
            this.com_bps.TabIndex = 3;
            this.com_bps.Text = "115200";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(142, 29);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(53, 12);
            this.label7.TabIndex = 2;
            this.label7.Text = "波特率：";
            // 
            // com_no
            // 
            this.com_no.FormattingEnabled = true;
            this.com_no.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10",
            "11",
            "12",
            "13",
            "14",
            "15",
            "16",
            "17",
            "18",
            "19",
            "20",
            "21",
            "22",
            "23",
            "24",
            "25",
            "26",
            "27",
            "28",
            "29",
            "30"});
            this.com_no.Location = new System.Drawing.Point(66, 26);
            this.com_no.Name = "com_no";
            this.com_no.Size = new System.Drawing.Size(47, 20);
            this.com_no.TabIndex = 1;
            this.com_no.Text = "1";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(18, 29);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(53, 12);
            this.label8.TabIndex = 0;
            this.label8.Text = "端口号：";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.chkNet);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.txtPort);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.txtIP);
            this.groupBox2.Location = new System.Drawing.Point(34, 51);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(611, 59);
            this.groupBox2.TabIndex = 11;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "网络端口：";
            this.groupBox2.Enter += new System.EventHandler(this.groupBox2_Enter);
            // 
            // chkNet
            // 
            this.chkNet.AutoSize = true;
            this.chkNet.Location = new System.Drawing.Point(6, 25);
            this.chkNet.Name = "chkNet";
            this.chkNet.Size = new System.Drawing.Size(48, 16);
            this.chkNet.TabIndex = 15;
            this.chkNet.Text = "有效";
            this.chkNet.UseVisualStyleBackColor = true;
            this.chkNet.CheckedChanged += new System.EventHandler(this.chkNet_CheckedChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(493, 23);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "端口号：";
            // 
            // txtPort
            // 
            this.txtPort.Location = new System.Drawing.Point(552, 20);
            this.txtPort.Name = "txtPort";
            this.txtPort.Size = new System.Drawing.Size(54, 21);
            this.txtPort.TabIndex = 2;
            this.txtPort.Text = "10000";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(244, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 12);
            this.label1.TabIndex = 1;
            this.label1.Text = "IP地址：";
            // 
            // txtIP
            // 
            this.txtIP.Location = new System.Drawing.Point(303, 20);
            this.txtIP.Name = "txtIP";
            this.txtIP.Size = new System.Drawing.Size(115, 21);
            this.txtIP.TabIndex = 0;
            this.txtIP.Text = "192.168.1.100";
            // 
            // FrmConfig
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Ivory;
            this.ClientSize = new System.Drawing.Size(676, 292);
            this.Controls.Add(this.panel1);
            this.Name = "FrmConfig";
            this.Text = "通讯参数配置";
            this.Load += new System.EventHandler(this.FrmConfig_Load);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button save;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        public System.Windows.Forms.CheckBox chkGPRS;
        public System.Windows.Forms.CheckBox chkDown;
        public System.Windows.Forms.CheckBox chkBattery;
        public System.Windows.Forms.CheckBox chkSysTime;
        public System.Windows.Forms.CheckBox chk4852;
        public System.Windows.Forms.CheckBox chk4851;
        public System.Windows.Forms.CheckBox chkWatchDog;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label lable;
        public System.Windows.Forms.TextBox txtmeter3;
        public System.Windows.Forms.TextBox txtmeter2;
        public System.Windows.Forms.TextBox txtmeter1;
        private System.Windows.Forms.GroupBox groupBox4;
        public System.Windows.Forms.ComboBox comRedWire_stopbit;
        private System.Windows.Forms.Label label9;
        public System.Windows.Forms.ComboBox comRedWire_checkbit;
        private System.Windows.Forms.Label label10;
        public System.Windows.Forms.ComboBox comRedWire_bps;
        private System.Windows.Forms.Label label11;
        public System.Windows.Forms.ComboBox comRedWire_no;
        private System.Windows.Forms.Label label12;
        public System.Windows.Forms.CheckBox chkRedWire;
        private System.Windows.Forms.Label label13;
        public System.Windows.Forms.TextBox txtIP;
        public System.Windows.Forms.TextBox txtPort;
        public System.Windows.Forms.ComboBox com_stopbit;
        public System.Windows.Forms.ComboBox com_checkbit;
        public System.Windows.Forms.ComboBox com_bps;
        public System.Windows.Forms.ComboBox com_no;
        public System.Windows.Forms.CheckBox allchecked;
        private System.Windows.Forms.Label Information;
        private System.Windows.Forms.Label label4;
        public System.Windows.Forms.TextBox txtmeter4;
        public System.Windows.Forms.CheckBox chk4853;
        public System.Windows.Forms.CheckBox chkRS485;
        public System.Windows.Forms.CheckBox chkNet;
    }
}