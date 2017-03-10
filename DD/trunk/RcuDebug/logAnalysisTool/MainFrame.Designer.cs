namespace CJQTest
{
    partial class MainFrame
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
            this.components = new System.ComponentModel.Container();
            this.button3 = new System.Windows.Forms.Button();
            this.saveResult = new System.Windows.Forms.Button();
            this.config = new System.Windows.Forms.Button();
            this.btnInfo = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.Stop = new System.Windows.Forms.Button();
            this.Start = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.txtAddr = new System.Windows.Forms.TextBox();
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.label4 = new System.Windows.Forms.Label();
            this.btnTool = new System.Windows.Forms.Button();
            this.btnLoad = new System.Windows.Forms.Button();
            this.btnParamterLoad = new System.Windows.Forms.Button();
            this.btnClear = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btnReadSet = new System.Windows.Forms.Button();
            this.btnSetAction = new System.Windows.Forms.Button();
            this.btnSet = new System.Windows.Forms.Button();
            this.lvRunInfo = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.btnQuere = new System.Windows.Forms.Button();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.btnIrdtSet = new System.Windows.Forms.Button();
            this.btnIrdtEdit = new System.Windows.Forms.Button();
            this.btnDoorSet = new System.Windows.Forms.Button();
            this.DSScenebtn = new System.Windows.Forms.Button();
            this.btnSceneSet = new System.Windows.Forms.Button();
            this.btnScene = new System.Windows.Forms.Button();
            this.btnDrSet = new System.Windows.Forms.Button();
            this.btnDoorcardScene = new System.Windows.Forms.Button();
            this.btnPowKeyScene = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.btnCtlCommand = new System.Windows.Forms.Button();
            this.txtParam = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label26 = new System.Windows.Forms.Label();
            this.cmbFunc = new System.Windows.Forms.ComboBox();
            this.txtSubno = new System.Windows.Forms.TextBox();
            this.label25 = new System.Windows.Forms.Label();
            this.label24 = new System.Windows.Forms.Label();
            this.txtCtlDNo = new System.Windows.Forms.TextBox();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.btnRcuState = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.SuspendLayout();
            // 
            // button3
            // 
            this.button3.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button3.Location = new System.Drawing.Point(619, 3);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(62, 29);
            this.button3.TabIndex = 4;
            this.button3.Text = "说明";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Visible = false;
            this.button3.Click += new System.EventHandler(this.ABout_Click);
            // 
            // saveResult
            // 
            this.saveResult.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.saveResult.Location = new System.Drawing.Point(110, 3);
            this.saveResult.Name = "saveResult";
            this.saveResult.Size = new System.Drawing.Size(91, 29);
            this.saveResult.TabIndex = 2;
            this.saveResult.Text = "保存测试结果";
            this.saveResult.UseVisualStyleBackColor = true;
            this.saveResult.Visible = false;
            this.saveResult.Click += new System.EventHandler(this.saveResult_Click);
            // 
            // config
            // 
            this.config.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.config.Location = new System.Drawing.Point(12, 3);
            this.config.Name = "config";
            this.config.Size = new System.Drawing.Size(77, 29);
            this.config.TabIndex = 0;
            this.config.Text = "通讯参数";
            this.config.UseVisualStyleBackColor = true;
            this.config.Click += new System.EventHandler(this.config_Click);
            // 
            // btnInfo
            // 
            this.btnInfo.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.btnInfo.Location = new System.Drawing.Point(351, 3);
            this.btnInfo.Name = "btnInfo";
            this.btnInfo.Size = new System.Drawing.Size(78, 29);
            this.btnInfo.TabIndex = 1;
            this.btnInfo.Text = "通讯记录";
            this.btnInfo.UseVisualStyleBackColor = true;
            this.btnInfo.Click += new System.EventHandler(this.btnInfo_Click);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // Stop
            // 
            this.Stop.Location = new System.Drawing.Point(94, 864);
            this.Stop.Name = "Stop";
            this.Stop.Size = new System.Drawing.Size(75, 23);
            this.Stop.TabIndex = 7;
            this.Stop.Text = "停止测试";
            this.Stop.UseVisualStyleBackColor = true;
            this.Stop.Visible = false;
            this.Stop.Click += new System.EventHandler(this.Stop_Click);
            // 
            // Start
            // 
            this.Start.Location = new System.Drawing.Point(2, 864);
            this.Start.Name = "Start";
            this.Start.Size = new System.Drawing.Size(75, 23);
            this.Start.TabIndex = 5;
            this.Start.Text = "启动测试";
            this.Start.UseVisualStyleBackColor = true;
            this.Start.Visible = false;
            this.Start.Click += new System.EventHandler(this.Start_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(30, 50);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(70, 14);
            this.label1.TabIndex = 9;
            this.label1.Text = "RCU地址：";
            // 
            // txtAddr
            // 
            this.txtAddr.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.txtAddr.Location = new System.Drawing.Point(127, 47);
            this.txtAddr.MaxLength = 8;
            this.txtAddr.Name = "txtAddr";
            this.txtAddr.Size = new System.Drawing.Size(100, 23);
            this.txtAddr.TabIndex = 8;
            this.txtAddr.Text = "50050001";
            // 
            // serialPort
            // 
            this.serialPort.RtsEnable = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.Location = new System.Drawing.Point(12, 67);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(0, 16);
            this.label2.TabIndex = 10;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(44, 48);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(0, 12);
            this.label3.TabIndex = 11;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(381, 67);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(0, 12);
            this.label4.TabIndex = 14;
            // 
            // btnTool
            // 
            this.btnTool.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.btnTool.Location = new System.Drawing.Point(435, 3);
            this.btnTool.Name = "btnTool";
            this.btnTool.Size = new System.Drawing.Size(96, 29);
            this.btnTool.TabIndex = 5;
            this.btnTool.Text = "工具";
            this.btnTool.UseVisualStyleBackColor = true;
            this.btnTool.Visible = false;
            this.btnTool.Click += new System.EventHandler(this.button2_Click_1);
            // 
            // btnLoad
            // 
            this.btnLoad.Location = new System.Drawing.Point(29, 19);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(88, 23);
            this.btnLoad.TabIndex = 25;
            this.btnLoad.Text = "编辑设备档案";
            this.btnLoad.UseVisualStyleBackColor = true;
            this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
            // 
            // btnParamterLoad
            // 
            this.btnParamterLoad.Location = new System.Drawing.Point(360, 19);
            this.btnParamterLoad.Name = "btnParamterLoad";
            this.btnParamterLoad.Size = new System.Drawing.Size(75, 23);
            this.btnParamterLoad.TabIndex = 26;
            this.btnParamterLoad.Text = "档案下发";
            this.btnParamterLoad.UseVisualStyleBackColor = true;
            this.btnParamterLoad.Click += new System.EventHandler(this.btnParamterLoad_Click);
            // 
            // btnClear
            // 
            this.btnClear.Location = new System.Drawing.Point(185, 19);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(75, 23);
            this.btnClear.TabIndex = 27;
            this.btnClear.Text = "档案清除";
            this.btnClear.UseVisualStyleBackColor = true;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnLoad);
            this.groupBox1.Controls.Add(this.btnParamterLoad);
            this.groupBox1.Controls.Add(this.btnClear);
            this.groupBox1.Location = new System.Drawing.Point(33, 82);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(460, 48);
            this.groupBox1.TabIndex = 28;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "档案管理";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btnReadSet);
            this.groupBox2.Controls.Add(this.btnSetAction);
            this.groupBox2.Controls.Add(this.btnSet);
            this.groupBox2.Location = new System.Drawing.Point(33, 152);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(460, 54);
            this.groupBox2.TabIndex = 29;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "RCU参数设置";
            this.groupBox2.Visible = false;
            // 
            // btnReadSet
            // 
            this.btnReadSet.Location = new System.Drawing.Point(299, 20);
            this.btnReadSet.Name = "btnReadSet";
            this.btnReadSet.Size = new System.Drawing.Size(75, 23);
            this.btnReadSet.TabIndex = 2;
            this.btnReadSet.Text = "读参数";
            this.btnReadSet.UseVisualStyleBackColor = true;
            this.btnReadSet.Click += new System.EventHandler(this.btnReadSet_Click);
            // 
            // btnSetAction
            // 
            this.btnSetAction.Location = new System.Drawing.Point(185, 20);
            this.btnSetAction.Name = "btnSetAction";
            this.btnSetAction.Size = new System.Drawing.Size(75, 23);
            this.btnSetAction.TabIndex = 1;
            this.btnSetAction.Text = "设置参数";
            this.btnSetAction.UseVisualStyleBackColor = true;
            this.btnSetAction.Click += new System.EventHandler(this.btnSetAction_Click);
            // 
            // btnSet
            // 
            this.btnSet.Location = new System.Drawing.Point(29, 20);
            this.btnSet.Name = "btnSet";
            this.btnSet.Size = new System.Drawing.Size(88, 23);
            this.btnSet.TabIndex = 0;
            this.btnSet.Text = "参数编辑";
            this.btnSet.UseVisualStyleBackColor = true;
            this.btnSet.Click += new System.EventHandler(this.btnSet_Click);
            // 
            // lvRunInfo
            // 
            this.lvRunInfo.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3});
            this.lvRunInfo.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.lvRunInfo.GridLines = true;
            this.lvRunInfo.Location = new System.Drawing.Point(13, 64);
            this.lvRunInfo.Name = "lvRunInfo";
            this.lvRunInfo.Size = new System.Drawing.Size(498, 291);
            this.lvRunInfo.TabIndex = 30;
            this.lvRunInfo.UseCompatibleStateImageBehavior = false;
            this.lvRunInfo.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "监测项";
            this.columnHeader1.Width = 123;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "运行状态信息";
            this.columnHeader2.Width = 433;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.btnQuere);
            this.groupBox3.Controls.Add(this.lvRunInfo);
            this.groupBox3.Location = new System.Drawing.Point(33, 410);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(517, 329);
            this.groupBox3.TabIndex = 31;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "运行状态查询";
            this.groupBox3.Visible = false;
            // 
            // btnQuere
            // 
            this.btnQuere.Location = new System.Drawing.Point(200, 20);
            this.btnQuere.Name = "btnQuere";
            this.btnQuere.Size = new System.Drawing.Size(75, 23);
            this.btnQuere.TabIndex = 31;
            this.btnQuere.Text = "启动查询";
            this.btnQuere.UseVisualStyleBackColor = true;
            this.btnQuere.Click += new System.EventHandler(this.btnQuere_Click);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.btnIrdtSet);
            this.groupBox4.Controls.Add(this.btnIrdtEdit);
            this.groupBox4.Controls.Add(this.btnDoorSet);
            this.groupBox4.Controls.Add(this.DSScenebtn);
            this.groupBox4.Controls.Add(this.btnSceneSet);
            this.groupBox4.Controls.Add(this.btnScene);
            this.groupBox4.Controls.Add(this.btnDrSet);
            this.groupBox4.Controls.Add(this.btnDoorcardScene);
            this.groupBox4.Controls.Add(this.btnPowKeyScene);
            this.groupBox4.Controls.Add(this.button5);
            this.groupBox4.Location = new System.Drawing.Point(33, 212);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(742, 106);
            this.groupBox4.TabIndex = 30;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "场景配置";
            this.groupBox4.Visible = false;
            // 
            // btnIrdtSet
            // 
            this.btnIrdtSet.Location = new System.Drawing.Point(659, 20);
            this.btnIrdtSet.Name = "btnIrdtSet";
            this.btnIrdtSet.Size = new System.Drawing.Size(75, 23);
            this.btnIrdtSet.TabIndex = 9;
            this.btnIrdtSet.Text = "设置";
            this.btnIrdtSet.UseVisualStyleBackColor = true;
            this.btnIrdtSet.Click += new System.EventHandler(this.btnIrdtSet_Click);
            // 
            // btnIrdtEdit
            // 
            this.btnIrdtEdit.Location = new System.Drawing.Point(530, 20);
            this.btnIrdtEdit.Name = "btnIrdtEdit";
            this.btnIrdtEdit.Size = new System.Drawing.Size(109, 23);
            this.btnIrdtEdit.TabIndex = 8;
            this.btnIrdtEdit.Text = "红外检测场景编辑";
            this.btnIrdtEdit.UseVisualStyleBackColor = true;
            this.btnIrdtEdit.Click += new System.EventHandler(this.btnIrdtEdit_Click);
            // 
            // btnDoorSet
            // 
            this.btnDoorSet.Location = new System.Drawing.Point(433, 20);
            this.btnDoorSet.Name = "btnDoorSet";
            this.btnDoorSet.Size = new System.Drawing.Size(75, 23);
            this.btnDoorSet.TabIndex = 7;
            this.btnDoorSet.Text = "设置";
            this.btnDoorSet.UseVisualStyleBackColor = true;
            this.btnDoorSet.Click += new System.EventHandler(this.btnDoorSet_Click);
            // 
            // DSScenebtn
            // 
            this.DSScenebtn.Location = new System.Drawing.Point(288, 20);
            this.DSScenebtn.Name = "DSScenebtn";
            this.DSScenebtn.Size = new System.Drawing.Size(108, 23);
            this.DSScenebtn.TabIndex = 6;
            this.DSScenebtn.Text = "房门场景编辑";
            this.DSScenebtn.UseVisualStyleBackColor = true;
            this.DSScenebtn.Click += new System.EventHandler(this.DSScenebtn_Click);
            // 
            // btnSceneSet
            // 
            this.btnSceneSet.Location = new System.Drawing.Point(433, 61);
            this.btnSceneSet.Name = "btnSceneSet";
            this.btnSceneSet.Size = new System.Drawing.Size(75, 23);
            this.btnSceneSet.TabIndex = 5;
            this.btnSceneSet.Text = "设置";
            this.btnSceneSet.UseVisualStyleBackColor = true;
            this.btnSceneSet.Click += new System.EventHandler(this.btnSceneSet_Click);
            // 
            // btnScene
            // 
            this.btnScene.Location = new System.Drawing.Point(288, 61);
            this.btnScene.Name = "btnScene";
            this.btnScene.Size = new System.Drawing.Size(108, 23);
            this.btnScene.TabIndex = 4;
            this.btnScene.Text = "普通场景编辑";
            this.btnScene.UseVisualStyleBackColor = true;
            this.btnScene.Click += new System.EventHandler(this.btnScene_Click);
            // 
            // btnDrSet
            // 
            this.btnDrSet.Location = new System.Drawing.Point(173, 58);
            this.btnDrSet.Name = "btnDrSet";
            this.btnDrSet.Size = new System.Drawing.Size(75, 23);
            this.btnDrSet.TabIndex = 3;
            this.btnDrSet.Text = "设置";
            this.btnDrSet.UseVisualStyleBackColor = true;
            this.btnDrSet.Click += new System.EventHandler(this.btnDrSet_Click);
            // 
            // btnDoorcardScene
            // 
            this.btnDoorcardScene.Location = new System.Drawing.Point(28, 58);
            this.btnDoorcardScene.Name = "btnDoorcardScene";
            this.btnDoorcardScene.Size = new System.Drawing.Size(108, 23);
            this.btnDoorcardScene.TabIndex = 2;
            this.btnDoorcardScene.Text = "门卡场景编辑";
            this.btnDoorcardScene.UseVisualStyleBackColor = true;
            this.btnDoorcardScene.Click += new System.EventHandler(this.btnDoorcardScene_Click);
            // 
            // btnPowKeyScene
            // 
            this.btnPowKeyScene.Location = new System.Drawing.Point(174, 20);
            this.btnPowKeyScene.Name = "btnPowKeyScene";
            this.btnPowKeyScene.Size = new System.Drawing.Size(75, 23);
            this.btnPowKeyScene.TabIndex = 1;
            this.btnPowKeyScene.Text = "设置";
            this.btnPowKeyScene.UseVisualStyleBackColor = true;
            this.btnPowKeyScene.Click += new System.EventHandler(this.btnPowKeyScene_Click);
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(29, 20);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(108, 23);
            this.button5.TabIndex = 0;
            this.button5.Text = "总电源场景编辑";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.btnCtlCommand);
            this.groupBox5.Controls.Add(this.txtParam);
            this.groupBox5.Controls.Add(this.label5);
            this.groupBox5.Controls.Add(this.label26);
            this.groupBox5.Controls.Add(this.cmbFunc);
            this.groupBox5.Controls.Add(this.txtSubno);
            this.groupBox5.Controls.Add(this.label25);
            this.groupBox5.Controls.Add(this.label24);
            this.groupBox5.Controls.Add(this.txtCtlDNo);
            this.groupBox5.Location = new System.Drawing.Point(33, 324);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(566, 80);
            this.groupBox5.TabIndex = 32;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "远程控制";
            // 
            // btnCtlCommand
            // 
            this.btnCtlCommand.Location = new System.Drawing.Point(453, 42);
            this.btnCtlCommand.Name = "btnCtlCommand";
            this.btnCtlCommand.Size = new System.Drawing.Size(75, 23);
            this.btnCtlCommand.TabIndex = 85;
            this.btnCtlCommand.Text = "命令下发";
            this.btnCtlCommand.UseVisualStyleBackColor = true;
            this.btnCtlCommand.Click += new System.EventHandler(this.btnCtlCommand_Click);
            // 
            // txtParam
            // 
            this.txtParam.Location = new System.Drawing.Point(332, 42);
            this.txtParam.MaxLength = 12;
            this.txtParam.Name = "txtParam";
            this.txtParam.Size = new System.Drawing.Size(103, 21);
            this.txtParam.TabIndex = 84;
            this.txtParam.Text = "0";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(343, 17);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(29, 12);
            this.label5.TabIndex = 83;
            this.label5.Text = "参数";
            // 
            // label26
            // 
            this.label26.AutoSize = true;
            this.label26.Location = new System.Drawing.Point(185, 17);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(53, 12);
            this.label26.TabIndex = 82;
            this.label26.Text = "特殊功能";
            // 
            // cmbFunc
            // 
            this.cmbFunc.FormattingEnabled = true;
            this.cmbFunc.Items.AddRange(new object[] {
            "无特殊要求",
            "调光灯调亮一级",
            "调光灯调暗一级",
            "一键自动调光",
            "一键自动调亮",
            "一键自动调暗",
            "总电源开关",
            "键盘背光,",
            "夜灯",
            "继电器开",
            "继电器关",
            "打开门窗电机",
            "关闭门窗电机",
            "空调单管阀",
            "空调双管冷阀",
            "空调双管热阀",
            "空调风机速度控制",
            "空调数码管背光控制",
            "紧急按钮",
            "大门门磁检测",
            "廊灯",
            "清理房间",
            "请勿打扰",
            "门铃",
            "输入端电平吸合触发",
            "输入端高电平触发",
            "输入端低电平触发",
            "灯亮度设置",
            "洗衣服务",
            "场景组合1设置",
            "场景组合2设置",
            "场景组合3设置",
            "场景组合4设置",
            "场景组合5设置",
            "场景组合6设置",
            "场景组合7设置",
            "左床头灯",
            "右床头灯",
            "空调温度设置",
            "空调风速",
            "整流器电压设置",
            "整流器开",
            "整流器关",
            "回路电压设置",
            "回路整流器开",
            "回路整流器关"});
            this.cmbFunc.Location = new System.Drawing.Point(169, 44);
            this.cmbFunc.Name = "cmbFunc";
            this.cmbFunc.Size = new System.Drawing.Size(144, 20);
            this.cmbFunc.TabIndex = 81;
            // 
            // txtSubno
            // 
            this.txtSubno.Location = new System.Drawing.Point(94, 43);
            this.txtSubno.MaxLength = 2;
            this.txtSubno.Name = "txtSubno";
            this.txtSubno.Size = new System.Drawing.Size(57, 21);
            this.txtSubno.TabIndex = 80;
            // 
            // label25
            // 
            this.label25.AutoSize = true;
            this.label25.Location = new System.Drawing.Point(110, 17);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(41, 12);
            this.label25.TabIndex = 79;
            this.label25.Text = "子路号";
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(27, 17);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(77, 12);
            this.label24.TabIndex = 78;
            this.label24.Text = "控制的设备号";
            // 
            // txtCtlDNo
            // 
            this.txtCtlDNo.Location = new System.Drawing.Point(29, 43);
            this.txtCtlDNo.MaxLength = 2;
            this.txtCtlDNo.Name = "txtCtlDNo";
            this.txtCtlDNo.Size = new System.Drawing.Size(37, 21);
            this.txtCtlDNo.TabIndex = 77;
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.btnRcuState);
            this.groupBox6.Location = new System.Drawing.Point(584, 152);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(183, 54);
            this.groupBox6.TabIndex = 30;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "RCU状态";
            // 
            // btnRcuState
            // 
            this.btnRcuState.Location = new System.Drawing.Point(35, 20);
            this.btnRcuState.Name = "btnRcuState";
            this.btnRcuState.Size = new System.Drawing.Size(88, 23);
            this.btnRcuState.TabIndex = 0;
            this.btnRcuState.Text = "状态查询";
            this.btnRcuState.UseVisualStyleBackColor = true;
            this.btnRcuState.Click += new System.EventHandler(this.btnRcuState_Click);
            // 
            // MainFrame
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.ClientSize = new System.Drawing.Size(781, 746);
            this.Controls.Add(this.groupBox6);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.btnTool);
            this.Controls.Add(this.saveResult);
            this.Controls.Add(this.btnInfo);
            this.Controls.Add(this.config);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.txtAddr);
            this.Controls.Add(this.Stop);
            this.Controls.Add(this.Start);
            this.MaximizeBox = false;
            this.Name = "MainFrame";
            this.Text = "CPC档案配置工具";
            this.Load += new System.EventHandler(this.MainFrame_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button config;
        private System.Windows.Forms.Button btnInfo;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.Button saveResult;
        private System.Windows.Forms.Button Stop;
        private System.Windows.Forms.Button Start;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtAddr;
        public System.IO.Ports.SerialPort serialPort;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button btnTool;
        private System.Windows.Forms.Button btnLoad;
        private System.Windows.Forms.Button btnParamterLoad;
        private System.Windows.Forms.Button btnClear;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button btnSetAction;
        private System.Windows.Forms.Button btnSet;
        private System.Windows.Forms.ListView lvRunInfo;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button btnQuere;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.Button btnReadSet;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Button btnPowKeyScene;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button btnDrSet;
        private System.Windows.Forms.Button btnDoorcardScene;
        private System.Windows.Forms.Button btnSceneSet;
        private System.Windows.Forms.Button btnScene;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Button btnCtlCommand;
        public System.Windows.Forms.TextBox txtParam;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label26;
        public System.Windows.Forms.ComboBox cmbFunc;
        public System.Windows.Forms.TextBox txtSubno;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.Label label24;
        public System.Windows.Forms.TextBox txtCtlDNo;
        private System.Windows.Forms.Button btnDoorSet;
        private System.Windows.Forms.Button DSScenebtn;
        private System.Windows.Forms.Button btnIrdtSet;
        private System.Windows.Forms.Button btnIrdtEdit;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.Button btnRcuState;
    }
}

