namespace Launcher
{
    partial class Form1
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
            this.button1 = new System.Windows.Forms.Button();
            this.resolutionDropdown = new System.Windows.Forms.ComboBox();
            this.aaDropdown = new System.Windows.Forms.ComboBox();
            this.windowedCheckbox = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(12, 160);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(157, 62);
            this.button1.TabIndex = 0;
            this.button1.Text = "Launch Game";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // resolutionDropdown
            // 
            this.resolutionDropdown.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.resolutionDropdown.FormattingEnabled = true;
            this.resolutionDropdown.Location = new System.Drawing.Point(17, 24);
            this.resolutionDropdown.Name = "resolutionDropdown";
            this.resolutionDropdown.Size = new System.Drawing.Size(121, 21);
            this.resolutionDropdown.TabIndex = 1;
            // 
            // aaDropdown
            // 
            this.aaDropdown.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.aaDropdown.FormattingEnabled = true;
            this.aaDropdown.Location = new System.Drawing.Point(17, 73);
            this.aaDropdown.Name = "aaDropdown";
            this.aaDropdown.Size = new System.Drawing.Size(121, 21);
            this.aaDropdown.TabIndex = 2;
            // 
            // windowedCheckbox
            // 
            this.windowedCheckbox.AutoSize = true;
            this.windowedCheckbox.Location = new System.Drawing.Point(17, 110);
            this.windowedCheckbox.Name = "windowedCheckbox";
            this.windowedCheckbox.Size = new System.Drawing.Size(77, 17);
            this.windowedCheckbox.TabIndex = 3;
            this.windowedCheckbox.Text = "Windowed";
            this.windowedCheckbox.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(14, 8);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(60, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Resolution:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(14, 57);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(67, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Anti Aliasing:";
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.aaDropdown);
            this.panel1.Controls.Add(this.windowedCheckbox);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.resolutionDropdown);
            this.panel1.Location = new System.Drawing.Point(12, 16);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(157, 138);
            this.panel1.TabIndex = 6;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(184, 241);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.button1);
            this.Name = "Form1";
            this.Text = "Launcher";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ComboBox resolutionDropdown;
        private System.Windows.Forms.ComboBox aaDropdown;
        private System.Windows.Forms.CheckBox windowedCheckbox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Panel panel1;
    }
}

