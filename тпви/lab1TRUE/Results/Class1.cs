using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using Newtonsoft.Json;
using Results.Models;
namespace BSTU.Results.Collection
{
    public class Results
    {
        private string _jsonFilePath;
        private List<ResultsModel> _results;
        private bool _isBlocked = false; //nostalgia 
        private int _maxKey = 0;
        private object _lock = new object();

        public Results(string jsonFilePath)
        {
            _jsonFilePath = jsonFilePath;
            if (File.Exists(jsonFilePath))
            {
                var jsontext = File.ReadAllText(_jsonFilePath);
                _results = JsonConvert.DeserializeObject<List<ResultsModel>>(jsontext);
                if (_results == null)
                {
                    _results = new List<ResultsModel>();
                }
                if (_results.Count > 0)
                {
                    _maxKey = _results.Max(r => r.Id);
                }
                else _maxKey = 0;
            }
            else
            {
                _results = new List<ResultsModel>();
            }
        }

        public List<ResultsModel> GetAllResults()
        {
            lock (_lock)
            {
                return _results.Select(r => new ResultsModel{Id = r.Id,Value = r.Value}).ToList();
            }
        }

        public string GetOneResults(int key)
        {
            lock (_lock)
            {
                var item = _results.FirstOrDefault(r => r.Id == key);
                if (item != null)
                {
                    return item.Value;
                }
                return string.Empty;
            }
        }

        public int AddOneResult(string value)
        {
            lock (_lock)
            {
                int key = ++_maxKey;
                var newItem = new ResultsModel{Id = key,Value = value};
                _results.Add(newItem);
                SaveToJson();
                return key;
            }
        }

        public int UpdateOneResult(int key, string newValue)
        {
            lock (_lock)
            {
                var item = _results.FirstOrDefault(r => r.Id == key);
                if (item != null)
                {
                    item.Value = newValue;
                    SaveToJson();
                    return key;
                }
                return -1;
            }
        }

        public int DeleteOneResult(int key)
        {
            lock (_lock)
            {
                var item = _results.FirstOrDefault(r => r.Id == key);
                if (item == null)
                {
                    return -1;
                }
                _results.Remove(item);
                SaveToJson();
                return key;
            }
        }

        private bool SaveToJson()
        {
            lock (_lock)
            {
                var json = JsonConvert.SerializeObject(_results, Formatting.Indented);
                File.WriteAllText(_jsonFilePath, json);
                return true;
            }
        }
    }
}