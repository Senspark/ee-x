using System;
using System.Runtime.InteropServices;

namespace EE.Internal {
    internal class MessageBridgeImplIos : IMessageBridgeImpl {
        private static Func<string, string, string> _callback;

        [DllImport("__Internal")]
        private static extern string ee_staticCall(string tag, string message);

        private static string ee_callCppInternal(string tag, string message) {
            return _callback(tag, message);
        }

        public void SetCallCppCallback(Func<string, string, string> callback) {
            _callback = callback;
        }

        public string Call(string tag, string message) {
            return ee_staticCall(tag, message);
        }
    }
}