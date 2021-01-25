export interface IAsyncHelper<Result> {
    isProcessing: boolean;
    process(processor: () => void, finalizer: (result: Result) => void): Promise<Result>;
    resolve(result: Result): void;
}