namespace EETest {
    public static class ServiceUtils {
        public static void Initialize() {
            EE.PluginManager.InitializePlugins();
            var sceneLoader = new EE.DefaultSceneLoader();
            EE.ServiceLocator.Provide(sceneLoader);
        }
    }
}