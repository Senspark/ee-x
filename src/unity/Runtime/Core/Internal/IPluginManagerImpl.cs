namespace EE.Internal {
    internal interface IPluginManagerImpl {
        void InitializePlugins(string version);
        void SetLogLevel(LogLevel level);
        object GetActivity();
        void SetActivity(object activity);
        void AddPlugin(string name);
        void RemovePlugin(string name);
    }
}