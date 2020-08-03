namespace EE.Internal {
    internal interface IMessageBridgeImpl {
        string Call(string tag, string message);
    }
}