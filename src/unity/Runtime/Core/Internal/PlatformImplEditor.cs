using System.Threading.Tasks;

using UnityEngine;

namespace EE.Internal {
    internal class PlatformImplEditor : IPlatformImpl {
        public bool IsApplicationInstalled(string applicationId) {
            return false;
        }

        public bool OpenApplication(string applicationId) {
            return false;
        }

        public string GetApplicationId() {
            return Application.identifier;
        }

        public string GetApplicationName() {
            return Application.productName;
        }

        public string GetVersionName() {
            return Application.version;
        }

        public string GetVersionCode() {
            return "";
        }

        public string GetSha1Signature() {
            return "";
        }

        public bool IsInstantApp() {
            return false;
        }

        public bool IsTablet() {
            return false;
        }

        public float GetDensity() {
            return Screen.dpi;
        }

        public (int, int) GetViewSize() {
            return GetScreenSize();
        }

        public (int, int) GetScreenSize() {
            return (Screen.width, Screen.height);
        }

        public Task<string> GetDeviceId() {
            return Task.FromResult("");
        }

        public bool SendMail(string recipient, string subject, string body) {
            return false;
        }

        public Task<bool> TestConnection(string hostName, float timeOut) {
            return Task.FromResult(false);
        }

        public Task<string> GetInstallReferrerUrl() {
            return Task.FromResult("");
        }
    }
}