using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
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

        private MessageBridge() {
            _callbackCounter = 0;
            _handlerLock = new SpinLock();
            _handlers = new Dictionary<string, MessageHandler>();
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

#if UNITY_ANDROID
        private static readonly AndroidJavaClass _clazz = new AndroidJavaClass("com.ee.internal.MessageBridgeKt");

        public string Call(string tag, string message = "") {
            return _clazz.CallStatic<string>("ee_staticCall", tag, message);
        }
#else // UNITY_ANDROID
        [DllImport("__Internal")]
        private static extern string ee_staticCall(string tag, string message);

        public string Call(string tag, string message = "") {
            return ee_staticCall(tag, message);
        }
#endif // UNITY_ANDROID

        private static string ee_callCppInternal(string tag, string message) {
            var bridge = Instance;
            var result = bridge.CallCpp(tag, message);
            return result;
        }

        public Task<string> CallAsync(string tag, string message = "") {
            var source = new TaskCompletionSource<string>();
            var callbackTag = $"{tag}{_callbackCounter++}";
            RegisterHandler(callbackMessage => {
                DeregisterHandler(callbackTag);
                source.SetResult(callbackMessage);
                return "";
            }, callbackTag);
            var request = new {
                callback_tag = callbackTag,
                message
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