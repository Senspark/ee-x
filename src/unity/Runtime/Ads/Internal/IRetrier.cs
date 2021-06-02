using System;
using System.Threading.Tasks;

namespace EE.Internal {
    using Processor = Func<Task<bool>>;

    public interface IRetrier {
        Task Process(Processor processor);
        void Stop();
    }
}