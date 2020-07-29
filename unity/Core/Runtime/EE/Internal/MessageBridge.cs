using System;
using System.Threading.Tasks;

namespace EE.Internal {
    public class MessageBridge : IMessageBridge {
        public bool RegisterHandler(Func<string, string> handler, string tag) {
            throw new NotImplementedException();
        }

        public bool DeregisterHandler(Func<string, string> handler, string tag) {
            throw new NotImplementedException();
        }

        public string Call(string tag, string message = "") {
            throw new NotImplementedException();
        }

        public Task<string> CallAsync(string tag, string message = "") {
            throw new NotImplementedException();
        }
    }
}