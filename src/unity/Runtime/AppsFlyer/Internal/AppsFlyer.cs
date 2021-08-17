using System;

using UnityEngine;

namespace EE.Internal {
    using Destroyer = Action;

    internal class AppsFlyer : IAppsFlyer {
        private const string kTag = nameof(AppsFlyer);
        private const string kPrefix = "AppsFlyerBridge";
        private const string kInitialize = kPrefix + "Initialize";
        private const string kStartTracking = kPrefix + "StartTracking";
        private const string kGetDeviceId = kPrefix + "GetDeviceId";
        private const string kSetDebugEnabled = kPrefix + "SetDebugEnabled";
        private const string kSetStopTracking = kPrefix + "SetStopTracking";
        private const string kTrackEvent = kPrefix + "TrackEvent";

        private readonly IMessageBridge _bridge;
        private readonly ILogger _logger;
        private readonly Destroyer _destroyer;

        public AppsFlyer(IMessageBridge bridge, ILogger logger, Destroyer destroyer) {
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
            public string devKey;
            public string iosAppId;
        }

        public void Initialize(string devKey, string iosAppId) {
            var request = new InitializeRequest {
                devKey = devKey,
                iosAppId = iosAppId,
            };
            _bridge.Call(kInitialize, JsonUtility.ToJson(request));
        }

        public void StartTracking() {
            _bridge.Call(kStartTracking);
        }

        public string GetDeviceId() {
            return _bridge.Call(kGetDeviceId);
        }

        public void SetDebugEnabled(bool enabled) {
            _bridge.Call(kSetDebugEnabled, Utils.ToString(enabled));
        }

        public void SetStopTracking(bool enabled) {
            _bridge.Call(kSetStopTracking, Utils.ToString(enabled));
        }
    }
}