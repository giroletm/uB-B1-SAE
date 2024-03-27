namespace SAE201205_editor
{
    partial class DoubleNumAskForm
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
            this.elem1Label = new System.Windows.Forms.Label();
            this.elem1NumUpDown = new System.Windows.Forms.NumericUpDown();
            this.elem2NumUpDown = new System.Windows.Forms.NumericUpDown();
            this.okButton = new System.Windows.Forms.Button();
            this.elem2Label = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.elem1NumUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.elem2NumUpDown)).BeginInit();
            this.SuspendLayout();
            // 
            // elem1Label
            // 
            this.elem1Label.AutoSize = true;
            this.elem1Label.Location = new System.Drawing.Point(12, 9);
            this.elem1Label.Name = "elem1Label";
            this.elem1Label.Size = new System.Drawing.Size(42, 13);
            this.elem1Label.TabIndex = 0;
            this.elem1Label.Text = "Label1:";
            // 
            // elem1NumUpDown
            // 
            this.elem1NumUpDown.Location = new System.Drawing.Point(60, 7);
            this.elem1NumUpDown.Maximum = new decimal(new int[] {
            512,
            0,
            0,
            0});
            this.elem1NumUpDown.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.elem1NumUpDown.Name = "elem1NumUpDown";
            this.elem1NumUpDown.Size = new System.Drawing.Size(310, 20);
            this.elem1NumUpDown.TabIndex = 1;
            this.elem1NumUpDown.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // elem2NumUpDown
            // 
            this.elem2NumUpDown.Location = new System.Drawing.Point(60, 33);
            this.elem2NumUpDown.Maximum = new decimal(new int[] {
            512,
            0,
            0,
            0});
            this.elem2NumUpDown.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.elem2NumUpDown.Name = "elem2NumUpDown";
            this.elem2NumUpDown.Size = new System.Drawing.Size(310, 20);
            this.elem2NumUpDown.TabIndex = 2;
            this.elem2NumUpDown.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // okButton
            // 
            this.okButton.Location = new System.Drawing.Point(158, 59);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(75, 23);
            this.okButton.TabIndex = 3;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // elem2Label
            // 
            this.elem2Label.AutoSize = true;
            this.elem2Label.Location = new System.Drawing.Point(12, 35);
            this.elem2Label.Name = "elem2Label";
            this.elem2Label.Size = new System.Drawing.Size(42, 13);
            this.elem2Label.TabIndex = 4;
            this.elem2Label.Text = "Label2:";
            // 
            // DoubleNumAskForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(382, 94);
            this.Controls.Add(this.elem2Label);
            this.Controls.Add(this.okButton);
            this.Controls.Add(this.elem2NumUpDown);
            this.Controls.Add(this.elem1NumUpDown);
            this.Controls.Add(this.elem1Label);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "DoubleNumAskForm";
            this.ShowIcon = false;
            this.Text = "Question";
            ((System.ComponentModel.ISupportInitialize)(this.elem1NumUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.elem2NumUpDown)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label elem1Label;
        private System.Windows.Forms.NumericUpDown elem1NumUpDown;
        private System.Windows.Forms.NumericUpDown elem2NumUpDown;
        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.Label elem2Label;
    }
}