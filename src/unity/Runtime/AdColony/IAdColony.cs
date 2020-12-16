using System.Threading.Tasks;

namespace EE {
    public interface IAdColony : IPlugin {
        Task<bool> Initialize(string appId, params string[] zoneIds);
    }
}