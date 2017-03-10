namespace CJQTest
{
    partial class FrmRcuState
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.lvRcuStateInfo = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
            this.SuspendLayout();
            // 
            // lvRcuStateInfo
            // 
            this.lvRcuStateInfo.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3});
            this.lvRcuStateInfo.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.lvRcuStateInfo.GridLines = true;
            this.lvRcuStateInfo.Location = new System.Drawing.Point(44, 73);
            this.lvRcuStateInfo.Name = "lvRcuStateInfo";
            this.lvRcuStateInfo.Size = new System.Drawing.Size(498, 291);
            this.lvRcuStateInfo.TabIndex = 31;
            this.lvRcuStateInfo.UseCompatibleStateImageBehavior = false;
            this.lvRcuStateInfo.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "监测项";
            this.columnHeader1.Width = 217;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "运行状态信息";
            this.columnHeader2.Width = 433;
            // 
            // FrmRcuState
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(586, 436);
            this.Controls.Add(this.lvRcuStateInfo);
            this.Name = "FrmRcuState";
            this.Text = "设备状态显示";
            this.ResumeLayout(false);

        }

        #endregion

        public System.Windows.Forms.ListView lvRcuStateInfo;
        public System.Windows.Forms.ColumnHeader columnHeader1;
        public System.Windows.Forms.ColumnHeader columnHeader2;
        public System.Windows.Forms.ColumnHeader columnHeader3;
    }
}