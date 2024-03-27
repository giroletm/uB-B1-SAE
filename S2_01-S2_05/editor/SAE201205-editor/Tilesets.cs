using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace SAE201205_editor
{
    public class Tileset
    {
        public string name = "";
        public string filename = "";
        public string ifilename = "";
        public List<Tile> tiles = new List<Tile>();
        public List<Category> categories = new List<Category>();
        public Bitmap image = null;

        public class Tile
        {
            public int id = -1;
            public ushort x = 0;
            public ushort y = 0;
            public ushort w = 32;
            public ushort h = 32;
            public bool solid = false;
            public bool overPlayer = false;
            public bool overrideColl = false;

            public Rectangle rect => new Rectangle(x, y, w, h);

            public Tile() { }

            public Tile(int id, ushort x, ushort y, ushort w, ushort h)
            {
                this.id = id;
                this.x = x;
                this.y = y;
                this.w = w;
                this.h = h;
            }
        }

        public class Category
        {
            public string name = "";
            public List<Tile> tiles = new List<Tile>();
        }


        private Tileset(string name, string filename, string ifilename)
        {
            this.name = name;
            this.filename = filename;
            this.ifilename = ifilename;

            tiles = new List<Tile>();
            categories = new List<Category>();
        }

        private Tileset(string json, string title)
        {
            JObject jo = JObject.Parse(json);
            JToken root = jo.Root;

            
            this.name = root.SelectToken("name").ToObject<string>();
            this.filename = root.SelectToken("filename").ToObject<string>();
            this.ifilename = title;

            JToken jtiles = root.SelectToken("tiles");
            tiles = new List<Tile>();

            foreach (JToken jtile in jtiles.Children())
            {
                Tile tile = new Tile();

                tile.id = jtile.SelectToken("id").ToObject<int>();
                tile.x = jtile.SelectToken("x").ToObject<ushort>();
                tile.y = jtile.SelectToken("y").ToObject<ushort>();
                tile.w = jtile.SelectToken("w").ToObject<ushort>();
                tile.h = jtile.SelectToken("h").ToObject<ushort>();
                int flags = jtile.SelectToken("f").ToObject<int>();
                tile.solid = (flags & 1) > 0;
                tile.overPlayer = (flags & 2) > 0;
                tile.overrideColl = (flags & 4) > 0;

                tiles.Add(tile);
            }

            JToken jcategories = root.SelectToken("categories");
            categories = new List<Category>();

            foreach (JToken jcategory in jcategories.Children())
            {
                Category category = new Category();

                category.name = jcategory.SelectToken("name").ToObject<string>();

                category.tiles = new List<Tile>();
                foreach (JToken jtileID in jcategory.SelectToken("tileIDs").Children())
                {
                    int tid = jtileID.ToObject<int>();
                    Tile found = tiles.Find(x => x.id == tid);
                    if(found != null)
                        category.tiles.Add(found);
                }

                categories.Add(category);
            }
        }

        public static Tileset FromFile(string path)
        {
            try
            {
                return new Tileset(File.ReadAllText(path), Path.GetFileName(path));
            }
            catch (Exception e)
            {
                return null;
            }
        }

        public static Tileset FromJSON(string json, string ifilename)
        {
            try
            {
                return new Tileset(json, ifilename);
            }
            catch(Exception e)
            {
                return null;
            }
        }

        public static Tileset FromEmpty(string name, string filename, string ifilename)
        {
            return new Tileset(name, filename, ifilename);
        }

        public bool ToFile(string path)
        {
            try
            {
                JObject root = new JObject();
                root.Add("name", this.name);
                root.Add("filename", this.filename);

                JArray jtiles = new JArray();
                foreach (Tile tile in tiles)
                {
                    JObject jtile = new JObject();
                    jtile.Add("id", tile.id);
                    jtile.Add("x", tile.x);
                    jtile.Add("y", tile.y);
                    jtile.Add("w", tile.w);
                    jtile.Add("h", tile.h);
                    int flags = ((tile.solid ? 1 : 0) << 0) | ((tile.overPlayer ? 1 : 0) << 1) | ((tile.overrideColl ? 1 : 0) << 2);
                    jtile.Add("f", flags);

                    jtiles.Add(jtile);
                }

                root.Add("tiles", jtiles);

                JArray jcategories = new JArray();
                foreach (Category category in categories)
                {
                    JObject jcategory = new JObject();
                    jcategory.Add("name", category.name);

                    JArray jtileIDs = new JArray();
                    foreach (Tile tile in category.tiles)
                        jtileIDs.Add(tile.id);

                    jcategory.Add("tileIDs", jtileIDs);

                    jcategories.Add(jcategory);
                }

                root.Add("categories", jcategories);

                File.WriteAllText(path, root.ToString());

                return true;
            }
            catch(Exception e)
            {
                return false;
            }
        }
    }

}
