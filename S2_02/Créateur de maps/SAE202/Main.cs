using Cyotek.Windows.Forms;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SAE202
{
    public partial class Main : Form
    {
        public static float scale = 10.0f;
        public Main()
        {
            InitializeComponent();
        }

        List<Biome> biomes = new List<Biome>();
        List<Polygon> polygons = new List<Polygon>();

        Bitmap draggerBmp = MakeDragger(Color.Black);
        Bitmap draggerSelBmp = MakeDragger(Color.Red);
        Bitmap draggerWorkBmp = MakeDragger(Color.Blue);

        private void Main_Load(object sender, EventArgs e)
        {
            biomes = new List<Biome>();
            polygons = new List<Polygon>();

            mapDefaultBiomeComboBox.Items.Clear();
            mapSelectedPointGroupBox.Enabled = false;

            biomeListBox.Items.Clear();
            biomePropertiesGroupBox.Enabled = false;
            biomeNameLabel.Text = "";
            removeBiomeButton.Enabled = false;

            originalPath = "";
            this.Text = "SAE 2.02 Map Editor";

            RefreshImage();
        }

        private void Main_Resize(object sender, EventArgs e) => RefreshImage();

        private void Main_KeyDown(object sender, KeyEventArgs e)
        {
            if(e.KeyCode == Keys.Delete && mapDisplayPictureBox.Focused) {
                mapDisplayPictureBox_SupprKeyPress(sender, e);
                e.Handled = true;
            }
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e) => Main_Load(sender, e);

        string originalPath = "";
        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog dialog = new OpenFileDialog())
            {
                dialog.Filter = "Graph à biomes|*.rsmg|Tous les fichiers (*.*)|*.*";
                dialog.FilterIndex = 1;
                dialog.RestoreDirectory = true;
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    string output = dialog.FileName;
                    LoadFile(output);
                }
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (originalPath == null || originalPath == "")
                saveAsToolStripMenuItem_Click(sender, e);
            else
                SaveFile();
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dialog;
            dialog = new SaveFileDialog();
            dialog.Filter = "Graph à biomes|*.rsmg|Tous les fichiers (*.*)|*.*";
            dialog.DefaultExt = "rsmg";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                originalPath = dialog.FileName;
                this.Text = "SAE 2.02 Map Editor - " + Path.GetFileName(originalPath);

                SaveFile();
            }
        }

        private void LoadFile(string path)
        {
            List<Polygon> polygons = new List<Polygon>();
            List<Biome> biomes = new List<Biome>();
            int defaultBiomeIdx = -1;

            if (Utils.LoadWorldmap(File.ReadAllBytes(path), out polygons, out biomes, out defaultBiomeIdx))
            {
                this.polygons = polygons;
                this.biomes = biomes;

                biomeListBox.Items.Clear();
                foreach(Biome biome in biomes)
                {
                    biomeListBox.Items.Add("Biome \"" + biome.name + "\"");
                    mapDefaultBiomeComboBox.Items.Add(biome.name);
                }
                mapDefaultBiomeComboBox.SelectedIndex = defaultBiomeIdx;
                biomeListBox.SelectedIndex = 0;

                RefreshImage();

                originalPath = path;
                this.Text = "SAE 2.02 Map Editor - " + Path.GetFileName(path);
            }
        }

        private void SaveFile()
        {
            int defaultBiomeIdx = mapDefaultBiomeComboBox.SelectedIndex;
            File.WriteAllBytes(originalPath, Utils.SaveWorldmap(polygons, biomes, defaultBiomeIdx));
        }

        private void exportImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dialog;
            dialog = new SaveFileDialog();
            dialog.Filter = "Portable Network Graphics|*.png|Tous les fichiers (*.*)|*.*";
            dialog.DefaultExt = "png";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                RefreshImage();

                mapDisplayPictureBox.Image.Save(dialog.FileName);
            }
        }

        private void editScaleToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (NumUpDownAskForm numUpDownAskForm = new NumUpDownAskForm((decimal)scale, 0.0m, decimal.MaxValue, 0.01m))
            {
                numUpDownAskForm.Title = "Changer l'échelle";
                if (numUpDownAskForm.ShowDialog() == DialogResult.OK)
                {
                    float result = (float)numUpDownAskForm.NumValue;
                    if (result == 0.0f)
                        MessageBox.Show("Impossible de mettre l'échelle à cette valeur !", "Échelle invalide", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    else
                        scale = result;

                    RefreshImage();
                }
            }
        }

        private void multiplyAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (NumUpDownAskForm numUpDownAskForm = new NumUpDownAskForm(1.0m, 0.0m, decimal.MaxValue, 0.001m))
            {
                numUpDownAskForm.Title = "Multiplier tout";
                if (numUpDownAskForm.ShowDialog() == DialogResult.OK)
                {
                    foreach (Polygon poly in polygons)
                    {
                        foreach (Point2D point in poly.points)
                        {
                            point.X *= (float)numUpDownAskForm.NumValue;
                            point.Y *= (float)numUpDownAskForm.NumValue;
                        }
                    }

                    RefreshImage();
                }
            }
        }

        private void generateNodesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dialog;
            dialog = new SaveFileDialog();
            dialog.Filter = "Portable Network Graphics|*.png|Tous les fichiers (*.*)|*.*";
            dialog.DefaultExt = "png";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                float nbMaillesParUnit = 0.1f;

                using (NumUpDownAskForm numUpDownAskForm = new NumUpDownAskForm((decimal)nbMaillesParUnit, 0.0m, decimal.MaxValue, 0.001m))
                {
                    numUpDownAskForm.Title = "Entrer le nombre de mailles par unités";
                    if (numUpDownAskForm.ShowDialog() == DialogResult.OK)
                    {
                        float result = (float)numUpDownAskForm.NumValue;
                        if (result == 0.0f)
                            MessageBox.Show("Impossible de faire un maillage de 0 mailles par unité", "Nombre invalide", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        else
                            nbMaillesParUnit = result;
                    }
                    else
                        return;
                }

                float nbUnitParMailles = 1.0f / nbMaillesParUnit;

                int x, y, w, h;
                CalculateMapDisplayInfo(out x, out y, out w, out h);

                Bitmap bmp = new Bitmap(w, h);

                Color defaultColor = (mapDefaultBiomeComboBox.SelectedIndex < 0) ? Color.LightGray : biomes[mapDefaultBiomeComboBox.SelectedIndex].color;

                float aNbUnitParMailles = nbUnitParMailles * scale;

                using (Graphics gfx = Graphics.FromImage(bmp))
                using (SolidBrush brush = new SolidBrush(defaultColor))
                {
                    gfx.FillRectangle(brush, 0, 0, bmp.Width, bmp.Height);

                    for (float cX = 0.0f; cX < w; cX += aNbUnitParMailles)
                    {
                        for (float cY = 0.0f; cY < h; cY += aNbUnitParMailles)
                        {
                            Color biomeColor = defaultColor;

                            Polygon p = GetClosePolygon(new Point2D((cX + x) / scale, (cY + y) / scale));
                            if (p != null)
                            {
                                if (p.biome != null)
                                    biomeColor = p.biome.color;
                                else
                                    biomeColor = Color.LightGray;
                            }

                            brush.Color = biomeColor;

                            gfx.FillRectangle(brush, cX - (aNbUnitParMailles / 2.0f), cY - (aNbUnitParMailles / 2.0f), aNbUnitParMailles, aNbUnitParMailles);
                        }
                    }

                    for (float cX = 0.0f; cX < w; cX += aNbUnitParMailles)
                    {
                        for (float cY = 0.0f; cY < h; cY += aNbUnitParMailles)
                        {
                        }
                    }
                }

                bmp.Save(dialog.FileName);
                bmp.Dispose();
            }

        }

        private static Bitmap MakeDragger(Color color)
        {
            Bitmap bmp = new Bitmap(7, 7);

            using (Graphics gfx = Graphics.FromImage(bmp))
            {
                SolidBrush brush = new SolidBrush(color);

                Pen p = new Pen(brush);

                gfx.DrawLines(p, new Point[] { new Point(2, 2), new Point(4, 2), new Point(4, 4), new Point(2, 4), new Point(2, 2) });

                gfx.DrawLines(p, new Point[] { new Point(0, 2), new Point(0, 0), new Point(2, 0) });

                gfx.DrawLines(p, new Point[] { new Point(6, 2), new Point(6, 0), new Point(4, 0) });

                gfx.DrawLines(p, new Point[] { new Point(0, 4), new Point(0, 6), new Point(2, 6) });

                gfx.DrawLines(p, new Point[] { new Point(4, 6), new Point(6, 6), new Point(6, 4) });
            }

            return bmp;
        }

        private void addBiomeButton_Click(object sender, EventArgs e)
        {
            Biome biome = new Biome();

            biomes.Insert(biomeListBox.SelectedIndex + 1, biome);
            refreshBiomeBoxes();
            biomeListBox.Items.Insert(biomeListBox.SelectedIndex + 1, "Biome \"" + biome.name + "\"");
            biomeListBox.SelectedIndex++;

        }

        private void removeBiomeButton_Click(object sender, EventArgs e)
        {
            int idx = biomeListBox.SelectedIndex;
            if (idx < 0)
                return;

            biomes.RemoveAt(biomeListBox.SelectedIndex);
            refreshBiomeBoxes();
            biomeListBox.Items.RemoveAt(biomeListBox.SelectedIndex);

            biomeListBox.SelectedIndex = (idx == 0 && biomes.Count > 0) ? 0 : (idx - 1);

        }

        bool skipBiomeListRefresh = false;
        private void biomeListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(skipBiomeListRefresh)
            {
                skipBiomeListRefresh = false;
                return;
            }

            int idx = biomeListBox.SelectedIndex;
            if (idx < 0)
            {
                removeBiomeButton.Enabled = false;
                biomePropertiesGroupBox.Enabled = false;
                return;
            }

            removeBiomeButton.Enabled = true;
            biomePropertiesGroupBox.Enabled = true;

            biomeNameLabel.Text = biomes[idx].name;
            biomeChangeNameTextBox.Text = biomes[idx].name;
            biomeChangeColorButton.BackColor = biomes[idx].color;
            biomeChangeIncomingWeightNumUpDown.Value = (decimal)biomes[idx].incomingWeight;
            biomeChangeInsideWeightNumUpDown.Value = (decimal)biomes[idx].insideWeight;
        }

        private void biomeChangeNameTextBox_TextChanged(object sender, EventArgs e)
        {
            int idx = biomeListBox.SelectedIndex;
            biomes[idx].name = biomeChangeNameTextBox.Text;
            biomeNameLabel.Text = biomeChangeNameTextBox.Text;
            skipBiomeListRefresh = true;
            biomeListBox.Items[idx] = "Biome \"" + biomeChangeNameTextBox.Text + "\"";
            mapDefaultBiomeComboBox.Items[idx] = biomeChangeNameTextBox.Text;
        }

        private void biomeChangeColorButton_Click(object sender, EventArgs e)
        {
            int idx = biomeListBox.SelectedIndex;
            ColorPickerDialog colorPickerDialog = new ColorPickerDialog();
            colorPickerDialog.Color = biomes[idx].color;

            if (colorPickerDialog.ShowDialog() == DialogResult.OK)
            {
                biomes[idx].color = colorPickerDialog.Color;
                biomeChangeColorButton.BackColor = colorPickerDialog.Color;
            }
        }

        private void biomeChangeIncomingWeightNumUpDown_ValueChanged(object sender, EventArgs e)
        {
            int idx = biomeListBox.SelectedIndex;
            biomes[idx].incomingWeight = (float)biomeChangeIncomingWeightNumUpDown.Value;
        }

        private void biomeChangeInsideWeightNumUpDown_ValueChanged(object sender, EventArgs e)
        {
            int idx = biomeListBox.SelectedIndex;
            biomes[idx].insideWeight = (float)biomeChangeInsideWeightNumUpDown.Value;
        }

        void refreshBiomeBoxes()
        {
            string currSel = mapDefaultBiomeComboBox.SelectedItem as string;
            int idx = -1;
            for(int i = 0; i < biomes.Count; i++)
            {
                if(biomes[i].name == currSel)
                {
                    idx = i;
                    break;
                }
            }

            mapDefaultBiomeComboBox.Items.Clear();

            for (int i = 0; i < biomes.Count; i++)
                mapDefaultBiomeComboBox.Items.Add(biomes[i].name);

            mapDefaultBiomeComboBox.SelectedIndex = idx;
        }

        public void CalculateMapDisplayInfo(out int x, out int y, out int w, out int h)
        {

            float minW = mapDisplayPanel.Width - 6;
            float minH = mapDisplayPanel.Height - 6;

            if (polygons.Count <= 0)
            {
                w = (int)Math.Ceiling(minW);
                h = (int)Math.Ceiling(minH);
                x = -(int)w / 2;
                y = -(int)h / 2;

                return;
            }

            float minX = float.MaxValue;
            float maxX = float.MinValue;
            float minY = float.MaxValue;
            float maxY = float.MinValue;

            foreach (Polygon polygon in polygons)
            {
                foreach (Point2D point in polygon.points)
                {
                    if (point.X < minX)
                        minX = point.X;
                    if (point.X > maxX)
                        maxX = point.X;
                    if (point.Y < minY)
                        minY = point.Y;
                    if (point.Y > maxY)
                        maxY = point.Y;
                }
            }

            minX *= scale;
            minY *= scale;
            maxX *= scale;
            maxY *= scale;

            w = (int)Math.Ceiling(maxX - minX);
            h = (int)Math.Ceiling(maxY - minY);
            x = (int)minX;
            y = (int)minY;

            if(minW > w)
            {
                x -= (int)Math.Ceiling((minW - w) / 2.0);
                w = (int)Math.Ceiling(minW);
            }

            if(minH > h)
            {
                y -= (int)Math.Ceiling((minH - h) / 2.0);
                h = (int)Math.Ceiling(minH);
            }
        }

        void RefreshImage()
        {
            if(mapDisplayPictureBox.Image != null)
            {
                Image img = mapDisplayPictureBox.Image;
                mapDisplayPictureBox.Image = null;
                img.Dispose();
            }

            int x, y, w, h;
            CalculateMapDisplayInfo(out x, out y, out w, out h);

            Bitmap bmp = new Bitmap(w, h);
            Color defaultColor = (mapDefaultBiomeComboBox.SelectedIndex < 0) ? Color.LightGray : biomes[mapDefaultBiomeComboBox.SelectedIndex].color;

            using (Graphics gfx = Graphics.FromImage(bmp))
            using (SolidBrush brush = new SolidBrush(defaultColor))
            {
                gfx.FillRectangle(brush, 0, 0, bmp.Width, bmp.Height);

                foreach (Polygon polygon in polygons)
                {
                    Color biomeColor = (polygon.biome == null) ? Color.LightGray : polygon.biome.color;
                    brush.Color = biomeColor;

                    PointF[] pts = new PointF[polygon.points.Count];
                    for(int i = 0; i < pts.Length; i++)
                    {
                        pts[i].X = ((polygon.points[i].X) * scale - x);
                        pts[i].Y = ((polygon.points[i].Y) * scale - y);
                    }

                    gfx.FillPolygon(brush, pts);
                    using (Pen pen = new Pen(brush))
                    {
                        pen.Color = Color.Black;
                        gfx.DrawLines(pen, pts.Append(pts[0]).ToArray());
                    }

                    foreach(PointF point in pts)
                    {
                        CompositingMode cm = gfx.CompositingMode;

                        gfx.CompositingMode = CompositingMode.SourceOver;
                        gfx.DrawImage(draggerBmp, new PointF((float)Math.Round(point.X - 3), (float)Math.Round(point.Y - 3)));

                        gfx.CompositingMode = cm;
                    }
                }

                if (workingPolygon != null && inPolygonCreation)
                {
                    brush.Color = Color.Black;
                    using (Pen pen = new Pen(brush))
                    {
                        PointF[] pts = new PointF[workingPolygon.points.Count];
                        for (int i = 0; i < pts.Length; i++)
                        {
                            pts[i].X = ((workingPolygon.points[i].X) * scale - x);
                            pts[i].Y = ((workingPolygon.points[i].Y) * scale - y);
                        }

                        pen.Width = 3.0f;
                        pen.Color = Color.Black;
                        pen.DashStyle = System.Drawing.Drawing2D.DashStyle.Dot;
                        if(pts.Length != 1)
                            gfx.DrawLines(pen, pts);

                        foreach (PointF point in pts)
                        {
                            CompositingMode cm = gfx.CompositingMode;

                            gfx.CompositingMode = CompositingMode.SourceOver;
                            gfx.DrawImage(draggerWorkBmp, new PointF((float)Math.Round(point.X - 3), (float)Math.Round(point.Y - 3)));

                            gfx.CompositingMode = cm;
                        }
                    }
                }

                if (selectedPolygon != null)
                {
                    brush.Color = Color.FromArgb(128, 255, 255, 255);

                    PointF[] pts = new PointF[selectedPolygon.points.Count];
                    for (int i = 0; i < pts.Length; i++)
                    {
                        pts[i].X = ((selectedPolygon.points[i].X) * scale - x);
                        pts[i].Y = ((selectedPolygon.points[i].Y) * scale - y);
                    }

                    gfx.FillPolygon(brush, pts);
                }

                if (selectedPoint != null)
                {
                    CompositingMode cm = gfx.CompositingMode;

                    gfx.CompositingMode = CompositingMode.SourceOver;
                    gfx.DrawImage(draggerSelBmp, new PointF((float)Math.Round(((selectedPoint.X) * scale - x) - 3), (float)Math.Round(((selectedPoint.Y) * scale - y) - 3)));

                    gfx.CompositingMode = cm;
                }
            }


            mapDisplayPictureBox.Image = bmp;
            mapDisplayPictureBox.Size = bmp.Size;
        }

        bool inPolygonCreation = false;
        Polygon workingPolygon = null;
        Point2D selectedPoint = null;
        Polygon selectedPolygon = null;
        private void mapDisplayPictureBox_Click(object sender, EventArgs e)
        {
            mapDisplayPictureBox.Focus();

            int x, y, w, h;
            CalculateMapDisplayInfo(out x, out y, out w, out h);

            MouseEventArgs mea = (e as MouseEventArgs);
            MouseButtons button = mea.Button;

            selectedPolygon = null;
            if (button == MouseButtons.Right)
            {
                if(!inPolygonCreation)
                {
                    if(biomes.Count <= 0)
                    {
                        MessageBox.Show("Merci de créer au moins un biome avant d'ajouter un polygone.", "Impossible de créer un polygone", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return;
                    }

                    workingPolygon = new Polygon();

                    workingPolygon.points = new List<Point2D>();

                    float cX = (mea.X + x) / scale;
                    float cY = (mea.Y + y) / scale;

                    Point2D nPoint = PlacePoint(cX, cY);
                    selectedPoint = null;

                    mapSelectedPointGroupBox.Enabled = true;
                    mapSelectedPointPosXNumUpDown.Value = (decimal)nPoint.X;
                    mapSelectedPointPosYNumUpDown.Value = (decimal)nPoint.Y;

                    selectedPoint = nPoint;

                    workingPolygon.points.Add(selectedPoint);
                    workingPolygon.biome = null;

                    selectedPolygon = workingPolygon;

                    RefreshImage();
                    inPolygonCreation = true;
                }
                else
                {
                    workingPolygon.points.Add(workingPolygon.points[0]);
                    selectedPoint = workingPolygon.points[0];
                    RefreshImage();
                    workingPolygon.points.RemoveAt(workingPolygon.points.Count-1);
                    using (ComboAskForm comboAskForm = new ComboAskForm(mapDefaultBiomeComboBox.Items))
                    {
                        comboAskForm.Title = "Choisir un biome";
                        if (comboAskForm.ShowDialog() == DialogResult.OK)
                        {
                            workingPolygon.biome = biomes[comboAskForm.ComboIndex];
                            polygons.Add(workingPolygon);
                        }

                        workingPolygon = null;
                        selectedPoint = null;
                        selectedPolygon = null;
                        RefreshImage();
                    }
                    inPolygonCreation = false;
                }
            }
            else if(button == MouseButtons.Left)
            {
                float cX = (mea.X + x) / scale;
                float cY = (mea.Y + y) / scale;

                if (!inPolygonCreation)
                {
                    selectedPoint = GetClosePoint(cX, cY);
                    if (selectedPoint != null)
                    {
                        mapSelectedPointGroupBox.Enabled = true;
                        mapSelectedPointPosXNumUpDown.Value = (decimal)selectedPoint.X;
                        mapSelectedPointPosYNumUpDown.Value = (decimal)selectedPoint.Y;
                    }
                    else
                    {
                        mapSelectedPointGroupBox.Enabled = false;

                        Polygon old = selectedPolygon;
                        selectedPolygon = GetClosePolygon(new Point2D(cX, cY));
                        if (old == selectedPolygon)
                            selectedPolygon = null;
                    }

                    RefreshImage();
                }
                else
                {
                    Point2D nPoint = PlacePoint(cX, cY);
                    selectedPoint = null;

                    mapSelectedPointGroupBox.Enabled = true;
                    mapSelectedPointPosXNumUpDown.Value = (decimal)nPoint.X;
                    mapSelectedPointPosYNumUpDown.Value = (decimal)nPoint.Y;

                    selectedPoint = nPoint;

                    workingPolygon.points.Add(selectedPoint);
                    RefreshImage();
                }
            }
        }
        private void mapDisplayPictureBox_DoubleClick(object sender, EventArgs e)
        {
            mapDisplayPictureBox.Focus();

            MouseEventArgs mea = (e as MouseEventArgs);
            MouseButtons button = mea.Button;

            if (button == MouseButtons.Left && selectedPolygon != null)
            {
                using (ComboAskForm comboAskForm = new ComboAskForm(mapDefaultBiomeComboBox.Items))
                {
                    comboAskForm.Title = "Changer le biome";
                    comboAskForm.ComboIndex = (selectedPolygon.biome == null) ? -1 : biomes.FindIndex(biome => biome.name == selectedPolygon.biome.name);
                    if (comboAskForm.ShowDialog() == DialogResult.OK)
                    {
                        selectedPolygon.biome = (comboAskForm.ComboIndex < 0) ? null : biomes[comboAskForm.ComboIndex];
                        RefreshImage();
                    }
                }
            }
        }

        private void mapDisplayPictureBox_MouseEnter(object sender, EventArgs e)
        {
            mapCoordsXLabel.Visible = true;
            mapCoordsYLabel.Visible = true;
            mapCoordsSeparatorLabel.Visible = true;
        }

        private void mapDisplayPictureBox_MouseLeave(object sender, EventArgs e)
        {
            mapCoordsXLabel.Visible = false;
            mapCoordsYLabel.Visible = false;
            mapCoordsSeparatorLabel.Visible = false;
        }

        private void mapDisplayPictureBox_MouseMove(object sender, MouseEventArgs e)
        {
            int x, y, w, h;
            CalculateMapDisplayInfo(out x, out y, out w, out h);

            mapCoordsXLabel.Text = ((e.X + x) / scale).ToString();
            mapCoordsYLabel.Text = ((e.Y + y) / scale).ToString();
        }

        private void mapDisplayPictureBox_SupprKeyPress(object sender, KeyEventArgs e)
        {
            if(selectedPolygon != null)
            {
                polygons.Remove(selectedPolygon);
                selectedPolygon = null;
                RefreshImage();
            }
        }

        private Point2D GetClosestPoint(Point2D A, Point2D B, Point2D P)
        {
            Point2D AtoP = new Point2D(P.X - A.X, P.Y - A.Y);
            Point2D AtoB = new Point2D(B.X - A.X, B.Y - A.Y);

            float atb2 = AtoB.X * AtoB.X + AtoB.Y * AtoB.Y;
            float atp_dot_atb = AtoP.X * AtoB.X + AtoP.Y * AtoB.Y;

            float t = atp_dot_atb / atb2;

            return new Point2D(A.X + AtoB.X * t, A.Y + AtoB.Y * t);
        }

        private float GetPointDistance(Point2D a, Point2D b)
        {
            float x = a.X - b.X;
            float y = a.Y - b.Y;
            return (float)Math.Sqrt(x * x + y * y);
        }

        private Point2D PlacePoint(float x, float y)
        {
            Point2D currPoint = new Point2D(x, y);

            Point2D closePoint = GetClosePoint(x, y);
            if (closePoint != null)
            {
                currPoint.X = closePoint.X;
                currPoint.Y = closePoint.Y;
            }
            else
            {
                bool stop = false;
                foreach(Polygon poly in (workingPolygon != null) ? polygons.Append(workingPolygon) : polygons)
                {
                    for(int i = 0; i < poly.points.Count; i++)
                    {
                        int next = (i == (poly.points.Count - 1)) ? 0 : i + 1;
                        Point2D p = GetClosestPoint(poly.points[i], poly.points[next], currPoint);
                        if(GetPointDistance(currPoint, p) <= (3.0f / scale))
                        {
                            currPoint.X = p.X;
                            currPoint.Y = p.Y;

                            stop = true;
                            break;
                        }
                    }

                    if (stop)
                        break;
                }
            }

            return currPoint;
        }

        private Point2D GetClosePoint(float x, float y)
        {
            float pad = (3.0f / scale);
            foreach (Polygon polygon in polygons)
                foreach(Point2D point in polygon.points)
                    if(x > point.X - pad && x < point.X + pad && y > point.Y - pad && y < point.Y + pad)
                        return point;

            return null;
        }

        private Polygon GetClosePolygon(Point2D pt)
        {
            for(int i = polygons.Count - 1; i >= 0; i--)
                if (polygons[i].collisionPoint(pt))
                    return polygons[i];

            return null;
        }

        private void mapDefaultBiomeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            RefreshImage();
        }

        private void mapSelectedPointPosXNumUpDown_ValueChanged(object sender, EventArgs e)
        {
            if(selectedPoint != null)
            {
                selectedPoint.X = (float)mapSelectedPointPosXNumUpDown.Value;
                RefreshImage();
            }
        }

        private void mapSelectedPointPosYNumUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (selectedPoint != null)
            {
                selectedPoint.Y = (float)mapSelectedPointPosYNumUpDown.Value;
                RefreshImage();
            }
        }
    }
}
