using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

using UnityEngine;

namespace EE.Internal {
    using MessageHandler = Func<string, string>;

    public class MessageBridge : IMessageBridge {
        public static MessageBridge Instance { get; } = new MessageBridge();

        private int _callbackCounter;
        private SpinLock _handlerLock;
        private readonly Dictionary<string, MessageHandler> _handlers;
        private readonly IMessageBridgeImpl _impl;

        private MessageBridge() {
            _callbackCounter = 0;
            _handlerLock = new SpinLock();
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

        public bool RegisterHandler(MessageHandler handler, string tag) {
            var lockTaken = false;
            try {
                _handlerLock.Enter(ref lockTaken);
                if (_handlers.ContainsKey(tag)) {
                    System.Diagnostics.Debug.Assert(false);
                    return false;
                }
                _handlers.Add(tag, handler);
            } finally {
                if (lockTaken) {
                    _handlerLock.Exit(false);
                }
            }
            return true;
        }

        public bool DeregisterHandler(string tag) {
            var lockTaken = false;
            try {
                _handlerLock.Enter(ref lockTaken);
                if (!_handlers.ContainsKey(tag)) {
                    System.Diagnostics.Debug.Assert(false);
                    return false;
                }
                _handlers.Remove(tag);
            } finally {
                if (lockTaken) {
                    _handlerLock.Exit(false);
                }
            }
            return true;
        }

        private MessageHandler FindHandler(string tag) {
            var lockTaken = false;
            try {
                _handlerLock.Enter(ref lockTaken);
                if (_handlers.TryGetValue(tag, out var handler)) {
                    return handler;
                }
            } finally {
                if (lockTaken) {
                    _handlerLock.Exit(false);
                }
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
                return "";
            }, callbackTag);
            var request = new CallAsyncRequest {
                callback_tag = callbackTag,
                message = message
            };
            Call(tag, JsonUtility.ToJson(request));
            return source.Task;
        }

        private string CallCpp(string tag, string message) {
            var handler = FindHandler(tag);
            if (handler == null) {
                System.Diagnostics.Debug.Assert(false);
                return "";
            }
            return handler(message);
        }
    }
}