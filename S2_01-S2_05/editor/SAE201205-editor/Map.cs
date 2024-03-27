using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAE201205_editor
{
    public  class Map
    {
        public string name = "";
        public int musicID = -1;

        public List<Tileset> tilesets = new List<Tileset>();
        public Point originPoint = new Point(0, 0);
        public Size size = new Size(256, 256);

        public List<Tile>[] tilemaps = new List<Tile>[2] { new List<Tile>(), new List<Tile>() };
        public List<Special> specials = new List<Special>();

        public class Tile
        {
            public Tileset tileset = null;
            public Tileset.Tile tile = null;

            public Tile(Tileset tileset, Tileset.Tile tile)
            {
                this.tileset = tileset;
                this.tile = tile;
            }
        }

        public class Special
        {
            public string type = "actor";
            public int id = -1;
            public Point position = new Point(-1, -1);
            public Dictionary<string, object> settings = new Dictionary<string, object>();

            public Special(string type, int id, Point position, Dictionary<string, object> settings)
            {
                this.type = type;
                this.id = id;
                this.position = position;
                this.settings = settings;
            }
        }

        private Map(string name)
        {
            this.name = name;

            tilesets = new List<Tileset>();
            originPoint = new Point(0, 0);

            tilemaps = new List<Tile>[2] { new List<Tile>(), new List<Tile>() };
            specials = new List<Special>();
        }

        private Map(string json, string path)
        {
            JObject jo = JObject.Parse(json);
            JToken root = jo.Root;


            this.name = root.SelectToken("name").ToObject<string>();
            this.musicID = root.SelectToken("music").ToObject<int>();

            JToken jtilesets = root.SelectToken("tilesets");
            tilesets = new List<Tileset>();

            Dictionary<int, Tileset> tilesetsIndices = new Dictionary<int, Tileset>();
            foreach (JToken jtileset in jtilesets.Children())
            {
                Tileset tileset = Tileset.FromFile(Path.GetDirectoryName(path) + Path.DirectorySeparatorChar + jtileset.SelectToken("filename").ToObject<string>());

                tilesetsIndices.Add(jtileset.SelectToken("id").ToObject<int>(), tileset);
                tilesets.Add(tileset);
            }

            JToken joriginpoint = root.SelectToken("originpoint");
            originPoint = new Point(joriginpoint.SelectToken("x").ToObject<int>(), joriginpoint.SelectToken("y").ToObject<int>());

            JToken jsize = root.SelectToken("size");
            size = new Size(jsize.SelectToken("w").ToObject<int>(), jsize.SelectToken("h").ToObject<int>());

            JToken jtilemaps = root.SelectToken("tilemaps");
            int nbLayers = jtilemaps.Children().Count();
            tilemaps = new List<Tile>[nbLayers];
            for (int i = 0; i < nbLayers; i++)
                tilemaps[i] = new List<Tile>();

            int tID = 0;
            foreach (JToken jtilemap in jtilemaps.Children())
            {
                foreach(JToken jtile in jtilemap.Children())
                {
                    Tileset ts = null;
                    Tileset.Tile tl = null;

                    if (jtile.Children().Count() > 0)
                    {
                        int tsId = jtile.SelectToken("ts").ToObject<int>();
                        int tlId = jtile.SelectToken("tl").ToObject<int>();

                        ts = tilesetsIndices[tsId];
                        tl = ts.tiles.Find(t => t.id == tlId);
                    }

                    tilemaps[tID].Add(new Tile(ts, tl));
                }

                tID++;
            }

            JToken jspecials = root.SelectToken("special");
            specials = new List<Special>();

            foreach (JToken jspecial in jspecials.Children())
            {
                string spType = jspecial.SelectToken("type").ToObject<string>();
                int spId = jspecial.SelectToken("id").ToObject<int>();
                JToken jpos = jspecial.SelectToken("position");
                int spPosX = jpos.SelectToken("x").ToObject<int>();
                int spPosY = jpos.SelectToken("y").ToObject<int>();
                JToken spSettings = jspecial.SelectToken("settings");

                Dictionary<string, object> setsDict = new Dictionary<string, object>();
                foreach(JProperty spSetting in spSettings)
                {
                    JTokenType tType = spSetting.Value.Type;
                    switch(tType)
                    {
                        case JTokenType.Integer:
                            {
                                setsDict.Add(spSetting.Name, spSetting.Value.ToObject<int>());
                                break;
                            }
                        case JTokenType.Boolean:
                            {
                                setsDict.Add(spSetting.Name, spSetting.Value.ToObject<bool>());
                                break;
                            }
                        case JTokenType.String:
                            {
                                setsDict.Add(spSetting.Name, spSetting.Value.ToObject<string>());
                                break;
                            }
                        default:
                            {
                                break;
                            }
                    }
                }

                Special special = new Special(spType, spId, new Point(spPosX, spPosY), setsDict);

                specials.Add(special);
            }
        }

        public static Map FromFile(string path)
        {
            try
            {
                return new Map(File.ReadAllText(path), path);
            }
            catch (Exception e)
            {
                return null;
            }
        }

        public static Map FromJSON(string json, string path)
        {
            try
            {
                return new Map(json, path);
            }
            catch (Exception e)
            {
                return null;
            }
        }

        public static Map FromEmpty(string name)
        {
            return new Map(name);
        }

        public bool ToFile(string path)
        {
            try
            {
                JObject root = new JObject();
                root.Add("name", this.name);
                root.Add("music", this.musicID);

                JArray jtilesets = new JArray();
                for(int tID = 0; tID < tilesets.Count; tID++)
                {
                    JObject jtileset = new JObject();
                    jtileset.Add("filename", tilesets[tID].ifilename);
                    jtileset.Add("id", tID);

                    jtilesets.Add(jtileset);
                }
                root.Add("tilesets", jtilesets);

                JObject joriginpoint = new JObject();
                joriginpoint.Add("x", originPoint.X);
                joriginpoint.Add("y", originPoint.Y);
                root.Add("originpoint", joriginpoint);

                JObject jsize = new JObject();
                jsize.Add("w", size.Width);
                jsize.Add("h", size.Height);
                root.Add("size", jsize);

                JArray jtilemaps = new JArray();
                for (int tmID = 0; tmID < tilemaps.Length; tmID++)
                {
                    JArray jtilemap = new JArray();

                    int isRelevant = -1;
                    int idx = 0;
                    foreach(Tile tile in tilemaps[tmID])
                    {
                        JObject jtile = new JObject();
                        int tsID = tilesets.IndexOf(tile.tileset);
                        if (tsID >= 0)
                        {
                            jtile.Add("ts", tsID);
                            jtile.Add("tl", tile.tile.id);
                            isRelevant = idx;
                        }

                        jtilemap.Add(jtile);
                        idx++;
                    }

                    if (isRelevant >= 0)
                    {
                        for (int i = (tilemaps[tmID].Count - 1); i > isRelevant; i--)
                        {
                            jtilemap.RemoveAt(i);
                        }

                        jtilemaps.Add(jtilemap);
                    }
                }
                root.Add("tilemaps", jtilemaps);

                JArray jspecials = new JArray();
                foreach(Special special in specials)
                {
                    JObject jspecial = new JObject();

                    jspecial.Add("type", special.type);
                    jspecial.Add("id", special.id);

                    JObject jpos = new JObject();
                    jpos.Add("x", special.position.X);
                    jpos.Add("y", special.position.Y);
                    jspecial.Add("position", jpos);

                    JObject jsettings = new JObject();
                    foreach(KeyValuePair<string, object> setting in special.settings)
                    {
                        if(setting.Value is int)
                            jsettings.Add(setting.Key, (int)setting.Value);
                        else if(setting.Value is bool)
                            jsettings.Add(setting.Key, (bool)setting.Value);
                        else if(setting.Value is string)
                            jsettings.Add(setting.Key, (string)setting.Value);
                    }
                    jspecial.Add("settings", jsettings);

                    jspecials.Add(jspecial);
                }
                root.Add("special", jspecials);

                File.WriteAllText(path, root.ToString());

                return true;
            }
            catch (Exception e)
            {
                return false;
            }
        }
    }
}
