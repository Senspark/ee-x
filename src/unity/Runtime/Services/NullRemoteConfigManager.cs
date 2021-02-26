using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace EE {
    public class NullRemoteConfigManager : IRemoteConfigManager {
        private readonly Dictionary<string, object> _defaultValues;

        public NullRemoteConfigManager(Dictionary<string, object> defaultValues) {
            _defaultValues = defaultValues;
        }

        public Task<bool> Initialize() => Task.FromResult(true);

        public void Destroy() {
        }

        public DateTime Timestamp => DateTime.Now;

        public bool GetBool(string key) => (bool) _defaultValues[key];

        public long GetLong(string key) => (long) _defaultValues[key];

        public double GetDouble(string key) => (double) _defaultValues[key];

        public string GetString(string key) => (string) _defaultValues[key];
    }
}