using System;
using System.Collections.Generic;
using System.Threading.Tasks;

using UnityEngine;

namespace EE.Internal {
    internal class PlatformImplNative : IPlatformImpl {
        private const string kPrefix = "Platform_";
        private const string kIsApplicationInstalled = kPrefix + "isApplicationInstalled";
        private const string kOpenApplication = kPrefix + "openApplication";
        private const string kGetApplicationId = kPrefix + "getApplicationId";
        private const string kGetApplicationName = kPrefix + "getApplicationName";
        private const string kGetVersionName = kPrefix + "getVersionName";
        private const string kGetVersionCode = kPrefix + "getVersionCode";
        private const string kGetApplicationSignatures = kPrefix + "getApplicationSignatures";
        private const string kIsInstantApp = kPrefix + "isInstantApp";
        private const string kIsTablet = kPrefix + "isTablet";
        private const string kGetDensity = kPrefix + "getDensity";
        private const string kGetViewSize = kPrefix + "getViewSize";
        private const string kGetScreenSize = kPrefix + "getScreenSize";
        private const string kGetDeviceId = kPrefix + "getDeviceId";
        private const string kGetSafeInset = kPrefix + "getSafeInset";
        private const string kSendMail = kPrefix + "sendMail";
        private const string kTestConnection = kPrefix + "testConnection";
        private const string kShowInstallPrompt = kPrefix + "showInstallPrompt";
        private const string kGetInstallReferrer = kPrefix + "getInstallReferrer";

        private readonly IMessageBridge _bridge;

        public PlatformImplNative(IMessageBridge bridge) {
            _bridge = bridge;
        }

        public bool IsApplicationInstalled(string applicationId) {
            var response = _bridge.Call(kIsApplicationInstalled, applicationId);
            return Utils.ToBool(response);
        }

        public bool OpenApplication(string applicationId) {
            var response = _bridge.Call(kOpenApplication, applicationId);
            return Utils.ToBool(response);
        }

        public string GetApplicationId() {
            return _bridge.Call(kGetApplicationId);
        }

        public string GetApplicationName() {
            return _bridge.Call(kGetApplicationName);
        }

        public string GetVersionName() {
            return _bridge.Call(kGetVersionName);
        }

        public string GetVersionCode() {
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

        public string GetSha1Signature() {
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

        public bool IsInstantApp() {
#if UNITY_ANDROID
            var response = _bridge.Call(kIsInstantApp);
            return Utils.ToBool(response);
#else // UNITY_ANDROID
            return false;
#endif // UNITY_ANDROID
        }

        public bool IsTablet() {
            var response = _bridge.Call(kIsTablet);
            return Utils.ToBool(response);
        }

        public float GetDensity() {
            var response = _bridge.Call(kGetDensity);
            return float.Parse(response);
        }

        [Serializable]
        private struct ViewSizeResponse {
            public int width;
            public int height;
        }

        public (int, int) GetViewSize() {
            var response = _bridge.Call(kGetViewSize);
            var json = JsonUtility.FromJson<ViewSizeResponse>(response);
            return (json.width, json.height);
        }

        [Serializable]
        private struct ScreenSizeResponse {
            public int width;
            public int height;
        }

        public (int, int) GetScreenSize() {
            var response = _bridge.Call(kGetScreenSize);
            var json = JsonUtility.FromJson<ScreenSizeResponse>(response);
            return (json.width, json.height);
        }

        public async Task<string> GetDeviceId() {
            var response = await _bridge.CallAsync(kGetDeviceId);
            return response;
        }

        [Serializable]
        private struct SendMailRequest {
            public string recipient;
            public string subject;
            public string body;
        }

        public bool SendMail(string recipient, string subject, string body) {
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

        public async Task<bool> TestConnection(string hostName, float timeOut) {
            var request = new TestConnectionRequest {
                host_name = hostName,
                time_out = timeOut,
            };
            var response = await _bridge.CallAsync(kTestConnection, JsonUtility.ToJson(request));
            return Utils.ToBool(response);
        }

        public async Task<InstallReferrer> GetInstallReferrer() {
            var result = new InstallReferrer();
#if UNITY_ANDROID
            var response = await _bridge.CallAsync(kGetInstallReferrer);
            result.raw = response;
#else // UNITY_ANDROID
            result.raw = "";
#endif // UNITY_ANDROID
            var parameters = result.raw.Split('&');
            var m = new Dictionary<string, string>();
            foreach (var parameter in parameters) {
                var entries = parameter.Split('=');
                if (entries.Length >= 2) {
                    m[entries[0]] = entries[1];
                }
            }
            result.utm_source = m.TryGetValue("utm_source", out var source) ? source : "";
            result.utm_medium = m.TryGetValue("utm_medium", out var medium) ? medium : "";
            result.utm_term = m.TryGetValue("utm_medium", out var term) ? term : "";
            result.utm_content = m.TryGetValue("utm_content", out var content) ? content : "";
            result.utm_campaign = m.TryGetValue("utm_campaign", out var campaign) ? campaign : "";
            return result;
        }
    }
}