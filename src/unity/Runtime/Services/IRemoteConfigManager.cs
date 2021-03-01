using System;

using UnityEngine.Scripting;

namespace EE {
    [Preserve]
    [Service("EE" + nameof(IRemoteConfigManager))]
    public interface IRemoteConfigManager : IService {
        DateTime Timestamp { get; }
        bool GetBool(string key);
        long GetLong(string key);
        double GetDouble(string key);
        string GetString(string key);
    }
}