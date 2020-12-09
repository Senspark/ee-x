using System.Threading.Tasks;
using UnityEngine;

namespace EE.Internal {
    public class InstallReferrerBridge {
        private static readonly AndroidJavaClass _class = new AndroidJavaClass("com.ee.internal.InstallReferrerBridge");
        
        public static async Task<string> GetUrl() {
            return _class.CallStatic<string>("getUrl");
        }

        public static  long GetClickTime() {
            return _class.CallStatic<long>("getClickTime");
        }
    }
}
