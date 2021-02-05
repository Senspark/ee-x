using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

using UnityEngine;

namespace EE.Internal {
    using Destroyer = Action;

    internal class AdColony : IAdColony {
        private const string kTag = nameof(AdColony);
        private const string kPrefix = "AdColonyBridge";
        private const string kInitialize = kPrefix + "Initialize";

        private readonly IMessageBridge _bridge;
        private readonly ILogger _logger;
        private readonly Destroyer _destroyer;

        public AdColony(IMessageBridge bridge, ILogger logger, Destroyer destroyer) {
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
            public string appId;
            public List<string> zoneIds;
        }

        public async Task<bool> Initialize(string appId, params string[] zoneIds) {
            var request = new InitializeRequest {
                appId = appId,
                zoneIds = zoneIds.ToList()
            };
            var response = await _bridge.CallAsync(kInitialize, JsonUtility.ToJson(request));
            return Utils.ToBool(response);
        }
    }
}