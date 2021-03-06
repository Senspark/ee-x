using System;
using System.Threading.Tasks;

using UnityEngine;

namespace EE.Internal {
    using Destroyer = Action;

    internal class Adjust : IAdjust {
        private const string kTag = nameof(Adjust);
        private const string kPrefix = "AdjustBridge";
        private const string kInitialize = kPrefix + "Initialize";
        private const string kSetEnabled = kPrefix + "SetEnabled";
        private const string kGetAdvertisingIdentifier = kPrefix + "GetAdvertisingIdentifier";
        private const string kGetDeviceIdentifier = kPrefix + "GetDeviceIdentifier";
        private const string kSetPushToken = kPrefix + "SetPushToken";
        private const string kTrackEvent = kPrefix + "TrackEvent";

        private readonly IMessageBridge _bridge;
        private readonly ILogger _logger;
        private readonly Destroyer _destroyer;

        public Adjust(IMessageBridge bridge, ILogger logger, Destroyer destroyer) {
            _bridge = bridge;
            _logger = logger;
            _destroyer = destroyer;
            _logger.Debug($"{kTag}: constructor");
        }

        public void Destroy() {
            _logger.Debug($"{kTag}: constructor");
            _destroyer();
        }

        [Serializable]
        private struct InitializeRequest {
            public string token;
            public int environment;
            public int logLevel;
            public bool eventBufferingEnabled;
        }

        public void Initialize(AdjustConfig config) {
            var request = new InitializeRequest {
                token = config._token,
                environment = (int) config._environment,
                logLevel = (int) config._logLevel,
                eventBufferingEnabled = config._eventBufferingEnabled,
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

        public void SetPushToken(string token) {
            _bridge.Call(kSetPushToken, token);
        }

        public void TrackEvent(string token) {
            _bridge.Call(kTrackEvent, token);
        }
    }
}