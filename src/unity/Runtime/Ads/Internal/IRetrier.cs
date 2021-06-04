using System;
using System.Threading.Tasks;

namespace EE.Internal {
    using Processor = Func<Task<bool>>;

    internal interface IRetrier {
        Task Process(Processor processor);
        void Stop();
    }
}