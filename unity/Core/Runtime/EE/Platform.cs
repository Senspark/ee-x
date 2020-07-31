using System;
using System.Collections.Generic;
using System.Threading.Tasks;

using UnityEngine;

namespace EE {
    public static class Platform {
        private const string kPrefix = "Platform_";
        private static readonly string kIsApplicationInstalled = $"{kPrefix}isApplicationInstalled";
        private static readonly string kOpenApplication = $"{kPrefix}openApplication";
        private static readonly string kGetApplicationId = $"{kPrefix}getApplicationId";
        private static readonly string kGetApplicationName = $"{kPrefix}getApplicationName";
        private static readonly string kGetVersionName = $"{kPrefix}getVersionName";
        private static readonly string kGetVersionCode = $"{kPrefix}getVersionCode";
        private static readonly string kGetApplicationSignatures = $"{kPrefix}getApplicationSignatures";
        private static readonly string kIsInstantApp = $"{kPrefix}isInstantApp";
        private static readonly string kIsTablet = $"{kPrefix}isTablet";
        private static readonly string kGetDensity = $"{kPrefix}getDensity";
        private static readonly string kGetDeviceId = $"{kPrefix}getDeviceId";
        private static readonly string kGetSafeInset = $"{kPrefix}getSafeInset";
        private static readonly string kSendMail = $"{kPrefix}sendMail";
        private static readonly string kTestConnection = $"{kPrefix}testConnection";
        private static readonly string kShowInstallPrompt = $"{kPrefix}showInstallPrompt";

        private static IMessageBridge _bridge;

        public static void RegisterHandlers(IMessageBridge bridge) {
            _bridge = bridge;
        }

        public static bool IsApplicationInstalled(string applicationId) {
            var response = _bridge.Call(kIsApplicationInstalled, applicationId);
            return Utils.ToBool(response);
        }

        public static bool OpenApplication(string applicationId) {
            var response = _bridge.Call(kOpenApplication, applicationId);
            return Utils.ToBool(response);
        }

        public static string GetApplicationId() {
            return _bridge.Call(kGetApplicationId);
        }

        public static string GetApplicationName() {
            return _bridge.Call(kGetApplicationName);
        }

        public static string GetVersionName() {
            return _bridge.Call(kGetVersionName);
        }

        public static string GetVersionCode() {
            return _bridge.Call(kGetVersionCode);
        }

        [Serializable]
        private struct GetSha1SignatureRequest {
            public string algorithm;
        }

        [Serializable]
        private struct GetSha1SignatureResponse {
            public List<string> signatures;
        }

        public static string GetSha1Signature() {
#if UNITY_ANDROID
            var request = new GetSha1SignatureRequest {
                algorithm = "SHA1",
            };
            var response = _bridge.Call(kGetApplicationSignatures, JsonUtility.ToJson(request));
            var json = JsonUtility.FromJson<GetSha1SignatureResponse>(response);
            var signatures = json.signatures;
            return signatures.Count == 0 ? "" : signatures[0];
#else // UNITY_ANDROID
            return "";
#endif // UNITY_ANDROID
        }

        public static bool IsInstantApp() {
#if UNITY_ANDROID
            var response = _bridge.Call(kIsInstantApp);
            return Utils.ToBool(response);
#else // UNITY_ANDROID
            return false;
#endif // UNITY_ANDROID
        }

        public static bool IsTablet() {
            var response = _bridge.Call(kIsTablet);
            return Utils.ToBool(response);
        }

        public static float GetDensity() {
            var response = _bridge.Call(kGetDensity);
            return float.Parse(response);
        }

        public static async Task<string> GetDeviceId() {
            var response = await _bridge.CallAsync(kGetDeviceId);
            return response;
        }

        [Serializable]
        private struct SendMailRequest {
            public string recipient;
            public string subject;
            public string body;
        }

        public static bool SendMail(string recipient, string subject, string body) {
            var request = new SendMailRequest {
                recipient = recipient,
                subject = subject,
                body = body
            };
            var response = _bridge.Call(kSendMail, JsonUtility.ToJson(recipient));
            return Utils.ToBool(response);
        }

        [Serializable]
        private struct TestConnectionRequest {
            public string host_name;
            public float time_out;
        }

        public static async Task<bool> TestConnection(string hostName, float timeOut) {
            var request = new TestConnectionRequest {
                host_name = hostName,
                time_out = timeOut,
            };
            var response = await _bridge.CallAsync(kTestConnection, JsonUtility.ToJson(request));
            return Utils.ToBool(response);
        }
    }
}