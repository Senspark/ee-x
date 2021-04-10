using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace EE.Internal {
    internal interface IFirebaseRemoteConfigImpl {
        bool IsLastFetchSuccessful { get; }
        DateTime FetchTime { get; }
        void Initialize();
        Task SetDefaultsAsync(Dictionary<string, object> defaults);
        Task FetchAsync(TimeSpan cacheExpiration);
        Task<bool> ActivateAsync();
        bool GetBool(string key);
        long GetLong(string key);
        double GetDouble(string key);
        string GetString(string key);
    }
}