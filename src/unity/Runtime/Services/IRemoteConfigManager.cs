using UnityEngine.Scripting;

namespace EE {
    [Preserve]
    [Service("EE" + nameof(IConfigManager))]
    public interface IConfigManager : IService {
    }
}