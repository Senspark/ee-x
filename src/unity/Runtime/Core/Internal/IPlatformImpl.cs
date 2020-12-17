using System.Threading.Tasks;

namespace EE.Internal {
    public struct InstallReferrer {
        public string raw;
        public string utm_source;
        public string utm_medium;
        public string utm_term;
        public string utm_content;
        public string utm_campaign;
    }

    public interface IPlatformImpl {
        bool IsApplicationInstalled(string applicationId);
        bool OpenApplication(string applicationId);
        string GetApplicationId();
        string GetApplicationName();
        string GetVersionName();
        string GetVersionCode();
        string GetSha1Signature();
        bool IsInstantApp();
        bool IsTablet();
        float GetDensity();
        (int, int) GetViewSize();
        (int, int) GetScreenSize();
        Task<string> GetDeviceId();
        bool SendMail(string recipient, string subject, string body);
        Task<bool> TestConnection(string hostName, float timeOut);
        Task<InstallReferrer> GetInstallReferrer();
    }
}