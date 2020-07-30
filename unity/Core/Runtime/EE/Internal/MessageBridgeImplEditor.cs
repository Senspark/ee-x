using System;

namespace EE.Internal {
    internal class MessageBridgeImplEditor : IMessageBridgeImpl {
        public void SetCallCppCallback(Func<string, string, string> callback) {
            // FIXME.
        }

        public string Call(string tag, string message) {
            // FIXME.
            return "";
        }
    }
}