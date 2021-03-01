using System.Runtime.CompilerServices;

using UnityEngine.Scripting;

namespace EE {
    [Preserve]
    [Service("EE" + nameof(ILogManager))]
    public interface ILogManager : IService {
        void Log(string message = "",
            [CallerMemberName] string memberName = "",
            [CallerFilePath] string sourceFilePath = "",
            [CallerLineNumber] int sourceLineNumber = 0);
    }
}