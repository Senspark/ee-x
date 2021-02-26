using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace EE {
    public class FirebaseRemoteConfigManager : IRemoteConfigManager {
        private readonly IDataManager _dataManager;
        private Task<bool> _initializer;
        private bool _initialized;
        private bool _fetched;
        private readonly Dictionary<string, object> _defaultValues;

        public Task<bool> Initialize() {
            throw new NotImplementedException();
        }

        public void Destroy() {
        }

        public DateTime Timestamp { get; }

        public bool GetBool(string key) {
            throw new NotImplementedException();
        }

        public long GetLong(string key) {
            throw new NotImplementedException();
        }

        public double GetDouble(string key) {
            throw new NotImplementedException();
        }

        public string GetString(string key) {
            throw new NotImplementedException();
        }
    }
}