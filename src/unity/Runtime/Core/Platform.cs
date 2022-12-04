using System.Collections.Generic;
using System.Threading.Tasks;

using EE.Internal;

namespace EE {
    public struct InstallReferrer {
        public string raw;
        public string utm_source;
        public string utm_medium;
        public string utm_term;
        public string utm_content;
        public string utm_campaign;
    }

    public struct SafeInset {
        public int left;
        public int right;
        public int top;
        public int bottom;
    }

    public enum AuthorizationStatus {
        NotDetermined,
        Restricted,
        Denied,
        Authorized,
        Other,
    }

    public static class Platform {
        private static IPlatformImpl _impl;

        internal static void Initialize(IMessageBridge bridge) {
#if UNITY_EDITOR || UNITY_STANDALONE_OSX || UNITY_WEBGL
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

        public static List<string> GetApplicationSignatures(string algorithm) {
            return _impl.GetApplicationSignatures(algorithm);
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

        public static SafeInset GetSafeInset() {
            return _impl.GetSafeInset();
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

        public static async Task<AuthorizationStatus> RequestTrackingAuthorization() {
            return await _impl.RequestTrackingAuthorization();
        }
    }
}