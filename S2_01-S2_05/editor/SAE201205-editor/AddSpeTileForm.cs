using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SAE201205_editor
{
    public partial class AddSpeTileForm : Form
    {
        private SpeTiles speTiles = null;
        //private SpeTiles.TileType tileType = null;

        public int SelectedID => actorComboBox.SelectedIndex-1;

        public Dictionary<string, object> ParamVals
        {
            get
            {
                Dictionary<string, object> paramVal = new Dictionary<string, object>();

                SpeTiles.TileType tileType = speTiles.tileTypes[actorComboBox.SelectedIndex - 1];
                for (int i = 0; i < inControls.Count; i++)
                {
                    string pName = tileType.settings[i].iName;

                    if (inControls[i] is NumericUpDown)
                    {
                        paramVal.Add(pName, (int)(inControls[i] as NumericUpDown).Value);
                    }
                    else if (inControls[i] is CheckBox)
                    {
                        paramVal.Add(pName, (inControls[i] as CheckBox).Checked);
                    }
                    else if (inControls[i] is TextBox)
                    {
                        paramVal.Add(pName, (inControls[i] as TextBox).Text);
                    }
                    else
                        continue;
                }

                return paramVal;
            }
        }

        public AddSpeTileForm(SpeTiles speTiles, Map.Special selectedTileType=null)
        {
            InitializeComponent();

            this.speTiles = speTiles;

            actorComboBox.Items.Clear();
            actorComboBox.Items.Add("<None>");
            foreach(SpeTiles.TileType tileType in speTiles.tileTypes)
            {
                actorComboBox.Items.Add(tileType.name);
            }

            int actID = (selectedTileType == null) ? -1 : selectedTileType.id;
            actorComboBox.SelectedIndex = actID+1;
            
            if (actID < 0)
                return;

            int i = 0;
            foreach(KeyValuePair<string, object> param in selectedTileType.settings)
            {
                if(param.Value is int)
                {
                    (inControls[i] as NumericUpDown).Value = (int)param.Value;
                }
                else if(param.Value is bool)
                {
                    (inControls[i] as CheckBox).Checked = (bool)param.Value;
                }
                else if(param.Value is string)
                {
                    (inControls[i] as TextBox).Text = (string)param.Value;
                }

                i++;
            }
        }

        List<Label> descLabels = new List<Label>();
        List<Control> inControls = new List<Control>();
        private void actorComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            foreach(Label label in descLabels)
                this.Controls.Remove(label);

            foreach(Control control in inControls)
                this.Controls.Remove(control);

            descLabels.Clear();
            inControls.Clear();

            int y = 39;

            if (actorComboBox.SelectedIndex > 0)
            {
                SpeTiles.TileType tileType = speTiles.tileTypes[actorComboBox.SelectedIndex - 1];
                foreach (SpeTiles.Setting setting in tileType.settings)
                {
                    Control newCtrl = null;

                    switch (setting.type)
                    {
                        case "number":
                            {
                                NumericUpDown numUpDown = new NumericUpDown();
                                numUpDown.Minimum = 0;
                                numUpDown.Maximum = int.MaxValue;

                                newCtrl = numUpDown;
                                break;
                            }
                        case "boolean":
                            {
                                newCtrl = new CheckBox();
                                break;
                            }
                        case "string":
                            {
                                TextBox textbox = new TextBox();
                                textbox.MaxLength = short.MaxValue;

                                newCtrl = textbox;
                                break;
                            }
                        default:
                            {
                                break;
                            }
                    }

                    if (newCtrl == null)
                        continue;

                    Label newLabel = new Label();
                    this.Controls.Add(newLabel);
                    descLabels.Add(newLabel);

                    newLabel.AutoSize = true;
                    newLabel.Text = setting.dName + ":";
                    newLabel.Left = 12;
                    newLabel.Top = y + 3;

                    this.Controls.Add(newCtrl);
                    inControls.Add(newCtrl);

                    newCtrl.Left = 18 + newLabel.Width;
                    newCtrl.Top = y;
                    newCtrl.Width = this.Width - 28 - newCtrl.Left;
                    newCtrl.Anchor |= AnchorStyles.Right;

                    y += newCtrl.Height + 6;
                }
            }

            okButton.Top = y;
            y += okButton.Height;

            this.Height = y + 48;
        }
    }
}
