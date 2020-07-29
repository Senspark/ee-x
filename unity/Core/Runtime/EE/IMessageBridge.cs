using System;
using System.Threading.Tasks;

namespace EE {
    using MessageHandler = Func<string, string>;

    public interface IMessageBridge {
        bool RegisterHandler(MessageHandler handler, string tag);
        bool DeregisterHandler(MessageHandler handler, string tag);
        string Call(string tag, string message = "");
        Task<string> CallAsync(string tag, string message = "");
    }
}