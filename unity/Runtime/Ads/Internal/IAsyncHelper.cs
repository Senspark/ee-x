using System;
using System.Threading.Tasks;

namespace EE.Internal {
    public interface IAsyncHelper<Result> {
        /// <summary>
        /// Checks whether this helper is processing.
        /// </summary>
        bool IsProcessing { get; }

        /// <summary>
        /// Asynchronously processes.
        /// </summary>
        Task<Result> Process(Action processor, Action<Result> finalizer);

        /// <summary>
        /// Resolves the result.
        /// </summary>
        void Resolve(Result result);
    }
}