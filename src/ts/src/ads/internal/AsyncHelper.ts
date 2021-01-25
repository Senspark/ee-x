import { IAsyncHelper } from "./IAsyncHelper";

export class AsyncHelper<Result> implements IAsyncHelper<Result> {
    private _promise?: Promise<Result>;
    private _resolver?: (result: Result) => void;
    private _finalizer?: (result: Result) => void;
    private _processing: boolean;

    public constructor() {
        this._processing = false;
    }

    public get isProcessing(): boolean {
        return this._processing;
    }

    public process(processor: () => void, finalizer: (result: Result) => void): Promise<Result> {
        if (this._processing) {
            // Waiting.
        } else {
            this._promise = new Promise<Result>(resolver => {
                this._processing = true;
                this._finalizer = finalizer;
                processor();
            });
        }
        return this._promise!;
    }

    public resolve(result: Result): void {
        this._finalizer && this._finalizer(result);
        this._finalizer = undefined;
        this._resolver && this._resolver(result);
        this._resolver = undefined;
        this._processing = false;
    }

}