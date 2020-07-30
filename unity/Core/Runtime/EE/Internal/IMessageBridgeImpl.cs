using System;

namespace EE.Internal {
    internal interface IMessageBridgeImpl {
        void SetCallCppCallback(Func<string, string, string> callback);
        string Call(string tag, string message);
    }
}