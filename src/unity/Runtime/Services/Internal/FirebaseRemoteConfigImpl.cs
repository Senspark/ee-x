using System;
using System.Collections.Generic;
using System.Reflection;
using System.Threading.Tasks;

namespace EE.Internal {
    public class FirebaseRemoteConfigImpl : IFirebaseRemoteConfigImpl {
        private readonly MethodInfo _methodSetDefaultsAsync;
        private readonly MethodInfo _methodFetchAsync;
        private readonly MethodInfo _methodActivateAsync;
        private readonly MethodInfo _methodGetBool;
        private readonly MethodInfo _methodGetLong;
        private readonly MethodInfo _methodGetDouble;
        private readonly MethodInfo _methodGetString;
        private readonly object _instance;

        public FirebaseRemoteConfigImpl() {
            var type = Type.GetType("Firebase.RemoteConfig.FirebaseRemoteConfig, Firebase.RemoteConfig");
            if (type == null) {
                throw new ArgumentException("Cannot find FirebaseRemoteConfig");
            }
            _methodSetDefaultsAsync = type.GetMethod("SetDefaulsAsync",
                new[] {typeof(Dictionary<string, object>)});
            _methodFetchAsync = type.GetMethod("FetchAsync",
                new[] {typeof(TimeSpan)});
            _methodActivateAsync = type.GetMethod("ActivateAsync");
            _instance = type.GetProperty("DefaultInstance")?.GetValue(null);
        }

        public async Task SetDefaultsAsync(Dictionary<string, object> defaultValues) {
            var task = (Task) _methodSetDefaultsAsync.Invoke(_instance, new object[] {defaultValues});
            await task.ConfigureAwait(false);
        }

        public async Task FetchAsync(TimeSpan cacheExpiration) {
            var task = (Task) _methodFetchAsync.Invoke(_instance, new object[] {cacheExpiration});
            await task.ConfigureAwait(false);
        }

        public async Task<bool> ActivateAsync() {
            var result = await (Task<bool>) _methodActivateAsync.Invoke(_instance, null);
            return result;
        }

        public bool GetBool(string key) {
            // Firebase.RemoteConfig.FirebaseRemoteConfig.DefaultInstance.GetValue("a");
        }
    }
}