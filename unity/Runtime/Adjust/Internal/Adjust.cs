using System;
using System.Threading.Tasks;

using UnityEngine;

namespace EE.Internal {
    internal class Adjust : IAdjust {
        private const string kPrefix = "AdjustBridge";
        private const string kInitialize = kPrefix + "Initialize";
        private const string kSetEnabled = kPrefix + "SetEnabled";
        private const string kGetAdvertisingIdentifier = kPrefix + "GetAdvertisingIdentifier";
        private const string kGetDeviceIdentifier = kPrefix + "GetDeviceIdentifier";
        private const string kTrackEvent = kPrefix + "TrackEvent";

        private readonly IMessageBridge _bridge;

        public Adjust(IMessageBridge bridge) {
            _bridge = bridge;
        }

        public void Destroy() {
        }

        [Serializable]
        private struct InitializeRequest {
            public string token;
            public int environment;
            public int logLevel;
        }

        public void Initialize(AdjustConfig config) {
            var request = new InitializeRequest {
                token = config._token,
                environment = (int) config._environment,
                logLevel = (int) config._logLevel
            };
            _bridge.Call(kInitialize, JsonUtility.ToJson(request));
        }

        public void SetEnabled(bool enabled) {
            _bridge.Call(kSetEnabled, Utils.ToString(enabled));
        }

        public async Task<string> GetAdvertisingIdentifier() {
            var response = await _bridge.CallAsync(kGetAdvertisingIdentifier);
            return response;
        }

        public string GetDeviceIdentifier() {
            var response = _bridge.Call(kGetDeviceIdentifier);
            return response;
        }

        public void TrackEvent(string token) {
            _bridge.Call(kTrackEvent, token);
        }
    }
}