using System.Threading.Tasks;

using EE.Internal;

namespace EE {
    public static class Platform {
        private static IPlatformImpl _impl;

        public static void RegisterHandlers(IMessageBridge bridge) {
#if UNITY_EDITOR
            _impl = new PlatformImplEditor();
#else // UNITY_EDITOR
            _impl = new PlatformImplNative(bridge);
#endif // UNITY_EDITOR
        }

        public static bool IsApplicationInstalled(string applicationId) {
            return _impl.IsApplicationInstalled(applicationId);
        }

        public static bool OpenApplication(string applicationId) {
            return _impl.OpenApplication(applicationId);
        }

        public static string GetApplicationId() {
            return _impl.GetApplicationId();
        }

        public static string GetApplicationName() {
            return _impl.GetApplicationName();
        }

        public static string GetVersionName() {
            return _impl.GetVersionName();
        }

        public static string GetVersionCode() {
            return _impl.GetVersionCode();
        }

        public static string GetSha1Signature() {
            return _impl.GetSha1Signature();
        }

        public static bool IsInstantApp() {
            return _impl.IsInstantApp();
        }

        public static bool IsTablet() {
            return _impl.IsTablet();
        }

        public static float GetDensity() {
            return _impl.GetDensity();
        }

        public static (int, int) GetViewSize() {
            return _impl.GetViewSize();
        }

        public static (int, int) GetScreenSize() {
            return _impl.GetScreenSize();
        }

        public static async Task<string> GetDeviceId() {
            return await _impl.GetDeviceId();
        }

        public static bool SendMail(string recipient, string subject, string body) {
            return _impl.SendMail(recipient, subject, body);
        }

        public static async Task<bool> TestConnection(string hostName, float timeOut) {
            return await _impl.TestConnection(hostName, timeOut);
        }

        public static async Task<InstallReferrer> GetInstallReferrer() {
            return await _impl.GetInstallReferrer();
        }
    }
}