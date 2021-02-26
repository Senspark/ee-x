using System.Threading.Tasks;

namespace EETest {
    public static class ServiceUtils {
        public static async Task Initialize() {
            EE.PluginManager.InitializePlugins();

#if UNITY_EDITOR
            var logManager = new EE.NullLogManager();
            var analyticsManager = new EE.NullAnalyticsManager(logManager);
#else // UNITY_EDITOR
            var logManager = new EE.FirebaseLogManager();
            var analyticsManager = new EE.FirebaseAnalyticsManager();
#endif // UNITY_EDITOR
            var sceneLoader = new EE.DefaultSceneLoader();

            await logManager.Initialize();
            await analyticsManager.Initialize();
            await sceneLoader.Initialize();

            EE.ServiceLocator.Provide(logManager);
            EE.ServiceLocator.Provide(analyticsManager);
            EE.ServiceLocator.Provide(sceneLoader);
        }
    }
}