using System;
using System.Threading.Tasks;

namespace EE {
    using MessageHandler = Action<string>;

    public interface IMessageBridge {
        void RegisterHandler(MessageHandler handler, string tag);
        void DeregisterHandler(string tag);
        string Call(string tag, string message = "");
        Task<string> CallAsync(string tag, string message = "");
    }
}