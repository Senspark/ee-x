using System;
using System.Reflection;

using UnityEngine.Assertions;

namespace EE.Internal {
    internal class FirebaseCrashlyticsImpl : IFirebaseCrashlyticsImpl {
        private readonly MethodInfo _methodLog;

        public FirebaseCrashlyticsImpl() {
            var type = Type.GetType("Firebase.Crashlytics.Crashlytics, Firebase.Crashlytics");
            Assert.IsNotNull(type);
            _methodLog = type.GetMethod("Log", new[] {typeof(string)});
        }

        public void Log(string message) {
            _methodLog.Invoke(null, new object[] {message});
        }
    }
}