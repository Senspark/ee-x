using System;
using System.Reflection;

namespace EE.Internal {
    internal class FirebaseAnalyticsImpl : IFirebaseAnalyticsImpl {
        private readonly MethodInfo _methodSetCurrentScreen;
        private readonly MethodInfo _methodLogEventString;
        private readonly MethodInfo _methodLogEventStringParameters;

        public FirebaseAnalyticsImpl() {
            var type = Type.GetType("Firebase.Analytics.FirebaseAnalytics, Firebase.Analytics");
            if (type == null) {
                throw new ArgumentException("Cannot find FirebaseAnalytics");
            }
            _methodSetCurrentScreen = type.GetMethod("SetCurrentScreen",
                new[] {typeof(string), typeof(string)});
            _methodLogEventString = type.GetMethod("LogEvent",
                new[] {typeof(string)});
            _methodLogEventStringParameters = type.GetMethod("LogEvent",
                new[] {typeof(string), FirebaseParameter.InternalType.MakeArrayType()});
        }

        public void SetCurrentScreen(string screenName, string screenClass) {
            _methodSetCurrentScreen.Invoke(null, new object[] {screenName, screenClass});
        }

        public void LogEvent(string name) {
            _methodLogEventString.Invoke(null, new object[] {name});
        }

        public void LogEvent(string name, params FirebaseParameter[] parameters) {
            var firebaseParameters = Array.CreateInstance(FirebaseParameter.InternalType, parameters.Length);
            for (var i = 0; i < parameters.Length; ++i) {
                firebaseParameters.SetValue(parameters[i].Internal, i);
            }
            _methodLogEventStringParameters.Invoke(null, new object[] {name, firebaseParameters});
        }
    }
}