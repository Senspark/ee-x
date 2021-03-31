using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

using EE.Internal;

using Jsonite;

using UnityEngine.Assertions;

namespace EE {
    public class FirebaseRemoteConfigManager : IRemoteConfigManager {
        private class Data {
            public int fetch_time;
        }

        private readonly IDataManager _dataManager;
        private readonly Dictionary<string, object> _defaults;
        private readonly IFirebaseRemoteConfigImpl _impl;
        private Task<bool> _initializer;
        private bool _initialized;
        private Data _data;
        private bool _fetched;

        public FirebaseRemoteConfigManager(
            IDataManager dataManager,
            string defaults) : this(
            dataManager,
            FirebaseUtils.ParseDefaults(defaults)) {
        }

        public FirebaseRemoteConfigManager(
            IDataManager dataManager,
            Dictionary<string, object> defaults) {
            _dataManager = dataManager;
            _defaults = defaults;
            _impl = new FirebaseRemoteConfigImpl();
        }

        public Task<bool> Initialize() => _initializer = _initializer ?? (_initializer = InitializeImpl(1f));

        public void Destroy() {
        }

        private async Task<bool> InitializeImpl(float timeOut) {
            await _dataManager.Initialize();
            await Task.WhenAny(
                Task.Delay((int) (timeOut * 1000)),
                InitializeImpl());
            if (_fetched) {
                await ActivateConfig();
            }
            return true;
        }

        private async Task<bool> InitializeImpl() {
            if (!await _dataManager.Initialize()) {
                return false;
            }
            if (!await FirebaseManager.Initialize()) {
                return false;
            }
            LoadData();
            _impl.Initialize();
            await ActivateConfig();
            await _impl.SetDefaultsAsync(_defaults);
            _initialized = true;
            await _impl.FetchAsync(TimeSpan.Zero);
            _fetched = true;
            return true;
        }

        private void LoadData() {
            _data = _dataManager.Get("ee_firebase_remote_config", new Data {
                fetch_time = 0,
            });
        }

        private void SaveData() {
            _dataManager.Set("ee_firebase_remote_config", _data);
        }

        private async Task ActivateConfig() {
            if (_impl.IsLastFetchSuccessful) {
                if (await _impl.ActivateAsync()) {
                    var fetchTime = _impl.FetchTime;
                    var seconds = (int) new DateTimeOffset(fetchTime).ToUnixTimeSeconds();
                    _data.fetch_time = seconds;
                    SaveData();
                }
            }
        }

        public DateTime Timestamp => DateTimeOffset.FromUnixTimeSeconds(_data.fetch_time).UtcDateTime;

        public bool GetBool(string key) {
            if (!_initialized) {
                return (bool) _defaults[key];
            }
            return _impl.GetBool(key);
        }

        public long GetLong(string key) {
            if (!_initialized) {
                return (long) _defaults[key];
            }
            return _impl.GetLong(key);
        }

        public double GetDouble(string key) {
            if (!_initialized) {
                return (double) _defaults[key];
            }
            return _impl.GetDouble(key);
        }

        public string GetString(string key) {
            if (!_initialized) {
                return (string) _defaults[key];
            }
            return _impl.GetString(key);
        }
    }
}