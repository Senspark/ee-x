namespace EE.Internal {
    internal interface IFirebaseAnalyticsImpl {
        void SetCurrentScreen(string screenName);
        void LogEvent(string name);
        void LogEvent(string name, (string, object)[] parameters);
    }
}