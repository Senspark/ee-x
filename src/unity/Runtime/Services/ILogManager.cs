using System.Runtime.CompilerServices;

namespace EE {
    [Service("EE" + nameof(ILogManager))]
    public interface ILogManager : IService {
        void Log(string message = "",
            [CallerMemberName] string memberName = "",
            [CallerFilePath] string sourceFilePath = "",
            [CallerLineNumber] int sourceLineNumber = 0);
    }
}