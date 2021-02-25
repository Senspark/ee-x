using System;
using System.Reflection;

namespace EE.Internal {
    internal class FirebaseCrashlyticsImpl : IFirebaseCrashlyticsImpl {
        private readonly MethodInfo _method;

        public FirebaseCrashlyticsImpl() {
            var type = Type.GetType("Firebase.Crashlytics.Crashlytics, Firebase.Crashlytics");
            if (type == null) {
                throw new ArgumentException("Cannot find FirebaseCrashlytics");
            }
            _method = type.GetMethod("Log", new[] {typeof(string)});
        }

        public void Log(string message) {
            _method.Invoke(null, new object[] {message});
        }
    }
}