using System;
using System.Threading.Tasks;

namespace EE.Internal {
    public class NullRetrier : IRetrier {
        public Task Process(Func<Task<bool>> processor) {
            return Task.FromResult<object>(null);
        }

        public void Stop() {
        }
    }
}