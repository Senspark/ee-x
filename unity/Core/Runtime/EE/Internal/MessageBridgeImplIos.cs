using System;
using System.Runtime.InteropServices;

using AOT;

namespace EE.Internal {
    using CallCppCallback = Func<string, string, string>;

    internal class MessageBridgeImplIos : IMessageBridgeImpl {
        private static CallCppCallback _callCppCallback;

        [DllImport("__Internal")]
        private static extern string ee_staticCall(string tag, string message);

        [DllImport("__Internal")]
        private static extern void ee_initializeMessageBridge(IntPtr pointer);

        [MonoPInvokeCallback(typeof(CallCppCallback))]
        private static string ee_callCppInternal(string tag, string message) {
            return _callCppCallback(tag, message);
        }

        public MessageBridgeImplIos(CallCppCallback callback) {
            var del = new CallCppCallback(ee_callCppInternal);
            var pointer = Marshal.GetFunctionPointerForDelegate(del);
            ee_initializeMessageBridge(pointer);
            _callCppCallback = callback;
        }

        public string Call(string tag, string message) {
            return ee_staticCall(tag, message);
        }
    }
}