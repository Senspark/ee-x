using System;
using System.Reflection;

namespace EE.Internal {
    internal class FirebaseAnalyticsImpl : IFirebaseAnalyticsImpl {
        private readonly MethodInfo _methodSetCurrentScreen;

        public FirebaseAnalyticsImpl() {
            var type = Type.GetType("Firebase.Analytics.FirebaseAnalytics, Firebase.Analytics");
            if (type == null) {
                throw new ArgumentException("Cannot find FirebaseAnalytics");
            }
            _methodSetCurrentScreen = type.GetMethod("SetCurrentScreen", new[] {typeof(string), typeof(string)});
        }

        public void SetCurrentScreen(string screenName, string screenClass) {
            _methodSetCurrentScreen.Invoke(null, new object[] {screenName, screenClass});
        }
    }
}