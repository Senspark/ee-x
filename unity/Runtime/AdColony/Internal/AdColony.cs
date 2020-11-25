using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

using UnityEngine;

namespace EE.Internal {
    internal class AdColony : IAdColony {
        private const string kPrefix = "AdColonyBridge";
        private const string kInitialize = kPrefix + "Initialize";

        private readonly IMessageBridge _bridge;

        public AdColony(IMessageBridge bridge) {
            _bridge = bridge;
        }

        public void Destroy() {
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
            await Thread.SwitchToLibraryThread();
            return Utils.ToBool(response);
        }
    }
}