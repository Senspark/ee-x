export class ScopeGuard {
    private _disposed = false;
    private readonly _undoFunction: () => void;

    public constructor(undoFunction: () => void) {
        this._undoFunction = undoFunction;
    }

    public dispose(): void {
        if (!this._disposed) {
            this._undoFunction();
        }
        this._disposed = true;
    }
}