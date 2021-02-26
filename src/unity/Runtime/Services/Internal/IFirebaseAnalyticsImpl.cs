namespace EE.Internal {
    internal interface IFirebaseAnalyticsImpl {
        void SetCurrentScreen(string screenName, string screenClass);
        void LogEvent(string name);
        void LogEvent(string name, params FirebaseParameter[] parameters);
    }
}