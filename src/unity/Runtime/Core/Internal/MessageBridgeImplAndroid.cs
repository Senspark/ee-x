using System;

using UnityEngine;

namespace EE.Internal {
    using CallCppCallback = Action<string, string>;

    internal class MessageBridgeImplAndroid : IMessageBridgeImpl {
        private class Callback : AndroidJavaProxy {
            private static CallCppCallback _callCppCallback;

            public Callback(CallCppCallback callback)
                : base("com.ee.internal.UnityMessageBridge$Callback") {
                _callCppCallback = callback;
            }

            private void callCpp(string tag, string message) {
                _callCppCallback(tag, message);
            }
        }

        private readonly AndroidJavaClass _clazz = new AndroidJavaClass("com.ee.internal.UnityMessageBridge");

        public MessageBridgeImplAndroid(CallCppCallback callback) {
            _clazz.CallStatic("initialize", new Callback(callback));
        }

        public string Call(string tag, string message) {
            return _clazz.CallStatic<string>("call", tag, message);
        }
    }
}