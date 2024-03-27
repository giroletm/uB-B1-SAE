using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SAE201205_editor
{
    public partial class Main : Form
    {
        private static readonly string Title = "SAE 2.01 & 2.05 Map Editor";
        private static readonly Bitmap UnknownBmp = GenerateUnknownTexture(new Size(16, 16));
        private static readonly Bitmap TransparentBmp = GenerateTransparentTexture(new Size(16, 16));

        string originalPath = "";
        Map map = Map.FromEmpty("Nouvelle carte");
        List<Tileset> tilesets => map.tilesets;

        SpeTiles speTiles = SpeTiles.FromFile("Objects.json");

        public Main()
        {
            InitializeComponent();
        }

        private static Bitmap GenerateTransparentTexture(Size size)
        {
            return GeneratePattern(size, Color.White, Color.FromArgb(0xBF, 0xBF, 0xBF));
        }

        private static Bitmap GenerateUnknownTexture(Size size)
        {
            return GeneratePattern(size, Color.Black, Color.FromArgb(0xFF, 0x00, 0xFF));
        }

        private static Bitmap GeneratePattern(Size size, Color col1, Color col2)
        {
            Bitmap bmp = new Bitmap(size.Width, size.Height);
            using (Graphics g = Graphics.FromImage(bmp))
            {
                SolidBrush wbrush = new SolidBrush(col1);
                SolidBrush gbrush = new SolidBrush(col2);
                for (int y = 0; y < size.Height; y += 8)
                {
                    for (int x = 0; x < size.Width; x += 8)
                    {
                        bool isW = (x % 16) >= 8;
                        if ((y % 16) >= 8)
                            isW = !isW;

                        g.FillRectangle(isW ? wbrush : gbrush, new Rectangle(x, y, 8, 8));
                    }
                }
            }

            return bmp;
        }

        private void Main_Load(object sender, EventArgs e)
        {
            originalPath = "";
            this.Text = Title;

            tObjPreviewPictureBox.BackgroundImage = TransparentBmp;
            mapBuilderPBox.BackgroundImage = TransparentBmp;
            Main_Resize(sender, e);

            RefreshTilesetsListBox();

            layer0VisibilityCheckBox.Checked = true;
            layer1VisibilityCheckBox.Checked = true;
            layer2VisibilityCheckBox.Checked = true;

            #if !DEBUG
            newToolStripMenuItem.Visible = false;
            debugToolStripMenuItem.Visible = false;
            addTileFolderButton.Enabled = false;
            removeTileFolderButton.Enabled = false;
            importToolStripMenuItem.Visible = false;
            exportToolStripMenuItem.Visible = false;
            toolStripSeparator2.Visible = false;
            addTilesetButton.Enabled = false;
            removeTilesetButton.Enabled = false;
            addTObjButton.Enabled = false;
            removeTObjButton.Enabled = false;
            #endif

            //RefreshImage();
        }

        private void Main_Resize(object sender, EventArgs e) {
            int reparW = (tilesetPropsGroupBox.Width - 42 - tilesetNameLabel.Width - tilesetIFilenameLabel.Width - tilesetFilenameLabel.Width) / 3;

            tilesetNameTextBox.Width = reparW;
            tilesetIFilenameTextBox.Width = reparW;
            tilesetFilenameTextBox.Width = reparW;

            int x = 6;

            tilesetNameLabel.Location = new Point(x, tilesetNameLabel.Location.Y);
            x += tilesetNameLabel.Width + 6;
            tilesetNameTextBox.Location = new Point(x, tilesetNameTextBox.Location.Y);
            x += reparW + 6;

            tilesetIFilenameLabel.Location = new Point(x, tilesetIFilenameLabel.Location.Y);
            x += tilesetIFilenameLabel.Width + 6;
            tilesetIFilenameTextBox.Location = new Point(x, tilesetIFilenameTextBox.Location.Y);
            x += reparW + 6;

            tilesetFilenameLabel.Location = new Point(x, tilesetFilenameLabel.Location.Y);
            x += tilesetFilenameLabel.Width + 6;
            tilesetFilenameTextBox.Location = new Point(x, tilesetFilenameTextBox.Location.Y);
            x += reparW + 6;

            tObjPreviewPictureBox.Location = new Point((tObjPropertiesGroupBox.Width / 2) - (tObjPreviewPictureBox.Width / 2), (tObjPropertiesGroupBox.Height / 2) - (tObjPreviewPictureBox.Height / 2) + 70);
            tObjPreviewLabel.Location = new Point((tObjPropertiesGroupBox.Width / 2) - (tObjPreviewLabel.Width / 2), tObjPreviewPictureBox.Location.Y - 16);

            //RefreshImage();
        }

        private void Main_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Delete && mapBuilderPBox.Focused) {
                mapBuilderPBox_SupprKeyPress(sender, e);
                e.Handled = true;
            }
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e) => Main_Load(sender, e);

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog dialog = new OpenFileDialog())
            {
                dialog.Filter = "Carte de RPG (*.rsmc)|*.rsmc|Tous les fichiers (*.*)|*.*";
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
            dialog.Filter = "Carte de RPG (*.rsmc)|*.rsmc|Tous les fichiers (*.*)|*.*";
            dialog.DefaultExt = "rsmc";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                originalPath = dialog.FileName;
                this.Text = Title + " - " + Path.GetFileName(originalPath);

                foreach(Tileset tileset in tilesets)
                {
                    RefreshTilesetImage(tileset);
                }

                SaveFile();
            }
        }

        private void LoadFile(string path)
        {
            map = Map.FromFile(path);
            if (map == null)
                return;

            originalPath = path;
            this.Text = Title + " - " + Path.GetFileName(originalPath);

            foreach (Tileset tileset in tilesets)
            {
                RefreshTilesetImage(tileset);
            }

            RefreshTilesetsListBox();

            mapBuilderPBox.ClearAllTiles();
            mapBuilderPBox.WTiles = map.size.Width;
            mapBuilderPBox.HTiles = map.size.Height;
            int layer = 0;
            foreach (List<Map.Tile> tilemap in map.tilemaps)
            {
                mapBuilderPBox.Layer = layer;

                for(int i = 0; i < tilemap.Count; i++)
                {
                    int x = i % map.size.Width;
                    int y = i / map.size.Width;

                    mapBuilderPBox.PlaceTile(x, y, tilemap[i].tile, (tilemap[i].tile == null) ? null : GetImageForTObj(tilemap[i].tile, tilemap[i].tileset));
                }

                layer++;
            }

            mapBuilderPBox.Layer = 0;
            mapBuilderPBox.Invalidate();


            /*
            if (Utils.LoadWorldmap(File.ReadAllBytes(path), out polygons, out biomes, out defaultBiomeIdx))
            {
                this.polygons = polygons;
                this.biomes = biomes;

                tilesetsListBox.Items.Clear();
                foreach(Biome biome in biomes)
                {
                    tilesetsListBox.Items.Add("Biome \"" + biome.name + "\"");
                    //mapDefaultBiomeComboBox.Items.Add(biome.name);
                }
                //mapDefaultBiomeComboBox.SelectedIndex = defaultBiomeIdx;
                tilesetsListBox.SelectedIndex = 0;

                //RefreshImage();

                originalPath = path;
                this.Text = "SAE 2.01 & 2.05 Map Editor - " + Path.GetFileName(path);
            }
            */
        }

        private void SaveFile()
        {
            foreach (Tileset tileset in tilesets)
                tileset.ToFile(Path.GetDirectoryName(originalPath) + Path.DirectorySeparatorChar + tileset.ifilename);

            speTiles.ToFile("Objects.json");

            map.size = new Size(mapBuilderPBox.WTiles, mapBuilderPBox.HTiles);
            map.tilemaps = new List<Map.Tile>[PictureBoxMapBuilder.MAXLAYERS];
            for (int layer = 0; layer < PictureBoxMapBuilder.MAXLAYERS; layer++)
                map.tilemaps[layer] = new List<Map.Tile>();

            for(int y = 0; y < mapBuilderPBox.HTiles; y++)
            {
                for (int x = 0; x < mapBuilderPBox.WTiles; x++)
                {
                    for(int layer = 0; layer < PictureBoxMapBuilder.MAXLAYERS; layer++)
                    {
                        PictureBoxMapBuilder.TileInfo tinfo = (mapBuilderPBox.tilemaps[layer])[x, y];
                        Tileset.Tile stile = (tinfo == null) ? null : (tinfo.LinkedObject as Tileset.Tile);

                        Tileset stileset = null;
                        foreach (Tileset tileset in tilesets)
                        {
                            if (tileset.tiles.IndexOf(stile) >= 0)
                            {
                                stileset = tileset;
                                break;
                            }
                        }

                        map.tilemaps[layer].Add(new Map.Tile(stileset, stile));
                    }
                }
            }

            map.ToFile(originalPath);
        }

        private void importTilesetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog dialog = new OpenFileDialog())
            {
                dialog.Filter = "JavaScript Object Notation (*.json)|*.json|Tous les fichiers (*.*)|*.*";
                dialog.FilterIndex = 1;
                dialog.RestoreDirectory = true;
                dialog.Multiselect = true;
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    foreach (string filename in dialog.FileNames)
                    {
                        Tileset tileset = Tileset.FromFile(filename);
                        if (tileset != null)
                        {
                            tilesets.Add(tileset);

                            /*
                            Console.WriteLine("Name: " + tileset.name);
                            Console.WriteLine("Filename: " + tileset.filename);
                            Console.WriteLine("Tiles:");
                            foreach (Tileset.Tile tile in tileset.tiles)
                            {
                                Console.WriteLine("\tTile: " + tile.id + " -> " + tile.x + ";" + tile.y + " " + tile.w + "*" + tile.h);
                            }
                            Console.WriteLine("Categories:");
                            foreach (Tileset.Category category in tileset.categories)
                            {
                                Console.Write("\tCategory: " + category.name + " -> [");
                                for (int i = 0; i < category.tiles.Count; i++)
                                {
                                    Console.Write(category.tiles[i].id);
                                    if (i != category.tiles.Count - 1)
                                        Console.Write(", ");
                                }
                                Console.WriteLine("]");
                            }
                            */

                            //tileset.ToFile("test2.json");

                            RefreshTilesetImage(tileset);
                            RefreshTilesetsListBox();
                        }
                    }
                }
            }
        }

        private void exportImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dialog;
            dialog = new SaveFileDialog();
            dialog.Filter = "Portable Network Graphics|*.png|Tous les fichiers (*.*)|*.*";
            dialog.DefaultExt = "png";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                //RefreshImage();

                //mapDisplayPictureBox.Image.Save(dialog.FileName);
                Bitmap bmp = new Bitmap(1, 1);
                BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, 1, 1), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb); ;
                bmpData.Width = mapBuilderPBox.Width;
                bmpData.Height = mapBuilderPBox.Height;
                bmpData.PixelFormat = PixelFormat.Format32bppArgb;
                bmpData.Stride = bmpData.Width * 4;

                byte[] bytes = new byte[bmpData.Width * bmpData.Height * 4];

                GCHandle pinnedArray = GCHandle.Alloc(bytes, GCHandleType.Pinned);
                bmpData.Scan0 = pinnedArray.AddrOfPinnedObject();

                bmp.UnlockBits(bmpData);

                bmp.Save("a.png");
            }
        }

        private void magieToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (SelectedTileset == null || SelectedTileset.image == null)
                return;

            for(ushort y = 0; y < SelectedTileset.image.Height; y += 48)
            {
                for (ushort x = 0; x < SelectedTileset.image.Width; x += 48)
                {
                    bool doBreak = false;
                    for(int px = 0; px < 48; px++)
                    {
                        for (int py = 0; py < 48; py++)
                        {
                            if (SelectedTileset.image.GetPixel(x + px, y + py).A != 0)
                            {
                                int nID = 0;
                                if (SelectedTileset.tiles.Count > 0)
                                    nID = SelectedTileset.tiles.Max(t => t.id) + 1;

                                SelectedTileset.tiles.Add(new Tileset.Tile(nID, x, y, 48, 48));
                                doBreak = true;
                                break;
                            }
                        }

                        if (doBreak)
                            break;
                    }
                }
            }

            RefreshTObjsListView();
        }

        private Tileset SelectedTileset => (tilesetsListBox.SelectedIndex < 0) ? null : tilesets[tilesetsListBox.SelectedIndex];

        private string GetNameForTilesetsListBoxItem(Tileset tileset) => tileset.name + " (" + tileset.filename + ")";

        private void RefreshSelectedTilesetsListBoxItem(Tileset tileset)
        {
            noTilesetsRefresh = true;
            tilesetsListBox.Items[tilesetsListBox.SelectedIndex] = GetNameForTilesetsListBoxItem(tileset);
            noTilesetsRefresh = false;
        }

        private void RefreshTilesetsListBox()
        {
            int selIdx = tilesetsListBox.SelectedIndex;

            tilesetsListBox.Items.Clear();

            foreach (Tileset tileset in tilesets)
            {
                tilesetsListBox.Items.Add(GetNameForTilesetsListBoxItem(tileset));
            }

            #if DEBUG
            tilesetPropsGroupBox.Enabled = removeTilesetButton.Enabled = tilesetsListBox.Items.Count != 0;
            #endif

            if (selIdx == -1) {
                if (tilesetsListBox.Items.Count == 0)
                    tilesetsListBox.SelectedIndex = -1;
                else
                    tilesetsListBox.SelectedIndex = 0;
            }
            else
            {
                if (selIdx > tilesetsListBox.Items.Count - 1)
                    tilesetsListBox.SelectedIndex = tilesetsListBox.Items.Count - 1;
                else
                    tilesetsListBox.SelectedIndex = selIdx;
            }
        }

        bool noTilesetsRefresh = false;
        private void tilesetsListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (noTilesetsRefresh)
                return;

            RefreshTObjsListView();

            tilesetNameTextBox.Text = SelectedTileset.name;
            tilesetIFilenameTextBox.Text = SelectedTileset.ifilename;
            tilesetFilenameTextBox.Text = SelectedTileset.filename;
        }

        private string GenerateNewTilesetIFilename()
        {
            int nb = 1;
            string oname = "tileset_example";
            string name = oname;

            foreach (Tileset tileset in tilesets)
            {
                if (tileset.ifilename == name + ".json")
                {
                    nb++;
                    name = oname + nb;
                }
            }

            return name + ".json";
        }

        private void addTilesetButton_Click(object sender, EventArgs e)
        {
            Tileset tileset = Tileset.FromEmpty("Nouveau tileset", "exemple.png", GenerateNewTilesetIFilename());

            int selIdx = tilesetsListBox.SelectedIndex;
            tilesets.Insert(selIdx + 1, tileset);

            RefreshTilesetsListBox();

            tilesetsListBox.SelectedIndex = selIdx + 1;
        }

        private void removeTilesetButton_Click(object sender, EventArgs e)
        {
            int selIdx = tilesetsListBox.SelectedIndex;
            tilesets.RemoveAt(selIdx);

            RefreshTilesetsListBox();

            if (selIdx != 0)
                tilesetsListBox.SelectedIndex = selIdx - 1;
        }

        private void tilesetNameTextBox_TextChanged(object sender, EventArgs e)
        {
            SelectedTileset.name = tilesetNameTextBox.Text;
            RefreshSelectedTilesetsListBoxItem(SelectedTileset);
        }

        private void tilesetIFilenameTextBox_TextChanged(object sender, EventArgs e)
        {
            SelectedTileset.ifilename = tilesetIFilenameTextBox.Text;
            RefreshSelectedTilesetsListBoxItem(SelectedTileset);
        }

        private void tilesetFilenameTextBox_TextChanged(object sender, EventArgs e)
        {
            SelectedTileset.filename = tilesetFilenameTextBox.Text;
            RefreshSelectedTilesetsListBoxItem(SelectedTileset);
            RefreshSelectedTilesetImage();
        }

        private void RefreshTilesetImage(Tileset tileset)
        {
            Bitmap newBmp = null;

            if (originalPath != "")
            {
                string imgPath = Path.GetDirectoryName(originalPath) + Path.DirectorySeparatorChar + tileset.filename;
                if (File.Exists(imgPath))
                {
                    try
                    {
                        newBmp = new Bitmap(imgPath);
                    }
                    catch (Exception) { }
                }
            }
            tileset.image = newBmp;

            RefreshPreviewTObj();
            foreach(Tileset.Tile tile in tileset.tiles)
            {
                RefreshSelectedTObjsListViewItem(tile);
            }
        }

        private void RefreshSelectedTilesetImage() => RefreshTilesetImage(SelectedTileset);

        private Tileset.Tile SelectedTObj => (tObjsListBox.SelectedIndex < 0) ? null : SelectedTileset.tiles[tObjsListBox.SelectedIndex];

        private string GetNameForTObjsListViewItem(Tileset.Tile tile) => "Tile ID " + tile.id + " (" + tile.x + ";" + tile.y + " " + tile.w + "*" + tile.h + ")";

        private void RefreshSelectedTObjsListViewItem(Tileset.Tile tile)
        {
            if (SelectedTileset == null)
                return;

            noTObjsRefresh = true;

            int idx = SelectedTileset.tiles.IndexOf(tile);
            if (idx < 0)
                return;

            tObjsListBox.Items[idx] = GetNameForTObjsListViewItem(tile);
            tObjsListBox.Images[idx] = GetImageForTObj(tile, SelectedTileset);

            noTObjsRefresh = false;
        }

        private void RefreshTObjsListView()
        {
            int selIdx = tObjsListBox.SelectedIndex;

            tObjsListBox.Items.Clear();
            tObjsListBox.Images.Clear();

            foreach (Tileset.Tile tile in SelectedTileset.tiles)
            {
                tObjsListBox.Items.Add(GetNameForTObjsListViewItem(tile));
                tObjsListBox.Images.Add(GetImageForTObj(tile, SelectedTileset));
            }

            #if DEBUG
            tObjPropertiesGroupBox.Enabled = removeTObjButton.Enabled = tObjsListBox.Items.Count != 0;
            #endif

            if (selIdx == -1)
            {
                if (tObjsListBox.Items.Count == 0)
                    tObjsListBox.SelectedIndex = -1;
                else
                    tObjsListBox.SelectedIndex = 0;
            }
            else
            {
                if (selIdx > tObjsListBox.Items.Count - 1)
                    tObjsListBox.SelectedIndex = tObjsListBox.Items.Count - 1;
                else
                    tObjsListBox.SelectedIndex = selIdx;
            }

            RefreshTilesTreeView();
        }

        bool noTObjsRefresh = false;

        private void tObjsListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (noTObjsRefresh)
                return;

            noTObjBacksave = true;
            tObjIDNumUpDown.Value = SelectedTObj.id;
            tObjXNumUpDown.Value = SelectedTObj.x;
            tObjYNumUpDown.Value = SelectedTObj.y;
            tObjWNumUpDown.Value = SelectedTObj.w;
            tObjHNumUpDown.Value = SelectedTObj.h;
            tObjSCheckBox.Checked = SelectedTObj.solid;
            tObjOCheckBox.Checked = SelectedTObj.overPlayer;
            tObjCCheckBox.Checked = SelectedTObj.overrideColl;
            noTObjBacksave = false;

            RefreshPreviewTObj();

            RefreshSelectedTObjsListViewItem(SelectedTObj);
        }

        private void addTObjButton_Click(object sender, EventArgs e)
        {
            int nID = 0;
            if(SelectedTileset.tiles.Count > 0)
                nID = SelectedTileset.tiles.Max(x => x.id) + 1;

            Tileset.Tile tile = new Tileset.Tile(nID, 0, 0, 32, 32);

            int selIdx = tObjsListBox.SelectedIndex;
            SelectedTileset.tiles.Insert(selIdx + 1, tile);

            RefreshTObjsListView();

            tObjsListBox.SelectedIndex = selIdx + 1;
        }

        private void removeTObjButton_Click(object sender, EventArgs e)
        {
            int selIdx = tObjsListBox.SelectedIndex;
            SelectedTileset.tiles.RemoveAt(selIdx);

            RefreshTObjsListView();

            if (selIdx != 0)
                tObjsListBox.SelectedIndex = selIdx - 1;
        }

        bool noTObjBacksave = false;
        private void tObjIDNumUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (noTObjBacksave)
                return;

            SelectedTObj.id = (int)tObjIDNumUpDown.Value;
            RefreshSelectedTObjsListViewItem(SelectedTObj);
        }

        private void tObjXNumUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (noTObjBacksave)
                return;

            SelectedTObj.x = (ushort)tObjXNumUpDown.Value;
            RefreshSelectedTObjsListViewItem(SelectedTObj);
            RefreshPreviewTObj();
        }

        private void tObjYNumUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (noTObjBacksave)
                return;

            SelectedTObj.y = (ushort)tObjYNumUpDown.Value;
            RefreshSelectedTObjsListViewItem(SelectedTObj);
            RefreshPreviewTObj();
        }

        private void tObjWNumUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (noTObjBacksave)
                return;

            SelectedTObj.w = (ushort)tObjWNumUpDown.Value;
            RefreshSelectedTObjsListViewItem(SelectedTObj);
            RefreshPreviewTObj();
        }

        private void tObjHNumUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (noTObjBacksave)
                return;

            SelectedTObj.h = (ushort)tObjHNumUpDown.Value;
            RefreshSelectedTObjsListViewItem(SelectedTObj);
            RefreshPreviewTObj();
        }

        private void tObjSCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (noTObjBacksave)
                return;

            SelectedTObj.solid = tObjSCheckBox.Checked;
        }

        private void tObjOCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (noTObjBacksave)
                return;

            SelectedTObj.overPlayer = tObjOCheckBox.Checked;
        }

        private void tObjCCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (noTObjBacksave)
                return;

            SelectedTObj.overrideColl = tObjCCheckBox.Checked;
        }


        private Bitmap GetImageForTObj(Tileset.Tile tile, Tileset tileset)
        {
            Bitmap bmp = new Bitmap(64, 64);

            if (tileset == null || tileset.image == null)
                return bmp;

            using (Graphics g = Graphics.FromImage(bmp))
            {
                g.DrawImage(tileset.image, new Rectangle(0, 0, 64, 64), tile.rect, GraphicsUnit.Pixel);
            }

            return bmp;
        }

        private void RefreshPreviewTObj()
        {
            if (SelectedTObj == null)
                return;

            if(SelectedTileset.image == null)
            {
                tObjPreviewPictureBox.Image = null;
                tObjPreviewPictureBox.BackgroundImage = UnknownBmp;
                return;
            }

            Bitmap bmp = GetImageForTObj(SelectedTObj, SelectedTileset);

            tObjPreviewPictureBox.Image = bmp;
            tObjPreviewPictureBox.BackgroundImage = TransparentBmp;
        }

        private Tileset.Tile SelectedTile => (tObjsTreeView.SelectedNode == null) ? null : (Tileset.Tile)tObjsTreeView.SelectedNode.Tag;
        private Tileset SelectedTileTileset
        {
            get
            {
                foreach(Tileset tileset in tilesets)
                    if (tileset.tiles.IndexOf(SelectedTile) >= 0)
                        return tileset;

                return null;
            }
        }

        private void RefreshTilesTreeView()
        {
            tObjsTreeView.Nodes.Clear();
            tObjsTreeView.CleanImageList();
            int folderIdx = tObjsTreeView.ImageList.Images.IndexOfKey("Folder");

            foreach (Tileset tileset in tilesets)
            {
                int tilesetID = tilesets.IndexOf(tileset);

                int tNodeIdx = tObjsTreeView.Nodes.Add(new TreeNode(tileset.name, folderIdx, folderIdx));
                TreeNode tNode = tObjsTreeView.Nodes[tNodeIdx];

                List<Tileset.Tile> remainingTiles = new List<Tileset.Tile>(tileset.tiles);
                foreach (Tileset.Category category in tileset.categories)
                {
                    int cNodeIdx = tNode.Nodes.Add(new TreeNode(category.name, folderIdx, folderIdx));
                    TreeNode cNode = tNode.Nodes[cNodeIdx];

                    foreach (Tileset.Tile tile in category.tiles)
                    {
                        string imgKey = tilesetID + "-" + tile.id.ToString();
                        tObjsTreeView.ImageList.Images.Add(imgKey, GetImageForTObj(tile, tileset));

                        int imgIdx = tObjsTreeView.ImageList.Images.IndexOfKey(imgKey);
                        TreeNode tn = new TreeNode(GetNameForTObjsListViewItem(tile), imgIdx, imgIdx);
                        tn.Tag = tile;
                        cNode.Nodes.Add(tn);

                        int tileIdx = remainingTiles.IndexOf(tile);
                        if (tileIdx >= 0)
                            remainingTiles.RemoveAt(tileIdx);
                    }
                }

                foreach (Tileset.Tile tile in remainingTiles)
                {
                    string imgKey = tilesetID + "-" + tile.id.ToString();
                    tObjsTreeView.ImageList.Images.Add(imgKey, GetImageForTObj(tile, tileset));

                    int imgIdx = tObjsTreeView.ImageList.Images.IndexOfKey(imgKey);
                    TreeNode tn = new TreeNode(GetNameForTObjsListViewItem(tile), imgIdx, imgIdx);
                    tn.Tag = tile;
                    tNode.Nodes.Add(tn);
                }
            }

            tObjsTreeView.Invalidate();
            return;
        }

        private void addTileFolderButton_Click(object sender, EventArgs e)
        {
            TreeNode node = tObjsTreeView.SelectedNode;
            if (node == null)
                return;

            while (node.Parent != null)
                node = node.Parent;

            node.Expand();

            int folderIdx = tObjsTreeView.ImageList.Images.IndexOfKey("Folder");
            int nodeIdx = node.Nodes.Add(new TreeNode("Nouvelle catégorie", folderIdx, folderIdx));
            tObjsTreeView.LabelEdit = true;
            node.Nodes[nodeIdx].BeginEdit();
        }

        private void tObjsTreeView_AfterLabelEdit(object sender, NodeLabelEditEventArgs e)
        {
            tObjsTreeView.LabelEdit = false;
        }

        private void removeTileFolderButton_Click(object sender, EventArgs e)
        {

        }

        private void mapBuilderPBox_Click(object sender, EventArgs e)
        {
            mapBuilderPBox.Focus();
        }

        private void mapBuilderPBox_MouseEnter(object sender, EventArgs e)
        {
            currentCoordsLabel.Visible = true;
        }

        private void mapBuilderPBox_MouseLeave(object sender, EventArgs e)
        {
            currentCoordsLabel.Visible = false;
        }

        private void mapBuilderPBox_MouseMove(object sender, MouseEventArgs e)
        {
            Point pt = mapBuilderPBox.GetPointForPos(e.Location);
            currentCoordsLabel.Text = "X: " + (pt.X + map.originPoint.X) + " ; Y: " + (pt.Y + map.originPoint.Y);
        }

        private void mapBuilderPBox_MiddleClick(object sender, PictureBoxMapBuilder.MiddleClickEventArgs e)
        {
            if (e.TileInfo == null)
            {
                tObjsTreeView.SelectedNode = null;
                tObjsTreeView_AfterSelect(sender, null);
                return;
            }

            foreach(TreeNode node in tObjsTreeView.Collect())
            {
                if (node.Tag == e.TileInfo.LinkedObject)
                {
                    tObjsTreeView.SelectedNode = node;
                    break;
                }
            }

        }

        private void mapBuilderPBox_StartTilePlacement(object sender, PictureBoxMapBuilder.StartTilePlacementEventArgs e)
        {
            e.LinkedObject = SelectedTile;
            e.DisplayBitmap = GetImageForTObj(SelectedTile, SelectedTileTileset);
        }

        private void mapBuilderPBox_TileDoubleClick(object sender, PictureBoxMapBuilder.StartTilePlacementEventArgs e)
        {
            int rtileX = e.TileX + map.originPoint.X;
            int rtileY = e.TileY + map.originPoint.Y;

            Map.Special special = map.specials.Find(sp => sp.position.X == rtileX && sp.position.Y == rtileY);

            using (AddSpeTileForm astf = new AddSpeTileForm(speTiles, special))
            {
                if(astf.ShowDialog() == DialogResult.OK)
                {
                    if (special == null && astf.SelectedID >= 0)
                        map.specials.Add(new Map.Special("actor", astf.SelectedID, new Point(rtileX, rtileY), astf.ParamVals));
                    else {
                        if (astf.SelectedID >= 0)
                        {
                            special.id = astf.SelectedID;
                            special.settings = astf.ParamVals;
                        }
                        else
                            map.specials.Remove(special);
                    }
                }
            }
        }

        private void mapBuilderPBox_PostPaint(object sender, PaintEventArgs e)
        {
            mapPreviewPBox.Image = mapBuilderPBox.AsBitmap(1, true);

            foreach(Map.Special special in map.specials)
            {
                Point pos = new Point((special.position.X - map.originPoint.X) * mapBuilderPBox.PixelsPerTile, (special.position.Y - map.originPoint.Y) * mapBuilderPBox.PixelsPerTile);
                Rectangle rect = new Rectangle(pos, new Size(mapBuilderPBox.PixelsPerTile, mapBuilderPBox.PixelsPerTile));
                e.Graphics.DrawRectangle(new Pen(Color.Red), rect);

                string text = speTiles.tileTypes[special.id].name;
                foreach(KeyValuePair<string, object> kvp in special.settings)
                    text += Environment.NewLine + kvp.Value;

                Size estimatedSize = TextRenderer.MeasureText(text, this.Font);

                int targetSize = mapBuilderPBox.PixelsPerTile - 8;
                float neededPtsW = this.Font.Size * (float)targetSize / (float)estimatedSize.Width;
                float neededPtsH = this.Font.Size * (float)targetSize / (float)estimatedSize.Height;
                float neededPts = Math.Min(neededPtsW, neededPtsH);

                StringFormat format = new StringFormat();
                format.LineAlignment = StringAlignment.Center;
                format.Alignment = StringAlignment.Center;

                if(neededPts > 0.0f)
                    e.Graphics.DrawString(text, new Font(this.Font.FontFamily, neededPts, this.Font.Style), new SolidBrush(Color.Red), rect, format);
            }
        }

        private void mapBuilderPanel_CTRLMouseWheel(object sender, MouseEventArgs e)
        {

            int viewportWidthBefore = mapBuilderPanel.Width - (mapBuilderPanel.VerticalScroll.Visible ? SystemInformation.VerticalScrollBarWidth : 0);
            int viewportHeightBefore = mapBuilderPanel.Height - (mapBuilderPanel.HorizontalScroll.Visible ? SystemInformation.HorizontalScrollBarHeight : 0);

            double scrollbarLeftRatio = (double)mapBuilderPanel.HorizontalScroll.Value / (double)mapBuilderPanel.HorizontalScroll.Maximum;
            double scrollbarTopRatio = (double)mapBuilderPanel.VerticalScroll.Value / (double)mapBuilderPanel.VerticalScroll.Maximum;
            int scrollbarRightPos = (mapBuilderPanel.HorizontalScroll.Value + viewportWidthBefore);
            double scrollbarRightRatio = (double)(mapBuilderPanel.HorizontalScroll.Value + viewportWidthBefore) / (double)mapBuilderPanel.HorizontalScroll.Maximum;
            int scrollbarTopPos = (mapBuilderPanel.VerticalScroll.Value + viewportHeightBefore);
            double scrollbarBottomRatio = (double)(mapBuilderPanel.VerticalScroll.Value + viewportHeightBefore) / (double)mapBuilderPanel.VerticalScroll.Maximum;

            if (e.Delta < 0)
            {
                if (mapBuilderPBox.PixelsPerTile > 8)
                    mapBuilderPBox.PixelsPerTile /= 2;
            }
            else if (e.Delta > 0)
            {
                if (mapBuilderPBox.PixelsPerTile < 128)
                    mapBuilderPBox.PixelsPerTile *= 2;
            }

            mapBuilderPBox.Invalidate();
            mapBuilderPanel.PerformLayout();

            int viewportWidthAfter = mapBuilderPanel.Width - (mapBuilderPanel.VerticalScroll.Visible ? SystemInformation.VerticalScrollBarWidth : 0);
            int viewportHeightAfter = mapBuilderPanel.Height - (mapBuilderPanel.HorizontalScroll.Visible ? SystemInformation.HorizontalScrollBarHeight : 0);

            int zoomLeft = (int)Math.Round(mapBuilderPanel.HorizontalScroll.Maximum * scrollbarLeftRatio);
            int zoomTop = (int)Math.Round(mapBuilderPanel.VerticalScroll.Maximum * scrollbarTopRatio);

            int goRight = (int)Math.Round(mapBuilderPanel.HorizontalScroll.Maximum * scrollbarRightRatio) - zoomLeft - viewportWidthBefore;
            int goDown = (int)Math.Round(mapBuilderPanel.VerticalScroll.Maximum * scrollbarBottomRatio) - zoomTop - viewportHeightBefore;

            int zoomFinalX = (int)Math.Round(zoomLeft + (goRight * ((double)e.X / (double)viewportWidthBefore)));
            int zoomFinalY = (int)Math.Round(zoomTop + (goDown * ((double)e.Y / (double)viewportHeightBefore)));

            if (zoomFinalX < 0)
                zoomFinalX = 0;
            if (zoomFinalX > mapBuilderPanel.HorizontalScroll.Maximum)
                zoomFinalX = mapBuilderPanel.HorizontalScroll.Maximum;

            if (mapBuilderPanel.HorizontalScroll.Visible)
                mapBuilderPanel.HorizontalScroll.Value = zoomFinalX;

            if (zoomFinalY < 0)
                zoomFinalY = 0;
            if (zoomFinalY > mapBuilderPanel.VerticalScroll.Maximum)
                zoomFinalY = mapBuilderPanel.VerticalScroll.Maximum;

            if (mapBuilderPanel.VerticalScroll.Visible)
                mapBuilderPanel.VerticalScroll.Value = zoomFinalY;

            mapBuilderPanel.PerformLayout();
        }

        private void mapBuilderPBox_SupprKeyPress(object sender, KeyEventArgs e)
        {
            if (mapBuilderPBox.SelectedTile != null)
                mapBuilderPBox.PlaceTile(mapBuilderPBox.SelectedTile?.X ?? 0, mapBuilderPBox.SelectedTile?.Y ?? 0, null, null);
        }

        private void layerRadioButtons_CheckedChanged(object sender, EventArgs e)
        {
            mapBuilderPBox.Layer = layer0RadioButton.Checked ? 0 : (layer1RadioButton.Checked ? 1 : 2);
            mapBuilderPBox.Invalidate();
        }

        private void layer2VisibilityCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            mapBuilderPBox.LayersVisibility[2] = layer2VisibilityCheckBox.Checked;
            mapBuilderPBox.Invalidate();
        }

        private void layer1VisibilityCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            mapBuilderPBox.LayersVisibility[1] = layer1VisibilityCheckBox.Checked;
            mapBuilderPBox.Invalidate();
        }

        private void layer0VisibilityCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            mapBuilderPBox.LayersVisibility[0] = layer0VisibilityCheckBox.Checked;
            mapBuilderPBox.Invalidate();
        }

        private void tObjsTreeView_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (SelectedTile == null) {
                selTilePreviewPBox.Image = null;
                return;
            }

            selTilePreviewPBox.Image = GetImageForTObj(SelectedTile, SelectedTileTileset);
        }

        private void maxSizeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            mapBuilderPBox.WTiles = 256;
            mapBuilderPBox.HTiles = 256;
        }

        private void changeSizeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (DoubleNumAskForm afsf = new DoubleNumAskForm(mapBuilderPBox.WTiles, mapBuilderPBox.HTiles, 1, 512, "Quelle taille de map ?", "Largeur:", "Hauteur:", "Vous avez entré une taille plus petite que celle actuelle, des données risquent d'être perdues." + Environment.NewLine + "Êtes vous sûr de vouloir continuer ?"))
            {
                if (afsf.ShowDialog() == DialogResult.OK)
                {
                    mapBuilderPBox.WTiles = afsf.Sel1;
                    mapBuilderPBox.HTiles = afsf.Sel2;
                }
            }
        }

        private void editOriginPointToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (DoubleNumAskForm afsf = new DoubleNumAskForm(map.originPoint.X, map.originPoint.Y, int.MinValue, int.MaxValue, "Quelle point d'origine ?", "X:", "Y:", ""))
            {
                Point origPt = map.originPoint;
                if (afsf.ShowDialog() == DialogResult.OK)
                {
                    map.originPoint.X = afsf.Sel1;
                    map.originPoint.Y = afsf.Sel2;

                    foreach(Map.Special special in map.specials)
                    {
                        special.position = new Point(special.position.X - origPt.X + map.originPoint.X, special.position.Y - origPt.Y + map.originPoint.Y);
                    }
                }
            }
        }

        private void changeMusicIDToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (NumUpDownAskForm nudaf = new NumUpDownAskForm(-1, -1, 255, 1, true))
            {
                nudaf.Title = "Changer le numéro de musique";
                nudaf.NumValue = map.musicID;

                if(nudaf.ShowDialog() == DialogResult.OK)
                {
                    map.musicID = (int)nudaf.NumValue;
                }
            }
        }
    }
}
