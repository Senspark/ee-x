using System;
using System.Collections.Generic;
using System.Reflection;
using System.Threading.Tasks;

using UnityEngine.Assertions;

namespace EE.Internal {
    public class FirebaseRemoteConfigImpl : IFirebaseRemoteConfigImpl {
        private readonly PropertyInfo _propertyDefaultInstance;
        private readonly PropertyInfo _propertyInfo;
        private readonly MethodInfo _methodSetDefaultsAsync;
        private readonly MethodInfo _methodFetchAsync;
        private readonly MethodInfo _methodActivateAsync;
        private readonly MethodInfo _methodGetValue;
        private readonly PropertyInfo _propertyBooleanValue;
        private readonly PropertyInfo _propertyLongValue;
        private readonly PropertyInfo _propertyDoubleValue;
        private readonly PropertyInfo _propertyStringValue;
        private readonly PropertyInfo _propertyFetchTime;
        private readonly PropertyInfo _propertyLastFetchStatus;
        private readonly object _lastFetchStatusSuccess;
        private object _instance;

        public FirebaseRemoteConfigImpl() {
            var type0 = Type.GetType("Firebase.RemoteConfig.FirebaseRemoteConfig, Firebase.RemoteConfig");
            Assert.IsNotNull(type0);
            _propertyDefaultInstance = type0.GetProperty("DefaultInstance");
            _propertyInfo = type0.GetProperty("Info");
            _methodSetDefaultsAsync = type0.GetMethod("SetDefaulsAsync", new[] {typeof(Dictionary<string, object>)});
            _methodFetchAsync = type0.GetMethod("FetchAsync", new[] {typeof(TimeSpan)});
            _methodActivateAsync = type0.GetMethod("ActivateAsync", new Type[] { });
            _methodGetValue = type0.GetMethod("GetValue", new[] {typeof(string)});

            var type1 = Type.GetType("Firebase.RemoteConfig.ConfigValue, Firebase.RemoteConfig");
            Assert.IsNotNull(type1);
            _propertyBooleanValue = type1.GetProperty("BooleanValue");
            _propertyLongValue = type1.GetProperty("LongValue");
            _propertyDoubleValue = type1.GetProperty("DoubleValue");
            _propertyStringValue = type1.GetProperty("StringValue");

            var type2 = Type.GetType("Firebase.RemoteConfig.ConfigInfo, Firebase.RemoteConfig");
            Assert.IsNotNull(type2);
            _propertyFetchTime = type2.GetProperty("FetchTime");
            _propertyLastFetchStatus = type2.GetProperty("LastFetchStatus");

            var type3 = Type.GetType("Firebase.RemoteConfig.LastFetchStatus, Firebase.RemoteConfig");
            Assert.IsNotNull(type3);
            _lastFetchStatusSuccess = Enum.Parse(type3, "Success");
        }

        public bool IsLastFetchSuccessful {
            get {
                var info = _propertyInfo.GetValue(_instance);
                var status = _propertyLastFetchStatus.GetValue(info);
                return _lastFetchStatusSuccess.Equals(status);
            }
        }

        public DateTime FetchTime {
            get {
                var info = _propertyInfo.GetValue(_instance);
                return (DateTime) _propertyFetchTime.GetValue(info);
            }
        }

        public void Initialize() {
            _instance = _propertyDefaultInstance.GetValue(null);
            Assert.IsNotNull(_instance);
        }

        public async Task SetDefaultsAsync(Dictionary<string, object> defaults) {
            var task = (Task) _methodSetDefaultsAsync.Invoke(_instance, new object[] {defaults});
            await task.ConfigureAwait(false);
        }

        public async Task FetchAsync(TimeSpan cacheExpiration) {
            var task = (Task) _methodFetchAsync.Invoke(_instance, new object[] {cacheExpiration});
            await task.ConfigureAwait(false);
        }

        public async Task<bool> ActivateAsync() {
            var result = await (Task<bool>) _methodActivateAsync.Invoke(_instance, new object[] { });
            return result;
        }

        public bool GetBool(string key) {
            var value = _methodGetValue.Invoke(_instance, new object[] {key});
            return (bool) _propertyBooleanValue.GetValue(value);
        }

        public long GetLong(string key) {
            var value = _methodGetValue.Invoke(_instance, new object[] {key});
            return (long) _propertyLongValue.GetValue(value);
        }

        public double GetDouble(string key) {
            var value = _methodGetValue.Invoke(_instance, new object[] {key});
            return (double) _propertyDoubleValue.GetValue(value);
        }

        public string GetString(string key) {
            var value = _methodGetValue.Invoke(_instance, new object[] {key});
            return (string) _propertyStringValue.GetValue(value);
        }
    }
}