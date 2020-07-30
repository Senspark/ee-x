using System;

using UnityEngine;

namespace EE.Internal {
    internal class MessageBridgeImplAndroid : IMessageBridgeImpl {
        private static Func<string, string, string> _callback;

        private readonly AndroidJavaClass _clazz = new AndroidJavaClass("com.ee.internal.MessageBridgeKt");

        private static string ee_callCppInternal(string tag, string message) {
            return _callback(tag, message);
        }

        public void SetCallCppCallback(Func<string, string, string> callback) {
            _callback = callback;
        }

        public string Call(string tag, string message) {
            return _clazz.CallStatic<string>("ee_staticCall", tag, message);
        }
    }
}