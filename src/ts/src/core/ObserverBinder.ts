import { IObserverManager } from "./IObserverManager";
import { ObserverHandle } from "./ObserverHandle";

export class ObserverBinder<Observer> {
    private readonly _handle: ObserverHandle;
    private readonly _manager: IObserverManager<Observer>;

    public constructor(handle: ObserverHandle, manager: IObserverManager<Observer>) {
        this._handle = handle;
        this._manager = manager;
    }

    public addObserver(observer: Observer): ObserverBinder<Observer> {
        const id = this._manager.addObserver(observer);
        const manager = this._manager;
        this._handle.add(() => {
            manager.removeObserver(id);
        });
        return this;
    }
}