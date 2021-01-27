using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

using UnityEngine;

namespace EE.Internal {
    using MessageHandler = Action<string>;

    internal class MessageBridge : IMessageBridge {
        private int _callbackCounter;
        private readonly Dictionary<string, MessageHandler> _handlers;
        private readonly IMessageBridgeImpl _impl;

        public MessageBridge() {
            _callbackCounter = 0;
            _handlers = new Dictionary<string, MessageHandler>();
            _impl =
#if UNITY_EDITOR
                new MessageBridgeImplEditor();
#elif UNITY_ANDROID
                new MessageBridgeImplAndroid(CallCpp);
#elif UNITY_IOS
                new MessageBridgeImplIos(CallCpp);
#endif
        }

        public void RegisterHandler(MessageHandler handler, string tag) {
            if (_handlers.ContainsKey(tag)) {
                throw new ArgumentException($"Failed to register handler: {tag}");
            }
            _handlers.Add(tag, handler);
        }

        public void DeregisterHandler(string tag) {
            if (!_handlers.ContainsKey(tag)) {
                throw new ArgumentException($"Failed to deregister handler: {tag}");
            }
            _handlers.Remove(tag);
        }

        private MessageHandler FindHandler(string tag) {
            if (_handlers.TryGetValue(tag, out var handler)) {
                return handler;
            }
            return null;
        }

        public string Call(string tag, string message = "") {
            return _impl.Call(tag, message);
        }

        [Serializable]
        private struct CallAsyncRequest {
            public string callback_tag;
            public string message;
        }

        public Task<string> CallAsync(string tag, string message = "") {
            var source = new TaskCompletionSource<string>();
            var callbackTag = $"{tag}{_callbackCounter++}";
            RegisterHandler(callbackMessage => {
                DeregisterHandler(callbackTag);
                source.SetResult(callbackMessage);
            }, callbackTag);
            var request = new CallAsyncRequest {
                callback_tag = callbackTag,
                message = message
            };
            Call(tag, JsonUtility.ToJson(request));
            return source.Task;
        }

        private void CallCpp(string tag, string message) {
            var handler = FindHandler(tag);
            if (handler == null) {
                throw new ArgumentException($"Failed to call handler: {tag}");
            }
            handler(message);
        }
    }
}