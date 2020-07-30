using System;

using UnityEngine;

namespace EE.Internal {
    internal class MessageBridgeImplAndroid : IMessageBridgeImpl {
        private static Func<string, string, string> _callCppCallback;

        private readonly AndroidJavaClass _clazz = new AndroidJavaClass("com.ee.internal.MessageBridgeKt");

        private static string ee_callCppInternal(string tag, string message) {
            return _callCppCallback(tag, message);
        }

        public MessageBridgeImplAndroid(Func<string, string, string> callback) {
            _callCppCallback = callback;
        }

        public string Call(string tag, string message) {
            return _clazz.CallStatic<string>("ee_staticCall", tag, message);
        }
    }
}