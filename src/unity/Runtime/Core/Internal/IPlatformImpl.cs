using System.Threading.Tasks;

namespace EE.Internal {
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
        Task<string> GetInstallReferrerUrl();
    }
}